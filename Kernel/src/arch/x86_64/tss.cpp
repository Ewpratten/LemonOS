#include <tss.h>

#include <string.h>
#include <paging.h>
#include <physicalallocator.h>

extern "C"
void LoadTSS(uint64_t address);

namespace TSS
{
    tss_t tss __attribute__((aligned(16)));

    void Initialize(){

        LoadTSS((uintptr_t)&tss);

        memset(&tss, 0, sizeof(tss_t));
        
        // Set up Interrupt Stack Tables
        tss.ist1 = (uint64_t)Memory::KernelAllocate4KPages(1);
        tss.ist2 = (uint64_t)Memory::KernelAllocate4KPages(1);
        tss.ist3 = (uint64_t)Memory::KernelAllocate4KPages(1);

        Memory::KernelMapVirtualMemory4K(Memory::AllocatePhysicalMemoryBlock(), tss.ist1, 1);
        Memory::KernelMapVirtualMemory4K(Memory::AllocatePhysicalMemoryBlock(), tss.ist2, 1);
        Memory::KernelMapVirtualMemory4K(Memory::AllocatePhysicalMemoryBlock(), tss.ist3, 1);

        memset((void*)tss.ist1, 0, PAGE_SIZE_4K);
        memset((void*)tss.ist2, 0, PAGE_SIZE_4K);
        memset((void*)tss.ist3, 0, PAGE_SIZE_4K);

        asm("mov %%rsp, %0" : "=r"(tss.rsp0));
            
        asm volatile("ltr %%ax" :: "a"(0x2B));

    }

    void SetKernelStack(uint64_t stack){
        tss.rsp0 = stack;
    }
}