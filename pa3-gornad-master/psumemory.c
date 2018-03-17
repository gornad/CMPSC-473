#include <assert.h>
#include "psumemory.h"
#include <sys/mman.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

typedef struct __header_t
{
    int size;
    int magic;
} header_t;

typedef struct __node_t
{
    int size;
    struct __node_t* next;
} node_t;

FILE *file_pointer;

int algorithm;
node_t* head;
int sizeOfRegion_global;
struct timespec begin_switch;
struct timespec end_switch;
int not_enough_space = 0;
int external_fragmentation = 0;

int psumeminit(int algo, int sizeofregion)
{

    algorithm = algo;
    sizeOfRegion_global = sizeofregion;
    head = mmap(NULL, sizeofregion, PROT_READ | PROT_WRITE,MAP_ANON | MAP_PRIVATE, -1, 0);
    head->size = sizeofregion - sizeof(node_t);
    head->next = NULL;

    return 0;
}

// Merges two free memory regions so that their combined size can be used effectively
void coalesce()
{
    node_t* current = head;

    while(current != NULL)
    {
        node_t* check = head;
	node_t* prev = head;
	// Point to correct next
        node_t* next = (node_t*)((char*)current + current->size + sizeof(node_t));

        while(check != NULL)
        {
	    // Check if pointer is reached, if not advance
            if(check != next)
            {
                prev = check;
                check = check->next;
            }

            else
            {
		prev->next = check->next;
                current->size += (check->size + sizeof(node_t));
                next = (node_t*)((char*)current + sizeof(node_t) + current->size);

                if(check != head)
                {
		    // If border node detected
                    check->next = NULL;
                    check = prev->next;

                }
                else
                {
                    head = head->next;
                    check->next = NULL;
                    check = head;
                }
            }
        }

        current = current->next;
    }
}

// Splits the free space when malloc'ing
void* split(node_t* ptr, node_t* prev, int size)
{
    int minimum = (ptr->size - size + sizeof(node_t)) - sizeof(header_t);

    header_t* header = (header_t*)(ptr);

    // Node does not have space
    if(minimum >= sizeof(node_t))
    {    struct timespec begin_switch;
    struct timespec end_switch;

        node_t cpy = (*ptr);

        // Shift forward
        ptr = (node_t*)((char*)ptr + size + sizeof(header_t));
        ptr->size = minimum - sizeof(node_t);
        ptr->next = cpy.next;

    }

    else
    {
	// Increase the requested space as needed and look for another free slot
        size += minimum;
        ptr = ptr->next;
    }

    // Edge case if prev==NULL
    if(prev)
        prev->next = ptr;
    else
	// Reallocate head
        head = ptr;

    header->size = size;
    header->magic = 1234567;

    return ((char*)header + sizeof(header_t));
}

void* psumalloc(int size)
{
    //file_pointer = fopen("malloc_data", "a+");
    //clock_gettime(CLOCK_MONOTONIC, &begin_switch);

    header_t* magic_check;
    void* returning;
    int maxmin_space;
    int total_space = 0;
    node_t* temp = head, *prev = NULL, *fitptr = NULL, *fitprev = NULL;


    // BEST-FIT
    if(algorithm == 0)  
    {
	// Variable size that changes according to algorithm
        maxmin_space = sizeOfRegion_global;

        while(temp != NULL)
        {
            int remainingSize = (temp->size -size) + (sizeof(node_t) - sizeof(header_t));

            if((remainingSize >=0) && remainingSize <= maxmin_space)
            {
                maxmin_space = remainingSize;
                fitptr = temp;
                fitprev = prev;
            }
	    
	    // Check for magic number for failure/fragmentation tests
	    magic_check = (header_t*)fitptr;

            if(magic_check!= NULL && magic_check->magic - 1234567 != 0) {
		total_space += magic_check->size;
	    }

            prev = temp;

            // Advance
            temp = temp->next;
        }

    }

    // WORST-FIT
    else if(algorithm == 1)
    {
        maxmin_space = 0;

        while(temp != NULL)
        {
            int remainingSize = (temp->size -size) + (sizeof(node_t) - sizeof(header_t));

            if(remainingSize > maxmin_space)
            {

                maxmin_space = remainingSize;
                fitptr = temp;
                fitprev = prev;
            }

	    // Check for magic number for failure/fragmentation tests
	    magic_check = (header_t*)fitptr;

            if(magic_check!= NULL && magic_check->magic - 1234567 != 0) {
		total_space += magic_check->size;
	    }
	    
            prev = temp;
	    
 	    // Advance
            temp = temp->next;
        }

    }


    // Failure
    if(!fitptr) {
	    printf("No free block found");
	    if(size > total_space) {
		not_enough_space++;
		printf("not_enough_space: %d \n", not_enough_space);
	    }

	    else if(size <= total_space) {
		external_fragmentation++;
		printf("external_fragmentation: %d \n", external_fragmentation);
	    }		
            return NULL;
    }
    
    // Perform needed split
    returning = split(fitptr, fitprev, size);

    // Code used for time measurments
    //clock_gettime(CLOCK_MONOTONIC, &end_switch);
    //fprintf(file_pointer, "%lld \n", ((long long)end_switch.tv_nsec - (long long)begin_switch.tv_nsec));
    //fclose(file_pointer);
    return returning;
}


int psufree(void* ptr)
{
    //file_pointer = fopen("free_data", "a+");
    //clock_gettime(CLOCK_MONOTONIC, &begin_switch);
    node_t* temp;

    if(!ptr)
        return -1;

    // Correct location
    ptr =(void*)((char*)ptr-sizeof(header_t));

    if(((header_t*)ptr)->magic != 1234567)
        return -1;

    // Perform correct size allocation
    temp = (node_t*)ptr;
    temp->size = sizeof(header_t) + ((header_t*)ptr)->size - sizeof(node_t);
    temp->next = head;
    head = temp;

    // Merge
    coalesce();

    // Code used for time measurments
    //clock_gettime(CLOCK_MONOTONIC, &end_switch);
    //fprintf(file_pointer, "%lld \n", ((long long)end_switch.tv_nsec - (long long)begin_switch.tv_nsec));
    //fclose(file_pointer);
    return 0;
}

void psumemdump()
{
    // print free list contents
    node_t* current = head;

    while(current != NULL)
    {
        printf("Node: %p\n Next: %p\n\n Size: %d\n", current, current->next, current->size);

        current = current->next;
    }
}


