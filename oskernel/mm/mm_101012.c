#include "../include/asm/system.h"
#include "../include/linux/kernel.h"
#include "../include/linux/mm.h"
#include "../include/string.h"

void virtual_memory_init() {
    int* pdt = (int*)get_free_page();

    // 清零
    memset(pdt, 0, PAGE_SIZE);

    for (int i = 0; i < 4; ++i) {
        // pdt里面的每项，即pde, 内容是ptt + 尾12位的权限位
        int ptt = (int)get_free_page() + (i * 0x1000);
        int pde = 0b00000000000000000000000000000111 | ptt;

        pdt[i] = pde;

        int* ptt_arr = (int*)ptt;

        if (0 == i) {
            // 第一块映射区，给内核用
            for (int j = 0; j < 0x400; ++j) {
                int* item = &ptt_arr[j];
                int virtual_addr = j * 0x1000;
                *item = 0b00000000000000000000000000000111 | virtual_addr;
            }
        } else {
            for (int j = 0; j < 0x400; ++j) {
                int* item = &ptt_arr[j];

                int virtual_addr = j * 0x1000;
                virtual_addr + virtual_addr + i * 0x400 * 0x1000;
            }
        }
    }


    BOCHS_DEBUG_MAGIC

    set_cr3((uint)pdt);

    enable_page();

    BOCHS_DEBUG_MAGIC
}