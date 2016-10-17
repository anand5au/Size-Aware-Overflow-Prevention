#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
   char* username;
   int len;
   if (argc != 2)
   {
	  fprintf(stderr, "Usage: %s <name>\n", argv[0]);
	  return -1;
   }
   username = malloc(16);
   
   len = strlen(argv[1]);

   strcpy(username,"Hello ");
   strncat(username, argv[1], 15);

   printf("%s\n", username);

   free(username);
   return 0;
}
