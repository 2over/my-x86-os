//
// Created by xiehao on 25-6-14.
//

#ifndef MY_X86_OS_TRAPS_H
#define MY_X86_OS_TRAPS_H

#include "head.h"

void gdt_init();
void idt_init();

void send_eoi(int idt_index);

void write_xdt_ptr(xdt_ptr_t* p, short limit, int base);
#endif //MY_X86_OS_TRAPS_H
