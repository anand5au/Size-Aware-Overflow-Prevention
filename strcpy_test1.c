#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

struct st
{
	char buffer[SIZE];
	char filename[SIZE];
};

int main(int argc, char *argv[])
{
	struct st* s = (struct st *) malloc(sizeof(struct st));
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s <filename> <buffer>\n", argv[0]);
		return -1;
	}
	strcpy(s->filename, argv[1]);
	strcpy(s->buffer, argv[2]);
	printf("Executing filename %s\n", s->filename);
	execlp(s->filename, s->filename, s->buffer, (char *) 0);
	return 0;
}
