#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/string.h"


void kernel_main(void) {
    console_init();

    char* s = "cover";
    console_write(s, strlen(s));
    while(true);
}