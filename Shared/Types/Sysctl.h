#pragma once

struct sysctl_oid;

/*
 * SLIST_ENTRY for sysctl_oid
 * This is the linked list node structure
 */
struct sysctl_oid_list_entry {
    struct sysctl_oid* sle_next;  /* next element */
};

/*
 * SLIST_HEAD for sysctl_oid_list
 * This is the list head structure
 */
struct sysctl_oid_list {
    struct sysctl_oid* slh_first; /* first element */
};

struct sysctl_req {
    struct thread* td;        /* Calling thread (do you have struct thread?) */
    int              lock;      /* Locking state */
    void* oldptr;    /* Pointer to old (current) value buffer */
    size_t           oldlen;    /* Size of old buffer */
    size_t           oldidx;    /* Current index in old buffer */
    int            (*oldfunc)(struct sysctl_req*, const void*, size_t);
    void* newptr;    /* Pointer to new value buffer */
    size_t           newlen;    /* Size of new buffer */
    size_t           newidx;    /* Current index in new buffer */
    int            (*newfunc)(struct sysctl_req*, void*, size_t);
    size_t           validlen;  /* Valid data length */
    int              flags;     /* Request flags */
};

struct sysctl_oid {
    struct sysctl_oid_list* oid_parent;    /* 0x00: Parent's child list */
    struct sysctl_oid_list_entry oid_link;      /* 0x08: Linked list entry (8 bytes) */
    int                          oid_number;    /* 0x10: OID number */
    int                          oid_kind;      /* 0x14: Type and flags */
    void* oid_arg1;      /* 0x18: First argument */
    intptr_t                     oid_arg2;      /* 0x20: Second argument */
    const char* oid_name;      /* 0x28: Node name string */
    int                        (*oid_handler)(  /* 0x30: Handler function */
        struct sysctl_oid* oidp,
        void* arg1,
        intptr_t arg2,
        struct sysctl_req* req);
    const char* oid_fmt;       /* 0x38: Format string */
    int                          oid_refcnt;    /* 0x40: Reference count */
    uint32_t                        oid_running;   /* 0x44: Handler running count */
    const char* oid_descr;     /* 0x48: Description string */
	int oid_allowed;
	int oid_unknown;
};

struct sysctl_ctx_entry {
    struct sysctl_oid* entry;
    struct sysctl_ctx_entry* link_next;
};

struct sysctl_ctx_list {
    struct sysctl_ctx_entry* slh_first;
};

/*
 * CTLTYPE - Type field in oid_kind
 * Defines what type of data this sysctl holds
 */
#define CTLTYPE             0xf         /* Mask for type */
#define CTLTYPE_NODE        1           /* Node (directory/parent) */
#define CTLTYPE_INT         2           /* Integer (32-bit) */
#define CTLTYPE_STRING      3           /* String */
#define CTLTYPE_S64         4           /* Signed 64-bit integer */
#define CTLTYPE_OPAQUE      5           /* Opaque data structure */
#define CTLTYPE_STRUCT      CTLTYPE_OPAQUE  /* Alias */
#define CTLTYPE_UINT        6           /* Unsigned integer (32-bit) */
#define CTLTYPE_LONG        7           /* Long integer */
#define CTLTYPE_ULONG       8           /* Unsigned long integer */
#define CTLTYPE_U64         9           /* Unsigned 64-bit integer */

 /*
  * CTLFLAG - Access flags in oid_kind
  * Defines permissions and behavior
  */
#define CTLFLAG_RD          0x80000000  /* Read-only */
#define CTLFLAG_WR          0x40000000  /* Write-only */
#define CTLFLAG_RW          (CTLFLAG_RD | CTLFLAG_WR)  /* Read-write */
#define CTLFLAG_ANYBODY     0x10000000  /* Anyone can set */
#define CTLFLAG_SECURE      0x08000000  /* Permit set only if securelevel <= 0 */
#define CTLFLAG_PRISON      0x04000000  /* Prison root can set */
#define CTLFLAG_DYN         0x02000000  /* Dynamically allocated */
#define CTLFLAG_SKIP        0x01000000  /* Skip in sysctl listings */
#define CTLFLAG_TUN         0x00080000  /* Tunable variable */
#define CTLFLAG_RDTUN       (CTLFLAG_RD | CTLFLAG_TUN)  /* Read-only tunable */
#define CTLFLAG_RWTUN       (CTLFLAG_RW | CTLFLAG_TUN)  /* Read-write tunable */
#define CTLFLAG_MPSAFE      0x00040000  /* Handler is MP safe */
#define CTLFLAG_VNET        0x00080000  /* VNET jail-aware */
#define CTLFLAG_DYING       0x00010000  /* Node is being removed */
#define CTLFLAG_CAPRD       0x00020000  /* Can read in capability mode */
#define CTLFLAG_CAPWR       0x00040000  /* Can write in capability mode */
#define CTLFLAG_STATS       0x00800000  /* Statistics */
#define CTLFLAG_NOFETCH     0x01000000  /* Don't fetch tunable from env */

/*
* Special OID numbers
*/
#define OID_AUTO            (-1)        /* Auto-assign OID number */
#define OID_ROOT            0           /* Root node */

/*
* sysctl_req flags
*/
#define SYSCTL_LOCK_HELD    0x0001      /* Lock is held */
#define SYSCTL_LOCK_SLEEP   0x0002      /* Can sleep for lock */

/*
 * Type definitions for sysctl handler functions
 */

 /* Standard handler signature (SYSCTL_HANDLER_ARGS expanded) */
typedef int (*sysctl_handler_t)(struct sysctl_oid* oidp, void* arg1,
    intptr_t arg2, struct sysctl_req* req);

/* Specific handler types for kernel functions you'll need to resolve */
typedef int (*sysctl_handle_int_t)(struct sysctl_oid*, void*,
    intptr_t, struct sysctl_req*);
typedef int (*sysctl_handle_long_t)(struct sysctl_oid*, void*,
    intptr_t, struct sysctl_req*);
typedef int (*sysctl_handle_string_t)(struct sysctl_oid*, void*,
    intptr_t, struct sysctl_req*);
typedef int (*sysctl_handle_opaque_t)(struct sysctl_oid*, void*,
    intptr_t, struct sysctl_req*);

#define SYSCTL_HANDLER_ARGS struct sysctl_oid *oidp, void *arg1,	\
	intptr_t arg2, struct sysctl_req *req

/* Helper macros */
#define __DESCR(x) x
#define SYSCTL_ADD_ASSERT_TYPE(type, ptr) (ptr)
#define SYSCTL_ASSERT_TYPE(type, ptr, parent, name)

/* Macro to get children list from a sysctl_oid */
#define SYSCTL_CHILDREN(oidp) ((struct sysctl_oid_list *)(oidp)->oid_arg1)

/* This constructs a "raw" MIB oid. */
#define SYSCTL_OID(parent, nbr, name, kind, a1, a2, handler, fmt, descr) \
	static struct sysctl_oid sysctl__##parent##_##name = {		 \
		&sysctl_##parent##_children, { NULL }, nbr, kind,	 \
		a1, a2, #name, handler, fmt, 0, 0, __DESCR(descr) };	 \
	DATA_SET(sysctl_set, sysctl__##parent##_##name)

#define SYSCTL_ADD_OID(ctx, parent, nbr, name, kind, a1, a2, handler, fmt, descr) \
	sysctl_add_oid(ctx, parent, nbr, name, kind, a1, a2, handler, fmt, __DESCR(descr))

/* This constructs a node from which other oids can hang. */
#define SYSCTL_NODE(parent, nbr, name, access, handler, descr)		    \
	struct sysctl_oid_list SYSCTL_NODE_CHILDREN(parent, name);	    \
	SYSCTL_OID(parent, nbr, name, CTLTYPE_NODE|(access),		    \
	    (void*)&SYSCTL_NODE_CHILDREN(parent, name), 0, handler, "N", descr)

#define SYSCTL_ADD_NODE(ctx, parent, nbr, name, access, handler, descr)	    \
	sysctl_add_oid(ctx, parent, nbr, name, CTLTYPE_NODE|(access),	    \
	NULL, 0, handler, "N", __DESCR(descr))

/* Oid for a string.  len can be 0 to indicate '\0' termination. */
#define SYSCTL_STRING(parent, nbr, name, access, arg, len, descr) \
	SYSCTL_OID(parent, nbr, name, CTLTYPE_STRING|(access), \
		arg, len, sysctl_handle_string, "A", descr)

#define SYSCTL_ADD_STRING(ctx, parent, nbr, name, access, arg, len, descr)  \
	sysctl_add_oid(ctx, parent, nbr, name, CTLTYPE_STRING|(access),	    \
	arg, len, sysctl_handle_string, "A", __DESCR(descr))

/* Oid for an int.  If ptr is NULL, val is returned. */
#define	SYSCTL_INT(parent, nbr, name, access, ptr, val, descr)		\
	SYSCTL_ASSERT_TYPE(INT, ptr, parent, name);			\
	SYSCTL_OID(parent, nbr, name,					\
	    CTLTYPE_INT | CTLFLAG_MPSAFE | (access),			\
	    ptr, val, sysctl_handle_int, "I", descr)

#define	SYSCTL_ADD_INT(ctx, parent, nbr, name, access, ptr, val, descr)	\
	sysctl_add_oid(ctx, parent, nbr, name,				\
	    CTLTYPE_INT | CTLFLAG_MPSAFE | (access),			\
	    SYSCTL_ADD_ASSERT_TYPE(INT, ptr), val,			\
	    sysctl_handle_int, "I", __DESCR(descr))

/* Oid for an unsigned int.  If ptr is NULL, val is returned. */
#define	SYSCTL_UINT(parent, nbr, name, access, ptr, val, descr)		\
	SYSCTL_ASSERT_TYPE(UINT, ptr, parent, name);			\
	SYSCTL_OID(parent, nbr, name,					\
	    CTLTYPE_UINT | CTLFLAG_MPSAFE | (access),			\
	    ptr, val, sysctl_handle_int, "IU", descr)

#define	SYSCTL_ADD_UINT(ctx, parent, nbr, name, access, ptr, val, descr) \
	sysctl_add_oid(ctx, parent, nbr, name,				\
	    CTLTYPE_UINT | CTLFLAG_MPSAFE | (access),			\
	    SYSCTL_ADD_ASSERT_TYPE(UINT, ptr), val,			\
	    sysctl_handle_int, "IU", __DESCR(descr))

/* Oid for a long.  The pointer must be non NULL. */
#define	SYSCTL_LONG(parent, nbr, name, access, ptr, val, descr)		\
	SYSCTL_ASSERT_TYPE(LONG, ptr, parent, name);			\
	SYSCTL_OID(parent, nbr, name,					\
	    CTLTYPE_LONG | CTLFLAG_MPSAFE | (access),			\
	    ptr, val, sysctl_handle_long, "L", descr)

#define	SYSCTL_ADD_LONG(ctx, parent, nbr, name, access, ptr, descr)	\
	sysctl_add_oid(ctx, parent, nbr, name,				\
	    CTLTYPE_LONG | CTLFLAG_MPSAFE | (access),			\
	    SYSCTL_ADD_ASSERT_TYPE(LONG, ptr), 0,			\
	    sysctl_handle_long,	"L", __DESCR(descr))

/* Oid for an unsigned long.  The pointer must be non NULL. */
#define	SYSCTL_ULONG(parent, nbr, name, access, ptr, val, descr)	\
	SYSCTL_ASSERT_TYPE(ULONG, ptr, parent, name);			\
	SYSCTL_OID(parent, nbr, name,					\
	    CTLTYPE_ULONG | CTLFLAG_MPSAFE | (access),			\
	    ptr, val, sysctl_handle_long, "LU", descr)

#define	SYSCTL_ADD_ULONG(ctx, parent, nbr, name, access, ptr, descr)	\
	sysctl_add_oid(ctx, parent, nbr, name,				\
	    CTLTYPE_ULONG | CTLFLAG_MPSAFE | (access),			\
	    SYSCTL_ADD_ASSERT_TYPE(ULONG, ptr), 0,			\
	    sysctl_handle_long, "LU", __DESCR(descr))

/* Oid for a quad.  The pointer must be non NULL. */
#define	SYSCTL_QUAD(parent, nbr, name, access, ptr, val, descr)		\
	SYSCTL_ASSERT_TYPE(INT64, ptr, parent, name);			\
	SYSCTL_OID(parent, nbr, name,					\
	    CTLTYPE_S64 | CTLFLAG_MPSAFE | (access),			\
	    ptr, val, sysctl_handle_64, "Q", descr)

#define	SYSCTL_ADD_QUAD(ctx, parent, nbr, name, access, ptr, descr)	\
	sysctl_add_oid(ctx, parent, nbr, name,				\
	    CTLTYPE_S64 | CTLFLAG_MPSAFE | (access),			\
	    SYSCTL_ADD_ASSERT_TYPE(INT64, ptr), 0,			\
	    sysctl_handle_64, "Q", __DESCR(descr))

#define	SYSCTL_UQUAD(parent, nbr, name, access, ptr, val, descr)	\
	SYSCTL_ASSERT_TYPE(UINT64, ptr, parent, name);			\
	SYSCTL_OID(parent, nbr, name,					\
	    CTLTYPE_U64 | CTLFLAG_MPSAFE | (access),			\
	    ptr, val, sysctl_handle_64, "QU", descr)

#define	SYSCTL_ADD_UQUAD(ctx, parent, nbr, name, access, ptr, descr)	\
	sysctl_add_oid(ctx, parent, nbr, name,				\
	    CTLTYPE_U64 | CTLFLAG_MPSAFE | (access),			\
	    SYSCTL_ADD_ASSERT_TYPE(UINT64, ptr), 0,			\
	    sysctl_handle_64, "QU", __DESCR(descr))

/* Oid for an opaque object.  Specified by a pointer and a length. */
#define SYSCTL_OPAQUE(parent, nbr, name, access, ptr, len, fmt, descr) \
	SYSCTL_OID(parent, nbr, name, CTLTYPE_OPAQUE|(access), \
		ptr, len, sysctl_handle_opaque, fmt, descr)

#define SYSCTL_ADD_OPAQUE(ctx, parent, nbr, name, access, ptr, len, fmt, descr)\
	sysctl_add_oid(ctx, parent, nbr, name, CTLTYPE_OPAQUE|(access),	    \
	ptr, len, sysctl_handle_opaque, fmt, __DESCR(descr))

/* Oid for a struct.  Specified by a pointer and a type. */
#define SYSCTL_STRUCT(parent, nbr, name, access, ptr, type, descr) \
	SYSCTL_OID(parent, nbr, name, CTLTYPE_OPAQUE|(access), \
		ptr, sizeof(struct type), sysctl_handle_opaque, \
		"S," #type, descr)

#define SYSCTL_ADD_STRUCT(ctx, parent, nbr, name, access, ptr, type, descr) \
	sysctl_add_oid(ctx, parent, nbr, name, CTLTYPE_OPAQUE|(access),	    \
	ptr, sizeof(struct type), sysctl_handle_opaque, "S," #type, __DESCR(descr))

/* Oid for a procedure.  Specified by a pointer and an arg. */
#define SYSCTL_PROC(parent, nbr, name, access, ptr, arg, handler, fmt, descr) \
	CTASSERT(((access) & CTLTYPE) != 0);				\
	SYSCTL_OID(parent, nbr, name, (access), \
		ptr, arg, handler, fmt, descr)

#define SYSCTL_ADD_PROC(ctx, parent, nbr, name, access, ptr, arg, handler, fmt, descr) \
	sysctl_add_oid(ctx, parent, nbr, name, (access),			    \
	ptr, arg, handler, fmt, __DESCR(descr))
