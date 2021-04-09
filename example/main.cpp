//cmake -DCPLEX_DIR=/Applications/CPLEX_Studio1210/cplex -DCMAKE_PREFIX_PATH=/Users/vikramansathiyanarayanan/Downloads/libtorch ..
//4.87283e+10 -> node 99
//data/open_\(50\,\ 50\)/10_1_uniform.yaml
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cuda.h>
#include <cuda_runtime_api.h>
#include <hybrid_BC/main.cuh>
#include <hybrid_BC/mapf.hpp>
#include <boost/program_options.hpp>

#include <yaml-cpp/yaml.h>


int main(int argc, char* argv[]) {

  namespace po = boost::program_options;
  // Declare the supported options.
  po::options_description desc("Allowed options");

  std::string inputFile;
  int k; //Independence detection
  
  desc.add_options()("help", "produce help message")(
      "input,i", po::value<std::string>(&inputFile)->required(),
      "Input file (YAML)")(
      "k", po::value<int>(&k)->default_value(0),
      "Approximation value");

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") != 0u) {
      std::cout << desc << "\n";
      return 0;
    }
  } catch (po::error& e) {
    std::cerr << e.what() << std::endl << std::endl;
    std::cerr << desc << std::endl;
    return 1;
  }

  YAML::Node agents_config = YAML::LoadFile(inputFile);

  std::vector<std::pair<int, int> > obstacles;
  std::vector<std::pair<int, int> > goals;
  std::vector<std::pair<int, int> > starts;

  std::string mapFile = agents_config["map_path"].as<std::string>();

  YAML::Node map_config = YAML::LoadFile(mapFile);

  const auto& dim = map_config["dimensions"];
  int dimx = dim[0].as<int>();
  int dimy = dim[1].as<int>();

  for (const auto& node : map_config["obstacles"]) {
    obstacles.emplace_back(std::make_pair(node[0].as<int>(), node[1].as<int>()));
  }


  for (const auto& node : agents_config["agents"]) {
    const auto& start = node["start"];
    const auto& goal = node["goal"];
    starts.emplace_back(std::make_pair(start[0].as<int>(), start[1].as<int>()));
    // std::cout << "s: " << startStates.back() << std::endl;
    goals.emplace_back(std::make_pair(goal[0].as<int>(), goal[1].as<int>()));
  }


  mapf_adapters::mapf mapf(dimx, dimy, obstacles, goals);
  std::vector<std::pair<int, int> > edges = mapf.get_graph().get_edges();

  bool approx = false;
  if(k) approx = true;


  
  std::map<int, float> bc_scores = compute_bc(edges, k, approx, 0);

  std::cout<<bc_scores.size()<<std::endl;
  for(int i=0; i<mapf.get_x()*mapf.get_y(); i++){
    std::cout<<i<<" "<<bc_scores[i]<<std::endl;
  }

  std::cout<<std::endl;
  return 0;
}
