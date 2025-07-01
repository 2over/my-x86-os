#include "../include/linux/kernel.h"

void* kernel_thread(void* arg) {
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
                if (((v & 0xFFFF) != 0xFFFF) && (v != 0)) {
                    printk("bus: %02d dev: %02d fun: %02d\n", bus, dev, fun);
                }
            }
        }
    }
}