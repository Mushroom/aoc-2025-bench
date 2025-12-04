#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

// Define the size of the grid (with padding)
#define ROW_SIZE (138 + 2)
#define TOTAL_ROWS (138 + 2)

#define NDEBUG 1

__attribute__((always_inline)) inline size_t coordToIndex(size_t x, size_t y)
{ 
    return y * ROW_SIZE + x;
}

#if !NDEBUG
void printPaperState(uint8_t* paperArray)
{
    std::cout << "[DEBUG] Paper State:" << std::endl;
    for (size_t y = 0; y < TOTAL_ROWS; y++)
    {
        for(size_t x = 0; x < ROW_SIZE; x++)
        {
            std::cout << ((paperArray[coordToIndex(x, y)] == 1) ? '@' : '.');
        }
        std::cout << std::endl;
    }
}
#endif

int FindViableRolls(uint8_t* paperArray, bool iterate)
{
    bool foundViableRolls = true;
    int viableRollCount = 0;

    // If we are not iterating (Part 1), only strip the removed indexes afterwards
    uint32_t* foundRollsSingleIteration;
    uint32_t foundRollsSingleIterationCount = 0;
    if(!iterate)
    {
        foundRollsSingleIteration = new uint32_t[2000]; // Testing indicates this is sufficient
    }

    do
    {
        // Reset indicator of viable rolls
        foundViableRolls = false;

        // Go though the (padded) array
        for (size_t y = 1; y < TOTAL_ROWS - 1; y++)
        {
            for(size_t x = 1; x < ROW_SIZE - 1; x++)
            {
                uint8_t paperCount = 0;
                size_t currentArrayIndex = coordToIndex(x, y);

                // Check if there is a roll at the current index
                if(!paperArray[currentArrayIndex])
                {
                    continue; // No paper here, skip
                }

                // Scan in a grid pattern around the given index for rolls
                for(int k = -1; k <= 1; k++)
                {
                    size_t startCoord = coordToIndex(x, y + k);
                    uint8_t* rowPointer = paperArray + startCoord;

                    for(int l = -1; l <= 1; l++)
                    {
                        if(k != 0 || l != 0)
                        {
                            paperCount += *(rowPointer + l);
                        }
                    }

                    // Exit early if the candidate is found to be not-viable already
                    if(paperCount > 3)
                    {
                        break;
                    }
                }

                // Process the viable roll
                if (paperCount <= 3)
                {
                    viableRollCount++;
#if !NDEBUG
                    std::cout << "Viable at (" << x << ", " << y << ")" << std::endl;
#endif
                    // Remove immediately if iterating (P2), otherwise do it retroactively (P1)
                    if(iterate)
                    {
                        paperArray[currentArrayIndex] = 0;
                    }
                    else
                    {
                        foundRollsSingleIteration[foundRollsSingleIterationCount++] = currentArrayIndex;
                    }

                    foundViableRolls = true;
                }
            }
        }
    } while(foundViableRolls && iterate);

    // Retroactively apply the roll removals from part 1 for part 2
    if(!iterate)
    {
        for(int foundRollIndex = 0; foundRollIndex < foundRollsSingleIterationCount; foundRollIndex++)
        {
            paperArray[foundRollsSingleIteration[foundRollIndex]] = 0;
        }
    }

    return viableRollCount;
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

    // Parse the char array into a padded byte array, where a 1 indicates a roll
    uint8_t paperArray[(ROW_SIZE) * (TOTAL_ROWS)] = {0};
    size_t sIdx = 0;

    // Note we don't start at zero, as we pad the array
    for (size_t i = 1; i < TOTAL_ROWS - 1; i++)
    {
        for(size_t j = 1; j < ROW_SIZE - 1; j++)
        {
            if (inputFileContent[sIdx] == '@')
            {
                paperArray[coordToIndex(j, i)] = 1;
            }
            sIdx++;
        }
        sIdx++;
    }

#if !NDEBUG
    printPaperState(paperArray);
#endif

    // Find the viable rolls (P2 continues where P1 left off for fewer iterations)
    int viableRollCountP1 = 0;
    viableRollCountP1 = FindViableRolls(paperArray, false);
    int viableRollCountP2 = FindViableRolls(paperArray, true) + viableRollCountP1;

    std::cout << viableRollCountP1 << std::endl;
    std::cout << viableRollCountP2 << std::endl;
}
