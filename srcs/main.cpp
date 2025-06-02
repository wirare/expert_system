#include "parsing.hpp"
#include "expert_system.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
		return 1;
	}

	std::ifstream file = openFile(argv[1]);

    System System;

    Parser(System, file);

    if (System.getErr())
        return 1;
    return 0;
}