#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define SECTORS 10000
#define TRACKS 100
#define PLATTERS 1
#define AVG_ROT_DELAY 2000 //in usec
#define TIME 50		         //skip waiting for full buffer, seconds
#define MAGIC -1  	       //dumb CV for finishing requests

int disk [SECTORS*TRACKS];

// structure of thread
typedef struct _thread_info{
  int tid;
  int sector_number;
  char op_name[11];
  int data;
  int arrival_time;
  struct timespec exit_time;
}thread_info;

thread_info *incoming[10001];
pthread_t disk_thread;

typedef struct _buffer_node{
  int req_id;
  char op_name[11];
  int sector_number;
  int data;
  struct _buffer_node *next;
}buffer_node;

buffer_node *b_head,*b_tail,*new_buffer_head;

int disk_head;      // disk head location
int buff_count;     // current buffer size
int limit;          // max buffer size
int direction;      // 0 for small to large, 1 for large to small

/* Buffer Synchronization Variables */
sem_t buff_mutex;   // Lock-Buffer is shared
sem_t empty_buffer; // CV-Wait on full buffer

int n,num_request_served;

/* DRIVER.C */
void init();
int sectorToTrack(int);
void read_disk(int sector_number);
void write_disk(int sector_number, int data);
void ENTER_OPERATION(char *op_name, int sector_number);
void EXIT_OPERATION(char *op_name, int sector_number);
/* DISK.C */
void *disk_ops(void *arg);
void MergeSort(buffer_node** headRef);
buffer_node *SortedMerge(buffer_node *a, buffer_node *b);					
void FrontBackSplit(buffer_node *source, buffer_node **frontRef, buffer_node **backRef);	
/* DISK_SIMULATION.C */
void *_thread_handler(void* arg);

struct timespec globalClock;
struct timespec beginWait;   // Task 3 timer start
struct timespec currentWait; // Task 3 current time
