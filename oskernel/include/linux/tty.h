//
// Created by xiehao on 25-6-13.
//

#ifndef MY_X86_OS_TTY_H
#define MY_X86_OS_TTY_H

#include "types.h"

void console_init(void);

void console_write(char *buf, u32 count);

#endif //MY_X86_OS_TTY_H
