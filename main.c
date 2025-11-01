// #include "tests.h"
#include "handle_args.h"

int main(int argc, char** argv) {
 
    if(argc < 2) {
        printf("missing arguments\n");
        return -1;
    }
    
    int command = find_command(argv[COMMAND]);
    return handlers[command](argc, argv);

    /* testes disponíveis em tests.h */
    // test1();
    // testImage();
    // testPacket();
}

