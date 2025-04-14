#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int count_student_lines(const char *student_filepath) {
  int student_line_counter = 0;
  char ch;

  FILE *student_file = fopen(student_filepath, "r");

  while ((ch = fgetc(student_file)) != EOF) {
    if (ch == '\n') {
      student_line_counter++;
    }
  }

  fclose(student_file);

  return student_line_counter;
}

int count_activity_lines(const char *activity_filepath) {
  int activity_line_counter = 0;
  char ch;

  FILE *activity_file = fopen(activity_filepath, "r");

  while ((ch = fgetc(activity_file)) != EOF) {
    if (ch == '\n') {
      activity_line_counter++;
    }
  }

  fclose(activity_file);

  return activity_line_counter;
}

void populate_grades_array(const char *activity_filepath, float **grades_array,
                           int *activity_counter, int absent_students,
                           int student_counter) {
  char *read_grades;
  FILE *activity_file;
  int i = 0;
  char *grade;
  char *endptr;

  *activity_counter = count_activity_lines(activity_filepath);

  *grades_array = (float *)malloc(student_counter * sizeof(float));

  read_grades = (char *)malloc(1024 * sizeof(char));

  activity_file = fopen(activity_filepath, "r");

  fgets(read_grades, 1024, activity_file);

  while (fgets(read_grades, 1024, activity_file)) {
    read_grades[strcspn(read_grades, "\r\n")] = '\0';

    strtok(read_grades, ",");
    grade = strtok(NULL, ",");

    (*grades_array)[i++] = strtol(grade, &endptr, 10);
  }

  while (i < student_counter) {
    (*grades_array)[i] = 0;
    i++;
  }

  fclose(activity_file);
  free(read_grades);
}

float calculate_grade_average(float *grades_array, int student_counter) {
  int i = 0;
  float grade_sum = 0.0;
  float grade_mean = 0.0;

  for (i = 0; i < student_counter; i++) {
    grade_sum += grades_array[i];
  }

  grade_mean = grade_sum / student_counter;

  printf("grade mean = %.2f\n", grade_mean);
  return grade_mean;
}

void standard_deviation(float *grades_array, int student_counter,
                        float grade_mean) {
  float standard_deviation = 0.0;
  int i = 0;

  for (i = 0; i < student_counter; i++) {
    standard_deviation += pow(grades_array[i] - grade_mean, 2);
  }

  standard_deviation = standard_deviation / student_counter;
  standard_deviation = sqrt(standard_deviation);

  printf("grade sd = %.2f\n", standard_deviation);
}

int main(int argc, char **argv) {
  const char *student_filepath;
  const char *activity_filepath;
  int student_counter, activity_counter = 0;
  int absent_students;
  float *grades_array;
  float grade_mean = 0;

  if (argc != 3) {
    printf("Error. Please enter 2 arguments.\n");
    return 1;
  }

  student_filepath = argv[1];
  activity_filepath = argv[2];

  activity_counter = count_activity_lines(activity_filepath);

  if (access(student_filepath, F_OK | R_OK) == 0 &&
      access(activity_filepath, F_OK | R_OK) == 0) {
    student_counter = count_student_lines(student_filepath) - 1;
    printf("total students = %d\n", student_counter);
    absent_students = student_counter - (activity_counter - 1);
    printf("absent students = %d\n", absent_students);

    populate_grades_array(activity_filepath, &grades_array, &activity_counter,
                          absent_students, student_counter);
    grade_mean = calculate_grade_average(grades_array, student_counter);
    standard_deviation(grades_array, student_counter, grade_mean);

    free(grades_array);
  } else {
    printf("Error. At least one file does not exist or is not readable.\n");
    return 1;
  }

  return 0;
}
