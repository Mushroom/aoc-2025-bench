#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <filesystem>

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

    int rotationSteps[5000]; // Assuming a maximum of 5000 steps for static array
    int rotationStepsCount = 0;

    // Parse the string into rotation steps
    for(int sIdx = 0; sIdx < inputFileContent.size(); sIdx++)
    {
        uint32_t number = 0;
        auto character = inputFileContent[sIdx];
        bool isNegative = character == 'L';
        while(std::isdigit(inputFileContent[++sIdx]))
        {
            number = number * 10 + (inputFileContent[sIdx] - '0');
        }

        rotationSteps[rotationStepsCount++] = isNegative ? -number : number;
    }

    int32_t dialPosition = 50;
    int zeroPassCount = 0;
    int zeroHitCount = 0;

    for(int i = 0; i < rotationStepsCount; i++)
    {
        int step = rotationSteps[i];
        // Calculate new dial position (not wrapped)
        int newDialPosition = dialPosition + step;

        // Break the step into full rotations and partial remainder
        int stepDivQuotient = step / 100;
        int stepDivRemainder = step % 100;

        // Calculate the partial remainder for the new dial position
        int offsetStepDivRemainder = newDialPosition % 100;

        // Handle case where multiple full rotations are made
        zeroPassCount += std::abs(stepDivQuotient);

        // Part 1: Handle exact zero hits
        if(offsetStepDivRemainder == 0)
        {
            dialPosition = 0;
            zeroHitCount += 1;
        }

        // If the step takes us anti-clockwise
        if(offsetStepDivRemainder < 0)
        {
            // Move the dial to the correct wrapped position
            dialPosition = 100 + offsetStepDivRemainder;

            // Part 2: We passed zero, make a note
            zeroPassCount += dialPosition > (100 - abs(stepDivRemainder));
        }

        // If the step takes us clockwise
        if (offsetStepDivRemainder > 0)
        {
            // Move the dial to the correct position
            dialPosition = offsetStepDivRemainder;

            // Part 2: We passed zero, make a note
            zeroPassCount += (dialPosition < stepDivRemainder);
        }
    }

    std::cout << zeroHitCount << std::endl;
    std::cout << zeroPassCount + zeroHitCount << std::endl;
}
