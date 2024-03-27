#include "kernel/types.h"
#include "user/user.h"

// Add a system call to xv6 that returns the amount of free memory available.

int main(int argc, char *args[]) {
    fprintf(1, "Free mem: %d bytes\n", freemem());
    exit(1);
}
