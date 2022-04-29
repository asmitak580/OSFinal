#include "libc.h"

void one(int fd) {
    printf("*** fd = %d\n",fd);
    printf("*** len = %d\n",len(fd));

    cp(fd,2);
    // printf("done with one method");
}

int main(int argc, char** argv) {
    // typedef unsigned char byte;
    // char *VGA = (char*)0xA0000000L;
    // uint16_t offset = 0;
    // uint32_t x = 0;
    // uint32_t y = 20;
    // for (int i = 0; i < 20; i++) {
    //     offset = x + y * 320;
    //     VGA[offset] = 4;
    //     y += 2;
    // }

    // *((uint32_t*)0xA0000) = 4;

    printf("*** %d\n",argc);
    for (int i=0; i<argc; i++) {
        printf("*** %s\n",argv[i]);
    }
    int fd = open("/etc/data.txt",0);
    one(fd);

    printf("*** close = %d\n",close(fd));

    one(fd);
    one(100);


    printf("*** open again %d\n",open("/etc/data.txt",0));
    printf("*** seek %ld\n",seek(3,17));
    
    int id = fork();
    // printf("fork return id: %d", id);

    if (id < 0) {
        printf("fork failed");
    } else if (id == 0) {
        /* child */
        printf("*** in child\n");
        int rc = execl("/sbin/shell","shell","a","b","c",0);
        printf("*** execl failed, rc = %d\n",rc);
    } else {
        /* parent */
        uint32_t status = 42;
        wait(id,&status);
        printf("*** back from wait %ld\n",status);

        int fd = open("/etc/panic.txt",0);
        cp(fd,1);
    }

    shutdown();
    return 0;
}
