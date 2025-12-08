#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
//#include "../util/util.h"

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

    int64_t lineLength = 0;
    int64_t lineCount = 0;
    // Scan until we hit a newline to determine line length
    while(inputFileContent[lineLength++] != '\n');

    // Scan until we hit an operator to determine number of rows of numbers
    while(char lineChar = inputFileContent[lineCount * lineLength])
    {
        if(lineChar == '+' || lineChar == '*')
        {
            break;
        }

        lineCount++;
    }

    // Preallocate the vectors for the various parsed numbers
    std::vector<std::vector<int64_t>> numbersP1;
    numbersP1.reserve(1024);
    std::vector<std::vector<int64_t>> numbersP2;
    numbersP2.reserve(1024);
    std::vector<uint8_t> operators;
    operators.reserve(1024);
    numbersP2.emplace_back();

    // Read the columns backwards
    for(int64_t fileColumn = lineLength - 2; fileColumn >= 0; fileColumn--)
    {
        int64_t numberP2 = 0;
        int64_t fileRow;

        // Loop through the rows top to bottom
        for(fileRow = 0; fileRow <= lineCount; fileRow++)
        {
            // Get the character at current position
            char columnChar = inputFileContent[(fileRow * lineLength) + fileColumn];

            // This column contains an operator, so therefore we know it's the final one
            if(columnChar == '+' || columnChar == '*')
            {
                std::vector<int64_t> p1RowNumbers;
                p1RowNumbers.reserve(4);
                // Here we populate figures for Part 1 (via an extra loop)
                for(int fileRow2 = 0; fileRow2 < lineCount; fileRow2++)
                {
                    // How far across the row we are scanning
                    size_t rowParseOffset = 0;

                    // Parse out the number for this row
                    int64_t part1Num = 0;

                    // Get the pointer to the first value in the row
                    char* startOfRow = inputFileContent + (fileRow2 * lineLength) + fileColumn;

                    // Trim leading spaces
                    while(startOfRow[rowParseOffset] == ' ') rowParseOffset++;

                    // Read out sequential characters into a number
                    while(startOfRow[rowParseOffset] >= '0' && startOfRow[rowParseOffset] <= '9')
                    {
                        part1Num = (part1Num * 10) + (startOfRow[rowParseOffset] - '0');
                        rowParseOffset++;
                    }

                    p1RowNumbers.emplace_back(part1Num);
                }

                numbersP1.emplace_back(p1RowNumbers);
                operators.emplace_back(columnChar);

                // Jump back an extra column because of the dividing space
                fileColumn--;
                break;
            }

            // Part 2: Construct the number if digits present
            if(columnChar != ' ')
                numberP2 = (numberP2 * 10) + (columnChar - '0');
        }

        // Store the constructed P2 value
        numbersP2.back().emplace_back(numberP2);

        // We exited early, so had an operator hit
        if(fileRow == lineCount)
        {
            // While there are more columns to process, create a new vector for them
            if(fileColumn > 0)
            {
                std::vector<int64_t> newP2Vec;
                newP2Vec.reserve(4);
                numbersP2.emplace_back(newP2Vec);
            }
        }
    }

    // Calculate both parts here
    int64_t answerP1 = 0;
    int64_t answerP2 = 0;
    for(int numberSetIdx = 0; numberSetIdx < operators.size(); numberSetIdx++)
    {

        std::vector<int64_t> currentP1 = numbersP1[numberSetIdx];
        int64_t columnAnswerP1 = currentP1[0];
        for(int p1Idx = 1; p1Idx < currentP1.size(); p1Idx++)
        {
            columnAnswerP1 = (operators[numberSetIdx] == '+' ? columnAnswerP1 + currentP1[p1Idx] : columnAnswerP1 * currentP1[p1Idx]);
        }

        std::vector<int64_t> currentP2 = numbersP2[numberSetIdx];
        int64_t columnAnswerP2 = currentP2[0];
        for(int p2Idx = 1; p2Idx < currentP2.size(); p2Idx++)
        {
            columnAnswerP2 = (operators[numberSetIdx] == '+' ? columnAnswerP2 + currentP2[p2Idx] : columnAnswerP2 * currentP2[p2Idx]);
        }

        answerP1 += columnAnswerP1;
        answerP2 += columnAnswerP2;
    }

    std::cout << answerP1 << std::endl;
    std::cout << answerP2 << std::endl;
}
