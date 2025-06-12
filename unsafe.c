#include <stdlib.h>
int main() {
    malloc(100);
    void* p = malloc(200);
    free(p);
    malloc(50);
    return 0;
}
