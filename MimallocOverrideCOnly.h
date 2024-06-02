#ifdef LODEPNG_NO_COMPILE_ALLOCATORS
#ifndef NEARESTNEIGHBOURUPSCALE_MIMALLOCOVERRIDECONLY_H
#define NEARESTNEIGHBOURUPSCALE_MIMALLOCOVERRIDECONLY_H
#include <mimalloc.h>
#define malloc(n)                               mi_malloc(n)
#define realloc(p,n)                            mi_realloc(p,n)
#define free(p)                                 mi_free(p)
#define strdup(s)                               mi_strdup(s)
#define realpath(p,q)                           mi_realpath(p,q)
#endif //NEARESTNEIGHBOURUPSCALE_MIMALLOCOVERRIDECONLY_H
#endif