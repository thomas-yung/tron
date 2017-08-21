// Dependencies
#include "util.h"

// Methods
// Check if a pointer is null and exit with failure if so
// Used after memory allocation
void checkAllocFail(void *ptr, char *fromMethod) {
  if (ptr == NULL) {
    printf("\nEXIT_FAILURE @ %s\n", fromMethod);
    exit(EXIT_FAILURE);
  }
}
