#ifndef SOLVER_H
#define SOLVER_H

#include "graph.h"
#include <iostream>
#include <vector>

// Magic tricks to have CPLEX behave well:
#ifndef IL_STD
#define IL_STD
#endif
#include <cstring>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
// End magic tricks

namespace cplex_example {
  class Solver {
    // The graph on which we are solving the TSP.
    Graph* g;
    
    // A vector containing CPLEX solution.
    std::vector<std::uint32_t> solution;

    // Costruct the integer solution obtained by CPLEX into a vector.
    void construct_solution(const IloCplex& cplex, const IloArray<IloNumVarArray>& x);

    // Prints the integer solution obtained by CPLEX to stdout.
    void print_solution(const char* filename);
    
  public:
    
    // Builds a solver for graph g.
    explicit Solver(Graph* graph);
    
    // Solves the TSP with CPLEX and prints the result.
    void solve_and_print(const char* filename);
  };
}

#endif