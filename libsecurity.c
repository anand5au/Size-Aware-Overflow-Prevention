#define _GNU_SOURCE
#include <dlfcn.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Heap
{
	void* ptrVariable;
	size_t size;
	struct Heap* next;
};

typedef struct Heap heap;
heap *head = NULL;
heap *end = NULL;

/******* memory allocation functions **************/
static void* (*real_malloc)(size_t) = NULL;
static void* (*real_calloc)(size_t, size_t) = NULL;
static void* (*real_realloc)(void*, size_t) = NULL;
static void (*real_free)(void*) = NULL;

/******* IO functions **************/
static char* (*real_gets)(char*) = NULL;
static char* (*real_fgets)(char*, int, FILE*) = NULL;

/******* string functions **************/
// static char* (*real_strcpy)(char*, const char*) = NULL;
static char* (*real_strncpy)(char*, const char*, size_t) = NULL;
// static char* (*real_strcat)(char*, const char*) = NULL;
static char* (*real_strncat)(char*, const char*, size_t) = NULL;

static void* (*real_memcpy)(void*, const void*, size_t) = NULL;
static void* (*real_memmove)(void*, const void*, size_t) = NULL;

void addtolist(void* temp, size_t s)
{
	real_malloc = dlsym(RTLD_NEXT, "malloc");
	heap *t = (heap*) real_malloc(sizeof(heap));
	t->ptrVariable = temp;
	t->size = s;
	t->next = NULL;
	if (head == NULL)
	{
		head = t;
		end = t;
	}
	else
	{
		end->next = t;
		end = t;
	}
}

void deletefromlist(void *ptr)
{
	real_free = dlsym(RTLD_NEXT, "free");
	heap *iterator = head;
	heap *prevnode = NULL;
	if (head == NULL)
	{
		return;
	}
	else if (iterator->ptrVariable == ptr)
	{
		iterator = head;
		head = head->next;
	}
	else
	{
		prevnode = iterator;
		iterator = iterator->next;
		while (iterator != NULL)
		{
			if (iterator->ptrVariable == ptr)
			{
				break;
			}
			prevnode = iterator;
			iterator = iterator->next;
		}
		prevnode->next = iterator->next;
		if (prevnode->next == NULL)
		{
			end = prevnode;
		}
	}
	real_free(iterator);
	return;
}

heap* findInList(void *node)
{
	heap *iterator = head;
	while (iterator != NULL)
	{
		// check if the source pointer is within the allocated memory
		if(node >= iterator->ptrVariable && node < (iterator->ptrVariable + iterator->size))
			break;
		iterator = iterator->next;
	}
	return iterator;
}

void* malloc(size_t size)
{
	real_malloc = dlsym(RTLD_NEXT, "malloc");
	void *temp = real_malloc(size);
	addtolist(temp, size);
	return temp;
}

void* realloc(void *ptr, size_t size)
{
	real_realloc = dlsym(RTLD_NEXT, "realloc");
	void *temp = real_realloc(ptr, size);
	addtolist(temp, size);
	return temp;
}

void* calloc(size_t count, size_t size)
{
	real_calloc = dlsym(RTLD_NEXT, "calloc");
	void *temp = real_calloc(count, size);
	addtolist(temp, count*size);
	return temp;
}

void free(void *ptr)
{
	real_free = dlsym(RTLD_NEXT, "free");
	real_free(ptr);
	deletefromlist(ptr);
	return;
}

char *strcpy(char* s1, const char* s2)
{
	heap* node = findInList(s1);
	size_t len = strlen(s2);
	if(node == NULL)
	{
		printf("Copying in a stack based buffer.\n");
	}
	// check if we have sufficient space for source buffer and null character
	else if (((node->ptrVariable + node->size) - (void *)s1 - 1) < strlen(s2))
	{
		printf("Buffer Overflow detected in the function \"strcpy\"!\n");
		printf("Copying only what destination buffer can hold.\n");
		len = ((node->ptrVariable + node->size) - (void *)s1 - 1);
	}
	real_strncpy = dlsym(RTLD_NEXT, "strncpy");
	s1 = real_strncpy(s1, s2, len);
	s1[len] = '\0';
	return s1;
}

char *strncpy(char* s1, const char* s2, size_t n)
{
	heap* node = findInList(s1);
	size_t len = n;
	if(node == NULL)
	{
		printf("Copying in a stack based buffer.\n");
	}
	// check if we have sufficient space for source buffer and null character
	else if (((node->ptrVariable + node->size) - (void *)s1 - 1) < n)
	{
		printf("Buffer Overflow detected in the function \"strncpy\"!\n");
		printf("Copying only what destination buffer can hold.\n");
		len = ((node->ptrVariable + node->size) - (void *)s1 - 1);
	}
	real_strncpy = dlsym(RTLD_NEXT, "strncpy");
	return real_strncpy(s1, s2, len);
}

char *strcat(char* s1, const char* s2)
{
	heap* node = findInList(s1);
	size_t old_dest_len = strlen(s1);
	size_t len = strlen(s2);
	if(node == NULL)
	{
		printf("Copying in a stack based buffer.\n");
	}
	// check if we have sufficient space for source buffer and null character
	else if (((node->ptrVariable + node->size) - (void *)s1 - old_dest_len - 1) < strlen(s2))
	{
		printf("Buffer Overflow detected in the function \"strcat\"!\n");
		printf("Appending only what destination buffer can hold.\n");
		len = ((node->ptrVariable + node->size) - (void *)s1 - old_dest_len - 1);
	}
	real_strncat = dlsym(RTLD_NEXT, "strncat");
	s1 = real_strncat(s1, s2, len);
	s1[old_dest_len + len] = '\0';
	return s1;
}

char *strncat(char* s1, const char* s2, size_t n)
{
	heap* node = findInList(s1);
	size_t old_dest_len = strlen(s1);
	size_t len = n;
	if(node == NULL)
	{
		printf("Copying in a stack based buffer.\n");
	}
	// check if we have sufficient space for source buffer and null character
	else if (((node->ptrVariable + node->size) - (void *)s1 - old_dest_len - 1) < n)
	{
		printf("Buffer Overflow detected in the function \"strncat\"!\n");
		printf("Appending only what destination buffer can hold.\n");
		len = ((node->ptrVariable + node->size) - (void *)s1 - old_dest_len - 1);
	}
	real_strncat = dlsym(RTLD_NEXT, "strncat");
	return real_strncat(s1, s2, len);
}

char *gets(char *s)
{
	heap* node = findInList(s);
	int len = 0;
	if(node == NULL)
	{
		real_gets = dlsym(RTLD_NEXT, "gets");
		return real_gets(s);
	}
	else
	{
		len = ((node->ptrVariable + node->size) - (void *)s);
		real_fgets = dlsym(RTLD_NEXT, "fgets");
		return real_fgets(s, len, stdin);
	}
}

char *fgets(char *s, int n, FILE *stream)
{
	heap* node = findInList(s);
	int len = n;
	if(node)
		len = ((node->ptrVariable + node->size) - (void *)s);

	real_fgets = dlsym(RTLD_NEXT, "fgets");
	return real_fgets(s, len, stream);
}

void *memcpy(void *s1, const void *s2, size_t n)
{
	heap* node = findInList(s1);
	size_t len = n;
	if(node == NULL)
	{
		printf("Copying in a stack based buffer.\n");
	}
	// check if we have sufficient space for source buffer and null character
	else if (((node->ptrVariable + node->size) - (void *)s1 - 1) < n)
	{
		printf("Buffer Overflow detected in the function \"memcpy\"!\n");
		printf("Copying only what destination buffer can hold.\n");
		len = ((node->ptrVariable + node->size) - (void *)s1 - 1);
	}
	real_memcpy = dlsym(RTLD_NEXT, "memcpy");
	return real_memcpy(s1, s2, len);
}

void *memmove(void *s1, const void *s2, size_t n)
{
	heap* node = findInList(s1);
	size_t len = n;
	if(node == NULL)
	{
		printf("Copying in a stack based buffer.\n");
	}
	// check if we have sufficient space for source buffer and null character
	else if (((node->ptrVariable + node->size) - (void *)s1 - 1) < n)
	{
		printf("Buffer Overflow detected in the function \"memmove\"!\n");
		printf("Moving only what destination buffer can hold.\n");
		len = ((node->ptrVariable + node->size) - (void *)s1 - 1);
	}
	real_memmove = dlsym(RTLD_NEXT, "memmove");
	return real_memmove(s1, s2, len);
}
