#ifndef _pcb_h_
#define _pcb_h

#include "semaphore.h"
#include "ext2.h"
#include "future.h"

struct FileDescriptor {
    
    Shared<Node> file;
    uint32_t offset;
    bool terminal;
    
    Atomic<uint32_t> ref_count{0};
    
public: 
    FileDescriptor(Shared<Node> node) {
        file = node;
    }

    friend class Shared<FileDescriptor>;
};

struct PCB {

    
    //file descriptor fd (index), make a struct, shared pointer to node, offset
    Shared<FileDescriptor> openFiles[10]; //0-9
    //child process
    Shared<PCB> children[10]; //10-19
    //sem process
    Shared<Semaphore> sem[10];  //20-29
    //future
    Future<uint32_t>* futStat = new Future<uint32_t>();
    //Shared<Future<uint32_t>> futStat = Shared<Future<uint32_t>>::make();
    //pid map to children 
    uint32_t pid; //10-19

    Atomic<uint32_t> ref_count{0};

public:

    PCB() {
        for (int i = 0; i < 10; i++) {
            if (i == 0 || i == 1 || i == 2) {
                openFiles[i] = new FileDescriptor(Shared<Node> {});
                openFiles[i]->terminal = true;
            } else {
                openFiles[i] = nullptr;
            }
            // openFiles[i]->terminal = false;
            // openFiles[i]->offset = 0;
            
            children[i] = nullptr;
            sem[i] = nullptr;
        }
    }

    friend class Shared<PCB>;
};

#endif