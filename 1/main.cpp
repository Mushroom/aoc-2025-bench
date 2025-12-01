#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

int main(int argc, char* argv[])
{
#if !NDEBUG
    std::cout << "[DEBUG] Loading file from: " << argv[1] << std::endl;
#endif

    // Open input file
    std::ifstream inputFileStream(argv[1]);

    int rotationSteps[5000]; // Assuming a maximum of 5000 steps for static array
    int rotationStepsCount = 0;

    // Read in all lines from the input file
    std::string inputLine;
    while(std::getline(inputFileStream, inputLine))
    {
        bool firstCharIsL = (inputLine[0] == 'L');
        rotationSteps[rotationStepsCount++] = (firstCharIsL ? -1 : 1) * std::stoi(inputLine.substr(1));
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
