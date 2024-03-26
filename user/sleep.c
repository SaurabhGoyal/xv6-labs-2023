#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *args[]) {
    if(argc < 2) {
        fprintf(2, "atleast 1 arg needed\n");
        exit(-1);
    }
    sleep(atoi(args[1]));
    exit(1);
}
