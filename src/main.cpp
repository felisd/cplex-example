#include "graph.h"
#include "solver.h"
#include <iostream>

int main(int argc, char *argv[]) {
	using namespace cplex_example;

	if (argc >= 2) {
		const char* problem_filename = argv[1];
		const char* solution_filename = "solution.xml";

		if (argc > 2) {
			solution_filename = argv[2];
		}

		std::cout << "Read " << problem_filename << std::endl;
		Graph graph = Graph(problem_filename);
		Solver solver = Solver(&graph);

		solver.solve_and_print(solution_filename);
	}
	else {
		std::cout << "Usage: " << argv[0] << " problemFile [solutionFile]" << std::endl;
		std::cout << "       e.g. " << argv[0] << " ../data/import/tutorial/snowball17.tsp ../data/solved/snowball17.xml" << std::endl;
	}

	return 0;
}