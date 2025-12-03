#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

// Calculate the index of the largest value in the given range from a pointer
__attribute__((always_inline)) inline uint8_t largestValueIndex(uint8_t* batteries, uint8_t startIndex, uint8_t endIndex)
{
    uint8_t largestIdx = 0;
    uint8_t largestValue = 0;
    for(uint8_t i = startIndex; i < endIndex; i++)
    {
        uint8_t value = *(batteries + i);
        if(value > largestValue)
        {
            largestValue = value;
            largestIdx = i;

            if(value == 9) // Early out, can't do better than 9
                break;
        }
    }

    return largestIdx;
}

// Calculate the "Joltage" value for given row and length
__attribute__((always_inline)) inline uint64_t calculatedJoltage(uint8_t* batteries, uint8_t joltageLength)
{
    uint8_t lastHighestIdx = 0;
    uint64_t total = 0;

    for(int i = joltageLength - 1; i >= 0; i--)
    {
        // Find highest value in the range, with the end truncated by how many have already been taken
        uint8_t highestIdx = largestValueIndex(batteries, lastHighestIdx, 100 - i);
        uint8_t highest = *(batteries + highestIdx);

        // Construct the total joltage number (base 10)
        total = total * 10 + highest;
        lastHighestIdx = highestIdx + 1;
    }

    return total;
}

int main(int argc, char* argv[])
{
#if !NDEBUG
    std::cout << "[DEBUG] Loading file from: " << argv[1] << std::endl;
#endif

    // Open input file, and read into string
    std::filesystem::path inputFilePath = argv[1];
    std::ifstream inputFileStream(inputFilePath, std::ios::in | std::ios::binary);
    const auto fileStreamSize = std::filesystem::file_size(inputFilePath);
    std::string inputFileContent(fileStreamSize, '\0');
    inputFileStream.read(inputFileContent.data(), fileStreamSize);

    uint8_t batteryArray[100*200] = {0};
    int batteryCellIndex = 0;

    // Parse the string into battery banks
    for(int sIdx = 0; sIdx < inputFileContent.size(); sIdx++)
    {
        while(std::isdigit(inputFileContent[sIdx]))
        {
            batteryArray[batteryCellIndex++] = inputFileContent[sIdx] - '0';
            sIdx++;
        }
    }

    uint64_t totalP1 = 0;
    uint64_t totalP2 = 0;

    for(int batteryRow = 0; batteryRow < 200; batteryRow++)
    {
        uint8_t* batteryRowPtr = batteryArray + (batteryRow * 100);

        // Part 1: highest pair
        totalP1 += calculatedJoltage(batteryRowPtr, 2);
        // Part 2: highest 12-digit number
        totalP2 += calculatedJoltage(batteryRowPtr, 12);
    }

    std::cout << totalP1 << std::endl;
    std::cout << totalP2 << std::endl;

    return 0;
}
