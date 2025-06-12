#include "mallocsanitize.h"

static void init_real_functions()
{
    if (!real_malloc)
	{
        real_malloc = dlsym(RTLD_NEXT, "malloc");
        real_free = dlsym(RTLD_NEXT, "free");
    }
}

static void record_alloc(void* ptr, size_t size)
{
    alloc_node_t* node = real_malloc(sizeof(alloc_node_t));
    node->ptr = ptr;
    node->size = size;
    node->next = alloc;
    alloc = node;
}

static void record_free(void* ptr)
{
    alloc_node_t** current = &alloc;
    while (*current)
	{
        if ((*current)->ptr == ptr)
		{
            alloc_node_t* to_remove = *current;
            *current = (*current)->next;
            real_free(to_remove);
            return;
        }
        current = &(*current)->next;
    }
}

void* malloc(size_t size)
{
    init_real_functions();
    void* ptr = real_malloc(size);
    if (ptr) {
        record_alloc(ptr, size);
    }
    return ptr;
}

 void free(void* ptr)
 {
    init_real_functions();
    if (ptr)
	{
        record_free(ptr);
        real_free(ptr);
    }
}

void __attribute__((destructor)) check_leaks()
{
    printf("\n=== LEAK REPORT ===\n");
    alloc_node_t* current = alloc;
    int leak_count = 0;
    
    while (current)
	{
        printf("LEAK: %zu bytes at %p\n", current->size, current->ptr);
        current = current->next;
        leak_count++;
    }
    
    if (leak_count == 0)
        printf("No leaks detected!\n");
	else
        printf("Found %d leaks\n", leak_count);

}