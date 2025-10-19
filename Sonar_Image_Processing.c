#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_INTENSITY_VALUE 256
#define ROUND_OFF_FACTOR 0.5

static inline int* getPointerAccess(int *basePointerToMatrix, int sizeOfMatrix, int rowIndex, int columnIndex) {
    return (basePointerToMatrix + (rowIndex * sizeOfMatrix + columnIndex));
}


void generateMatrix(int *matrix, int sizeOfMatrix) {
    int index;
    for (index = 0; index < sizeOfMatrix * sizeOfMatrix; index++) {
        *(matrix + index) = rand() % MAX_INTENSITY_VALUE;
    }
}


void displayMatrix(int *matrix, int sizeOfMatrix) {
    int currentRowIndex, currentColumnIndex;
    for (currentRowIndex = 0; currentRowIndex < sizeOfMatrix; currentRowIndex++) {
        for (currentColumnIndex = 0; currentColumnIndex < sizeOfMatrix; currentColumnIndex++) {
            printf("%5d", *getPointerAccess(matrix, sizeOfMatrix, currentRowIndex, currentColumnIndex));
        }
        printf("\n");
    }
}


void rotateMatrixClockwise(int *matrix, int sizeOfMatrix) {
    int currentRowIndex, currentColumnIndex;
    int tempVariable;

    for (currentRowIndex = 0; currentRowIndex < sizeOfMatrix; currentRowIndex++) {
        for (currentColumnIndex = currentRowIndex + 1; currentColumnIndex < sizeOfMatrix; currentColumnIndex++) {
            int *transposePointer1 = getPointerAccess(matrix, sizeOfMatrix, currentRowIndex, currentColumnIndex);
            int *transposePointer2 = getPointerAccess(matrix, sizeOfMatrix, currentColumnIndex, currentRowIndex);
            tempVariable = *transposePointer1;
            *transposePointer1 = *transposePointer2;
            *transposePointer2 = tempVariable;
        }
    }

    for (currentRowIndex = 0; currentRowIndex < sizeOfMatrix; currentRowIndex++) {
        int *startPtr = getPointerAccess(matrix, sizeOfMatrix, currentRowIndex, 0);       
        int *endPtr = getPointerAccess(matrix, sizeOfMatrix, currentRowIndex, sizeOfMatrix - 1); 
        while (startPtr < endPtr) {
            tempVariable = *startPtr;
            *startPtr = *endPtr;
            *endPtr = tempVariable;
            
            startPtr++;
            endPtr--;
        }
    }
}


void applySmoothingFilter(int *matrix, int sizeOfMatrix) {
    int *temporaryValues = (int *)calloc(sizeOfMatrix * sizeOfMatrix, sizeof(int));
    if (temporaryValues == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    int currentRowIndex, currentColumnIndex, neighbourRowIndex, neighbourColumnIndex;

    for (currentRowIndex = 0; currentRowIndex < sizeOfMatrix; currentRowIndex++) {
        for (currentColumnIndex = 0; currentColumnIndex < sizeOfMatrix; currentColumnIndex++) {
            int sumOfValues = 0;
            double countOfValues = 0.0;

            for (neighbourRowIndex = currentRowIndex - 1; neighbourRowIndex <= currentRowIndex + 1; neighbourRowIndex++) {
                for (neighbourColumnIndex = currentColumnIndex - 1; neighbourColumnIndex <= currentColumnIndex + 1; neighbourColumnIndex++) {
                    if (neighbourRowIndex >= 0 && neighbourRowIndex < sizeOfMatrix && neighbourColumnIndex >= 0 && neighbourColumnIndex < sizeOfMatrix) {
                        
                        int *neighbourPtr = getPointerAccess(matrix, sizeOfMatrix, neighbourRowIndex, neighbourColumnIndex);
                        sumOfValues += *neighbourPtr;
                        countOfValues++;
                    }
                }
            }

            int averageOfValues = (int)(sumOfValues / countOfValues + ROUND_OFF_FACTOR);
            *getPointerAccess(temporaryValues, sizeOfMatrix, currentRowIndex, currentColumnIndex) = averageOfValues; 
        }
    }

    
    for (currentRowIndex = 0; currentRowIndex < sizeOfMatrix * sizeOfMatrix; currentRowIndex++) {
        *(matrix + currentRowIndex) = *(temporaryValues + currentRowIndex);
    }
    free(temporaryValues); 
}


int main() {
    int sizeOfMatrix;
    printf("Enter size of the matrix (2-10): ");
    if (scanf("%d", &sizeOfMatrix) != 1 || sizeOfMatrix < 2 || sizeOfMatrix > 10) {
        printf("Invalid! Please enter an integer value between 2 and 10\n");
        return 1;
    }
    int *matrix = (int *)calloc(sizeOfMatrix * sizeOfMatrix, sizeof(int));
    if (matrix == NULL) {
        fprintf(stderr, "Dynamic memory allocation failed \n");
        return 1;
    }
    srand(time(NULL));

    printf("\nOriginal Randomly Generated Matrix:\n");
    generateMatrix(matrix, sizeOfMatrix);
    displayMatrix(matrix, sizeOfMatrix);

    printf("\nMatrix after 90 degrees Clockwise Rotation:\n");
    rotateMatrixClockwise(matrix, sizeOfMatrix);
    displayMatrix(matrix, sizeOfMatrix);

    printf("\nMatrix after Applying 3*3 Smoothing Filter:\n");
    applySmoothingFilter(matrix, sizeOfMatrix);
    displayMatrix(matrix, sizeOfMatrix);
    
    free(matrix);
    return 0;
}
