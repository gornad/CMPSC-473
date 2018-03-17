#include "driver.h"

void *disk_ops(void *arg)
{
  int *algo = arg;

  while(num_request_served < n) {
    /* Task 3.3 - EC */
    clock_gettime(CLOCK_MONOTONIC, &beginWait);
    clock_gettime(CLOCK_MONOTONIC, &currentWait);

    while(buff_count < limit && (currentWait.tv_sec - beginWait.tv_sec) < TIME) { //Spin lock + Timer
        sem_post(&empty_buffer);    // signal the empty buffer
        clock_gettime(CLOCK_MONOTONIC, &currentWait);
    }

    printf("Transferring buffer to the disk\n");
    new_buffer_head = b_head;
    b_head = b_tail = NULL;
    buff_count = 0;

    if(*algo == 0); //do nothing - ie. don't sort
    else if(*algo == 1) {
      // sort based on head direction
      MergeSort(&new_buffer_head);

      /* Change sort direction */
      if(direction == 0) 
        direction = 1;
      else 
        direction = 0;
    }

    buffer_node* temp_head = new_buffer_head;
    int y;
    while(temp_head!=NULL) {
      /* Variables for the service time */
      struct timespec s_begin;
      struct timespec s_end;
      /* Request received, capture the time */
      clock_gettime(CLOCK_MONOTONIC, &s_begin);

      /* Get the new head position, simulate movement, then move */
      y = temp_head->sector_number;
      int movement =  abs(sectorToTrack(y) - sectorToTrack(disk_head));
      usleep(AVG_ROT_DELAY + movement);
      printf("##DISK OPS##\tHead Movement: %d\n",movement);
      disk_head = temp_head->sector_number;
	    // END position change

      if(strcmp(temp_head->op_name,"READ") == 0) { /* READ */
        printf("##DISK OPS##\tDISK READ\n" );
        if(temp_head->sector_number >= SECTORS*TRACKS )
          printf("##DISK OPS##\tBAD SECTOR\n");
        else
          printf("##DISK OPS##\tREAD %d from SECTOR %d [Track %d]\n", disk[y], y, sectorToTrack(y) + 1);
      } else { /* WRITE */
        if(temp_head->sector_number >= SECTORS*TRACKS )
          printf("##DISK OPS##\tBAD SECTOR\n");
        else {
          disk[y] = temp_head->data;
          printf("##DISK OPS##\tWROTE %d into SECTOR %d [Track %d]\n", disk[y], y, sectorToTrack(y) + 1);
        }
      }

      /* Get the end time and display the service time */
      clock_gettime(CLOCK_MONOTONIC, &s_end);
      printf("##DISK OPS##\tRequest at sector: %d took %ld.%ld\n", temp_head->req_id, (s_end.tv_sec-s_begin.tv_sec), (s_end.tv_nsec-s_begin.tv_nsec));
      
      temp_head->req_id = MAGIC;   /* request is done! */
      temp_head = temp_head->next;
      num_request_served++;
    }  
  }
}

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(buffer_node** headRef)
{
  buffer_node* head = *headRef;
  buffer_node* a;
  buffer_node* b;
 
  /* Base case -- length 0 or 1 */
  if ((head == NULL) || (head->next == NULL))
  {
    return;
  }
 
  /* Split head into 'a' and 'b' sublists */
  FrontBackSplit(head, &a, &b); 
 
  /* Recursively sort the sublists */
  MergeSort(&a);
  MergeSort(&b);
 
  /* answer = merge the two sorted lists together */
  *headRef = SortedMerge(a, b);
}
 
/* See http://www.geeksforgeeks.org/?p=3622 for details of this 
   function */
buffer_node* SortedMerge(buffer_node* a, buffer_node* b)
{
  buffer_node* result = NULL;
 
  /* Base cases */
  if (a == NULL)
     return(b);
  else if (b==NULL)
     return(a);
  
  if(direction == 0) { /* SORT SMALL TO LARGE */
    /* Pick either a or b, and recur */
    if (a->sector_number <= b->sector_number) {
      result = a;
      result->next = SortedMerge(a->next, b);
    } else {
      result = b;
      result->next = SortedMerge(a, b->next);
    }
  } else { /* SORT LARGE TO SMALL */
    /* Pick either a or b, and recur */
    if (a->sector_number >= b->sector_number) {
      result = a;
      result->next = SortedMerge(a->next, b);
    } else {
      result = b;
      result->next = SortedMerge(a, b->next);
    }
  }
  return(result);
}
 
/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
     and return the two lists using the reference parameters.
     If the length is odd, the extra node should go in the front list.
     Uses the fast/slow pointer strategy.  */
void FrontBackSplit(buffer_node* source, buffer_node** frontRef, buffer_node** backRef) {
  buffer_node* fast;
  buffer_node* slow;
  if (source==NULL || source->next==NULL)
  {
    /* length < 2 cases */
    *frontRef = source;
    *backRef = NULL;
  }
  else
  {
    slow = source;
    fast = source->next;
 
    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL)
    {
      fast = fast->next;
      if (fast != NULL)
      {
	slow = slow->next;
	fast = fast->next;
      }
    }
 
    /* 'slow' is before the midpoint in the list, so split it in two
      at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
  }
}
