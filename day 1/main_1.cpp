#include <iostream>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <sstream>

#include <unordered_map>

std::unordered_map<std::string, char> numberStrToNumeric = {
  {"zero", '0'},  
  {"one", '1'},  
  {"two", '2'},  
  {"three", '3'},  
  {"four", '4'},  
  {"five", '5'},  
  {"six", '6'},  
  {"seven", '7'},  
  {"eight", '8'},  
  {"nine", '9'},  
};

int main()
{
    auto fileStream = std::ifstream{"input_1.txt"};

    std::string line;

    int total = 0;
    
    while(std::getline(fileStream, line))
    {
        std::cout << "line: " << line << std::endl;

        const auto firstCharItr = std::find_if(line.begin(), line.end(), [](const auto& v) {
            return v >= '0' && v <= '9';
        });

        const auto lastCharItr = std::find_if(line.rbegin(), line.rend(), [](const auto& v) {
            return v >= '0' && v <= '9';
        });

        size_t firstSpelled = std::string::npos;
        char firstSpelledVal;
        size_t lastSpelled = std::string::npos;
        char lastSpelledVal;

        for(const auto& [str, val] : numberStrToNumeric)
        {
            const auto posFirst = line.find(str);
            const auto posLast = line.rfind(str);
            if(posFirst != std::string::npos && posFirst < firstSpelled)
            {
                firstSpelled = posFirst;
                firstSpelledVal = val;
            }

            if(posLast != std::string::npos && (posLast > lastSpelled || lastSpelled == std::string::npos))
            {
                lastSpelled = posLast;
                lastSpelledVal = val;
            }
        }

        if(firstCharItr == line.end() || lastCharItr == line.rend())
        {
            assert(false);
        }

        std::stringstream strStream;

        if(firstCharItr - line.begin() < firstSpelled || firstSpelled == std::string::npos)
        {
            strStream << *firstCharItr;
        }
        else
        {
            strStream << firstSpelledVal;
        }

        if(line.size() - int(lastCharItr - line.rbegin()) > lastSpelled || lastSpelled == std::string::npos)
        {
            strStream << *lastCharItr;
        }
        else
        {
            strStream << lastSpelledVal;
        }

        const auto str = strStream.str();
        std::cout << str << std::endl;

        total += std::atoi(str.c_str());
    }

    std::cout << "Got total "<< total << std::endl;

    return 0;
}