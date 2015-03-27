#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

#define BUFSIZE 1024

#define LOG_D(...)  do { } while(0)
#define LOG_E(...)  do { } while(0)
#define LOG_I(...)  do { } while(0)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

uint8_t to_lowcase(char ch)
{
	if (ch >= 'a' && ch <= 'z') return ch;
	else if (ch >= 'A' && ch <= 'Z') return ch - 'A' + 'a';
        else return -1;
}

/* use a function template will be better! */
void swap(int &a, int &b)
{
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}


uint32_t count_line(char *filename)
{
	int lines = 0;
	char buf[BUFSIZE] = { 0 };
  	FILE *fp = NULL;
	fp = fopen(filename, "r");
	if (NULL == fp)
	{
		printf("Open file error! [ %s ]\n", filename);
		return -1;
	}
	while (fgets(buf, BUFSIZE, fp))
	{
		if ( strlen(buf) == BUFSIZE - 1 && 
		     buf[BUFSIZE-2] != '\n')
		{ /* do nothing */}
		else
		{ lines++; }
	}
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
	if ( strlen(buf) == BUFSIZE - 1 && 
	     buf[BUFSIZE-2] != '\n')
		{ /* do nothing */}
		else
		{ newlines++; }	
  }

  printf(" [ %s ]: %d lines\n", argv[1], newlines);

  return 0;
}

