#ifndef MAPFADAPTERS_GRAPH_HPP
#define MAPFADAPTERS_GRAPH_HPP

#include <iostream>
#include <vector>
#include <unordered_set>

namespace mapf_adapters{
	class Graph{
	public:
		Graph(int x, int y, std::vector<std::pair<int, int> > obstacles):
			x_dim(x),
			y_dim(y)	{
				adjacency_list.resize(x_dim*y_dim);
				generate_graph(obstacles);
		}
		Graph():
			x_dim(0),
			y_dim(0)	{

			}

		void generate_graph(std::vector<std::pair<int, int> > obstacles){
			//i*y_dim + j
			std::unordered_set<int> processed_obstacles;
			for(auto it = obstacles.begin(); it != obstacles.end(); ++it){
				int x = it->first;
				int y = it->second;
				int new_xy = x*y_dim + y;
				processed_obstacles.insert(new_xy);
			}

			for(int i = 0; i < x_dim; ++i){
				for(int j = 0; j < y_dim; ++j){
					int xy = i*y_dim + j;
					if(processed_obstacles.find(xy) != processed_obstacles.end()){
						//Obstacle
						adjacency_list[xy] = std::vector< int >();
					} else {
						//Not an Obstacle
						std::vector< int > temp;
						temp.clear();
						if(j-1 >= 0 && processed_obstacles.find((i*y_dim)+(j-1)) == processed_obstacles.end()){
							int left = (i*y_dim) + (j-1);
							temp.push_back(left);
							if(std::find(edges.begin(), edges.end(), std::make_pair(left, xy)) == edges.end())
								edges.push_back(std::make_pair(xy, left));
						}
						if(j+1 < y_dim && processed_obstacles.find((i*y_dim)+(j+1)) == processed_obstacles.end()){
							int right = (i*y_dim) + (j+1);
							temp.push_back(right);	
							if(std::find(edges.begin(), edges.end(), std::make_pair(right, xy)) == edges.end())
								edges.push_back(std::make_pair(xy, right));
						}
						if(i-1 >= 0 && processed_obstacles.find(((i-1)*y_dim)+(j)) == processed_obstacles.end()){
							int up = ((i-1)*y_dim) + j;
							temp.push_back(up);
							if(std::find(edges.begin(), edges.end(), std::make_pair(up, xy)) == edges.end())
								edges.push_back(std::make_pair(xy, up));
						}
						if(i+1 < x_dim && processed_obstacles.find(((i+1)*y_dim)+(j)) == processed_obstacles.end()){
							int down = ((i+1)*y_dim) + j;
							temp.push_back(down);
							if(std::find(edges.begin(), edges.end(), std::make_pair(down, xy)) == edges.end())
								edges.push_back(std::make_pair(xy, down));
						}
						adjacency_list[xy] = temp;
					}
				}
			}
		}

		std::vector<std::pair<int, int> > get_neighbor(int x, int y){
			int xy = x*y_dim + y;
			std::vector<std::pair<int, int> > temp = std::vector<std::pair<int, int> >();
			for(auto it=adjacency_list[xy].begin(); it!=adjacency_list[xy].end(); ++it){
				int val = *it;
				int i = val/y_dim;
				int j = val%y_dim;
				temp.push_back(std::make_pair(i, j));
			}
			return temp;
		}

		std::vector<std::vector<int> > get_adj(){	return adjacency_list;	}
		std::vector<std::pair<int, int> > get_edges(){	return edges;	}
	private:
		std::vector< std::vector< int > > adjacency_list;
		std::vector<std::pair<int, int> > edges;
		int x_dim, y_dim;
	};
}

#endif
