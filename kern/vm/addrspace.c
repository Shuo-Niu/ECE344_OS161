#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <thread.h>
#include <curthread.h>
#include <addrspace.h>
#include <vm.h>
#include <machine/vm.h>
#include <machine/spl.h>
#include <machine/tlb.h>

//#define DUMBVM_STACKPAGES    12
//
///*
// * Note! If OPT_DUMBVM is set, as is the case until you start the VM
// * assignment, this file is not compiled or linked or in any way
// * used. The cheesy hack versions in dumbvm.c are used instead.
// */
//
//int create_first_pt(struct addrspace* as){
//    as->first_pte = kmalloc(sizeof(struct firstpte) * NUM_PTE);
//    if(as->first_pte == NULL){
//        kprintf("Out of memory when creating master page table.");
//        return ENOMEM;
//    }
//
//    int i;
//    for(i = 0; i < NUM_PTE; i++){
//        as->first_pte[i].second_pte = NULL;
//    }
//
//    return 0;
//}
//
//int create_second_pt(struct firstpte* first_pte){
//    first_pte->second_pte = kmalloc(sizeof(struct secondpte) * NUM_PTE);
//    if(first_pte->second_pte == NULL){
//        kprintf("Out of memory when creating secondary page table.");
//        return ENOMEM;
//    }
//
//    int i;
//    for(i = 0; i < NUM_PTE; i++){
//        first_pte->second_pte[i].frame = 0;
//        first_pte->second_pte[i].valid = 0;
//        first_pte->second_pte[i].read_only = 0;
//    }
//
//    return 0;
//}
//
//struct addrspace *
//as_create(void)
//{
//	struct addrspace *as = kmalloc(sizeof(struct addrspace));
//	if (as==NULL) {
//        kprintf("Out of memory when creating addrspace.");
//		return NULL;
//	}
//
//    as->first_pte = NULL;
//    as->reg = NULL;
//
//    int result;
//	result = create_first_pt(as);
//	if(result){
//        return NULL;
//	}
//
//	return as;
//}
//
//int
//as_copy(struct addrspace *old, struct addrspace **ret)
//{
//	struct addrspace *newas;
//
//	newas = as_create();
//	if (newas==NULL) {
//		return ENOMEM;
//	}
//    /*
//     * After as_create(), newas is allocated size of addrspace.
//     * newas->reg is set to NULL and newas->first_pte is allocated size of firstpte * NUM_PTE;
//     * newas->first_pte[0:NUM_PTE].second_pte are set to NULL.
//     */
//
//    int i,j,res;
//    for(i = 0; i < NUM_PTE; i++){
//        if(old->first_pte[i].second_pte != NULL){
//            res = create_second_pt(&(newas->first_pte[i]));
//            if(res != 0){
//                return res;
//            }
//            for(j = 0; j < NUM_PTE; j++){
//                newas->first_pte[i].second_pte[j].frame = old->first_pte[i].second_pte[j].frame;
//                newas->first_pte[i].second_pte[j].valid = old->first_pte[i].second_pte[j].valid;
//                newas->first_pte[i].second_pte[j].read_only = 1; // set second_pte to read only
//            }
//        }
//    }
//
//    while(old->reg != NULL){
//        res = as_define_region(old->reg->v,old->reg->offset,newas,old->reg->vbase,old->reg->filesize,
//                               old->reg->readable,old->reg->writable,old->reg->executable);
//        old->reg = old->reg->next;
//    }
//
//	*ret = newas;
//	return 0;
//}
//
//void
//as_destroy(struct addrspace *as)
//{
//	int i,j;
//    for(i = 0; i < NUM_PTE; i++){
//        if(as->first_pte[i].second_pte == NULL){
//            continue;
//        } else {
//            for(j = 0; j < NUM_PTE; j++){
//                if(as->first_pte[i].second_pte[j].valid == 1){
//                    int page_num;
//                    page_num = (i << 10) | j;
//                    page_num <<= 12;
//
////                    unmap(curthread->pid,page_num); // unmap() is defined in vm.c
//                }
//            }
//            kfree(as->first_pte[i].second_pte);
//        }
//    }
//    kfree(as->first_pte);
//	kfree(as);
//}
//
//void
//as_activate(struct addrspace *as)
//{
//    /* Purely copied from dumbvm.c */
//    int i, spl;
//
//    (void)as;
//
//    spl = splhigh();
//
//    for (i=0; i<NUM_TLB; i++) {
//        TLB_Write(TLBHI_INVALID(i), TLBLO_INVALID(), i);
//    }
//
//    splx(spl);
//}
//
///*
// * Set up a segment at virtual address VADDR of size MEMSIZE. The
// * segment in memory extends from VADDR up to (but not including)
// * VADDR+MEMSIZE.
// *
// * The READABLE, WRITEABLE, and EXECUTABLE flags are set if read,
// * write, or execute permission should be set on the segment. At the
// * moment, these are ignored. When you write the VM system, you may
// * want to implement them.
// */
//int
//as_define_region(struct vnode* v, off_t offset, struct addrspace *as, vaddr_t vaddr, size_t sz,
//                 int readable, int writable, int executable)
//{
//    size_t npages;  // number of pages in this region
//
//    sz += vaddr & ~(vaddr_t)PAGE_FRAME; // PAGE_FRAME is defined in vm.h, which is a mask
//    vaddr &= PAGE_FRAME;
//
//    sz = ((sz - 1) + PAGE_SIZE) & PAGE_FRAME;
//
//    npages = sz / PAGE_SIZE;    // PAGE_SIZE is defined in vm.h, npages = total size / page size
//    //********************************************
//    kprintf("Number of pages: i=%d\n", npages);
//    //********************************************
//
//    if(as->reg == NULL){
//        as->reg = kmalloc(sizeof(struct region));
//        if(as->reg == NULL){
//            return ENOMEM;
//        }
//        as->reg->npages = npages;
//        as->reg->vbase = vaddr;
//        as->reg->readable = readable;
//        as->reg->writable = writable;
//        as->reg->executable = executable;
//        as->reg->v = v;
//        as->reg->offset = offset;
//        as->reg->filesize = sz;
//        as->reg->next = NULL; //********************************************
//        as->reg->in_stack = 0;  // this region is not stack
//    } else {
//        struct region* new_region;
//        struct region* previous;
//
//        new_region = as->reg;   // head of the region linked list
//        while(new_region != NULL){
//            previous = new_region;
//            new_region = new_region->next;
//        }
//        //********************************************
//        kprintf("End of loop, new_region == NULL now");
//        //********************************************
//        new_region = kmalloc(sizeof(struct region));
//        if(new_region == NULL){
//            return ENOMEM;
//        }
//        new_region->npages = npages;
//        new_region->vbase = vaddr;
//        new_region->readable = readable;
//        new_region->writable = writable;
//        new_region->executable = executable;
//        new_region->v = v;
//        new_region->offset = offset;
//        new_region->filesize = sz;
//        new_region->next = NULL; //********************************************
//        new_region->in_stack = 0;   // this region is not stack
//
//        previous->next = new_region;
//    }
//    return 0;
//}
//
//int
//as_prepare_load(struct addrspace *as)
//{
//	/*
//	 * Write this.
//	 */
//
//	(void)as;
//	return 0;
//}
//
//int
//as_complete_load(struct addrspace *as)
//{
//	/*
//	 * Write this.
//	 */
//
//	(void)as;
//	return 0;
//}
//
//int
//as_define_stack(struct addrspace *as, vaddr_t vaddr)
//{
//    if(as->reg == NULL){
//        as->reg = kmalloc(sizeof(struct region));
//        if(as->reg == NULL){
//            return ENOMEM;
//        }
//        as->reg->npages = 0;
//        as->reg->vbase = vaddr;
//        as->reg->readable = 1;
//        as->reg->writable = 1;
//        as->reg->executable = 0;
//        as->reg->v = NULL;      // no need because this is stack
//        as->reg->offset = 0;    // no need because this is stack
//        as->reg->filesize = 0;  // no need because this is stack
//        as->reg->next = NULL;   // no need because this is stack
//        as->reg->in_stack = 1;  // this region is stack
//    } else {
//        struct region* new_region;
//        struct region* previous;
//
//        new_region = as->reg;   // head of the region linked list
//        while(new_region != NULL){
//            previous = new_region;
//            new_region = new_region->next;
//        }
//        new_region = kmalloc(sizeof(struct region));
//        if(new_region == NULL){
//            return ENOMEM;
//        }
//        new_region->npages = 0;
//        new_region->vbase = vaddr;
//        new_region->readable = 1;
//        new_region->writable = 1;
//        new_region->executable = 0;
//        new_region->v = NULL;       // no need because this is stack
//        new_region->offset = 0;     // no need because this is stack
//        new_region->filesize = 0;   // no need because this is stack
//        new_region->next = NULL;    // no need because this is stack
//        new_region->in_stack = 1;   // this region is stack
//
//        previous->next = new_region;
//    }
//	return 0;
//}
//
////*****************************************************
//
//int frame_count;
//paddr_t paddr_start;
//
////initialization function
//void vm_bootstrap(void) {
//    paddr_t low, high;
//    ram_getsize(&low, &high);
//
//    kprintf("IN VM_BOOTSTRAP\n");
//
//    // each frame has size of PAGE_SIZE
//    // but also for each frame we lose the space for "frame" itself
//    frame_count = (high-low) / (PAGE_SIZE + sizeof(struct frame));
//
//    int core_map_size = frame_count * sizeof(struct frame);
//
//    // number of pages required to store core_map
//    core_map_size = (int) ((core_map_size - 1) / PAGE_SIZE ) + 1;
//
//    paddr_start = low + core_map_size;
//
//    // make core_map point to the beginning of the available physical address
//    core_map = PADDR_TO_KVADDR(low);
//
//
//
//    int i;
//    for (i=0; i < frame_count; i++) {
//        core_map[i].valid = 0;
//        core_map[i].page_entry = NULL;
//        core_map[i].id = 0;
//    }
//}
//
//vaddr_t alloc_kpages(int npages) {
//    kprintf("IN ALLOC_KPAGES\n");
//
//    paddr_t paddr;
//    int i, core_start;
//    int page_count = 0;
//    int spl = splhigh();
//
//    for (i=0; i < frame_count; i++) {
//        if (core_map[i].valid == 0) {
//            page_count++;
//
//            if (page_count == npages) {
//                break;
//
//            }
//        } else {
//            page_count = 0;
//        }
//    }
//    core_start = i + 1 - npages;
//
//    paddr = paddr_start + core_start * PAGE_SIZE;
//
//    for (i=core_start; i<core_start+npages; i++) {
//        core_map[i].valid = 1;
//        core_map[i].id = paddr;
//    }
//
//
//    if (paddr == 0) {
//        splx(spl);
//        return 0;
//    }
//    splx(spl);
//    return PADDR_TO_KVADDR(paddr);
//};
//
//void free_kpages(vaddr_t addr) {
//
//    kprintf("IN FREE_KPAGES\n");
//    int i;
//
//    int spl = splhigh();
//
//    int idx = (addr - paddr_start) / PAGE_SIZE;
//
//    paddr_t id = core_map[idx].id;
//
//    for (i=0; i < frame_count; i++) {
//        if (core_map[i].id == id) {
//            core_map[i].valid = 0;
//            core_map[i].id = 0;
//        }
//    }
//    splx(spl);
//};
//
//// Based on dumbvm/vm_fault()
//int vm_fault(int faulttype, vaddr_t faultaddress) {
//
//    vaddr_t vbase, vtop;
//    struct addrspace *as;
//    int spl;
//
//    paddr_t paddr;
//    int i;
//    u_int32_t ehi, elo;
//
//    spl = splhigh();
//
//    DEBUG(DB_VM, "dumbvm: fault: 0x%x\n", faultaddress);
//
//    as = curthread->t_vmspace;
//    if (as == NULL) {
//        /*
//         * No address space set up. This is probably a kernel
//         * fault early in boot. Return EFAULT so as to panic
//         * instead of getting into an infinite faulting loop.
//         */
//        return EFAULT;
//    }
//
//    struct region *region = as->reg;
//    int reg_exist = 0; // checking if there exists region which contains the faultaddress
//
//    // traverse through regions to find the one where faultaddress is located
//    while (region != NULL) {
//        vbase = region->vbase;
//
//        // stack region
//        if (region->in_stack == 1) {
//            // stack grows downwards
//            vtop = vbase - (region->npages)*PAGE_SIZE;
//            if (faultaddress <= vbase && faultaddress >= vtop) {
//                reg_exist = 1;
//                break;
//            }
//        }
//
//            // not stack region
//        else {
//            vtop = vbase + (region->npages)*PAGE_SIZE;
//            if (faultaddress >= vbase && faultaddress <= vtop) {
//                reg_exist = 1;
//                break;
//            }
//        }
//        region = region->next;
//    }
//
//    if (reg_exist == 0) {
//        splx(spl);
//        return EFAULT;
//    }
//
//
//    vaddr_t first, second;
//    faultaddress >> 12;             // get rid of the offset
//    second = faultaddress & 0x3ff;  // take only last ten bits
//    first = faultaddress >> 10;     // take only first ten bits
//
//    struct secondpte *secondary_page;   // pointer to secondary page containing the mapping of the faultaddress
//
//    switch(faulttype) {
//
//        case VM_FAULT_READONLY:
//            /* We always create pages read-write, so we can't get this */
//            panic("Got VM_FAULT_READONLY\n");
//            break;
//
//        case VM_FAULT_READ:
//            if (region->readable == 0) {
//                splx(spl);
//                return EINVAL;
//            }
//
//            if (as->first_pte[first].second_pte == NULL) {
//                int res;
//                res = create_second_pt(&as->first_pte[first]);
//                if(res != 0){
//                    return ENOMEM;
//                }
//            }
//
//            secondary_page = &(as->first_pte[first].second_pte[second]);
//
//            if (secondary_page->valid == 0) {
//                for (i=0; i < frame_count; i++) {
//                    if (core_map[i].valid == 0) {
//                        core_map[i].valid = 1;
//                        struct page_entry *pe = kmalloc(sizeof(struct page_entry));
//                        pe->secondary_page_entry = secondary_page;
//                        pe->next = NULL;
//                        core_map[i].page_entry = pe;
//                        paddr = paddr_start + i*PAGE_SIZE;
//                        paddr_t frame = paddr >> 12;
//                        secondary_page->frame = frame;
//                        secondary_page->valid = 1;
//
//                        // something else ? //
//                        return paddr;
//                    }
//                }
//            }
//            else {
//                paddr = secondary_page->frame;
//            }
//
//            break;
//
//        case VM_FAULT_WRITE:
//            if (region->writable == 0) {
//                splx(spl);
//                return EINVAL;
//            }
//
//            if (as->first_pte[first].second_pte == NULL) {
//                int res;
//                res = create_second_pt(&as->first_pte[first]);
//                if(res != 0){
//                    return ENOMEM;
//                }
//            }
//
//            secondary_page = &(as->first_pte[first].second_pte[second]);
//
//            if (secondary_page->valid == 0) {
//                for (i=0; i < frame_count; i++) {
//                    if (core_map[i].valid == 0) {
//                        core_map[i].valid = 1;
//                        paddr = paddr_start + i*PAGE_SIZE;
//                        paddr_t frame = paddr >> 12;
//                        secondary_page->frame = frame;
//                        secondary_page->valid = 1;
//
//                        // something else ? //
//                        return paddr;
//                    }
//                }
//            }
//
//            paddr = secondary_page->frame;
//    }
//
//
//
//    for (i=0; i<NUM_TLB; i++) {
//        TLB_Read(&ehi, &elo, i);
//        if (elo & TLBLO_VALID) {
//            continue;
//        }
//        ehi = faultaddress;
//        elo = paddr | TLBLO_DIRTY | TLBLO_VALID;
//        DEBUG(DB_VM, "dumbvm: 0x%x -> 0x%x\n", faultaddress, paddr);
//        TLB_Write(ehi, elo, i);
//        splx(spl);
//        return 0;
//    }
//    TLB_Random(ehi,elo);
//    splx(spl);
//    return 0;
//}

//**********************************************************************************************************************
//**********************************************************************************************************************
//**********************************************************************************************************************

#define DUMBVM_STACKPAGES    12

void
vm_bootstrap(void)
{
    /* Do nothing. */
}

static
paddr_t
getppages(unsigned long npages)
{
    int spl;
    paddr_t addr;

    spl = splhigh();

    addr = ram_stealmem(npages);

    splx(spl);
    return addr;
}

/* Allocate/free some kernel-space virtual pages */
vaddr_t
alloc_kpages(int npages)
{
    paddr_t pa;
    pa = getppages(npages);
    if (pa==0) {
        return 0;
    }
    return PADDR_TO_KVADDR(pa);
}

void
free_kpages(vaddr_t addr)
{
    /* nothing */

    (void)addr;
}

int
vm_fault(int faulttype, vaddr_t faultaddress)
{
    vaddr_t vbase1, vtop1, vbase2, vtop2, stackbase, stacktop;
    paddr_t paddr;
    int i;
    u_int32_t ehi, elo;
    struct addrspace *as;
    int spl;

    spl = splhigh();

    faultaddress &= PAGE_FRAME;

    DEBUG(DB_VM, "dumbvm: fault: 0x%x\n", faultaddress);

    switch (faulttype) {
        case VM_FAULT_READONLY:
            /* We always create pages read-write, so we can't get this */
            panic("dumbvm: got VM_FAULT_READONLY\n");
        case VM_FAULT_READ:
        case VM_FAULT_WRITE:
            break;
        default:
            splx(spl);
            return EINVAL;
    }

    as = curthread->t_vmspace;
    if (as == NULL) {
        /*
         * No address space set up. This is probably a kernel
         * fault early in boot. Return EFAULT so as to panic
         * instead of getting into an infinite faulting loop.
         */
        return EFAULT;
    }

    /* Assert that the address space has been set up properly. */
    assert(as->as_vbase1 != 0);
    assert(as->as_pbase1 != 0);
    assert(as->as_npages1 != 0);
    assert(as->as_vbase2 != 0);
    assert(as->as_pbase2 != 0);
    assert(as->as_npages2 != 0);
    assert(as->as_stackpbase != 0);
    assert((as->as_vbase1 & PAGE_FRAME) == as->as_vbase1);
    assert((as->as_pbase1 & PAGE_FRAME) == as->as_pbase1);
    assert((as->as_vbase2 & PAGE_FRAME) == as->as_vbase2);
    assert((as->as_pbase2 & PAGE_FRAME) == as->as_pbase2);
    assert((as->as_stackpbase & PAGE_FRAME) == as->as_stackpbase);

    vbase1 = as->as_vbase1;
    vtop1 = vbase1 + as->as_npages1 * PAGE_SIZE;
    vbase2 = as->as_vbase2;
    vtop2 = vbase2 + as->as_npages2 * PAGE_SIZE;
    stackbase = USERSTACK - DUMBVM_STACKPAGES * PAGE_SIZE;
    stacktop = USERSTACK;

    if (faultaddress >= vbase1 && faultaddress < vtop1) {
        paddr = (faultaddress - vbase1) + as->as_pbase1;
    }
    else if (faultaddress >= vbase2 && faultaddress < vtop2) {
        paddr = (faultaddress - vbase2) + as->as_pbase2;
    }
    else if (faultaddress >= stackbase && faultaddress < stacktop) {
        paddr = (faultaddress - stackbase) + as->as_stackpbase;
    }
    else {
        splx(spl);
        return EFAULT;
    }

    /* make sure it's page-aligned */
    assert((paddr & PAGE_FRAME)==paddr);

    for (i=0; i<NUM_TLB; i++) {
        TLB_Read(&ehi, &elo, i);
        if (elo & TLBLO_VALID) {
            continue;
        }
        ehi = faultaddress;
        elo = paddr | TLBLO_DIRTY | TLBLO_VALID;
        DEBUG(DB_VM, "dumbvm: 0x%x -> 0x%x\n", faultaddress, paddr);
        TLB_Write(ehi, elo, i);
        splx(spl);
        return 0;
    }

    kprintf("dumbvm: Ran out of TLB entries - cannot handle page fault\n");
    splx(spl);
    return EFAULT;
}

struct addrspace *
as_create(void)
{
    struct addrspace *as = kmalloc(sizeof(struct addrspace));
    if (as==NULL) {
        return NULL;
    }

    as->as_vbase1 = 0;
    as->as_pbase1 = 0;
    as->as_npages1 = 0;
    as->as_vbase2 = 0;
    as->as_pbase2 = 0;
    as->as_npages2 = 0;
    as->as_stackpbase = 0;

    return as;
}

void
as_destroy(struct addrspace *as)
{
    kfree(as);
}

void
as_activate(struct addrspace *as)
{
    int i, spl;

    (void)as;

    spl = splhigh();

    for (i=0; i<NUM_TLB; i++) {
        TLB_Write(TLBHI_INVALID(i), TLBLO_INVALID(), i);
    }

    splx(spl);
}

int
as_define_region(struct addrspace *as, vaddr_t vaddr, size_t sz,
                 int readable, int writeable, int executable)
{
    size_t npages;

    /* Align the region. First, the base... */
    sz += vaddr & ~(vaddr_t)PAGE_FRAME;
    vaddr &= PAGE_FRAME;

    /* ...and now the length. */
    sz = (sz + PAGE_SIZE - 1) & PAGE_FRAME;

    npages = sz / PAGE_SIZE;

    /* We don't use these - all pages are read-write */
    (void)readable;
    (void)writeable;
    (void)executable;

    if (as->as_vbase1 == 0) {
        as->as_vbase1 = vaddr;
        as->as_npages1 = npages;
        return 0;
    }

    if (as->as_vbase2 == 0) {
        as->as_vbase2 = vaddr;
        as->as_npages2 = npages;
        return 0;
    }

    /*
     * Support for more than two regions is not available.
     */
    kprintf("dumbvm: Warning: too many regions\n");
    return EUNIMP;
}

int
as_prepare_load(struct addrspace *as)
{
    assert(as->as_pbase1 == 0);
    assert(as->as_pbase2 == 0);
    assert(as->as_stackpbase == 0);

    as->as_pbase1 = getppages(as->as_npages1);
    if (as->as_pbase1 == 0) {
        return ENOMEM;
    }

    as->as_pbase2 = getppages(as->as_npages2);
    if (as->as_pbase2 == 0) {
        return ENOMEM;
    }

    as->as_stackpbase = getppages(DUMBVM_STACKPAGES);
    if (as->as_stackpbase == 0) {
        return ENOMEM;
    }

    return 0;
}

int
as_complete_load(struct addrspace *as)
{
    (void)as;
    return 0;
}

int
as_define_stack(struct addrspace *as, vaddr_t *stackptr)
{
    assert(as->as_stackpbase != 0);

    *stackptr = USERSTACK;
    return 0;
}

int
as_copy(struct addrspace *old, struct addrspace **ret)
{
    struct addrspace *new;

    new = as_create();
    if (new==NULL) {
        return ENOMEM;
    }

    new->as_vbase1 = old->as_vbase1;
    new->as_npages1 = old->as_npages1;
    new->as_vbase2 = old->as_vbase2;
    new->as_npages2 = old->as_npages2;

    if (as_prepare_load(new)) {
        as_destroy(new);
        return ENOMEM;
    }

    assert(new->as_pbase1 != 0);
    assert(new->as_pbase2 != 0);
    assert(new->as_stackpbase != 0);

    memmove((void *)PADDR_TO_KVADDR(new->as_pbase1),
            (const void *)PADDR_TO_KVADDR(old->as_pbase1),
            old->as_npages1*PAGE_SIZE);

    memmove((void *)PADDR_TO_KVADDR(new->as_pbase2),
            (const void *)PADDR_TO_KVADDR(old->as_pbase2),
            old->as_npages2*PAGE_SIZE);

    memmove((void *)PADDR_TO_KVADDR(new->as_stackpbase),
            (const void *)PADDR_TO_KVADDR(old->as_stackpbase),
            DUMBVM_STACKPAGES*PAGE_SIZE);

    *ret = new;
    return 0;
}
