#pragma once

/**
    * SLIST_HEAD: Defines a structure for the head of a singly-linked list.
    * @name: The name of the head structure.
    * @type: The type of the elements in the list.
*/
#define	SLIST_HEAD(name, type)						\
struct name {								\
	struct type *slh_first;	/* first element */			\
}

/**
    * SLIST_ENTRY: Defines the structure for the list entry in a list element.
    * @type: The type of the elements in the list.
*/
#define SLIST_ENTRY(type)                               \
struct {                                                \
    struct type *sle_next;  /* Pointer to the next item */  \
}

/**
    * SLIST_FIRST: Returns the first element in the list.
 */
#define SLIST_FIRST(head) 	 ((head)->slh_first)

/**
    * SLIST_NEXT: Returns the next element in the list.
    * @elm: The current element.
    * @field: The name of the SLIST_ENTRY field within the structure type.
*/
#define SLIST_NEXT(elm, field) 	 ((elm)->field.sle_next)

/**
    * SLIST_FOREACH: Traverses the list.
    * @var: The temporary variable (of 'type') used for iteration.
    * @head: The head of the list.
    * @field: The name of the SLIST_ENTRY field within the structure type.
*/
#define	SLIST_FOREACH(var, head, field)					\
	for ((var) = SLIST_FIRST((head));				\
	    (var);							\
	    (var) = SLIST_NEXT((var), field))

/**
    * LIST_HEAD: Defines a structure for the head of a doubly-linked list.
    * @name: The name of the head structure.
    * @type: The type of the elements in the list.
*/
#define	LIST_HEAD(name, type)						\
struct name {								\
	struct type *lh_first;	/* first element */			\
}

/**
    * LIST_ENTRY: Defines the structure for the list entry in a list element.
    * @type: The type of the elements in the list.
*/
#define	LIST_ENTRY(type)						\
struct {								\
	struct type *le_next;	/* next element */			\
	struct type **le_prev;	/* address of previous next element */	\
}

/**
    * LIST_FIRST: Returns the first element in the list.
*/
#define	LIST_FIRST(head)	((head)->lh_first)

/**
    * LIST_NEXT: Returns the next element in the list.
    * @elm: The current element.
    * @field: The name of the LIST_ENTRY field within the structure type.
*/
#define	LIST_NEXT(elm, field)	((elm)->field.le_next)

/**
    * LIST_FOREACH: Traverses the list.
    * @var: The temporary variable (of 'type') used for iteration.
    * @head: The head of the list.
    * @field: The name of the LIST_ENTRY field within the structure type.
*/
#define	LIST_FOREACH(var, head, field)					\
	for ((var) = LIST_FIRST((head));				\
	    (var);							\
	    (var) = LIST_NEXT((var), field))

/**
    * STAILQ_HEAD: Defines a structure for the head of a singly-linked tail queue.
    * @name: The name of the head structure.
    * @type: The type of the elements in the queue.
*/
#define	STAILQ_HEAD(name, type)						\
struct name {								\
	struct type *stqh_first;/* first element */			\
	struct type **stqh_last;/* addr of last next element */		\
}

/**
    * TAILQ_HEAD: Defines a structure for the head of a doubly-linked tail queue.
    * @name: The name of the head structure.
    * @type: The type of the elements in the queue.
*/
#define	TAILQ_HEAD(name, type)						\
struct name {								\
	struct type *tqh_first;	/* first element */			\
	struct type **tqh_last;	/* addr of last next element */		\
}

/**
    * TAILQ_ENTRY: Defines the structure for the queue entry in a queue element.
    * @type: The type of the elements in the queue.
*/
#define	TAILQ_ENTRY(type)						\
struct {								\
	struct type *tqe_next;	/* next element */			\
	struct type **tqe_prev;	/* address of previous next element */	\
}

/**
    * TAILQ_EMPTY: Checks if the queue is empty.
*/
#define	TAILQ_EMPTY(head)	((head)->tqh_first == NULL)

/**
    * TAILQ_FIRST: Returns the first element in the queue.
*/
#define	TAILQ_FIRST(head)	((head)->tqh_first)

/**
    * TAILQ_NEXT: Returns the next element in the queue.
    * @elm: The current element.
    * @field: The name of the TAILQ_ENTRY field within the structure type.
*/
#define	TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)

/**
    * TAILQ_FOREACH: Traverses the queue.
    * @var: The temporary variable (of 'type') used for iteration.
    * @head: The head of the queue.
    * @field: The name of the TAILQ_ENTRY field within the structure type.
*/
#define	TAILQ_FOREACH(var, head, field)					\
	for ((var) = TAILQ_FIRST((head));				\
	    (var);							\
	    (var) = TAILQ_NEXT((var), field))
