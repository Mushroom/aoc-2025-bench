#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

// Input dimensions (incl newlines)
const size_t rowSize = 142;
const size_t rowCount = 142;

__attribute__((always_inline)) inline size_t coordToIndex(size_t x, size_t y)
{ 
    return y * rowSize + x;
}

int main(int argc, char* argv[])
{
#if !NDEBUG
    std::cout << "[DEBUG] Loading file from: " << argv[1] << std::endl;
#endif

    // Open input file, and read into char array
    std::filesystem::path inputFilePath = argv[1];
    std::ifstream inputFileStream(inputFilePath, std::ios::in | std::ios::binary);
    const auto fileStreamSize = std::filesystem::file_size(inputFilePath);
    char* inputFileContent = new char[fileStreamSize];
    inputFileStream.read(inputFileContent, fileStreamSize);

    // We track beams in a cumulative fashion in a single dimension
    uint64_t beamsInCurrentColumn[rowSize] = {0};

    // FInd the start position
    for(size_t colIdx = 0; colIdx < rowSize; colIdx++)
    {
        size_t currentIndex = coordToIndex(colIdx, 0);
        if(inputFileContent[currentIndex] == 'S')
        {
            // Track the initial beam coming from the start point
            beamsInCurrentColumn[currentIndex] = 1;
            break;
        }
    }

    uint64_t answerP1 = 0;
    uint64_t answerP2 = 0;

    // Loop through the row in the input, starting below the start line
    for(size_t rowIdx = 1; rowIdx < rowCount; rowIdx++)
    {
        for(size_t colIdx = 0; colIdx < rowSize; colIdx++)
        {
            // Scan across each column, and determine if it is a splitter in the cussent position
            size_t idx = coordToIndex(colIdx, rowIdx);

            // If it is, and there is a beam coming into it, split it
            if(inputFileContent[idx] == '^' && beamsInCurrentColumn[colIdx] > 0)
            {
                // As we track multiple timelines, track how many of each beam are split
                beamsInCurrentColumn[colIdx - 1] += beamsInCurrentColumn[colIdx];
                beamsInCurrentColumn[colIdx + 1] += beamsInCurrentColumn[colIdx];

                // Part 1: Every beam is split equally every time, single timeline, so track split counts
                answerP1++;

                // This beam has now been split into adjacent cells, so reset counter
                beamsInCurrentColumn[colIdx] = 0;
            }
        }
    }

    // Part 2: Sum the tracked column counters, which handles timelines
    for(size_t colIdx = 0; colIdx < rowSize; colIdx++)
    {
        answerP2 += beamsInCurrentColumn[colIdx];
    }

    std::cout << answerP1 << std::endl;
    std::cout << answerP2 << std::endl;
}
