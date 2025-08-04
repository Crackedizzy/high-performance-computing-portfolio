#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <pthread.h>

int count=0;     // A counter used to track the number of combinations explored so far
int flag = 0;
pthread_mutex_t mutex;

struct AlphaNum {
    char *salt_and_encrypted;
    char start;
    char end;
};
/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void *crack(void *args){
  struct AlphaNum *alphanum = (struct AlphaNum *) args;
    char *salt_and_encrypted = alphanum->salt_and_encrypted;
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0 // incase you have modified the salt value, then should modifiy the number accordingly
  char plain[7];   // The combination of letters currently being checked // Please modifiy the number when you enlarge the encrypted password.
  char *enc;       // Pointer to the encrypted password

  substr(salt, salt_and_encrypted, 0, 6);

  for(x=alphanum->start; x<=alphanum->end; x++){
    for(y='A'; y<='Z'; y++){
      for(z=0; z<=99; z++){
      	
      	pthread_mutex_lock(&mutex);
      	
      	if (flag) {
        	pthread_mutex_unlock(&mutex);
                return NULL;
              }
        
        sprintf(plain, "%c%c%02d", x, y, z); 
        enc = (char *) crypt(plain, salt);
        
        count++;
        
        if(strcmp(salt_and_encrypted, enc) == 0){
	    printf("#%-8d%s %s\n", count, plain, enc);
	    flag = 1;
	    pthread_mutex_unlock(&mutex);
	    return NULL;	//uncomment this line if you want to speed-up the running time, program will find you the cracked password only without exploring all possibilites
        }else{
        printf("%-8d%s %s\n", count, plain, enc); //when the password is not found
        pthread_mutex_unlock(&mutex);
        }

      }
     }
   }
  return NULL;
}

int main(int argc, char *argv[]){
	
	int numThreads = atoi(argv[1]);
	pthread_t *threads = malloc(numThreads * sizeof(pthread_t));
	struct AlphaNum *alpha_num = malloc(numThreads * sizeof(struct AlphaNum));
    	char start = 'A';
    	int range = 26 / numThreads; //26 letters
    	pthread_mutex_init(&mutex, NULL);
	for (int i = 0; i < numThreads; i++) {
		alpha_num[i].salt_and_encrypted = "$6$AS$JES7faOlAI61x3RFAkgXUc5vliILCt2TO3gtW67fKL/py0N4l2MoEeI8ZrGmaYbgani8t2uXDD7NbPmDaVVcF/"; //IM09
		alpha_num[i].start = start + i * range;
		alpha_num[i].end = start + (i + 1) * range - 1;
        	pthread_create(&threads[i], NULL, crack, &alpha_num[i]);
    }
    
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    if (flag == 1){
    		printf("Password found!\n");
    		printf("%d solutions explored\n", count);
    }else{
    		printf("Password not found!\n");
    		printf("%d solutions explored\n", count);
          }
  
  pthread_mutex_destroy(&mutex);
  free(threads);
  free(alpha_num);
  

  return 0;
}

