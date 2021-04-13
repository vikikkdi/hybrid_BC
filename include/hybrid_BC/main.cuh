#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "parse.h"
#include "sequential.h"
#include "util.cuh"
#include "kernels.cuh"

std::map<int, double> compute_bc(std::vector<std::pair<int, int> > edges, int k, bool approx,	int device );