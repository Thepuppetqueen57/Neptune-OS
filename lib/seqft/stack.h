#ifndef _H_STACK
#define _H_STACK

#include <stdint.h>
#include <stdio.h>

#include "common.h"

typedef struct Stack Stack;

#define STACK_DEFAULT_ALLOC 4096

extern Stack* Stack_new(size_t item_size);
extern Stack* Stack_withCapacity(size_t item_size, size_t count);
extern void   Stack_free(Stack* s);

// ----------------------------------------------------------------------------

// Custom deallocator for complex types to be able to deallocate their own
// members if they manage memory. This deallocator should NEVER free the
// item pointer, it should only free fields of the struct that item pointer
// represents. Calling free(item) WILL crash because of double free.
extern void Stack_setDeallocator(Stack* s, void (*deallocator)(void* item));

// Sets the default amount of bytes allocated by the stack to house items.
extern void Stack_setDefaultAlloc(Stack* s, size_t size);

// ----------------------------------------------------------------------------

extern Stack* Stack_expandBy(Stack* s, size_t amount);
extern Stack* Stack_shrinkToFit(Stack* s);

// ----------------------------------------------------------------------------

#define Stack_push(s, v) Stack_pushFrom(s, &(typeof(v)) {v});
extern Stack* Stack_pushFrom(Stack* s, void* item);

// Returns a copy of the last element (caller's responsibility to free), and
// decrements the item count and head of the stack. Does not perform memory
// reallocation; the same memory block remains allocated, the item is just
// not considered anymore, and is overwritten if a new item is pushed.
extern void* Stack_pop(Stack* s);

// Always reallocates Stack to fit smaller size. Does not allocate memory for
// a copy. Will memcpy the popped item to cpyout if non-zero. Provide ptr to
// item being popped to custom deallocator (if present). Won't realloc to 0;
// defaults to default_capacity*item_size if item count reaches 0, and if a
// default_capacity is not specified/zero, defaults to STACK_DEFAULT_ALLOC.
extern void Stack_rePop(Stack* s, void* cpyout);

// This function does not reallocate memory, not is it intended to. It simply
// resets the stack count to 0 and sets the head to the base. To realloc the
// stack, and free its members via the custom deallocator (if set), use the
// Stack_reClear() function instead.
extern void Stack_clear(Stack* s);

// This will pass the items on the stack through the custom deallocator if it
// was provided, and reallocate the stack to size specified by efault_alloc 
// field, if set. If not, defaults to STACK_DEFAULT_ALLOC global macro (4096).
extern void Stack_reClear(Stack* s);

// ----------------------------------------------------------------------------

// Memcpy's all the elements stored in the stack to a newlly allocated void*,
// and writes the void* to the dest void**, and returns the number of elements.
extern size_t Stack_cloneData(Stack* s, void** dest);
extern Stack* Stack_deepClone(Stack* s, void(*item_cloner)(void* item, void* dest));

// ----------------------------------------------------------------------------

extern BOOL  Stack_empty(Stack* s);
extern void* Stack_first(Stack* s);
extern void* Stack_last(Stack* s);
extern void* Stack_itemAt(Stack* s, size_t index);

// ----------------------------------------------------------------------------

extern void*  Stack_getBase(Stack* s);
extern void*  Stack_getHead(Stack* s);
extern size_t Stack_getCount(Stack* s);
extern size_t Stack_getItemSize(Stack* s);
extern size_t Stack_getCapacity(Stack* s);

// Debug.
// ----------------------------------------------------------------------------
extern Stack* Stack_print(Stack* s);

#endif // _H_STACK
