#include "includes/map.hpp"
#include <climits>

void displayMap(std::vector<Cell> const &map)
{
    for (Cell cell : map)
    {
        if (cell.positions.x == 0)
        {
            std::cout << std::endl;
        };

        switch (cell.value)
        {
        case 0:
            std::cout << "  ";
            break;

        case 1:
            std::cout << "# ";
            break;

        case 2:
            std::cout << "X ";
            break;

        default:
            std::cout << "? ";
            break;
        }
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

std::vector<Cell> generateMap(size_t const height, size_t const width)
{
    std::srand(std::time(nullptr));
    std::vector<Cell> map{};
    for (size_t y{}; y < height; y++)
    {
        for (size_t x{}; x < width; x++)
        {
            unsigned int value{generateMapValue(true)};
            Cell cell{{x, y}, value};
            // std::cout << cell.positions.x << ", " << cell.positions.y << "; ";

            map.push_back(cell);
        }
    }
    return map;
}
// map height = lines
// map width = nb of element in one line

/*
    x controls position on the x-axis <---->
    y controls positions on the y-axis ↕
*/

void displayValuedMap(std::vector<Cell> const &map)
{
    std::cout << "\n" << std::endl;

    for(Cell cell: map)
    {
        if (cell.positions.x == 0)
        {
            std::cout << std::endl;
        };
        if(cell.value < INT_MAX) {
            std::cout << cell.value << " ";
        } else {
            std::cout << "X ";
        }
    }
}

void displayDirectedMap(std::vector<CellDirection> const &map)
{
    std::cout << "\n" << std::endl;

    for(CellDirection cell: map)
    {
        if (cell.positions.x == 0)
        {
            std::cout << std::endl;
        };
        switch (cell.direction)
        {
        case Direction::UP:
            std::cout << "^ ";
            break;

        case Direction::RIGHT:
            std::cout << "> ";
            break;

        case Direction::DOWN:
            std::cout << "v ";
            break;

        case Direction::LEFT:
            std::cout << "< ";
            break;
        case Direction::NONE:
            std::cout << "X ";
            break;
        default:
            std::cout << "X ";
            break;
        }
    }
}