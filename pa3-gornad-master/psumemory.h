int psumeminit(int algo, int sizeOfRegion);		// Expands the heap, sets best-fit/worst-fit algorithm to use and returns a 1 if successful/0 otherwise
void *psumalloc(int size);				// Returns a void pointer to the newly allocated memory in the heap
int psufree(void *ptr);					// Frees a memory region allocated by psumalloc and returns a 1 if successful/0 otherwise
void psumemdump();					// Displays free space information for debugging purposes
   							
							
