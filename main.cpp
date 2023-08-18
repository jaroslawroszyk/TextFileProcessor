#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <algorithm>
#include <vector>

namespace fs = std::filesystem;

std::string cleanLine(const std::string& line)
{
    auto cleaned = line;

    cleaned.erase(cleaned.begin(), std::find_if(cleaned.begin(), cleaned.end(),
        [](unsigned char ch) { return not std::isspace(ch) && ch != '-'; }));

    return cleaned;
}

std::vector<std::string> readfile(const std::string& file)
{
    std::vector<std::string> result{};
    std::ifstream inf(file);
    if (!inf.is_open())
    {
        throw std::runtime_error("Error opening file: " + file);
    }

    std::string line{};
    while (std::getline(inf, line))
    {
        std::string cleanedLine = cleanLine(line);
        if (!cleanedLine.empty())
        {
            result.push_back(cleanedLine);
        }
    }
    inf.close();
    return result;
}

void saveToNewFile(const std::vector<std::string>& processedLines)
{
    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open())
    {
        throw std::runtime_error("Error creating output file.");
    }

    for (const std::string& line : processedLines)
    {
        outputFile << line << std::endl;
    }
    outputFile.close();
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " folderPath" << std::endl;
        return 1;
    }

    try
    {
        std::string folderPath = argv[1];

        if (not fs::exists(folderPath) || not fs::is_directory(folderPath))
        {
            throw std::runtime_error("Invalid folder path: " + folderPath);
        }

        std::vector<std::string> processedLines{};
        for (const auto& entry : fs::directory_iterator(folderPath))
        {
            if (entry.is_regular_file())
            {
                std::vector<std::string> lines = readfile(entry.path());
                processedLines.insert(processedLines.end(), lines.begin(), lines.end());
            }
        }

        if (processedLines.empty())
        {
            std::cerr << "No valid lines found in the specified files." << std::endl;
            return 1;
        }

        std::sort(processedLines.begin(), processedLines.end());

        auto print = [](const auto& container) { for (const auto& el : container) { std::cout << el << std::endl; } };
        print(processedLines);
        saveToNewFile(processedLines);
    }
    catch (const std::exception& e)
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
