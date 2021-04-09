#include "util.cuh"

//Note: Times are returned in seconds
void start_clock(cudaEvent_t &start, cudaEvent_t &end)
{
	checkCudaErrors(cudaEventCreate(&start));
	checkCudaErrors(cudaEventCreate(&end));
	checkCudaErrors(cudaEventRecord(start,0));
}

float end_clock(cudaEvent_t &start, cudaEvent_t &end)
{
	float time;
	checkCudaErrors(cudaEventRecord(end,0));
	checkCudaErrors(cudaEventSynchronize(end));
	checkCudaErrors(cudaEventElapsedTime(&time,start,end));
	checkCudaErrors(cudaEventDestroy(start));
	checkCudaErrors(cudaEventDestroy(end));

	return time/(float)1000;
}


void choose_device(int &max_threads_per_block, int &number_of_SMs, int device)
{
	int count;
	checkCudaErrors(cudaGetDeviceCount(&count));
	cudaDeviceProp prop;

	if(device == -1)
	{
		int maxcc=0, bestdev=0;
		for(int i=0; i<count; i++)
		{
			checkCudaErrors(cudaGetDeviceProperties(&prop,i));
			if((prop.major + 0.1*prop.minor) > maxcc)
			{
				maxcc = prop.major + 0.1*prop.minor;
				bestdev = i;
			}	
		}

		checkCudaErrors(cudaSetDevice(bestdev));
		checkCudaErrors(cudaGetDeviceProperties(&prop,bestdev));
	}
	else if((device < -1) || (device >= count))
	{
		std::cerr << "Invalid device argument. Valid devices on this machine range from 0 through " << count-1 << "." << std::endl;
		exit(-1);
	}
	else
	{
		checkCudaErrors(cudaSetDevice(device));
		checkCudaErrors(cudaGetDeviceProperties(&prop,device));
	}

	std::cout << "Chosen Device: " << prop.name << std::endl;
	std::cout << "Compute Capability: " << prop.major << "." << prop.minor << std::endl;
	std::cout << "Number of Streaming Multiprocessors: " << prop.multiProcessorCount << std::endl;
	std::cout << "Size of Global Memory: " << prop.totalGlobalMem/(float)(1024*1024*1024) << " GB" << std::endl << std::endl;

	max_threads_per_block = prop.maxThreadsPerBlock;
	number_of_SMs = prop.multiProcessorCount;
}

void verify(graph g, const std::vector<float> bc_cpu, const std::vector<float> bc_gpu)
{
	double error = 0;
	double max_error = 0;
	for(int i=0; i<g.n; i++)
	{
		double current_error = abs(bc_cpu[i] - bc_gpu[i]);
		error += current_error*current_error;
		if(current_error > max_error)
		{
			max_error = current_error;
		}
	}
	error = error/(float)g.n;
	error = sqrt(error);
	std::cout << "RMS Error: " << error << std::endl;
	std::cout << "Maximum error: " << max_error << std::endl;
}
