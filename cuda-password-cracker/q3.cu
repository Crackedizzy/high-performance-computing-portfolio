#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD_LENGTH 4
#define ENCRYPTED_LENGTH 11
#define TOTAL_COMBINATIONS (26 * 26 * 10 * 10)



__device__ void crypt(char * rawPassword, char * newPassword){
	newPassword[0] = rawPassword[0] + 3;
	newPassword[1] = rawPassword[0] - 2;
	newPassword[2] = rawPassword[0] + 1;
	newPassword[3] = rawPassword[1] + 1;
	newPassword[4] = rawPassword[1] - 2;
	newPassword[5] = rawPassword[1] - 3;
	newPassword[6] = rawPassword[2] + 1;
	newPassword[7] = rawPassword[2] - 2;
	newPassword[8] = rawPassword[3] + 4;
	newPassword[9] = rawPassword[3] - 3;
	newPassword[10] = '\0';
	
	for(int i =0; i<10; i++){
		if(i >= 0 && i < 6){ //checking all lower case letter limits
			if(newPassword[i] > 122){
				newPassword[i] = (newPassword[i] - 122) + 97;
			}else if(newPassword[i] < 97){
				newPassword[i] = (97 - newPassword[i]) + 97;
			}
		}else{ //checking number section
			if(newPassword[i] > 57){
				newPassword[i] = (newPassword[i] - 57) + 48;
			}else if(newPassword[i] < 48){
				newPassword[i] = (48 - newPassword[i]) + 48;
			}
		}
	}
}

__global__ void kernel(char * password, char * result, int * found){
	int threadID = blockDim.x * blockIdx.x + threadIdx.x;
	if (threadID >= TOTAL_COMBINATIONS || *found) {
		return;
	}
	    char rawPassword[PASSWORD_LENGTH + 1];
	    rawPassword[0] = 'a' + (threadID / (26 * 10 * 10)) % 26;
	    rawPassword[1] = 'a' + (threadID / (10 * 10)) % 26;
	    rawPassword[2] = '0' + (threadID / 10) % 10;
	    rawPassword[3] = '0' + threadID % 10;
	    rawPassword[4] = '\0';
	
	char encryptedGuess[ENCRYPTED_LENGTH];
    	crypt(rawPassword, encryptedGuess);

	    int match = 1;
	    for (int i = 0; i < ENCRYPTED_LENGTH; i++) {
		if (encryptedGuess[i] != password[i]) {
		    match = 0;
		    break;
		}
	    }

	    if (match == 1) {
		*found = 1;
		for (int i = 0; i < PASSWORD_LENGTH; i++) {
		    result[i] = rawPassword[i];
		}
		result[PASSWORD_LENGTH] = '\0';
	    }
	}





int main(int argc, char ** argv){

    char targetEncrypted[ENCRYPTED_LENGTH] = "kfiqnm1770";
    int found = 0;
    char resultPassword[PASSWORD_LENGTH + 1];

    // device memory.
    char *d_encryptedTarget, *d_resultPassword;
    int *d_found;

    cudaMalloc(&d_encryptedTarget, ENCRYPTED_LENGTH);
    cudaMalloc(&d_found, sizeof(int));
    cudaMalloc(&d_resultPassword, PASSWORD_LENGTH + 1);

    cudaMemcpy(d_encryptedTarget, targetEncrypted, ENCRYPTED_LENGTH, cudaMemcpyHostToDevice);
    cudaMemcpy(d_found, &found, sizeof(int), cudaMemcpyHostToDevice);

    // Launching kernel
    kernel<<<dim3(1000,1,1), dim3(1000, 1, 1)>>>(d_encryptedTarget, d_resultPassword, d_found);

    // results back to host
    cudaMemcpy(&found, d_found, sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(resultPassword, d_resultPassword, PASSWORD_LENGTH + 1, cudaMemcpyDeviceToHost);

    // Check result
    if (found) {
        printf("Password found: %s\n", resultPassword);
    } else {
        printf("Password not found.\n");
    }

    // Free device memory.
    cudaFree(d_encryptedTarget);
    cudaFree(d_found);
    cudaFree(d_resultPassword);

    return 0;
}
