#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/string.h"
#include "../include/linux/mm.h"
#include "../include/linux/task.h"
#include "../include/linux/sched.h"

extern void clock_init();

void user_mode() {
    int age = 10;
    while(true);
}
void kernel_main(void) {
    console_init();
    gdt_init();
    idt_init();
    clock_init();

    print_check_memory_info();
    memory_init();
    memory_map_int();

    task_init();

    __asm__("sti;");

    while (true);
}
//void kernel_main(void) {
//    console_init();
//    gdt_init();
//    idt_init();
//    clock_init();
//    printk("cover os cool\n");
//
//    print_check_memory_info();
//
//    memory_init();
//    memory_map_int();
//
//    task_init();
//
//    __asm__ ("sti;");
//
//    while (true);
//}