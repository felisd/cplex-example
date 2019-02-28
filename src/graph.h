#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <string>
#include "tinyxml2.h"

namespace cplex_example {
  class Graph {
    // Number of nodes in the graph.
    // Nodes go from 0 to n_nodes - 1.
    std::uint32_t n_nodes;
    
    // Coordinates
    std::vector<float> x_coord;
    std::vector<float> y_coord;

    // Arc costs matrix. For every pair of
    // nodes i,j (with 0 < i,j < n_nodes)
    // it gives the cost of arc (i,j).
    std::vector<std::vector<float>> costs;
    
    // Generates a random graph with euclidean
    // distances and nodes randomly placed inside
    // a square of side square_side.
    void read_problem(const char* filename);
    
    std::vector<std::string> split(std::string str, char delimiter);
    bool contains(std::string str, std::string substr);

    tinyxml2::XMLDocument* doc;
    tinyxml2::XMLElement* append_xml_key(tinyxml2::XMLElement* root, const char* key, const char* value = NULL);
    void set_xml_attrib(tinyxml2::XMLElement* element, const char* attrib, const char* value);
  public:
    
    // Created a new (random) graph with n_nodes nodes
    explicit Graph(const char* filename);
    
    // Size of the graph (i.e. number of nodes).
    std::uint32_t size() const { return n_nodes; }
    
    // Cost of arc (i,j).
    float cost(std::uint32_t i, std::uint32_t j) { return costs[i][j]; }

    // Print according to sequence.
    void print(const char* filename, std::vector<std::uint32_t> sequence);
  };
}

#endif