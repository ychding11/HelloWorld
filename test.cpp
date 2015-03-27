#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

#define BUFSIZE 1024
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

uint8_t to_lowcase(char ch)
{
	if (ch >= 'a' && ch <= 'z') return ch;
	else if (ch >= 'A' && ch <= 'Z') return ch - 'A' + 'a';
        else return -1;
}

uint32_t count_line(char *filename)
{
	int lines = 0;

	return lines;
}

int main(int argc, char** argv)
{
  int newlines = 0;
  char buf[BUFSIZE] = { 0 };
  FILE* file;

  if (argc != 2)
  {
    printf("usage error!\n");
    return 1;
  }

  file = fopen(argv[1], "r");
  if (NULL == file)
  {
  	printf("Open file error! [ %s ]\n", argv[1]);
	return -1;
  }
  while (fgets(buf, BUFSIZE, file))
  {
   if (!(strlen(buf) == BUFSIZE-1 && buf[BUFSIZE-2] != '\n'))
      newlines++;	
  }

  printf("Number of lines in file [ %s ]: %d\n", argv[1], newlines);

  return 0;
}

