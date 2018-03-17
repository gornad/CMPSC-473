#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <time.h>
#include "threads.h"

void *counter ();
void *sleeping();
