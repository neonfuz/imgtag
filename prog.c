#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <inttypes.h>

#include "crc32.h"

#define err(retval, format, ...)		\
  do {						\
    fprintf(stderr, format, ##__VA_ARGS__);	\
    return retval;				\
  } while (0);


int main(int argc, char **argv)
{
  if (argc != 2)
    err(1, "Error, please specify exactly one file.\n");

  int fildes = open(argv[1], O_RDONLY);
  if (fildes < 0)
    err(2, "Error opening \"%s\"\n", argv[1]);

  struct stat st;

  if (fstat(fildes, &st) < 0)
    err(3, "Error stating file \"%s\"\n", argv[1]);

  char *file = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fildes, 0);

  if (file == MAP_FAILED)
    err(4, "Error mapping file to memory\n");

  printf("%s: 0x%" PRIx32 "\n", argv[1], crc32(0, file, st.st_size));
  printf("insert into test (filename, crc32) values(%s, %"PRIu32");\n", argv[1], crc32(0, file, st.st_size));

  return 0;
}
