#include <iostream>
#include <fstream>
#include <string>

int main()
{
    std::cout << "Hello GitHub!\n";

    std::ifstream example_input;
    example_input.open("./Input/cvrptw1.txt");

    std::string line;

    while (std::getline(example_input, line)) {
        std::cout << line << std::endl;
    }

    example_input.close();
    
    return 0;
}