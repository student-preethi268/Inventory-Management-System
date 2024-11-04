/*Write a program to implement heap management scheme.Implement the ‘allocate’ and ‘free’ functions.
 Ensure that adjacent free blocks are merged together.*/
#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 1000  //Define the size of the heap.

//STRUCTURE OF THE BLOCK.
typedef struct Block 
{
    size_t size; //size_t represents the size in Block.
    int free; //BLOCK = Allocated(0) or Free(1)
    struct Block *next;
} Block;

//To provide basis memory managment.
static char heap[HEAP_SIZE];  //Array character represenst HEAP MEMORY.
static Block *head = NULL;    //Head - a ptr to start of Linked List of memory block.

void initialize() //To set a Heap, when the program start.
{
    head = (Block *)heap; //Ptr helps to traversing(in memory block)during allocation and deallocation.
    head->size = HEAP_SIZE - sizeof(Block);
    head->free = 1;
    head->next = NULL;
}
//ALLOCATION FUNCTION = It allows programs to adapt to changing memory requirements and use memory efficiently.
void *allocate(size_t size) 
{
    Block *current = head;
    Block *previous = NULL;
    while (current)  // Traverse the linked list 
    {
        if (current->free && current->size >= size) 
        {
            if (current->size > size + sizeof(Block)) //checks the size of curr memory is > the requested size plus.
            { 
                // Split the block if it's larger than needed
                Block *newBlock = (Block *)((char *)current + sizeof(Block) + size); //caluclating the memory address.
                newBlock->size = current->size - size - sizeof(Block);
                newBlock->free = 1; //future allocation.
                newBlock->next = current->next;
                current->next = newBlock;
                current->size = size;
            }
            current->free = 0;
            return (void *)(current + 1); // Return pointer to the allocated memory
        }
        previous = current;
        current = current->next;
    }
    return NULL;  // If no suitable block is found, return NULL
} 

void freeMemory(void *ptr) 
{
    if (!ptr)
    {
        return;
    }
    Block *toFree = (Block *)ptr - 1;
    toFree->free = 1;  // free blocks = metadata.
    // Merge adjacent free blocks
    Block *current = head;
    while (current) 
    {
        if (current->free && current->next && current->next->free) 
        {
            current->size += sizeof(Block) + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}
void printBlocks() 
{
    Block *current = head;
    int count = 0;
    while (current) 
    {
        printf("Block %d: %lu bytes, %s\n", ++count, current->size, current->free ? "Free" : "Allocated");
        current = current->next;
    }
}

int main() 
{
    initialize();

    void *ptr1 = allocate(200);
    void *ptr2 = allocate(300);

    printf("After allocation:\n");
    printBlocks();

    freeMemory(ptr1);

    printf("\nAfter freeing memory:\n");
    printBlocks();
    return 0;
}
