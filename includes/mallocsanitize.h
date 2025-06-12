//#include  _GNU_SOURCE // now included in makefile
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

static void* (*real_malloc)(size_t) = NULL;
static void (*real_free)(void*) = NULL;

typedef struct alloc_node
{
	void*				ptr;
	size_t				size;
	struct alloc_node*	next;
}	alloc_node_t;

static alloc_node_t* alloc = NULL;

