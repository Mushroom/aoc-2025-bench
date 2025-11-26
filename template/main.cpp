#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
#if !NDEBUG
    std::cout << "[DEBUG] Loading file from: " << argv[1] << std::endl;
#endif

    // Read input file
    std::ifstream inputFileStream(argv[1]);

    // Load input into vector of strings
    std::vector<std::string> input;
    std::string inputLine;
    while(std::getline(inputFileStream, inputLine))
    {
        input.emplace_back(inputLine);
    }

    // Solve puzzle here
}
