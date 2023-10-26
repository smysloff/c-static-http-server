#include "readfile.h"

static char* die(FILE* fd)
{
  fclose(fd);
  return NULL;
}


char*
readfile(const char* name)
{
  char* content = NULL;

  FILE* fd;
  size_t size;


  /* open file */

  if (!(fd = fopen(name, "r")))
    return NULL;


  /* get file size */

  if (fseek(fd, 0L, SEEK_END)) die(fd);
  if (!(size = ftell(fd))) die(fd);
  rewind(fd);


  /* allocate memory */

  if(!(content = malloc(sizeof(char) * (size + 1))))
    die(fd);


  /* read file */
  
  fread(content, sizeof(char), size, fd);


  /* close file */
  
  fclose(fd);

  return content;
}
