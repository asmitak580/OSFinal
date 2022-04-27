#ifndef _VMM_H_
#define _VMM_H_

#include "stdint.h"

namespace gheith {
    extern uint32_t* make_pd();
    extern void delete_pd(uint32_t*);
    void map(uint32_t* pd, uint32_t va, uint32_t pa);
    void unmap(uint32_t* pd, uint32_t va);
}

namespace VMM {

    // Called (on the initial core) to initialize data structures, etc
    extern void global_init();

    // Called on each core to do per-core initialization
    extern void per_core_init();

    
}

#endif
