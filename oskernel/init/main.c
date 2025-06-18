#include "../include/linux/tty.h"
#include "../include/linux/kernel.h"
#include "../include/string.h"
#include "../include/linux/mm.h"
#include "../include/linux/task.h"
#include "../include/linux/sched.h"

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

//    virtual_memory_init();
    task_init();

    sched();

    // 测试分配虚拟内存
    void* p = kmalloc(1);
    printk("0x%p\n", p);

    kfree_s(p, 1);
    __asm__ ("sti;");

//    int i = 10/ 0;
    while (true);
}