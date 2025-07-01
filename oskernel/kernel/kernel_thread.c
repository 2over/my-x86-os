#include "../include/linux/kernel.h"

void* kernel_thread(void* arg) {
    printk("cover\n");
}