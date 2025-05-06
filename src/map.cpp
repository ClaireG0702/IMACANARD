#include "includes/map.hpp"

void displayMap(std::vector<std::vector<unsigned int>> const &map)
{
    for (std::vector<unsigned int> const &line : map)
    {
        for (unsigned int const &value : line)
        {
            switch (value)
            {
            case 0:
                std::cout << " ";
                break;

            case 1:
                std::cout << "#";
                break;

            case 2:
                std::cout << "X";
                break;

            default:
                std::cout << "?";
                break;
            }
        }
        std::cout << std::endl;
    }
}

unsigned int generateMapValue(bool random)
{
    if (random)
    {
        // cellular automata
    }
    return 2;
}

std::vector<std::vector<unsigned int>> generateMap(size_t const height, size_t const width)
{
    std::vector<std::vector<unsigned int>> map{};
    for (size_t i{}; i < height; i++)
    {
        std::vector<unsigned int> newLine{};
        for (size_t j{}; j < width; j++)
        {
            unsigned int newValue{generateMapValue(false)};
            newLine.push_back(newValue);
        }
        map.push_back(newLine);
    }
    return map;
}