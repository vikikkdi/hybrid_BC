#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <map>
#include "parse.h"
#include "sequential.h"
#include "util.cuh"
#include "kernels.cuh"

#include "main.cuh"

std::map<int, double> compute_bc(std::vector<std::pair<int, int> > edges, int k, bool approx,	int device ){
	int max_threads_per_block, number_of_SMs;
	choose_device(max_threads_per_block,number_of_SMs, device);
	graph g = parse_edgelist(edges);

	std::cout << "Number of nodes: " << g.n << std::endl;
	std::cout << "Number of edges: " << g.m << std::endl;

	//If we're approximating, choose source vertices at random
	std::set<int> source_vertices;
	if(approx)
	{
		if(k > g.n || k < 1)
		{
			k = g.n;
		}

		while(source_vertices.size() < k)
		{
			int temp_source = rand() % g.n;
			source_vertices.insert(temp_source);
		}
	}

	cudaEvent_t start,end;
	float CPU_time;
	std::vector<float> bc;
	if(false) //Only run CPU code if verifying
	{
		start_clock(start,end);
		bc = bc_cpu(g,source_vertices);
		CPU_time = end_clock(start,end);
	}

	float GPU_time;
	std::vector<double> bc_g;
	start_clock(start,end);
	bc_g = bc_gpu(g,max_threads_per_block,number_of_SMs,approx, k,source_vertices);
	GPU_time = end_clock(start,end);

	if(false)
	{
		//verify(g,bc,bc_g);
	}
	if(false)
	{
		//g.print_BC_scores(bc_g,"summa.txt");
	}

	std::cout << std::setprecision(9);
	if(false)
	{
		std::cout << "Time for CPU Algorithm: " << CPU_time << " s" << std::endl;
	}
	std::cout << "Time for GPU Algorithm: " << GPU_time << " s" << std::endl;

	std::map<int, double> bc_scores = g.get_BC_scores(bc_g);
	
	delete[] g.R;
	delete[] g.C;
	delete[] g.F;
	return bc_scores;
}