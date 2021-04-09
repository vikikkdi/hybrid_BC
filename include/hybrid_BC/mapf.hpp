#ifndef MAPFADAPTERS_MAPF_HPP
#define MAPFADAPTERS_MAPF_HPP

#include <iostream>
#include <vector>
#include "graph.hpp"

namespace mapf_adapters{
	class mapf{
	public:
		mapf(){}

		mapf(int x_dim, int y_dim, std::vector<std::pair<int, int> > obstacles, std::vector<std::pair<int, int> > goals):
			x_dim(x_dim),
			y_dim(y_dim),
			obstacles(obstacles),
			goals(goals)	{
				g = mapf_adapters::Graph(x_dim, y_dim, obstacles);
			}

		int get_x(){	return x_dim;	}
		int get_y(){	return y_dim;	}
		std::vector<std::pair<int, int> > get_obstacles(){	return obstacles;	}
		std::vector<std::pair<int, int> > get_goals(){	return goals;	}
		mapf_adapters::Graph get_graph(){	return g;	}
		
		mapf get_mapf(){	return *this;	}
		void update_goals(std::vector<std::pair<int, int> > new_goals){	this->goals = new_goals;	}

	private:
		int x_dim, y_dim;
		std::vector<std::pair<int, int> > obstacles, goals;
		mapf_adapters::Graph g;
	};


	class Solver{
		/*
		cbs, bcp, epea, icts, lazycbs -> bool solve(mapf_adapters::mapf map, std::vector<std::pair<int, int> > starts)
		cbsh, sat, smt-cbs -> bool solve(mapf_adapters::mapf map, std::vector<std::pair<int, int> > starts, std::string heuristics/encoding)
		ecbs -> bool solve(mapf_adapters::mapf map, std::vector<std::pair<int, int> > starts, int w)

		*/
	public:
		Solver()	{}
		
		virtual bool solve(mapf map, std::vector<std::pair<int, int> > starts, std::string outputFile)	{	return true;	}
		virtual bool solve(mapf map, std::vector<std::pair<int, int> > starts, std::pair<int, std::vector<std::vector< std::pair<std::pair<int, int>, int> > > > *solution)	{	return true;	}
	};
}

#endif
