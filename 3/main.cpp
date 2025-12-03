#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

__attribute__((always_inline)) inline std::pair<uint64_t, uint64_t> calculatedJoltage(uint8_t* batteries, uint8_t joltageLength1, uint8_t joltageLength2)
{
    // positions[d][count] = indexes where digit d occurs (0..99)
    uint8_t positions[10][100];
    uint8_t counts[10] = {0};

    // Loop through row, and note the counts and positions of each digit
    for(uint8_t i = 0; i < 100; ++i)
    {
        uint8_t d = batteries[i];
        positions[d][counts[d]++] = i;
    }

    // Each of the totals for parts 1 and 2
    std::pair<uint64_t, uint64_t> totals = {0, 0};

    // Process parts 1 and 2 using the same index data for the row
    for(int i = 0; i < 2; ++i)
    {
        uint8_t joltageLength = (i == 0) ? joltageLength1 : joltageLength2;

        uint8_t idxPointers[10] = {0};

        uint64_t total = 0;
        uint8_t startIndex = 0;

        // Get the initial end index (ie, there must be room for remainder of joltage cells)
        const uint8_t initialEnd = 101 - joltageLength;

        // Scan through the digits
        for(uint8_t t = 0; t < joltageLength; ++t)
        {
            // Allow for a futher end index based on how many cells have been chosen already
            uint8_t endIndex = initialEnd + t;

            // Pick highest digit available between indexes
            for(int d = 9; d >= 0; --d)
            {
                uint8_t p = idxPointers[d];

                // While pointer within range of counts, and position is before startIndex, advance pointer
                while(p < counts[d] && positions[d][p] < startIndex) ++p;

                // If pointer within range, and position is before endIndex, we can choose this digit
                if(p < counts[d] && positions[d][p] < endIndex)
                {
                    total = total * 10 + d;
                    startIndex = positions[d][p] + 1;
                    idxPointers[d] = p + 1;
                    break;
                }

                // Note where we scanned up to
                idxPointers[d] = p;
            }
        }

        (i == 0) ? totals.first = total : totals.second = total;
    }

    return totals;
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

        auto result = calculatedJoltage(batteryRowPtr, 2, 12);

        // Part 1: highest pair
        totalP1 += result.first;
        // Part 2: highest 12-digit number
        totalP2 += result.second;
    }

    std::cout << totalP1 << std::endl;
    std::cout << totalP2 << std::endl;

    return 0;
}
