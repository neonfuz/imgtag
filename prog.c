#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

#define warn(format, ...) fprintf(stderr, format, ##__VA_ARGS__)

int main(int argc, char **argv)
{
  if (argc < 2)
    err(1, "Error, please specify files as arguments.\n");

  int arg;
  for(arg=1; arg<argc; ++arg) {
    int fildes = open(argv[arg], O_RDONLY);
    if (fildes < 0)
      err(2, "Error opening \"%s\".\n", argv[arg]);
    
    struct stat st;
    
    if (fstat(fildes, &st) < 0)
      err(3, "Error stating file \"%s\"\n", argv[arg]);
    
    char *file = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fildes, 0);
    
    if (file == MAP_FAILED)
      err(4, "Error mapping file \"%s\" to memory\n", argv[arg]);
    
    //    printf("%s: 0x%" PRIx32 "\n", argv[arg], crc32(0, file, st.st_size));
    printf("insert into test (filename, crc32) values(\"%s\", %"PRIu32");\n", argv[arg], crc32(0, file, st.st_size));

    if(munmap(file, st.st_size) != 0)
      warn("Warning: couldn't unmap memory for file \"%s\"\n", argv[arg]);
    if(close(fildes) != 0)
      warn("Warning: couldn't close file \"%s\"\n", argv[arg]);
  }

  return 0;
}
