#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

// Check if a string consists of repeated segments
bool DoesRepeat(std::string s, int repeatCount)
{
    // If the string doesn't split evenly into segments of this size, we don't check it
    if(s.length() % repeatCount != 0)
    {
        return false;
    }

    // Split the string into segments given size, and compare each segment against the first
    int segmentLength = s.length() / repeatCount;
    for(int i = segmentLength; i < s.length(); i += segmentLength)
    {
        std::string segment1 = s.substr(0, segmentLength);
        std::string segment2 = s.substr(i, segmentLength);

        if(segment1 != segment2)
        {
            return false;
        }
    }

    return true;
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

    uint64_t ranges[128]; // Assuming a maximum of 128 ranges for static array
    int rangesCount = 0;

    // Parse the string into ranges
    for(int sIdx = 0; sIdx < inputFileContent.size(); sIdx++)
    {
        uint64_t number = 0;
        while(std::isdigit(inputFileContent[sIdx]))
        {
            number = number * 10 + (inputFileContent[sIdx] - '0');
            sIdx++;
        }

        ranges[rangesCount++] = number;
    }

    uint64_t totalP1 = 0;
    uint64_t totalP2 = 0;

    // Loop through each pair of numbers in the array, which is a range
    for(int i = 0; i < rangesCount; i += 2)
    {
        uint64_t rangeStart = ranges[i];
        uint64_t rangeEnd = ranges[i + 1];

        // Go through each number in the range
        for(uint64_t num = rangeStart; num <= rangeEnd; num++)
        {
            std::string numString = std::to_string(num);
            int numStringLength = numString.length();

            // Search for a run segments of x length
            for (int j = 2; j <= numString.length(); j++)
            {
                if(DoesRepeat(numString, j))
                {
                    // Part 1 only cares about pairs
                    if(j == 2)
                    {
                        totalP1 += num;
                    }

                    // Part 2 cares about all segment lengths
                    totalP2 += num;

                    break;
                }
            }
        }
    }

    std::cout << totalP1 << std::endl;
    std::cout << totalP2 << std::endl;

    return 0;
}
