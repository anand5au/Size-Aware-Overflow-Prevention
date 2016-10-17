#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
   char* username;
   if (argc != 3)
   {
	  fprintf(stderr, "Usage: %s <fname> <lname>\n", argv[0]);
	  return -1;
   }
   username = malloc(10);

   strcpy(username, argv[1]);
   strcpy(username + strlen(username), argv[2]);
   printf("Hello %s\n", username);

   free(username);
   return 0;
}
