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

unsigned int generateMapValue(bool const random) // TODO : remove random at the end of the project and the if (random)
{
    if (random)
    {
        int mapValue{std::rand() % 2}; // return 0 or 1 --> empty or plain
        return mapValue;
    }
    return 1;
}

std::vector<std::vector<unsigned int>> generateMap(size_t const height, size_t const width)
{
    std::srand(std::time(nullptr));
    std::vector<std::vector<unsigned int>> map{};
    for (size_t i{}; i < height; i++)
    {
        std::vector<unsigned int> newLine{};
        for (size_t j{}; j < width; j++)
        {
            unsigned int newValue{generateMapValue(true)};
            newLine.push_back(newValue);
        }
        map.push_back(newLine);
    }
    return map;
}

// map height = lines
// map width = nb of element in one line

/*
    x controls position on the x-axis <---->
    y controls positions on the y-axis ↕
*/

std::vector<Neighbour> ajacentBlocks(
    std::vector<std::vector<unsigned int>> const &map,
    unsigned int x, unsigned int y)
{
    /*
        if there's no elements,
        y > map size aka out of range by height,
        there's elements but x > line of first element aka out of range by width
    */

    if (map.empty() || y >= map.size() || (!map.empty() && x >= map[0].size()))
    {
        return {{}};
    }

    std::vector<Neighbour> neighbours{{}};
    unsigned int nbNeighbours{4};

    if (x > 0) // if it's on the left edge no left neighbour
    {
        Neighbour left{x - 1, y, 0};
    }
    else if (x < map[0].size()) // if it's on the right edge no right neighbour
    {
        Neighbour right{x + 1, y, 0};
    }
    else if (y > 0)
    {
        Neighbour up{x, y - 1, 0};
    }
    else if (y < map.size())
    {
        Neighbour down{x, y + 1, 0};
    };

    return {{}};
}

void cellularAutomata(std::vector<std::vector<unsigned int>> &map /*,unsigned int nbIterations*/)
{
    size_t height{map.size()};
    for (size_t y{}; y < height; y++)
    {
        size_t width{map[y].size()};
        for (size_t x{}; x < width; x++)
        {
            unsigned int value{map[y][x]};
        }
    }
}

