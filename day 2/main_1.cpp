#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string_view>
#include <cassert>
#include <charconv>

const std::unordered_map<std::string, int> totalCubeQuantities = {
    {"red", 12},
    {"green", 13},
    {"blue", 14}
};

struct BoxRoll
{
    std::string_view color;
    int numbers;

    [[nodiscard]] std::string toString() const
    {
        return std::string{color} + " - " + std::to_string(numbers);
    }
};

[[nodiscard]] BoxRoll parseBox(const std::string_view& box)
{
    const auto spacePos = box.find_last_of(' ');
    assert(spacePos != std::string::npos);
    const auto offset = (*box.begin() == ' ') ? 1 : 0;
    const auto numberStr = std::string_view{box.begin() + offset, box.begin() + spacePos};
    const auto colorStr = std::string_view{box.begin() + spacePos + 1, box.end()};

    int numbers;

    std::ignore = std::from_chars(numberStr.begin(), numberStr.end(), numbers);

    assert(numbers != 0);

    return {
        .color=colorStr,
        .numbers=numbers
    };
}

[[nodiscard]] std::unordered_map<std::string, int> parseGame(const std::string_view& game)
{
    std::size_t startPos = 0;
    auto commaPos = game.find_first_of(",");

    std::unordered_map<std::string, int> res;

    while(commaPos != std::string::npos)
    {
        const auto boxStr = std::string_view{game.begin() + startPos, game.begin() + commaPos};
        const auto box = parseBox(boxStr);

        res[std::string{box.color}] = box.numbers;

        startPos = commaPos + 1;
        commaPos = game.find_first_of(",", startPos);
    }

    const auto lastBoxStr = std::string_view{game.begin() + startPos, game.end()};
    const auto lastBox = parseBox(lastBoxStr);

    res[std::string{lastBox.color}] = lastBox.numbers;
    std::cout << "        " << lastBox.toString() << std::endl;

    return res;
}

[[nodiscard]] std::vector<std::unordered_map<std::string, int>> parseSets(const std::string_view& line)
{
    const auto colonPos = line.find_first_of(':');

    assert(colonPos != std::string::npos);

    const auto eachGamesStr = std::string_view{line.begin() + colonPos + 1, line.end()};

    std::cout << "    " << eachGamesStr << std::endl;

    std::vector<std::unordered_map<std::string, int>> res;

    std::size_t startPos = 0;
    auto semicolonPos = eachGamesStr.find_first_of(";");
    while(semicolonPos != std::string::npos)
    {
        const auto gameStr = std::string_view{eachGamesStr.begin() + startPos, eachGamesStr.begin() + semicolonPos};
        res.emplace_back(parseGame(gameStr));
        startPos = semicolonPos + 1;
        semicolonPos = eachGamesStr.find_first_of(";", startPos);
    }

    const auto lastGameStr = std::string_view{eachGamesStr.begin() + startPos, eachGamesStr.end()};
    res.emplace_back(parseGame(lastGameStr));

    return res;
}

bool isSetPossible(const std::unordered_map<std::string, int>& set)
{
    for(const auto& [color, number] : set)
    {
        const auto totalItr = totalCubeQuantities.find(color);
        assert(totalItr != totalCubeQuantities.end());
        if(number > totalItr->second)
        {
            return false;
        }
    }

    return true;
}

[[nodiscard]] std::unordered_map<std::string, int> getMinNumberOfBoxesPerGame(const std::vector<std::unordered_map<std::string, int>>& games)
{
    auto result = std::unordered_map<std::string, int>{
        {"blue", 0},
        {"red", 0},
        {"green", 0}
    };

    for(const auto& game : games)
    {
        for(const auto& [color, number] : game)
        {
            result[color] = std::max(number, result[color]);
        }
    }

    return result;
}

int main()
{
    std::ifstream inputFile{"input.txt"};

    std::string lineBuffer;

    int gameNumber = 1;

    int sumOfIds = 0;
    int sumOfPowers = 0;

    while(std::getline(inputFile, lineBuffer))
    {
        std::cout << lineBuffer << std::endl;
        const auto sets = parseSets(lineBuffer);

        const auto min = getMinNumberOfBoxesPerGame(sets);

        const auto getPower = [](const auto& map)
        {
            int res = 1;
            for(const auto& [_color, val] : map)
            {
                res *= val;
            }

            return res;
        };

        const auto power = getPower(min);

        sumOfPowers += power;

        bool wasPossible = true;

        for(const auto& set : sets)
        {
            if(!isSetPossible(set))
            {
                wasPossible = false;
                break;
            }
        }

        if(wasPossible)
        {
            sumOfIds += gameNumber;
        }

        gameNumber++;
    }

    std::cout << "Sum of ids " << sumOfIds << std::endl;
    std::cout << "Sum of powers " << sumOfPowers << std::endl;

    return 0;
}