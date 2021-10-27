
#include "mem.h"
#include "stdlib.h"
#include "string.h"
#include <pthread.h>
#include <stdio.h>

static BYTE _ram[RAM_SIZE];

static struct
{
	uint32_t proc; // ID of process currently uses this page
	int index;	   // Index of the page in the list of pages allocated
				   // to the process.
	int next;	   // The next page in the list. -1 if it is the last
				   // page.
} _mem_stat[NUM_PAGES];

static pthread_mutex_t mem_lock;

void init_mem(void)
{
	memset(_mem_stat, 0, sizeof(*_mem_stat) * NUM_PAGES);
	memset(_ram, 0, sizeof(BYTE) * RAM_SIZE);
	pthread_mutex_init(&mem_lock, NULL);
}

/* get offset of the virtual address */
static addr_t get_offset(addr_t addr)
{
	return addr & ~((~0U) << OFFSET_LEN);
}

/* get the first layer index */
static addr_t get_first_lv(addr_t addr)
{
	return addr >> (OFFSET_LEN + PAGE_LEN);
}

/* get the second layer index */
static addr_t get_second_lv(addr_t addr)
{
	return (addr >> OFFSET_LEN) - (get_first_lv(addr) << PAGE_LEN);
}

/* Search for page table table from the a segment table */
static struct page_table_t *get_page_table(
	addr_t index,				   // Segment level index
	struct seg_table_t *seg_table) // first level table
{
	/*
	 * TODO: Given the Segment index [index], you must go through each
	 * row of the segment table [seg_table] and check if the v_index
	 * field of the row is equal to the index
	 *
	 * */
	int i;
	for (i = 0; i < seg_table->size; i++)
	{
		// Enter your code here
		if (seg_table->table[i].v_index == index)
			return seg_table->table[i].pages;
	}
	return NULL;
}

/* Translate virtual address to physical address. If [virtual_addr] is valid,
 * return 1 and write its physical counterpart to [physical_addr].
 * Otherwise, return 0 */
static int translate(
	addr_t virtual_addr,   // Given virtual address
	addr_t *physical_addr, // Physical address to be returned
	struct pcb_t *proc)	   // Process uses given virtual address
{

	/* Offset of the virtual address */
	addr_t offset = get_offset(virtual_addr);
	/* The first layer index */
	addr_t first_lv = get_first_lv(virtual_addr);
	/* The second layer index */
	addr_t second_lv = get_second_lv(virtual_addr);

	/* Search in the first level */
	struct page_table_t *page_table = NULL;
	page_table = get_page_table(first_lv, proc->seg_table);
	if (page_table == NULL)
	{
		return 0;
	}

	int i;
	for (i = 0; i < page_table->size; i++)
	{
		if (page_table->table[i].v_index == second_lv)
		{
			/* TODO: Concatenate the offset of the virtual addess
			 * to [p_index] field of page_table->table[i] to
			 * produce the correct physical address and save it to
			 * [*physical_addr]  */
			*physical_addr = (page_table->table[i].p_index << OFFSET_LEN) + offset;
			return 1;
		}
	}
	return 0;
}

addr_t alloc_mem(uint32_t size, struct pcb_t *proc)
{
	pthread_mutex_lock(&mem_lock);
	addr_t ret_mem = 0;
	/* TODO: Allocate [size] byte in the memory for the
	 * process [proc] and save the address of the first
	 * byte in the allocated memory region to [ret_mem].
	 * */

	uint32_t num_pages = ((size % PAGE_SIZE) == 0) ? size / PAGE_SIZE : size / PAGE_SIZE + 1; // Number of pages we will use
	int mem_avail = 0;																		  // We could allocate new memory region or not?

	/* First we must check if the amount of free memory in
	 * virtual address space and physical address space is
	 * large enough to represent the amount of required
	 * memory. If so, set 1 to [mem_avail].
	 * Hint: check [proc] bit in each page of _mem_stat
	 * to know whether this page has been used by a process.
	 * For virtual memory space, check bp (break pointer).
	 * */
	int i, num_page = 0;
	for (i = 0; i < NUM_PAGES; i++)
	{
		if (_mem_stat[i].proc == 0)
			num_page++;
		if (num_page == num_pages && (proc->bp + (num_pages * PAGE_SIZE)) <= RAM_SIZE)
		{
			mem_avail = 1;
			break;
		}
	}

	if (mem_avail) /* We could allocate new memory region to the process */
	{
		ret_mem = proc->bp;
		proc->bp += num_pages * PAGE_SIZE;
		/* Update status of physical pages which will be allocated
		 * to [proc] in _mem_stat. Tasks to do:
		 * 	- Update [proc], [index], and [next] field
		 * 	- Add entries to segment table page tables of [proc]
		 * 	  to ensure accesses to allocated memory slot is
		 * 	  isValid. */
		num_page = 0;
		int page_previous_index = -1;
		for (i = 0; i < NUM_PAGES; i++)
		{
			if (_mem_stat[i].proc == 0)
			{
				// add process ID, page index allocated & next field
				_mem_stat[i].proc = proc->pid;
				_mem_stat[i].index = num_page;

				if (_mem_stat[i].index != 0)
					_mem_stat[page_previous_index].next = i;
				page_previous_index = i;

				// update status of segment table & page table
				addr_t virtual_addr = ret_mem + (num_page * PAGE_SIZE); // get current virtual address
				addr_t seg_index = get_first_lv(virtual_addr);			// get segment index
				addr_t page_index = get_second_lv(virtual_addr);		// get page index

				struct page_table_t *page_table = get_page_table(seg_index, proc->seg_table);
				if (page_table == NULL)
				{
					// If not found, add new row into segment table
					proc->seg_table->table[proc->seg_table->size].v_index = seg_index;
					// allocate new page
					proc->seg_table->table[proc->seg_table->size].pages = (struct page_table_t *)malloc(sizeof(struct page_table_t));
					// assign allocated page size to 0
					proc->seg_table->table[proc->seg_table->size].pages->size = 0;
					// update segment table size
					proc->seg_table->size++;
					page_table = proc->seg_table->table[proc->seg_table->size - 1].pages;
				}
				page_table->table[page_table->size].v_index = page_index;
				page_table->table[page_table->size].p_index = i; // location of the allocated page
				page_table->size++;

				num_page++; // update page index
				if (num_page == num_pages)
				{
					_mem_stat[i].next = -1;
					break;
				}
			}
			else
				continue; // page frame was used by another proc
		}
	}
	pthread_mutex_unlock(&mem_lock);
	return ret_mem;
}

int free_mem(addr_t address, struct pcb_t *proc)
{
	/*TODO: Release memory region allocated by [proc]. The first byte of
	 * this region is indicated by [address]. Task to do:
	 * 	- Set flag [proc] of physical page use by the memory block
	 * 	  back to zero to indicate that it is free.
	 * 	- Remove unused entries in segment table and page tables of
	 * 	  the process [proc].
	 * 	- Remember to use lock to protect the memory from other
	 * 	  processes.  */
	pthread_mutex_lock(&mem_lock);

	addr_t physical_addr; // physical address to free in memory

	// Find physical page in memory
	if (!translate(address, &physical_addr, proc))
		return 0;

	// Clear physical page in memory
	addr_t p_index = physical_addr >> OFFSET_LEN; // find physical index
	int num_pages = 0;							  // number of pages
	int i;
	for (i = p_index; i != -1; i = _mem_stat[i].next)
	{
		_mem_stat[i].proc = 0; // clear physical memory
		num_pages++;
	}

	// Clear virtual page in process
	for (i = 0; i < num_pages; i++)
	{
		addr_t virtual_addr = address + (i * PAGE_SIZE);
		addr_t seg_index = get_first_lv(virtual_addr);
		addr_t page_index = get_second_lv(virtual_addr);

		struct page_table_t *page_table = NULL;
		int j, index_of_page;
		// find page in seg_table
		for (j = 0; j < proc->seg_table->size; j++)
		{
			if (proc->seg_table->table[j].v_index == seg_index)
			{
				page_table = proc->seg_table->table[j].pages;
				index_of_page = j;
				break;
			}
		}
		// Remove unused entries in segment table and page tables of the process [proc].
		if (page_table != NULL)
		{
			for (j = 0; j < page_table->size; j++)
			{
				if (page_table->table[j].v_index == page_index)
				{
					page_table->table[j] = page_table->table[--page_table->size];
					break;
				}
			}
			if (page_table->size == 0)
			{
				free(page_table);
				proc->seg_table->table[index_of_page] = proc->seg_table->table[--proc->seg_table->size];
			}
		}
		else
			continue;
	}
	// Update break pointer
	if (proc->bp == address + num_pages * PAGE_SIZE)
		proc->bp -= num_pages * PAGE_SIZE;

	pthread_mutex_unlock(&mem_lock);
	return 1;
}

int read_mem(addr_t address, struct pcb_t *proc, BYTE *data)
{
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc))
	{
		*data = _ram[physical_addr];
		return 0;
	}
	else
	{
		return 1;
	}
}

int write_mem(addr_t address, struct pcb_t *proc, BYTE data)
{
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc))
	{
		_ram[physical_addr] = data;
		return 0;
	}
	else
	{
		return 1;
	}
}

void dump(void)
{
	int i;
	for (i = 0; i < NUM_PAGES; i++)
	{
		if (_mem_stat[i].proc != 0)
		{
			printf("%03d: ", i);
			printf("%05x-%05x - PID: %02d (idx %03d, nxt: %03d)\n",
				   i << OFFSET_LEN,
				   ((i + 1) << OFFSET_LEN) - 1,
				   _mem_stat[i].proc,
				   _mem_stat[i].index,
				   _mem_stat[i].next);
			int j;
			for (j = i << OFFSET_LEN;
				 j < ((i + 1) << OFFSET_LEN) - 1;
				 j++)
			{

				if (_ram[j] != 0)
				{
					printf("\t%05x: %02x\n", j, _ram[j]);
				}
			}
		}
	}
}
