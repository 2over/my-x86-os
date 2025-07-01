#include "../include/linux/kernel.h"

/**
 * 获取网卡的base address 0
 * 只获取第一块网卡的数据，暂时不支持多块网卡
 */
int get_network_card_ba0() {
    int ba0 = 0;
    int bus_max = 255;
    int dev_max = 32;
    int func_max = 7;

    for (int bus = 0; bus < bus_max; ++bus) {
        for (int dev = 0; dev < dev_max; ++dev) {
            for (int fun = 0; fun < func_max; ++fun) {
                int addr = 0x80000000 | (bus << 16) | (dev << 11) | (fun << 8) | (0 << 2);

                out_32(0xCF8, addr);

                int v = in_32(0xCFC);

                // 返回结果不是01xffff, 就说明那个位置有设备存在
                if (v != 0 && (v & 0xFFFF) != 0xFFFF) {

                    out_32(0xCF8, addr | (2 << 2));

                    int class_code = (in_32(0xCFC) & 0xff000000) >> 24;

                    // class code为2表示是网卡设备
                    if (2 == class_code) {
                        out_32(0xCF8, addr | (4 << 2));
                        ba0 = in_32(0xCFC);

                        break;
                    }
                }
            }
        }
    }

    return ba0;
}
void* kernel_thread(void* arg) {
    int ba0 = get_network_card_ba0();
    printk("%x\n", ba0);
}