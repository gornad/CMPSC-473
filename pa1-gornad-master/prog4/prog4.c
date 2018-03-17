#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "check.h"
#include <sys/time.h>
#include <sys/resource.h>

int x[5] = {1,2,3,4,5};
//double c = 10.0;

void allocate()
{
    int i;
    int *p;
    for(i =1 ; i<1000000 ; i++)
    {
      p = malloc(500 * sizeof(int));
      if(func(i)) { free (p);}
    }
}

void allocate1()
{
  int i;
  int *p;
  for (i=1 ; i<10000 ; i++)
  {
    p = malloc(1000 * sizeof(int));
    if(i & 1)
      free (p);
  }
}

void allocate2()
{
  int i;
  int *p;
  for (i=1 ; i<300000 ; i++)
  {
    p = malloc(10000 * sizeof(int));
    free (p);
  }
}


int main(int argc, char const *argv[]) {
  int i;
  int *p;

  printf("Executing the code ......\n");
  allocate();

  for (i=0 ; i<10000 ; i++)
  {
    p = malloc(1000 * sizeof(int));
    free (p);
  }
  
  // Record stats after the main program has run
  // Doing it before outputs false results
  // Define all stat values needed
  struct rusage usage;
  struct timeval user_CPU_time, system_CPU_time;
  long max_res_set_size, signals_received, vol_context_switches, inv_context_switches;

  getrusage(RUSAGE_SELF, &usage); // Map getrusage struct to local usage struct

  // Allocate values to local variables
  user_CPU_time = usage.ru_utime;
  system_CPU_time = usage.ru_stime;
  max_res_set_size = usage.ru_maxrss;
  signals_received = usage.ru_nsignals;
  vol_context_switches = usage.ru_nvcsw;
  inv_context_switches = usage.ru_nivcsw;

  printf("user_CPU_time started at: %ld.%lds\n", user_CPU_time.tv_sec, user_CPU_time.tv_usec);
  printf("system_CPU_time started at: %ld.%lds\n", system_CPU_time.tv_sec,system_CPU_time.tv_usec);
  printf("max_res_set_size is: %ld\n", usage.ru_maxrss);
  printf("signals_received is: %ld\n", usage.ru_nsignals);
  printf("vol_context_switches is: %ld\n", usage.ru_nvcsw);
  printf("inv_context_switches is: %ld\n", usage.ru_nivcsw);

  return 0;

}
