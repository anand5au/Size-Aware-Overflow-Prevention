#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
   char* username;
   if (argc != 2)
   {
	  fprintf(stderr, "Usage: %s <name>\n", argv[0]);
	  return -1;
   }
   username = malloc(10);

   strncpy(username, argv[1], 15);
   printf("Hello %s\n", username);

   free(username);
   return 0;
}
