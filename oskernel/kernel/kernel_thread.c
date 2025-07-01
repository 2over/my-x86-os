#include "../include/linux/kernel.h"
#include "../include/asm/io.h"

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
                        out_32(0xCF8, addr | (3 << 2));
                        uint header_type = (in_32(0xCFC) & 0x00ff0000) >> 16;
                        printk("vID: %04x dID: %04x, header_type: %04x\n",
                               (v & 0xFFFF), (v & 0xFFFF0000) >> 16, header_type);

                        out_32(0xCF8, addr | (4 << 2));
                        ba0 = in_32(0xCFC);

                        break;
                    }
                }
            }
        }
    }

    return ba0 & 0xffffff00;
}

/**
 * 用于配置CR寄存器中的高两位，即控制Page
 * @param page: 取值: 0, 1, 2, 3
 */
void set_cr_page(uchar page) {
    int ba0 = get_network_card_ba0();

    if (page > 3) return;

    uchar cr = in_byte(ba0);

    printk("old cr0 = %x\n", cr);

    cr &= 0x3f;
    cr |= page << 6;

    printk("new cr0 = %x\n", cr);
    out_byte(ba0, cr);
}


/**
 * 初始化网卡
 */
void network_card_init() {
    unsigned int base_addr = get_network_card_ba0();


    // 向CR寄存器写入: 00 100 0 01 表示: Page0, 网卡停止接收数据包
    out_byte(base_addr, 0x21);

    // 配置网卡相关寄存器, 我们只获取Mac地址，不发送/接收数据，不配置任何寄存器

    // 初始化完成，开启网卡
    // 向CR寄存器写入: 00 100 010, 表示:Page0, 网卡停止接收数据包
    out_byte(base_addr, 0x22);
}

void get_network_card_mac() {
    int ba0 = get_network_card_ba0();

    set_cr_page(0);

    // 从dma的0x0000开始读取数据，因为mac地址在其前12个字节，所以从0x0000开始读取
    out_byte(ba0 + 9, 0); // RSAR1: the high address of dma read
    out_byte(ba0 + 8, 0); // RSAR0: the low address of dma read

    // 一共读取0x000c个字节
    out_byte(ba0 + 0x0b, 0); // RBCR1: the high bits of read count
    out_byte(ba0 + 0xa, 0x0c); // RBCR0: the low bits of read count

    // 向CR寄存器写入: 00 001 0 10, 表示Page0, Remote Read, Start Command
    out_byte(ba0, 0x0a);
}


void* kernel_thread(void* arg) {
    network_card_init();

    get_network_card_mac();

    int ba0 = get_network_card_ba0();

    printk("\n");
    printk("Mac: ");

    for (int i = 0; i < 6; i++) {
        printk("%02x ", in_byte(ba0 + 0x10));

        in_byte(ba0 + 0x10);
    }

    printk("\n\n");
}