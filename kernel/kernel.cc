#include "debug.h"
#include "ide.h"
#include "ext2.h"
#include "elf.h"
#include "machine.h"
#include "libk.h"
#include "config.h"

Shared<Node> checkFile(const char* name, Shared<Node> node) {
    // CHECK(node != nullptr);
    // CHECK(node->is_file());
    Debug::printf("file %s is ok\n",name);
    return node;
}

Shared<Node> getFile(Shared<Ext2> fs, Shared<Node> node, const char* name) {
    return checkFile(name,fs->find(node,name));
}

Shared<Node> checkDir(const char* name, Shared<Node> node) {
    Debug::printf("checking %s\n",name);
    // CHECK (node != nullptr);
    // CHECK (node->is_dir());
    Debug::printf("directory %s is ok\n",name);
    return node;
}

Shared<Node> getDir(Shared<Ext2> fs, Shared<Node> node, const char* name) {
    return checkDir(name,fs->find(node,name));
}

void kernelMain(void) {
    auto d = Shared<Ide>::make(1);
    Debug::printf("mounting drive 1\n");
    auto fs = Shared<Ext2>::make(d);
    auto root = checkDir("/",fs->root);
    auto sbin = getDir(fs,root,"sbin");
    auto init = getFile(fs,sbin,"init");

    Debug::printf("loading init\n");
    uint32_t e = ELF::load(init);
    Debug::printf("entry %x\n",e);
    // auto userEsp = K::min(kConfig.ioAPIC,kConfig.localAPIC);
    auto userEsp = (uint32_t*) 0xefffe000;
    Debug::printf("user esp %x\n",userEsp);
    // Current state:
    //     - %eip points somewhere in the middle of kernelMain
    //     - %cs contains kernelCS (CPL = 0)
    //     - %esp points in the middle of the thread's kernel stack
    //     - %ss contains kernelSS
    //     - %eflags has IF=1
    // Desired state:
    //     - %eip points at e
    //     - %cs contains userCS (CPL = 3)
    //     - %eflags continues to have IF=1
    //     - %esp points to the bottom of the user stack
    //     - %ss contain userSS
    // User mode will never "return" from switchToUser. It will
    // enter the kernel through interrupts, exceptions, and system
    // calls.
    // userEsp = '0';
    userEsp[0] = 0;
    userEsp--;
    uint32_t stringPoint = (uint32_t) userEsp;
    memcpy(userEsp, "init", 4);
    userEsp--;
    *userEsp = 0;
    userEsp--;
    *userEsp = stringPoint;
    userEsp--;
    *userEsp = (uint32_t) (userEsp + 1);
    userEsp--;
    *userEsp = 1;
    // userEsp = (void*) userEsp - 4;
    // const char* initString = "init";
    // char** argv;
    // argv[0] = initString;
    // userEsp[1] = (uint32_t) userEsp + 2;
    // memcpy((void*) 0xfee01000, initString, 4);
    // userEsp[2] = 0xfee01000;
    //for execl userEsp[3] = 0xfee01000 + size of first string, userEsp[4] = 0xfee01000 + size of first string + second string and etc.
    //separat address space string 0xfee01000
    //userEsp[2] = 0xfee...
    // userEsp = argv;
    // for (int i = 0; i < 10; i++) {
    //     Debug::printf("stack: %x\n", userEsp[i]);
    // }
    switchToUser(e,(uint32_t) userEsp,0);
    Debug::panic("implement switchToUser in machine.S\n");
}

