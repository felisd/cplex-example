#include "graph.h"
#include "solver.h"
#include <iostream>
#include <sys/stat.h>

bool is_file_exist(const char* filename);

int main(int argc, char *argv[]) {
	using namespace cplex_example;

	if (argc >= 2) {
		const char* problem_filename = argv[1];

		if (is_file_exist(problem_filename))
		{
			std::cout << "Read " << problem_filename << std::endl;
			Graph graph = Graph(problem_filename);
			Solver solver = Solver(&graph);

			const char* solution_filename = "solution.xml";

			if (argc > 2) {
				const char* param_name = "";
				bool is_value = false;

				for (int i=2; i<argc; i++)
				{
					if (strncmp(argv[i],"-",1) == 0)
					{
						param_name = argv[i];
						is_value = true;
					}
					else if (is_value)
					{
						const char* param_value = argv[i];
						solver.set_param(param_name, param_value);
						is_value = false;
					}
					else
					{
						solution_filename = argv[i];
					}
				}
			}

			solver.solve_and_print(solution_filename);
		}
		else
		{
			std::cout << problem_filename << " does not exist!" << std::endl;
		}
	}
	else {
		std::cout << "Usage: " << argv[0] << " problemFile [solutionFile] [-paramName paramValue]";
		std::cout << std::endl;
		std::cout << "\nList of supported paramName:";
		std::cout << "\n  -GAP: relative gap tolerance (double, typically ranged from 0.0 to 0.1)";
		std::cout << std::endl;
		std::cout << "\nExample of usage:";
		std::cout << "\n  " << argv[0] << " ../data/import/tutorial/snowball17.tsp ../data/solved/snowball17.xml";
		std::cout << "\n  " << argv[0] << " ../data/import/tutorial/snowball17.tsp ../data/solved/snowball17.xml -GAP 0.01";
		std::cout << std::endl;

	}

	return 0;
}

bool is_file_exist(const char* filename) {
	struct stat buffer;
	return (stat (filename, &buffer) == 0);
}
