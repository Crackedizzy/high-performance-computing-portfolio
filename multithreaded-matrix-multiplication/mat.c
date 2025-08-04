#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


int main(int argc, char *argv[]) {

    FILE *fp;
    int count = 0;
    char c;
    double **matrixA;
    double **matrixB;
    double **resultMatrix;
    int numOfMatrices = 0;
    int row = 0, col = 0;
    int rows = 0, cols = 0;
    int rowsA = 0, rowsB = 0, colsA = 0, colsB = 0;
    double sum=0.0;
    int i,j=0;
    char * filename = argv[1];
    int threadCount = atoi(argv[2]);
    
    // Counting lines in file to understand the matrix structure
    fp = fopen(filename, "r");
    if (fp == NULL) {
    	printf("Error: Could not open file %s\n", filename);
    	return 1;
	}
    for (c = getc(fp); c != EOF; c = getc(fp)) {
        if (c == '\n')
            count++;
    }
    fclose(fp);
    
    
    fp = fopen(filename, "r");
    if (fp == NULL) {
    	printf("Error: Could not open file %s for reading\n", filename);
    	return 1;
	}
    for (int i = 0; i < count;) {
        if (fscanf(fp, "%d,%d", &rows, &cols) == -1) {
            i++;
        } else {
            numOfMatrices++;
            double **matrix = malloc(rows * sizeof(double *));
            for (row = 0; row < rows; row++) {
                matrix[row] = malloc(cols * sizeof(double));
                for (col = 0; col < cols - 1; col++) {
                    fscanf(fp, "%lf,", &matrix[row][col]);
                }
                fscanf(fp, "%lf", &matrix[row][col]);
            }
            i++;
            
            if (numOfMatrices == 1) {
                matrixA = matrix;
                rowsA = rows;
                colsA = cols;
            } else if (numOfMatrices == 2) {
                matrixB = matrix;
                rowsB = rows;
                colsB = cols;

                if (colsA == rowsB) {
                    // Matrix dimensions are compatible for multiplication
                    printf("Matrix A is %dx%d and Matrix B is %dx%d. The multiplication CAN be done.\n", rowsA, colsA, rowsB, colsB);
                    
                    if (threadCount > rowsA) {
                    	threadCount = rowsA;
                    	}

                    // Allocate memory for the result matrix
                    resultMatrix = malloc(rowsA * sizeof(double *));
                    for (int i = 0; i < rowsA; i++) {
                        resultMatrix[i] = malloc(colsB * sizeof(double));
                    }

                    // Set the number of threads in OpenMP
                    omp_set_num_threads(threadCount);

                    // Perform the matrix multiplication
				        // Parallelise the outer loop using OpenMP
					//#pragma omp parallel for private(i, j, k, sum) // Declare private variables for each thread
					    // Parallelise matrix addition using OpenMP with reduction
    				#pragma omp parallel for  shared(matrixA,matrixB, resultMatrix) private(i, j)reduction(+:sum)
					for (i = 0; i < rowsA; i++) {
					for (j = 0; j < colsB; j++) {
						resultMatrix[i][j] = 0.0;
					for (int k = 0; k < colsA; k++) {
						
					   resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
						
						}
					}
				}
					printf("Outside the loop\n");
				    // Write the result matrix to a file
                    FILE *outputFile = fopen("result.txt", "w");
                    if (outputFile == NULL) {
                	printf("Error: Could not open result.txt for writing.\n");
                	fclose(fp);
                	return 1;
            		}
                    fprintf(outputFile, "%d , %d\n", rowsA, colsB);
                    for (int i = 0; i < rowsA; i++) {
                        for (int j = 0; j < colsB; j++) {
                            fprintf(outputFile, "%lf ", resultMatrix[i][j]);
                        }
                        fprintf(outputFile, "\n");
                    }
                    fclose(outputFile);
                    printf("Matrix multiplication result saved to 'result.txt'.\n");
                } else {
                    printf("Matrix A is %dx%d and Matrix B is %dx%d. The multiplication CANNOT be done.\n", rowsA, colsA, rowsB, colsB);
                }

                numOfMatrices = 0;
            }
        }
    }
    fclose(fp);
    free(matrixA);
    free(matrixB);
    free(resultMatrix);


    return 0;
}
