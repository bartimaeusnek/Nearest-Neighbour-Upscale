#ifdef LODEPNG_NO_COMPILE_ALLOCATORS
#include <mimalloc.h>
void* lodepng_malloc(size_t size){
    return mi_malloc(size);
}
void* lodepng_realloc(void* ptr, size_t new_size){
    return mi_realloc(ptr, new_size);
}
void lodepng_free(void* ptr){
    mi_free(ptr);
}
#endif