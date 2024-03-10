#include "mem_malloc.h"
#include "printf.h"
#define free 0
#define aled 1
uint64_t heap_start;
uint64_t heap_end;

uint32_t round_up(uint32_t value, uint32_t n)
{
    return ((value + n - 1) / n) * n;
}
uint32_t round_down(uint32_t value, uint32_t n)
{
    return (value / n) * n;
}
inline uint32_t read_4byte(uint64_t addr)
{
    return *(uint32_t *)addr;
}
static uint64_t get_payload(uint64_t header_addr)
{
    return header_addr + 4;
}
static uint32_t get_blocksize(uint64_t header_addr)
{
    // if (header_addr >= heap_start + 4 && header_addr <= heap_end - 4 && (header_addr & 0x3 == 0x0))
    // {
    //     debug("header地址输入错误");
    // }
    uint32_t header_value = read_4byte(header_addr);
    return header_value & (0xffffffff - 1);
}
static uint32_t get_allocated(uint64_t header_addr)
{
    // if (header_addr >= heap_start + 4 && header_addr <= heap_end - 4 && (header_addr & 0x3 == 0x0))
    // {
    //     debug("header地址输入错误");
    // }
    uint32_t header_value = read_4byte(header_addr);
    return header_value & 0x1;
}
// find the next block with a high address
static uint64_t get_nextblock(uint64_t header_addr)
{
    uint32_t block_size = get_blocksize(header_addr);
    uint64_t next_block_addr = header_addr + block_size;
    // if (next_block_addr >= heap_start && next_block_addr <= heap_end - 8 && (header_addr & 0x3 == 0x0))
    // {
    //     debug("next header 计算错误");
    // }
    return next_block_addr;
}
// find the prve block with a low address
static uint64_t get_prveblock(uint64_t header_addr)
{
    uint32_t block_size = get_blocksize(header_addr - 4);
    uint64_t prv_block_addr = header_addr - block_size;
    // if (prv_block_addr >= heap_start && prv_block_addr <= heap_end - 16 && (header_addr & 0x3 == 0x0))
    // {
    //     debug("prv header 计算错误");
    // }
    return prv_block_addr;
}
static uint32_t set_block(uint64_t header_addr, uint32_t block_size, uint32_t allocated)
{
    if (block_size != 0)
    {
        *(uint32_t *)header_addr &= 0x000000001;
        *(uint32_t *)header_addr |= block_size;

        *(uint32_t *)(header_addr + block_size - 4) &= 0x000000001;
        *(uint32_t *)(header_addr + block_size - 4) |= block_size;
    }
    else // if block_size==0;
    {
        block_size = get_blocksize(header_addr);
    }

    *(uint32_t *)header_addr &= 0xffffffff - 1;
    *(uint32_t *)header_addr |= allocated * 0x1;

    *(uint32_t *)(header_addr + block_size - 4) &= 0xffffffff - 1;
    *(uint32_t *)(header_addr + block_size - 4) |= allocated & 0x1;
}
void *mem_alloc(uint32_t size)
{
    uint32_t request_blocksize = round_up(size, 8) + 4 + 4;
    uint64_t point_addr = heap_start;
    while (point_addr < heap_end)
    {
        uint32_t point_size = get_blocksize(point_addr);
        uint32_t point_allocated = get_allocated(point_addr);

        if (point_size >= request_blocksize && point_allocated == free)
        {
            // allocate this block
            if (point_size > size)
            {
                // split this block
                set_block(point_addr, request_blocksize, aled);
                // make next block
                uint64_t next_point_addr = point_addr + request_blocksize;
                set_block(next_point_addr, point_size - request_blocksize, free);
                return (void *)get_payload(point_addr);
            }
            else
            {
                // no need split this block
                set_block(point_addr, request_blocksize, aled);
                return (void *)get_payload(point_addr);
            }
        }
        else
        {
            point_addr = get_nextblock(point_addr);
        }
    }
    return 0;
}
int mem_free(uint64_t addr)
{
    // assert(heap_start>=addr&&addr<=heap_end);
    // assert(addr & 0x7 == 0x0);
    // assert(get_allocated(addr-4)==aled);
    addr -= 4;
    uint32_t addr_size = get_blocksize(addr);
    uint64_t next = get_nextblock(addr);
    uint64_t prev = get_prveblock(addr);
    uint32_t next_allocated = get_allocated(next);
    uint32_t prev_allocated = get_allocated(prev);
    uint32_t next_size = get_blocksize(next);
    uint32_t prev_size = get_blocksize(prev);
    if (next_allocated == aled && prev_allocated == aled)
    {
        set_block(addr, 0, free);
    }
    else if (next_allocated == free && prev_allocated == aled)
    {
        set_block(addr, next_size + addr_size, free);
    }
    else if (next_allocated == aled && prev_allocated == free)
    {
        set_block(prev, prev_size + addr_size, free);
    }
    else
    {
        set_block(prev, prev_size + addr_size + next_size, free);
    }
}
int heap_init(void)
{

    heap_start = round_up((bss_end + 1024 * 1024 * 8), 4);
    heap_end = heap_start + 8 + round_down((memory_end - 1 - heap_start - 8), 8);
    heap_start += 4;
    heap_end -= 4;

    *(uint32_t *)(heap_start - 4) = 0;
    *(uint32_t *)(heap_start - 4) |= 0x1;
    *(uint32_t *)(heap_end + 4) = 0;
    *(uint32_t *)(heap_end + 4) |= 0x1;
    set_block(heap_start, heap_end - heap_start, free);
}
void block_printf(void)
{
    int prve_state = aled;
    int this_state;
    uint64_t point_addr = heap_start;
    uint32_t size = heap_start;
    while (point_addr < heap_end)
    {

        size += get_blocksize(point_addr);

        printf("0x:%x----", point_addr);
        this_state = get_allocated(point_addr);
        if (this_state == free)
        {
            printf("F----");
        }
        else
        {
            printf("A----");
        }
        debug("0x%x", point_addr + get_blocksize(point_addr));
        point_addr = get_nextblock(point_addr);

        if ((this_state == prve_state) && (this_state == free) || (size != point_addr))
        {
            debug("堆栈空间结构错误");
            while (1)
                ;
        }
        prve_state = this_state;
    }
}
