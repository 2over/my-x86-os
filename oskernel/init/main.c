#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/string.h"
#include "../include/linux/mm.h"
#include "../include/linux/task.h"
#include "../include/linux/sched.h"
#include "../include/unistd.h"
#include "../include//string.h"
#include "../include/stdio.h"
#include "../include/stdlib.h"

extern void clock_init();

void user_mode() {
//    __asm__("int 0x80");

    int age = 10;
    char* str= "welcome";

    printf("%s, %d\n", str, 11);

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