//
// Created by xiehao on 25-6-13.
//

#ifndef MY_X86_OS_KERNEL_H
#define MY_X86_OS_KERNEL_H

#include "stdarg.h"

int vsprintf(char *buf, const char *fmt, va_list args);

int printk(const char* fmt);

#endif //MY_X86_OS_KERNEL_H
