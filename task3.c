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

void student_data(int student_line_counter, const char *student_filepath,
                  const char *activity_filepath, const char *results_filepath) {
  int i = 0, k = 0, n = 0, p = 0, q = 0, s = 0;
  char student_line[1024], activity_line[1024];
  int activity_line_counter;

  FILE *student_file;
  FILE *activity_file;
  FILE *results_file;

  char(*student_array)[5][32];
  char(*activity_array)[2][32];

  student_array = calloc(student_line_counter, sizeof(*student_array));
  activity_line_counter = count_student_lines(activity_filepath);
  activity_array = calloc(activity_line_counter, sizeof(*activity_array));

  student_file = fopen(student_filepath, "r");
  fgets(student_line, sizeof(student_line), student_file);

  while (fgets(student_line, sizeof(student_line), student_file) &&
         i < student_line_counter) {
    char *student_id = strtok(student_line, ",");
    char *surname = strtok(NULL, ",");
    char *forename = strtok(NULL, ",");

    if (forename) forename[strcspn(forename, "\n")] = '\0';
    if (surname) surname[strcspn(surname, "\n")] = '\0';
    if (student_id) student_id[strcspn(student_id, "\n")] = '\0';

    strncpy(student_array[i][0], student_id, 31);
    strncpy(student_array[i][1], forename, 31);
    strncpy(student_array[i][2], surname, 31);

    i++;
  }

  activity_file = fopen(activity_filepath, "r");

  fgets(activity_line, sizeof(activity_line), activity_file);

  while (fgets(activity_line, sizeof(activity_line), activity_file) &&
         k < activity_line_counter) {
    char *activity_id = strtok(activity_line, ",");
    char *grade = strtok(NULL, ",");

    if (activity_id) activity_id[strcspn(activity_id, "\n")] = '\0';
    if (grade) grade[strcspn(grade, "\n")] = '\0';

    strncpy(activity_array[k][0], activity_id, 31);
    strncpy(activity_array[k][1], grade, 31);

    k++;
  }

  for (p = 0; p < i; p++) {
    for (q = 0; q < k; q++) {
      if (strcmp(student_array[p][0], activity_array[q][0]) == 0) {
        strncpy(student_array[p][4], activity_array[q][1], 31);
      }
    }
  }

  while (s < i) {
    float grade_sum = 0.0;
    float grade_average = 0.0;
    int grade_amount_counter = 0;
    int x = 4;

    for (x = 4; x < 5; x++) {
      if (strlen(student_array[s][x]) > 0) {
        grade_sum += atof(student_array[s][x]);
        grade_amount_counter++;
      }
    }

    if (grade_amount_counter > 0) {
      grade_average = grade_sum / grade_amount_counter;
    }

    else {
      grade_average = 0.0;
      strncpy(student_array[s][4], "0", 31);
    }

    sprintf(student_array[s][3], "%.2f", grade_average);
    s++;
  }

  results_file = fopen(results_filepath, "w");

  fprintf(results_file, "id,last_name,first_name,average,grade01\n");
  for (n = 0; n < i; n++) {
    fprintf(results_file, "%s,%s,%s,%s,%s\n", student_array[n][0],
            student_array[n][2], student_array[n][1], student_array[n][3],
            student_array[n][4]);
  }

  fclose(results_file);
  fclose(activity_file);
  fclose(student_file);
  free(student_array);
  free(activity_array);
}

int main(int argc, char **argv) {
  const char *student_filepath;
  const char *activity_filepath;
  const char *results_filepath;
  int student_line_count;

  if (argc != 4) {
    printf("Error. Please enter 3 arguments.\n");
    return 1;
  }

  student_filepath = argv[1];
  activity_filepath = argv[2];
  results_filepath = argv[3];

  if (access(student_filepath, F_OK | R_OK) == 0 &&
      access(activity_filepath, F_OK | R_OK) == 0) {
    student_line_count = count_student_lines(student_filepath);
    student_data(student_line_count, student_filepath, activity_filepath,
                 results_filepath);
  }

  else {
    printf("Error. At least one file does not exist or is not readable.\n");
    return 1;
  }

  return 0;
}
