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

    std::string line{};
    while (std::getline(inf, line))
    {
        std::string cleanedLine = cleanLine(line);
        if (!cleanedLine.empty())
        {
            result.push_back(cleanedLine);
        }
    }
    return result;
}

void saveToNewFile(const std::vector<std::string>& processedLines)
{
    std::ofstream outputFile("output.txt");
    for (const std::string& line : processedLines)
    {
        outputFile << line << std::endl;
    }
    outputFile.close();
}

int main()
{
    std::string folderPath = "/home/jarek/cpp/ReadFilesFromDirectory/English";
    std::vector<std::string> processedLines{};

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (entry.is_regular_file())
        {
            std::vector<std::string> lines = readfile(entry.path());
            processedLines.insert(processedLines.end(), lines.begin(), lines.end());
        }
    }

    std::sort(processedLines.begin(), processedLines.end());

    auto print = [](auto& container) { for (auto& el : container) { std::cout << el << std::endl; } };
    print(processedLines);
    saveToNewFile(processedLines);
    // for (const std::string& line : processedLines)
    // {
        // std::cout << line << std::endl;
    // }

    return 0;
}
