#include "vmm.h"
#include "machine.h"
#include "idt.h"
#include "libk.h"
#include "blocking_lock.h"
#include "config.h"
#include "threads.h"
#include "debug.h"
#include "ext2.h"
#include "physmem.h"


namespace gheith {

    using namespace PhysMem;

    uint32_t* shared = nullptr;

    void map(uint32_t* pd, uint32_t va, uint32_t pa) {
        auto pdi = va >> 22;
        auto pti = (va >> 12) & 0x3FF;
        auto pde = pd[pdi];
        if ((pde & 1) == 0) {
            pde = PhysMem::alloc_frame() | 7;
            pd[pdi] = pde;
        }
        auto pt = (uint32_t*) (pde & 0xFFFFF000);
        pt[pti] = pa | 7;
    }

    void unmap(uint32_t* pd, uint32_t va) {
        auto pdi = va >> 22;
        auto pti = (va >> 12) & 0x3FF;
        auto pde = pd[pdi];
        if ((pde & 1) == 0) return;
        auto pt = (uint32_t*) (pd[pdi] & 0xFFFFF000);
        auto pte = pt[pti];
        if ((pte & 1) == 0) return;
        auto pa = pte & 0xFFFFF000;
        pt[pti] = 0;
        dealloc_frame(pa);
        invlpg(va);
    }

    uint32_t* make_pd() {
        auto pd = (uint32_t*) PhysMem::alloc_frame();

        auto m4 = 4 * 1024 * 1024;
        auto shared_size = 4 * (((kConfig.memSize + m4 - 1) / m4));

        memcpy(pd,shared,shared_size);

        map(pd,kConfig.ioAPIC,kConfig.ioAPIC);
        map(pd,kConfig.localAPIC,kConfig.localAPIC);

        return pd;
    }

    void delete_pd(uint32_t* pd) {
        for (unsigned i=512; i<1024; i++) {
            auto pde = pd[i];
            if (pde & 1) dealloc_frame(pde & 0xFFFFF000);
        }
        PhysMem::dealloc_frame((uint32_t)pd);
    }
}

namespace VMM {

void global_init() {
    using namespace gheith;
    shared = (uint32_t*) PhysMem::alloc_frame();

    for (uint32_t va = FRAME_SIZE; va < kConfig.memSize; va += FRAME_SIZE) {
        map(shared,va,va);
    }


}

void per_core_init() {
    using namespace gheith;

    Interrupts::protect([] {
        ASSERT(Interrupts::isDisabled());
        auto me = activeThreads[SMP::me()];
        vmm_on((uint32_t)me->pd);
    });
}

} /* namespace vmm */

extern "C" void vmm_pageFault(uintptr_t va_, uintptr_t *saveState) {
    using namespace gheith;
    auto me = current();
    ASSERT((uint32_t)me->pd == getCR3());
    ASSERT(me->saveArea.cr3 == getCR3());

    

    uint32_t va = PhysMem::framedown(va_);

    if (va >= 0x80000000) {
        auto pa = PhysMem::alloc_frame();
        map(me->pd,va,pa);
        return;
    }

    Debug::panic("*** can't handle page fault at %x\n",va_);
}
