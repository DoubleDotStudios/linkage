#include "include/io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *lk_read_file(const char *filename) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename, "rb");
  if (fp == NULL) {
    fclose(fp);
    printf("\e[31m[Reading Error]: Failed to read file: %s\e[0m\n", filename);
    exit(1);
  }

  char *buffer = (char *)calloc(1, sizeof(char));
  buffer[0] = '\0';

  while ((read = getline(&line, &len, fp)) != -1) {
    buffer = (char *)realloc(buffer, (strlen(buffer) + strlen(line) + 1) *
                                         sizeof(char));
    strcat(buffer, line);
  }

  fclose(fp);
  if (line)
    free(line);

  return buffer;
}

void lk_write_file(const char *filename, char *outbuffer) {
  FILE *fp;

  fp = fopen(filename, "wb");
  if (fp == NULL) {
    fclose(fp);
    printf("\e[31m[Reading Error]: Failed to write file: %s\e[0m\n", filename);
    exit(1);
  }

  fputs(outbuffer, fp);

  fclose(fp);
}
