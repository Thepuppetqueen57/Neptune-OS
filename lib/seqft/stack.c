#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "stack.h"

typedef struct Stack {
    void*  base;
    void*  head;
    size_t count;
    size_t item_size;
    size_t allocated;
    size_t capacity;
    size_t default_alloc;

    // Custom deallocator for complex types to be able to deallocate their own
    // members if they manage memory. This deallocator should NEVER free the
    // item pointer, it should only free fields of the struct that item pointer
    // represents. Calling free(item) WILL crash because of double free. This
    // field is set via Stack_setDeallocator.
    void (*deallocator)(void* item);
} Stack;

// ---------------------------------------------------------------------------- 
//
Stack* Stack_new(size_t item_size) {
    Stack* s = (Stack*)xmalloc(sizeof(Stack));
    memset(s, 0, sizeof(Stack));

    s->item_size = item_size;

    return s;
}

Stack* Stack_withCapacity(size_t item_size, size_t capacity) {
    Stack* s = (Stack*)xmalloc(sizeof(Stack));
    memset(s, 0, sizeof(Stack));

    size_t required_alloc = item_size * capacity;
    s->base               = xmalloc(required_alloc);
    s->head               = s->base;
    s->item_size          = item_size;
    s->allocated          = required_alloc;
    s->capacity           = capacity;
    s->default_alloc      = required_alloc;

    return s;
}

void Stack_free(Stack* s) {
    if(!s)
        return;

    if(s->base) {
        if(s->deallocator) {
            for(int i = 0; i < Stack_getCount(s); ++i) {
                void* item = Stack_itemAt(s, i);
                s->deallocator(item);
            }
        }

        free(s->base);
    }

    free(s);
}

// ----------------------------------------------------------------------------

void Stack_setDeallocator(Stack* s, void (*deallocator)(void* item)) {
    s->deallocator = deallocator;
}

void Stack_setDefaultAlloc(Stack* s, size_t size) {
    s->default_alloc = size;
}

// ----------------------------------------------------------------------------

// The amount is the amount of additional *items* the stack stack should have
// enough space to store. The new base pointer of the stack is returned.
// 0 will not result in a reallocation and return the existing base.
Stack* Stack_expandBy(Stack* s, size_t amount) {
    size_t bytes_required = (s->count + amount) * s->item_size;
    void*  new_base       = xrealloc(s->base, bytes_required);

    s->base      = new_base;
    s->allocated = bytes_required;
    s->head      = Stack_itemAt(s, s->count - 1);

    return s;
}
Stack* Stack_shrinkToFit(Stack* s) {
    Stack_expandBy(s, 0);
    return s;
}

// ----------------------------------------------------------------------------

Stack* Stack_pushFrom(Stack* s, void* item) {
    if(((s->count + 1) * s->item_size) > s->allocated) {
        Stack_expandBy(s, 1);
    }

    void* dest = s->base + (s->item_size * s->count);
    memcpy(dest, item, s->item_size);

    s->head = dest;
    s->count += 1;
    return s;
}

// Reduces the item count by 1, and returns a copy of the element previously
// at the head of the stack. If the item count is 0, return 0.
void* Stack_pop(Stack* s) {
    if(s->count == 0)
        return 0;

    void* item_copy = csrxmalloc(s->item_size);
    memcpy(item_copy, s->head, s->item_size);

    s->head -= s->item_size;
    s->count -= 1;

    return item_copy;
}

// Always reallocates Stack to fit smaller size. Does not allocate memory for
// a copy. Will memcpy the popped item to cpyout if non-zero. Provide ptr to
// item being popped to custom deallocator (if present). Won't realloc to 0;
// defaults to default_capacity*item_size if item count reaches 0, and if a
// default_capacity is not specified/zero, defaults to STACK_DEFAULT_ALLOC.
void Stack_rePop(Stack* s, void* cpyout) {
    if(!s || !s->base)
        return;

    printf("repop");
    if(s->count == 0) {
        return; // Nothing to pop.
    }

    if(cpyout) {
        // Write out a copy if caller requests it.
        memcpy(cpyout, s->head, s->item_size);
    }

    if(s->deallocator) {
        s->deallocator(s->head);
    }

    s->count -= 1;

    size_t required_alloc = s->count * s->item_size;

    if(required_alloc == 0) {
        required_alloc =
            s->default_alloc ? s->default_alloc : STACK_DEFAULT_ALLOC;
    }

    size_t head_delta = s->head - s->base;

    s->base      = xrealloc(s->base, required_alloc);
    s->head      = (s->base + head_delta) - s->item_size;
    s->allocated = required_alloc;
}

// This function does not reallocate memory, not is it intended to. It simply
// resets the stack count to 0 and sets the head to the base. To realloc the
// stack, and free its members via the custom deallocator (if set), use the
// Stack_reClear() function instead.
void Stack_clear(Stack* s) {
    if(!s || !s->base)
        return;

    s->count = 0;
    s->head  = s->base;
}

// This will pass the items on the stack through the custom deallocator if it
// was provided, and reallocate the stack to size specified by efault_alloc
// field, if set. If not, defaults to STACK_DEFAULT_ALLOC global macro (4096).
void Stack_reClear(Stack* s) { if(!s || !s->base)
        return;

    if(s->deallocator) {
        for(int i = 0; i < s->count; ++i) {
            s->deallocator(Stack_itemAt(s, i));
        }
    }

    size_t required_alloc =
        s->default_alloc ? s->default_alloc : STACK_DEFAULT_ALLOC;

    // s->base      = xrealloc(s->base, required_alloc);

    free(s->base);
    s->base = xmalloc(required_alloc);

    s->allocated = required_alloc;
    s->count     = 0;
    s->head      = s->base;
    s->capacity =
        s->allocated && s->item_size ? s->allocated / s->item_size : 0;
}

// ---------------------------------------------------------------------------- 

size_t Stack_cloneData(Stack* s, void** dest) {
    size_t size = s->count * s->item_size;

    void* clone = xmalloc(size);
    memcpy(clone, s->base, size);

    *dest = clone;
    return s->count;
}

Stack* Stack_deepClone(Stack* s, void (*item_cloner)(void* item, void* dest)) {
    Stack* clone         = (Stack*)csrxmalloc(sizeof(Stack));

    clone->count         = s->count;
    clone->item_size     = s->item_size;
    clone->allocated     = s->allocated;
    clone->capacity      = s->capacity;
    clone->default_alloc = s->default_alloc;
    clone->deallocator   = s->deallocator;

    clone->base = xmalloc(s->allocated);
    clone->head = clone->base + (s->head - s->base);

    if(item_cloner) {
        for(int i = 0; i < s->count; ++i) {
            void* item = Stack_itemAt(s, i);
            item_cloner(item, clone->base + (i * clone->item_size));
        }
    } else {
      memcpy(clone->base, s->base, s->allocated);
    }

    return clone;
}

// ----------------------------------------------------------------------------

BOOL Stack_empty(Stack* s) {
    return s->count == 0;
}

void* Stack_first(Stack* s) {
    if(s->count == 0)
        return 0;
    return s->base;
}

void* Stack_last(Stack* s) {
    if(s->count == 0)
        return 0;
    return s->head;
}

void* Stack_itemAt(Stack* s, size_t index) {
    if(!s->count || index >= s->count)
        return 0;

    return s->base + (index * s->item_size);
}

// ----------------------------------------------------------------------------

inline void* Stack_getBase(Stack* s) {
    return s->base;
}

inline void* Stack_getHead(Stack* s) {
    return s->head;
}

inline size_t Stack_getCount(Stack* s) {
    return s->count;
}

inline size_t Stack_getItemSize(Stack* s) {
    return s->item_size;
}

inline size_t Stack_getCapacity(Stack* s) {
    return s->allocated;
}

// ----------------------------------------------------------------------------

Stack* Stack_print(Stack* s) {
    printf("| Item Count: %zu | Item Size: %zu | Capacity: %zu | Allocated: "
           "%zu |\n",
           s->count,
           s->item_size,
           s->allocated / (s->item_size != 0 ? s->item_size : 1),
           s->allocated);

    printf("-------------------------------------------------------------\n");

    for(int i = 0; i < s->count; ++i) {
        void* item_location = s->base + (s->item_size * i);

        printf("%d: |", i);

        // Print item_size number of bytes in hex for each item.
        for(int b = 0; b < s->item_size; ++b) {
            char* byte = item_location + b;
            printf("%02x|", *byte);
        }

        printf("\n");
    }

    printf("\n\n");
    return s;
}
