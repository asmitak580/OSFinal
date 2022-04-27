#include "elf.h"
#include "debug.h"

uint32_t ELF::load(Shared<Node> file) {
     // MISSING();
    ElfHeader* header = new ElfHeader();
    //read in the elf header
    file->read_all(0, 52, (char *) header);
    //verify that ELF starts with magic numbers
    if ((header->maigc0 == 0x7F) && (header->magic1 == (unsigned char)'E') 
        && (header->magic2 == (unsigned char)'L') && (header->magic3 == (unsigned char)'F')) {
        uint32_t startOfProgramTable = header->phoff;
        for (uint16_t i = 0; i < header->phnum; i++) {
            ProgramHeader* programHead = new ProgramHeader();
            //read in the program headers
            file->read_all(startOfProgramTable + i * header->phentsize, sizeof(ProgramHeader), (char *) programHead);
            if (programHead->type == 1) {
               file->read_all(programHead->offset,programHead->filesz, (char*) programHead->vaddr); 
            }
        }
        return header->entry;
    } else {
        Debug::panic("");
    }
   
    return 0;
}


















































#if 0
    ElfHeader hdr;

    file->read(0,hdr);

    uint32_t hoff = hdr.phoff;

    for (uint32_t i=0; i<hdr.phnum; i++) {
        ProgramHeader phdr;
        file->read(hoff,phdr);
        hoff += hdr.phentsize;

        if (phdr.type == 1) {
            char *p = (char*) phdr.vaddr;
            uint32_t memsz = phdr.memsz;
            uint32_t filesz = phdr.filesz;

            Debug::printf("vaddr:%x memsz:0x%x filesz:0x%x fileoff:%x\n",
                p,memsz,filesz,phdr.offset);
            file->read_all(phdr.offset,filesz,p);
            bzero(p + filesz, memsz - filesz);
        }
    }

    return hdr.entry;
#endif
