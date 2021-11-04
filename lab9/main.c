#include <stdio.h>
#include <stdint.h>
#define TLB_SIZE 4
#define PAGE_TABLE_SIZE 10
#define OFF_LEN 12

typedef uint32_t address_t;
typedef uint32_t index_t;
typedef uint32_t offset_t;

struct tlb_slot_t
{
    index_t virtual;  // Index for virtual address
    index_t physical; // Index for physical address
    index_t count;    // Number of times this slot is accessed
};

struct tlb_slot_t tlb[TLB_SIZE];

struct tlb_slot_t page_table[PAGE_TABLE_SIZE];

/* Get index part of an address */
index_t get_index(address_t addr)
{
    return addr >> OFF_LEN;
}

/* Get offset part of an address */
offset_t get_offset(address_t addr)
{
    return addr & ~((~0U) << OFF_LEN);
}

/* Get the least accessed slot from TLB */
struct tlb_slot_t *get_least_accessed_slot()
{
    int i, j = 0;
    for (i = 0; i < TLB_SIZE; i++)
    {
        if (tlb[i].count < tlb[j].count)
        {
            j = i;
        }
    }
    return &tlb[j];
}

/* Translate virtual address to physical address */
int translate(address_t virtual, address_t *physical)
{
    // TODO: Translate virtual address to physical address .
    // Return 1 if the virtual address is valid, otherwise, return 0
    // If the virtual address is valid , writing its physical counterpart
    // to physical address .
    index_t page_index = get_index(virtual);
    int i;
    for (i = 0; i < TLB_SIZE; i++)
    {
        if (tlb[i].virtual == page_index)
        {
            *physical = (tlb[i].physical << OFF_LEN) + get_offset(virtual);
            tlb[i].count++;
            return 1;
        }
    }
    for (i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        if (page_table[i].virtual == page_index)
        {
            *physical = (page_table[i].physical << OFF_LEN) + get_offset(virtual);
            page_table[i].count++;
            return 1;
        }
    }
    return 0;
}

int main()
{
    /* Init page table */
    page_table[0] = (struct tlb_slot_t){0x00001, 0x52354, 0};
    page_table[1] = (struct tlb_slot_t){0x00002, 0xafb29, 0};
    page_table[2] = (struct tlb_slot_t){0x00003, 0x4b0dc, 0};
    page_table[3] = (struct tlb_slot_t){0x00004, 0x52ca0, 0};
    page_table[4] = (struct tlb_slot_t){0x00005, 0xa7cbd, 0};
    page_table[5] = (struct tlb_slot_t){0x17d42, 0x338a3, 0};
    page_table[6] = (struct tlb_slot_t){0x1238f, 0x28471, 0};
    page_table[7] = (struct tlb_slot_t){0xda234, 0x2341b, 0};
    page_table[8] = (struct tlb_slot_t){0xf1234, 0x1bca2, 0};
    page_table[9] = (struct tlb_slot_t){0x129af, 0x23133, 0};

    /* Init TLB */
    tlb[0] = page_table[0];
    tlb[1] = page_table[1];
    tlb[2] = page_table[2];
    tlb[3] = page_table[3];

    /* Init tests */
    int test[7] = {0x00003123, 0x00001524, 0x00002534, 0x17d42e52, 0x121aabdd, 0x000012ac, 0x00004a71};

    int i;
    printf("Page table\n");
    for (i = 0; i < PAGE_TABLE_SIZE; i++)
    {
        printf("%05x −−> %05x\n", page_table[i].virtual, page_table[i].physical);
    }

    /* Test */
    printf("Access pages\n");
    for (i = 0; i < 7; i++)
    {
        address_t addr;
        if (translate(test[i], &addr))
        {
            printf("%08x −−> %08x\n", test[i], addr);
        }
        else
        {
            printf("%08x −−> Illegal address\n", test[i]);
        }
    }

    /* The TLB */
    printf("TLB\n");
    for (i = 0; i < TLB_SIZE; i++)
    {
        printf("%d: %05x −−> %05x : %2d\n", i, tlb[i].virtual, tlb[i].physical, tlb[i].count);
    }
}

/*
    VPN = (VirtualAddress & VPN_MASK) >> SHIFT
    (Success, TlbEntry) = TLB_lookup(VPN)
    if (Success == True) // TLB Hit
        if (CanAccess(TlbEntry.ProtectBits) == True)
            Offset = VirtualAddress & OFFSET_MASK
            PhysAddr = (TlbEntry.PFN << SHIFT) | Offset
            AccessMemory(PhysAddr)
        else
            RaiseException(PROTECTION_FAULT)
    else
        PTEAddr = PTBR (page table base register) + (VPN * sizeof(PTE))
        PTE = AccessMemory(PTEAddr) // additional memory access
        if (PTE.Valid == False)
            RaiseException(SEGMENTATION_FAULT)
        else if (CanAccess(PTE.ProtectBits) == False)
            RaiseException(PROTECTION_FAULT)
        else
            TLB_Insert(VPN, PTE.PFN, PTE.ProtectBits)
            RetryInstruction()
*/