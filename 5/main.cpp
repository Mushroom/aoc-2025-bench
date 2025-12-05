#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>

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

    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    std::vector<uint64_t> availableIngredients;

    bool parsingRanges = true;
    bool parsedNumberIsFirst = true;

    // Parse the string into ranges and values
    for(int sIdx = 0; sIdx < fileStreamSize; sIdx++)
    {
        uint64_t number = 0;

        // Poor man's std::isdigit
        while('0' <= inputFileContent[sIdx] && inputFileContent[sIdx] <= '9')
        {
            number = number * 10 + (inputFileContent[sIdx] - '0');
            sIdx++;
        }

        if(parsingRanges)
        {
            if(parsedNumberIsFirst)
            {
                ranges.emplace_back(std::make_pair(number, (uint64_t)0));
                parsedNumberIsFirst = false;
            }
            else
            {
                ranges.back().second = number;
                parsedNumberIsFirst = true;
            }

            // Check if it's the section-splitting newline ahead, if so, not ranges any more
            if(inputFileContent[sIdx + 1] == '\n')
            {
                sIdx++;
                parsingRanges = false;
            }
        }
        else
        {
            availableIngredients.emplace_back(number);
        }
    }

    // Part 1
    uint64_t freshIngredientCount = 0;
    for(auto& ingredient : availableIngredients)
    {
        for(int i = 0; i < ranges.size(); i++)
        {
            if(ranges[i].first <= ingredient && ranges[i].second >= ingredient)
            {
                std::cout << ingredient << " is fresh" << std::endl;
                freshIngredientCount++;
                break;
            }
        }
    }

    // Part 2
    uint64_t availableIngredientCount = 0;
    // Sort the ranges based on the first value
    std::sort(ranges.begin(), ranges.end(), [](const std::pair<uint64_t, uint64_t> &a, const std::pair<uint64_t, uint64_t> &b)
    {
        return a.first < b.first;
    });

    std::vector<std::pair<uint64_t, uint64_t>> concatRanges;
    // Concatenate the ranges, trimming based on overlap
    for(auto& range : ranges)
    {
        // This range is first, or does not overlap
        if(concatRanges.size() == 0 || concatRanges.back().second < range.first)
        {
            concatRanges.emplace_back(range);
        }
        else
        {
            // Trim the overlap
            auto last = concatRanges.back();
            last.second = std::max(last.second, range.second);
            concatRanges.back() = last;
        }
    }

    // Sum the contained values in the ranges (inclusive, so +1)
    for(auto& concatRange : concatRanges)
    {
        availableIngredientCount += (concatRange.second - concatRange.first + 1);
    }

    std::cout << freshIngredientCount << std::endl;
    std::cout << availableIngredientCount << std::endl;

    return 0;
}
