#include "../include/linux/mm.h"
#include "../include/linux/kernel.h"

#define ARDS_ADDR   0x1100
// 把1M以下内存称为无效内存
#define VALID_MEMORY_FROM   0x100000
#define ZONE_VALID 1    // ARDS可用区域
#define ZONE_RESERVED 2 // ARDS不可用区域

physics_memory_info_t g_physics_memory;
physics_memory_map_t g_physics_memory_map;

void memory_init() {
    check_memory_info_t* p = (check_memory_info_t*)ARDS_ADDR;
    check_memory_item_t* p_data = (check_memory_item_t*)(ARDS_ADDR + 2);

    for (int i = 0; i < p->times; ++i) {
        check_memory_item_t* tmp = p_data + i;

        if (tmp->base_addr_low > 0 && tmp->type == ZONE_VALID) {
            g_physics_memory.addr_start = tmp->base_addr_low;
            g_physics_memory.valid_mem_size = tmp->length_low;
            g_physics_memory.addr_end = tmp->base_addr_low + tmp->length_low;
        }
    }

    if (VALID_MEMORY_FROM != g_physics_memory.addr_start) {
        printk("[%s:%d] no valid physiscs memory\n", __FILE__, __LINE__);
        return;
    }

//    g_physics_memory.pages_total = g_physics_memory.addr_end >> 12;
    g_physics_memory.pages_total = (g_physics_memory.addr_end - g_physics_memory.addr_start) >> 12;
    g_physics_memory.pages_used = 0;
    g_physics_memory.pages_free = g_physics_memory.pages_total - g_physics_memory.pages_used;
}

void memory_map_int() {
    // 验证
    if (VALID_MEMORY_FROM != g_physics_memory.addr_start) {
        printk("[%s:%d] no valid physics memory\n", __FILE__, __LINE__);
        return;
    }

    g_physics_memory_map.addr_base = (uint)VALID_MEMORY_FROM;
    g_physics_memory_map.map =(uchar*)0x10000;

    // 共有这么多物理页可用
    g_physics_memory_map.pages_total = g_physics_memory.pages_total;

    // 清零
    memset(g_physics_memory_map.map, 0, g_physics_memory_map.pages_total);

}


void print_check_memory_info() {
    check_memory_info_t* p = (check_memory_info_t*)ARDS_ADDR;
    check_memory_item_t* p_data = (check_memory_item_t*)(ARDS_ADDR + 2);

    unsigned short times = p -> times;

    printk("========================== memory check info ==========================\n");
    for (int i = 0; i < times; ++i) {
        check_memory_item_t* tmp = p_data + i;

        printk("\t%x, %x, %x, %x, %d\n", tmp->base_addr_high, tmp->base_addr_low,
               tmp->length_high, tmp->length_low, tmp->type);
    }

    printk("========================== memory check info ==========================\n");
}

void* get_free_page() {
    bool find = false;

    int i = g_physics_memory_map.bitmap_item_used;
    for (; i < g_physics_memory.pages_total; ++i) {
        if (0 == g_physics_memory_map.map[i]) {
            find = true;
            break;
        }
    }

    if (!find) {
        printk("memory used up!");
        return NULL;
    }

    g_physics_memory_map.map[i] = 1;
    g_physics_memory_map.bitmap_item_used++;

    void* ret = (void*)(g_physics_memory_map.addr_base + (i << 12));

    printk("[%s]return: 0x%X, used: %d pages\n", __FUNCTION__, ret, g_physics_memory_map.bitmap_item_used);

    return ret;
}
//void* get_free_page() {
//    bool find = false;
//    int i = g_physics_memory_map.bitmap_item_used;
//    for (; i < g_physics_memory.pages_total; ++i) {
//        if (0 == g_physics_memory_map.map[i]) {
//            find = true;
//            break;
//        }
//    }
//
//    if (!find) {
//        printk("memory used up!");
//        return NULL;
//    }
//
//    g_physics_memory_map.map[i] = 1;
//    g_physics_memory_map.bitmap_item_used++;
//
//    void* ret = (void*)(g_physics_memory_map.addr_base + (i << 12));
//
//    printk("[%s] return : 0x%X, used : %d pages\n", __FUNCTION__,
//           ret, g_physics_memory_map.bitmap_item_used);
//
//    return ret;
//}

//void free_page(void* p) {
//    if (p < g_physics_memory.addr_start || p > g_physics_memory.addr_end) {
//        printk("invalid address!");
//        return ;
//    }
//
//    int index = (int)(p - g_physics_memory_map.addr_base) >> 12;
//
//    g_physics_memory_map.map[index] = 0;
//    g_physics_memory_map.bitmap_item_used--;
//
//    printk("[%s] return : 0x%X, used: %d pages\n", __FUNCTION__, p,
//           g_physics_memory_map.bitmap_item_used);
//
//}

void free_page(void* p) {
    if (p < g_physics_memory.addr_start || p > g_physics_memory.addr_end) {
        printk("invalid address!");
        return;
    }

    int index = (int)(p - g_physics_memory_map.addr_base) >> 12;

    g_physics_memory_map.map[index] = 0;
    g_physics_memory_map.bitmap_item_used--;

    printk("[%s]return: 0x%X, used: %d pages\n", __FUNCTION__, p, g_physics_memory_map.bitmap_item_used);
}