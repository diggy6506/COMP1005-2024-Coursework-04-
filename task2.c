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
                  const char *activity_filepath) {
  int i = 0, k = 0, n = 0, p = 0, q = 0;
  char student_line[1024], activity_line[1024];
  FILE *student_file;
  FILE *activity_file;
  int activity_line_counter;

  char(*student_array)[4][32];
  char(*activity_array)[2][32];

  student_array = malloc(student_line_counter * sizeof(*student_array));

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

    strncpy(student_array[i][3], "0", 31);

    i++;
  }

  activity_line_counter = count_student_lines(activity_filepath);

  activity_array = malloc(activity_line_counter * sizeof(*activity_array));

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
        strncpy(student_array[p][3], activity_array[q][1], 31);
      }
    }
  }

  for (n = 0; n < i; n++) {
    printf("%s %s %s\n", student_array[n][1], student_array[n][2],
           student_array[n][3]);
  }

  fclose(activity_file);
  fclose(student_file);
  free(student_array);
  free(activity_array);
}

int main(int argc, char **argv) {
  const char *student_filepath;
  const char *activity_filepath;
  int student_line_count;

  if (argc != 3) {
    printf("Error. Please enter 2 arguments.\n");
    return 1;
  }

  student_filepath = argv[1];
  activity_filepath = argv[2];

  if (access(student_filepath, F_OK | R_OK) == 0 &&
      access(activity_filepath, F_OK | R_OK) == 0) {
    student_line_count = count_student_lines(student_filepath);
    student_data(student_line_count, student_filepath, activity_filepath);
  }

  else {
    printf("Error. At least one file does not exist or is not readable.\n");
    return 1;
  }

  return 0;
}
