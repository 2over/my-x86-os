#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/string.h"
#include "../include/linux/mm.h"

extern void clock_init();

void kernel_main(void) {
    console_init();
    gdt_init();
    idt_init();
    clock_init();
    printk("cover os cool\n");

    print_check_memory_info();

    memory_init();
    memory_map_int();

    virtual_memory_init();
    __asm__ ("sti;");

//    int i = 10/ 0;
    while (true);
}