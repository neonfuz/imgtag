#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdarg.h>

#include "crc32.h"

#define err(retval, format, ...) \
  fprintf(stderr, )

int err(int retval, char *format, ...)
{
  va_list ap;
  va_start(ap, format);

  return retval;
}

int main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, "Error, please specify exactly one file\n");
    return 1;
  }

  int fildes = open(argv[1], O_RDONLY);
  if (fildes < 0) {
    fprintf(stderr, "Error opening \"%s\"\n", argv[1]);
    return 2;
  }

  struct stat st;

  if (fstat(fildes, &st) < 0) {
    fprintf(stderr, "Error stating file \"%s\"\n", argv[1]);
    return 3;
  }

  char *file = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fildes, 0);

  if (file == MAP_FAILED) {
    fprintf(stderr, "Error mapping file to memory\n");
    return 4;
  }

  printf("%s: %ld\n", argv[1], crc32(0, file, st.st_size));

  return 0;
}
