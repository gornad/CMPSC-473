#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "PA4.h"

pthread_cond_t merchantwait1  = PTHREAD_COND_INITIALIZER;
pthread_cond_t merchantwait2  = PTHREAD_COND_INITIALIZER;
pthread_cond_t merchantwait3  = PTHREAD_COND_INITIALIZER;
pthread_cond_t empresswait   = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m     = PTHREAD_MUTEX_INITIALIZER;


int merchday1;
int merchday2;
int merchday3;
int empressday;
int empresslaunched = 0;
int sawmerchant1 = 0;
int sawmerchant2 = 0;
int sawmerchant3 = 0;
int days = 1;
int empresspresence=0;
int empresswentback = 0;
int merchantarrivedbeforeship = 0;
int allmerchantspassed = 0;
FILE *fs;
 


void *empresstravel(void *arg) {
	Mutex_lock(&m); 
	while(days<=10) {
		empresslaunched = 1;
		if(days == empressday) {	// Check if its the empress arrival day
			printf("DAY %d: The empress arrived in Calicut \n", days);
			empresspresence = 1;
		}
	     	if(((merchantarrivedbeforeship == 1)) && (days == empressday) && (empresswentback == 0)) {	// Check conditions for empress to immediately leave
			printf("DAY %d: The empress went back to the USA \n", days);
			empresspresence = 0;
			empresswentback = 1;
		}   
		if(allmerchantspassed == 3 && empresswentback == 0) {	// Check conditions to leave after trading with all merchants
			printf("DAY %d: The empress went back to the USA \n", days);
			empresspresence = 0;
			empresswentback = 1;
		}
		// The following while loops wait the empress on each day a trader is supposed to arrive to Calicut

		while (days == merchday1) {	   
			sawmerchant1 = 1;
			Cond_wait(&empresswait, &m);  
			days++;
		}
		Cond_signal(&merchantwait1); 

		// This if statements makes sure to evade infinite loops and reajust the time to its actual value
		if(sawmerchant1 == 1){
			days--;
			sawmerchant1=0;
		}

		while (days == merchday2) { 
			sawmerchant2 = 1;   
			Cond_wait(&empresswait, &m);  
			days++;
		}
		Cond_signal(&merchantwait2);

		if(sawmerchant2 == 1) {
			days--;
			sawmerchant2=0;
		}
		while (days == merchday3) {  
			sawmerchant3 = 1;
			Cond_wait(&empresswait, &m); 
			days++; 
		}
		Cond_signal(&merchantwait3);

		if(sawmerchant3 == 1){
			days--;
			sawmerchant3=0;
		}

 		days++; 
	}          
	Mutex_unlock(&m);
    return NULL;
}

void *merchanttravel1(void *arg) {

	Mutex_lock(&m);	
	int merchday = (int) arg;
	while (days != merchday1 && empresslaunched == 0) {   
	    Cond_wait(&merchantwait1, &m);  
	}

	if(empresspresence == 1) { //	Check if empress is present
		printf("DAY %d: The empress traded with one merchant ship (1) and the merchant ship left \n", merchday1);
		allmerchantspassed++;
	}

	else {	// Trade with locals
		printf("DAY %d: One merchant ship (1) traded with locals and left \n", merchday1);
		merchantarrivedbeforeship = 1;
	}

	Cond_signal(&empresswait);  
	Mutex_unlock(&m);  

    return NULL;
}

void *merchanttravel2(void *arg) {

	Mutex_lock(&m);  
	int merchday = (int) arg;
	while (days != merchday2 && empresslaunched == 0) {   
	    Cond_wait(&merchantwait2, &m);  
	} 
	if(empresspresence == 1) {
		printf("DAY %d: The empress traded with one merchant ship (2) and the merchant ship left \n", merchday2);
		allmerchantspassed++;
	}

	else {
		printf("DAY %d: One merchant ship (2) traded with locals and left \n", merchday2);
		merchantarrivedbeforeship = 1;
	}

	Cond_signal(&empresswait); 
	Mutex_unlock(&m);  

    return NULL;
}

void *merchanttravel3(void *arg) {

	Mutex_lock(&m);  
	int merchday = (int) arg;
	while (days != merchday3 && empresslaunched == 0) {   
	    Cond_wait(&merchantwait3, &m);  
	}

	if(empresspresence == 1) {
		printf("DAY %d: The empress traded with one merchant ship (3) and the merchant ship left \n", merchday3);
		allmerchantspassed++;
	}

	else {
		printf("DAY %d: One merchant ship (3) traded with locals and left \n", merchday3);
		merchantarrivedbeforeship = 1;
	}

	Cond_signal(&empresswait); 
	Mutex_unlock(&m);  

    return NULL;
}


int main(int argc, char *argv[]) {
	char ch, input[4], buffer[4];
	int i=0, j=0;

	// File input

	fs = fopen("input20", "r");              //<-- !!!!!!!!!!!!!!!!! CHANGE THIS VALUE FOR INPUT !!!!!!!!!!!!!!!!!!!!!!//
	while(1) {

		ch = fgetc(fs);

		if(ch == EOF){
			buffer[i] = ch;
			input[j] = atoi(buffer);
			break;
		}

		else if(ch == ',') {
 
			input[j] = atoi(buffer);
			j++;
			bzero(buffer, 4);
			i = 0;
			continue;
		}

		else {
			buffer[i] = ch;
			i++;
		}
	}
	
	// Global variable assignments

	empressday = input[0];
	merchday1 = input[1];
	merchday2 = input[2];
	merchday3 = input[3];
	
	// Thread creation and wait

	pthread_t merchant1, merchant2, merchant3, empress;
	Pthread_create( &empress, NULL, empresstravel, (void*)empressday);	
	Pthread_create( &merchant1, NULL, merchanttravel1, (void*)merchday1);
	Pthread_create( &merchant2, NULL, merchanttravel2, (void*)merchday2);
	Pthread_create( &merchant3, NULL, merchanttravel3, (void*)merchday3);


	Pthread_join(merchant1, NULL); 
	Pthread_join(merchant2, NULL); 
	Pthread_join(merchant3, NULL); 
	Pthread_join(empress, NULL); 

}

