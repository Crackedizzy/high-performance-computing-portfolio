#include <stdio.h>
#include <stdlib.h>

#include "lodepng.h"

__global__ void Blur(unsigned char * inputImage, unsigned char * outputImage, int width, int height){
	
	int r=0, g=0, b=0, a=0, n_count=0;
	int threadID = blockDim.x * blockIdx.x + threadIdx.x;
	int x = threadID % width;
	int y = threadID / width;
	
	if (x >= width || y >= height) {
		return;
	}

	for (int i = -1; i <= 1; i++) {
		for (int j = -1;  j <= 1; j++) {
			int xi = x + i;
			int yj = y + j;
			
			if ( xi >= 0 && xi < width && yj >= 0 && yj < height){//making sure im not scanning outside of the image
				 
				int pixel = (yj * width + xi) * 4;
				r += inputImage[pixel];
				g += inputImage[pixel+1];
				b += inputImage[pixel+2];
				a += inputImage[pixel+3];
				n_count++;
			}
		}
	}
	
	int pixel = (y * width + x) * 4;//mult by width to know hw many pixels are b4 this row
	
		outputImage[pixel] = r/n_count;
		outputImage[pixel+1] = g/n_count;
		outputImage[pixel+2] = b/n_count;
		outputImage[pixel+3] = a;
	
}

int main(int argc,  char ** argv){
	
	unsigned char * cpuImage;
	unsigned int width, height;
	char * filename = argv[1];
	char * newFilename = argv[2];
	
	lodepng_decode32_file(&cpuImage, &width, &height, filename);
	
	int arraySize = width * height * 4;
	int memorySize = arraySize * sizeof(unsigned char);
	
	unsigned char * gpuInput;
	unsigned char * gpuOutput;
	
	unsigned char cpuOutput[arraySize];
	
	cudaMalloc((void**) &gpuInput, memorySize);
	cudaMalloc((void**) &gpuOutput, memorySize);
	
	cudaMemcpy(gpuInput, cpuImage, memorySize, cudaMemcpyHostToDevice); 
	
	Blur<<<24, 100>>>(gpuInput, gpuOutput, width, height);
	cudaThreadSynchronize();
	
	cudaMemcpy(cpuOutput, gpuOutput, memorySize, cudaMemcpyDeviceToHost); 
	
	lodepng_encode32_file(newFilename, cpuOutput, width, height);
	
	cudaFree(gpuInput);
    	cudaFree(gpuOutput);
	
	return 0;
}
