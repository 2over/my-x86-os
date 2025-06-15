#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/string.h"


void kernel_main(void) {
    console_init();
    gdt_init();

//    char *s = "cover22222222222222";
//    console_write(s, strlen(s));
    printk("cover os cool\n");
    while (true);
}