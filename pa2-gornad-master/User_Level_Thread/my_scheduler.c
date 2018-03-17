#include "threads.h"
extern int scheduling_type;
extern thread_queue_t *ready_list;

void InsertAtHead(thread_t *t, thread_queue_t *q) {
	thread_node_t *node = malloc(sizeof(thread_node_t));
	node->thread = t;
	node->next = q->head;
	q->head = node;
	if(q->tail == NULL)
		q->tail = node;
	q->size++;
}

void thread_enqueue(thread_t *t, thread_queue_t *q){
	thread_node_t *node = malloc(sizeof(thread_node_t));
	node->thread = t;
	node->next = NULL;
	if (q->tail != NULL)
		q->tail->next = node;
	
	else 
		q->head = node;
	q->tail = node;
	q->size++;
}

thread_t* scheduler() {
	int total_weight = 0, cumulative_weight = 0, winner;
	thread_node_t *node;
	if(ready_list->size == 0)
		return NULL;
	node = ready_list->head;
	switch(scheduling_type)	{
		case 0:          // Round Robin
			node = ready_list->head;
			if(node->next != NULL) 
				ready_list->head = node->next; // Advance head node
			return node->thread;	// Return previous thread of head
			
		case 1:          // Lottery (code similar to book)
			while(node!=NULL) {   // Iterate and calculate total weight
				total_weight += node->thread->priority;
				node = node->next;
			}
			node = ready_list->head; // Reassign node to start of list
			winner = random() % total_weight; // Generate winner value
			while(node!=NULL) {
				cumulative_weight += node->thread->priority; 
				if (cumulative_weight > winner)
					break; // Found the winner
	    			node = node->next;
			}
			
			return node->thread;

		case 2:          //First come first serve
			return ready_list->head->thread;
		default:
			return NULL;
	}
}
