#pragma once

#define	EVENTHANDLER_PRI_FIRST	0
#define	EVENTHANDLER_PRI_ANY	10000
#define	EVENTHANDLER_PRI_LAST	20000

struct eventhandler_list {
	char* el_name;
	int	 el_flags;
#define EHL_INITTED	(1<<0)
	unsigned int el_runcount;
	mtx	el_lock;
	TAILQ_ENTRY(eventhandler_list) el_link;
	TAILQ_HEAD(, eventhandler_entry) el_entries;
};

struct eventhandler_entry {
	TAILQ_ENTRY(eventhandler_entry)	ee_link;
	int				ee_priority;
#define	EHE_DEAD_PRIORITY	(-1)
	void* ee_arg;
};

typedef struct eventhandler_entry* eventhandler_tag;