#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/string.h"


void kernel_main(void) {
    console_init();
    gdt_init();
    idt_init();
    printk("cover os cool\n");
    __asm__ ("sti;");

//    int i = 10/ 0;
    while (true);
}