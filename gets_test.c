#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
   char* username;
   username = malloc(10);

   gets(username);
   printf("Hello %s\n", username);

   free(username);
   return 0;
}


