#include <stdlib.h>
#include <stdio.h>

void allocate_and_free(int iterations, int step) {

    for (int i = 0; i < iterations; i++) {
        
        int size = (i + 1) * step;
        int *array = (int *)malloc(size * sizeof(int));
        if (!array) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(1);
        }

        for (int j = 0; j < size; j++) {
            array[j] = j * j;
        }

	// oslobadjanje memorije za svaku drugu iteraciju
        if (i % 2 == 0) {
            free(array);
        }
        
    } 
    
}

int main() {
    allocate_and_free(20, 1000);
    return 0;
}

