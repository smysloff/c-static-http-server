#include "readfile.h"

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

  if (fseek(fd, 0L, SEEK_END))
  {
    fclose(fd);
    return NULL;
  }

  if (!(size = ftell(fd)))
  {
    fclose(fd);
    return NULL;
  }
  
  rewind(fd);


  /* allocate memory */

  if(!(content = malloc(sizeof(char) * (size + 1))))
  {
    fclose(fd);
    return NULL;
  }


  /* read file */
  
  fread(content, sizeof(char), size, fd);


  /* close file */
  
  fclose(fd);

  return content;
}
