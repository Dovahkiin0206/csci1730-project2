#include <stdlib.h>
#include <stdio.h>

/**
 *Prints all environmental values.
 */
int main(int argc, char* argv[], char* envp[]) {

  for(int i = 0; envp[i] != NULL; i++){

    printf("\n%s", envp[i]);

  } // for

} // main
