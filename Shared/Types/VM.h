#pragma once

#define		VM_PROT_NONE 		((vm_prot_t) 0x00)
#define		VM_PROT_READ 		((vm_prot_t) 0x01)
#define		VM_PROT_WRITE 		((vm_prot_t) 0x02)
#define		VM_PROT_EXECUTE		((vm_prot_t) 0x04)
#define		VM_PROT_COPY 		((vm_prot_t) 0x08) 	 /* copy-on-read */

#define		VM_PROT_ALL 	 	(VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)
#define		VM_PROT_RW 	 		(VM_PROT_READ|VM_PROT_WRITE)
#define		VM_PROT_DEFAULT 	VM_PROT_ALL

#define 	VM_INHERIT_SHARE 	((vm_inherit_t) 0)
#define 	VM_INHERIT_COPY 	((vm_inherit_t) 1)
#define 	VM_INHERIT_NONE 	((vm_inherit_t) 2)
#define 	VM_INHERIT_DEFAULT	VM_INHERIT_COPY

typedef unsigned char   vm_prot_t;	    /* protection codes */
typedef char            vm_inherit_t;	/* inheritance codes */
typedef unsigned char   vm_flags_t;
typedef unsigned int    vm_eflags_t;
typedef	__vm_offset_t	vm_offset_t;
typedef	__vm_ooffset_t	vm_ooffset_t;
typedef	__vm_paddr_t	vm_paddr_t;
typedef	__vm_pindex_t	vm_pindex_t;
typedef	__vm_size_t	    vm_size_t;

union vm_map_object {
    struct vm_object* vm_object; 	 /* object object */
    struct vm_map* sub_map; 	 	 /* belongs to another map */
};

struct vm_map_entry {
    struct vm_map_entry* prev; 	 /* previous entry */
    struct vm_map_entry* next; 	 /* next entry */
    struct vm_map_entry* left; 	 /* left child in binary search tree */
    struct vm_map_entry* right; 	 /* right child in binary search tree */
    vm_offset_t start; 	 	 /* start address */
    vm_offset_t end; 	 	 /* end address */
    vm_offset_t avail_ssize; 	 /* amt can grow if this is a stack */
    vm_size_t adj_free; 	 	 /* amount of adjacent free space */
    vm_size_t max_free; 	 	 /* max free space in subtree */
    union vm_map_object object; 	 /* object I point to */
    vm_ooffset_t offset; 	 	 /* offset into object */
    vm_eflags_t eflags; 	 	 /* map entry flags */
    vm_prot_t protection; 	 	 /* protection code */
    vm_prot_t max_protection; 	 /* maximum protection */
    vm_inherit_t inheritance; 	 /* inheritance */
    int wired_count; 	 	 /* can be paged if = 0 */
    vm_pindex_t lastr; 	 	 /* last read */
    struct ucred* cred; 	 	 /* tmp storage for creator ref */
    char unk78[0x1]; //budget related?
    char unk79[0x14];
    char name[0x20]; //0x8D
    char unkAD[0x13];
};

struct vm_map {
    struct vm_map_entry header; 	 /* List of entries */
    struct sx lock; 	 	 	 /* Lock for map data (at 0xC0) */
    struct mtx system_mtx; 	 	 	 	 	 	 	 	 // at 0xE0
    int nentries; 	 	 	 /* Number of entries */
    vm_size_t size; 	 	 /* virtual size */
    unsigned int timestamp; 	 /* Version number */
    unsigned char needs_wakeup;
    unsigned char system_map; 	 /* (c) Am I a system map? */
    vm_flags_t flags; 	 	 /* flags for this vm_map */
};

typedef struct vm_map* vm_map_t;

struct vmspace {
    struct vm_map vm_map; 	 /* VM address map */
};