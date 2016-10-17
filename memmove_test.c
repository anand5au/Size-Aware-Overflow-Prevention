#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	int i=0;
	int *arr1 = (int *)malloc(sizeof(int)*3);
	int arr2[5] = {1,2,3,4,5};

	memmove(arr1, arr2, 20);

	for(;i<5;i++)
	{
		printf("%d ",arr1[i]);
	}

	printf("\n");
}
