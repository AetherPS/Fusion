#pragma once

struct lock_object 
{
    const char* lo_name;    // Name of the lock (for diagnostics)
    uint32_t lo_flags;      // Flags associated with the lock
    uint32_t lo_data;       // Generic data field
    void* lo_witness;       // Witness protocol support pointer
};

struct mtx
{
    struct lock_object lock_object;  /* Common lock properties. */
    volatile uintptr_t mtx_lock; 	 /* Owner and flags (volatile for concurrent access). */
};

struct sx {
    struct lock_object lock_object;
    volatile uintptr_t sx_lock;
};