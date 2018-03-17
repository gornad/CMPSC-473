#include <pthread.h>
#include <assert.h>
#include <sys/time.h>
#include <stdlib.h>


void Mutex_lock(pthread_mutex_t *m) {
    int rc = pthread_mutex_lock(m);
    assert(rc == 0);
}

void Mutex_unlock(pthread_mutex_t *m) {
    int rc = pthread_mutex_unlock(m);
    assert(rc == 0);
}

void Cond_wait(pthread_cond_t *c, pthread_mutex_t *m) {
    int rc = pthread_cond_wait(c, m);
    assert(rc == 0);
}

void Cond_signal(pthread_cond_t *c) {
    int rc = pthread_cond_signal(c);
    assert(rc == 0);
}


void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 	
		    void *(*start_routine)(void*), void *arg) {
    int rc = pthread_create(thread, attr, start_routine, arg);
    assert(rc == 0);
}

void Pthread_join(pthread_t thread, void **value_ptr) {
    int rc = pthread_join(thread, value_ptr);
    assert(rc == 0);
}

