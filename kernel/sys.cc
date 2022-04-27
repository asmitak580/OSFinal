#include "sys.h"
#include "stdint.h"
#include "idt.h"
#include "debug.h"
#include "machine.h"
#include "config.h"
#include "ext2.h"
#include "pcb.h"
#include "threads.h"
#include "physmem.h"
#include "elf.h"
#include "libk.h"


/**
 * QUESTIONS:
 * open()
 *  do we loop through fn to check if each 4 byte range is in user space
 **/

auto d = Shared<Ide>::make(1);
auto fs = Shared<Ext2>::make(d);
// uint32_t* frameGlobal;
uint32_t* user_esp;
uint32_t* user_eip;

//helper methods exit and write

void exit(int rc) {
    // Debug::shutdown();
    using namespace gheith;
    TCB* me = current();
    Shared<PCB> myProc = me->process;
    //child future set(status)
    myProc->futStat->set(rc);
    //child stop TCB 
    stop();
}

ssize_t write(int fd, void* buf, size_t nbytes) { 
    if (fd < 0 || fd > 9) {
        return -1;
    }
    using namespace gheith;
    TCB* me = current();
    Shared<PCB> myProc = me->process;
    if (myProc->openFiles[fd] == nullptr || myProc->openFiles[fd]->terminal == false) {
        return -1;
    }
    char* buff = (char*) buf;
    if ((fd == 1 || fd == 2) && !(((uint32_t) buf >= kConfig.ioAPIC && (uint32_t) buf < kConfig.ioAPIC + 4096) 
        || ((uint32_t) buf >= kConfig.localAPIC && (uint32_t) buf < kConfig.localAPIC + 4096) 
            || (uint32_t) buf < 0x80000000)) {
        for (uint32_t i = 0; i < (uint32_t) nbytes; i++) {
            if ((((uint32_t) (buff + i) >= kConfig.ioAPIC && (uint32_t) (buff + i) < kConfig.ioAPIC + 4096) 
            || ((uint32_t) (buff + i) >= kConfig.localAPIC && (uint32_t) (buff + i) < kConfig.localAPIC + 4096) 
            || (uint32_t) (buff + i) < 0x80000000)) {
                return i;
            }
            Debug::printf("%c", buff[i]);
        }
        return nbytes;
    } else {
        return -1;
    }
}



//File related syscalls
int open(const char* fn, int flags) {
    //while (check for null character '\0') fn[i] == '\0', then check fn+i for the shared space if statement
    if ((((uint32_t) fn >= kConfig.ioAPIC && (uint32_t) fn < kConfig.ioAPIC + 4096) 
        || ((uint32_t) fn >= kConfig.localAPIC && (uint32_t) fn < kConfig.localAPIC + 4096) 
            || (uint32_t) fn < 0x80000000)) {
                return -1;
    }
    using namespace gheith;
    TCB* me = current();
    Shared<PCB> myProc = me->process;
    auto root = fs->root;
    Shared<Node> openFile = fs->find(root, fn); 
    if (openFile != nullptr) {
        int fdIndex = 0;
        while (fdIndex < 10) {
            if (myProc->openFiles[fdIndex] == nullptr) {
                myProc->openFiles[fdIndex] = Shared<FileDescriptor>(new FileDescriptor(openFile));
                Shared<FileDescriptor> myFile = myProc->openFiles[fdIndex];
                myFile->offset = 0;
                myFile->terminal = false;
                // Debug::printf("fd in open: %d\n", fdIndex); //3
                return fdIndex;
                // Debug::printf("after return: %d\n", fdIndex);
            }
            fdIndex++;
        }
    }
    return -1;
}

ssize_t len(int fd) {
    if (fd < 0 || fd > 9) {
        return -1;
    }
    using namespace gheith;
    TCB* me = current();
    Shared<PCB> myProc = me->process;  
    if (myProc->openFiles[fd] != nullptr) {
        if (myProc->openFiles[fd]->terminal) {
            return 0;
        }
        return myProc->openFiles[fd]->file->size_in_bytes();
    }
    return -1;
}

ssize_t read(int fd, void* buf, size_t nbyte) {
    // Debug::printf("in read\n");
    if (fd < 0 || fd > 9) {
        return -1;
    }
     
    if ((((uint32_t) buf >= kConfig.ioAPIC && (uint32_t) buf < kConfig.ioAPIC + 4096) 
        || ((uint32_t) buf >= kConfig.localAPIC && (uint32_t) buf < kConfig.localAPIC + 4096) 
            || (uint32_t) buf < 0x80000000)) {
                return -1;
            }
    using namespace gheith;
    TCB* me = current();
    Shared<PCB> myProc = me->process;
    
    if (myProc->openFiles[fd] != nullptr) {
        Shared<Node> fileNode = myProc->openFiles[fd]->file;
        //myProc->openFiles[fd]->offset > fileNode->size_in_bytes() || 
        if (((fd == 0 || fd == 1 || fd == 2) && myProc->openFiles[fd]->terminal)) {
            return -1;
        }
        // Debug::printf("b4 checking if nbyte is more than size\n");
        if ((fileNode->size_in_bytes() + myProc->openFiles[fd]->offset) < nbyte) {
            nbyte = fileNode->size_in_bytes() - myProc->openFiles[fd]->offset;
        }
        // Debug::printf("before read all\n");
        //filesize - offset < nbytes
        //TODO: handle case if we read less than nbytes
        // Debug::printf("nbyte: %d\n", nbyte);
        uint32_t count = fileNode->read_all(myProc->openFiles[fd]->offset, nbyte, (char*) buf);
        // Debug::printf("count: %d\n", count);
        myProc->openFiles[fd]->offset += count;
        // Debug::printf("count in read: %d\n", count);
        return count;
    }
    return -1;
}

uint32_t seek(int fd, uint32_t offset) { //what is off_t
    if (fd < 0 || fd > 9) {
        return -1;
    }
    using namespace gheith;
    TCB* me = current();
    Shared<PCB> myProc = me->process;
    if (myProc->openFiles[fd] != nullptr && !myProc->openFiles[fd]->terminal) {
        myProc->openFiles[fd]->offset = offset;
        return offset;
    }
    return -1;
}

int close(int id) {
    using namespace gheith;
    TCB* me = current();
    Shared<PCB> myProc = me->process;
    if (id >= 0 && id <= 9) {
        //an open file
        myProc->openFiles[id] = nullptr;
        return 0;
    } else if (id >= 10 && id <= 19) {
        //a child
        myProc->children[id - 10] = nullptr;
        return 0;
    } else if (id >= 20 && id <= 29) {
        //a semaphore
        myProc->sem[id - 20] = nullptr;
        return 0;
    }
    return -1;
}

int sem(uint32_t init) {
    using namespace gheith;
    TCB* me = current();
    Shared<PCB> myProc = me->process;
    Shared<Semaphore> newSem = Shared<Semaphore>::make(init);
    uint32_t i = 0;
    while (i < 10) {
        if (myProc->sem[i] == nullptr) {
            //have an open space
            myProc->sem[i] = newSem;
            return i + 20;
        }
        i++;
    }
    return -1;
}

int up(int id) {
    if (id < 20 || id > 29) {
        return -1;
    }
    using namespace gheith;
    int index = id - 20;
    TCB* me = current();
    Shared<PCB> myProc = me->process;
    if (myProc->sem[index] != nullptr) {
        myProc->sem[index]->up();
        return 0;
    } else {
        return -1;
    }
}

int down(int id) {
    if (id < 20 || id > 29) {
        return -1;
    }
    using namespace gheith;
    int index = id - 20;
    TCB* me = current();
    Shared<PCB> myProc = me->process;
    if (myProc->sem[index] != nullptr) {
        myProc->sem[index]->down();
        return 0;
    } else {
        return -1;
    }
}

int shutdown() {
    Debug::shutdown();
    return 0;
}

int fork(){
    using namespace gheith;
    TCB* me = current();
    Shared<PCB> parent = me->process;
    int index = 0;
    while (index < 10) {
        if (parent->children[index] == nullptr) {
            uint32_t esp = (uint32_t) user_esp;
            uint32_t eip = (uint32_t) user_eip;
            //open space to add child in
            //copy the save area cr3 to child
            TCB* childThread = threadFork([esp, eip] {
                switchToUser(eip, esp, 0);
            });
            parent->children[index] = Shared<PCB>(new PCB());
            Shared<PCB>child = parent->children[index];
            childThread->process = child;
            for (int i = 0; i < 10; i++) {
                child->openFiles[i] = parent->openFiles[i];
                child->sem[i] = parent->sem[i];
            }
            //deep copy virtual memory
            for (int i = 512; i < 1024; i++) {
                if ((me->pd[i] % 2) == 1) {
                    //see if parent has pt then copy pt[j] check present bit
                    uint32_t* pt = (uint32_t*) (me->pd[i] & 0xfffff000);
                    for (int j = 0; j < 1024; j++) {
                        // Debug::printf("fork 4");
                        if ((pt[j] % 2) == 1) {
                            //mapping
                            //va - j * 4096 + (i * (4096 * 1024))
                            uint32_t va = (j * 4096);
                            va += (i << 22);
                            //check if va == apics, if it does then skip
                            if (!((va >= kConfig.ioAPIC && va < kConfig.ioAPIC + 4096) 
                                || (va >= kConfig.localAPIC && va < kConfig.localAPIC + 4096))) {
                                uint32_t pa = PhysMem::alloc_frame();
                                map(childThread->pd, va, pa);
                                memcpy((void*) pa, (void*) va, 4096);
                            }
                        }
                    }
                }
            }
            schedule(childThread);
            return index + 10;
        }
        index++;
    }
    return -1;
}

int wait(int id, uint32_t* status) {
    if (id >= 10 && id <= 19) {
        using namespace gheith;
        TCB* me = current();
        Shared<PCB> parent = me->process;
        *status = parent->children[id-10]->futStat->get();
        // memcpy(status, parent->futStat->get(), 4);
        //parent calls (deref) status = get() 
        return 0;
    }
    return -1;
}

int execl(const char* path, char** args, uint32_t len) {
    // Debug::printf("in execl\n");
    using namespace gheith;
    // Debug::printf("path %s\n", path);
    Shared<Node> file = fs->find(fs->root, path);
    uint32_t entryPoint = ELF::load(file);
    // Debug::printf("path %s\n", path);

    // for (uint32_t i = 0; i < len; i++) {
    //     Debug::printf("string: %d: %s\n", i, args[i]);
    // }
    
    // Debug::printf("loaded file pd\n");


    TCB* me = current();
    Shared<PCB> parent = me->process;
    for (int i = 0; i < 10; i++) {
        //delete children
        parent->children[i] = nullptr;
        //delete semaphores
        parent->sem[i] = nullptr;
    }
    //delete virtual memory
    // delete_pd(me->pd);
    // for (uint32_t i = 512; i < 1024; i++) {
    //     if ((me->pd[i] % 2) == 1) {
    //         uint32_t* pt = (uint32_t*) (me->pd[i] & 0xfffff000);
    //         for (uint32_t j = 0; j < 1024; j++) {
    //             if ((pt[j] % 2) == 1) {
    //                 unmap(me->pd, (j*4096)+(i<<22));
    //             }
    //         }
    //     }
    // }

    

    // Debug::printf("path %s\n", path);

    // for (uint32_t i = 0; i < len; i++) {
    //     Debug::printf("string: %d: %s\n", i, args[i]);
    // }
    
    //setting up the stack
    auto userEsp = (uint32_t*) 0xefffe000;

    // uint32_t* stringPointers = new uint32_t[len];

    //each stack element has to be 4 byte aligned
    for (uint32_t i = 0; i < len; i++) {
        // Debug::printf("stack: %x\n", userEsp);
        char* current = args[i];
        uint32_t size = K::strlen(current) + 1;
        // stringPointers[i] = (uint32_t) userEsp;
        // memcpy((void*)stringPointers[i], (void*)((uint32_t) userEsp), 4);
        while (size % 4 != 0) {
            size++;
        }
        // stringPointers[i] = (uint32_t) userEsp;
        memcpy(userEsp, args[i], size);
        userEsp -= (size/4);
    }
    // Debug::printf("first loop in execl pd\n");
    // Debug::printf("stack: %x\n", userEsp);
    *userEsp = 0; //null
    userEsp--;
    for (int i = len-1; i >= 0; i--) {
        // Debug::printf("stack: %x\n", userEsp);
        // Debug::printf("i: %d", i);
        *userEsp = (uint32_t) args[i];
        //*userEsp = stringPointers[i]
        userEsp--;
    }
    *userEsp = (uint32_t) (userEsp + 1);
    userEsp--;
    // Debug::printf("stack: %x\n", userEsp);
    *userEsp = len;
    // Debug::printf("before switch to user\n");

    switchToUser(entryPoint,(uint32_t) userEsp,0);

    return 0;

}

extern "C" int sysHandler(uint32_t eax, uint32_t *frame) {
    user_esp = (uint32_t*) frame[3];
    user_eip = (uint32_t*) frame[0];
    if (eax == 0) {
        exit(user_esp[1]);
    } else if (eax == 1) {
        return write(user_esp[1], (void*) user_esp[2], user_esp[3]);
    } else if (eax == 2) {
        return fork();
    } else if (eax == 3) {
        return sem(user_esp[1]);
    } else if (eax == 4) {
        return up(user_esp[1]);
    } else if (eax == 5) {
        return down(user_esp[1]);
    } else if (eax == 6) {
        return close(user_esp[1]);
    } else if (eax == 7) {
        shutdown();
    } else if (eax == 8) {
        return wait(user_esp[1], (uint32_t*) user_esp[2]);
    } else if (eax ==  9) {
        int count = 2;
        while (user_esp[count] != 0) {
            count++;
        }
        
        char** args = new char*[count - 2]; //how do i put this in kernel heap memory
        int i = 2;
        while (user_esp[i] != 0) {
            args[i-2] = new char[K::strlen((char*) user_esp[i]) + 1];
            memcpy(args[i-2],(char*) user_esp[i], K::strlen((char*) user_esp[i]) + 1);
            i++;
        }

        
        char* cpyPath = (char*) user_esp[1];

        int index = 0; 
        int charCnt = 0;
        while (cpyPath[index] != 0) {
            charCnt++;
            index++;
        }
        // Debug::printf("path %s\n", (char*) user_esp[1]);
        char* path = new char[charCnt+1];
        // Debug::printf("in execl sys handle\n");
        // for (int i = 0; i < K::strlen(cpyPath); i++) {
        //     memcpy((void*)path[i], (void*)cpyPath[i], 1);
        // }
        
        memcpy(path, (char*) user_esp[1], charCnt+1);
        path[charCnt] = 0;
        return execl(path, args, (uint32_t) count - 2); //how do I put all the arguments in this
    } else if (eax == 10) {
        return open((char*) user_esp[1], user_esp[2]);
    } else if (eax == 11) {
        return len(user_esp[1]);
    } else if (eax == 12) {
        return read(user_esp[1], (void*) user_esp[2], user_esp[3]);
    } else if (eax == 13) {
        return seek(user_esp[1], user_esp[2]);
    }
    return 0;
}


void SYS::init(void) {
    IDT::trap(48,(uint32_t)sysHandler_,3);
}