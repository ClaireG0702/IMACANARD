#include "includes/map.hpp"
#include <climits>

void displayMap(std::vector<Cell> const &map)
{
    float xMax{map.back().positions.x};
    for (Cell cell : map)
    {
        switch (cell.value)
        {
        case 0:
            std::cout << ". ";
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
        std::cout << " ";
        if (cell.positions.x == xMax)
        {
            std::cout << std::endl;
        };
    }
}

unsigned int generateMapValue(bool const random) // TODO : remove random at the end of the project and the if (random)
{
    if (random)
    {
        // TODO : possible to change std::rand to take into account probabilities
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

std::optional<Cell> findNeighbour(std::vector<Cell> const &map, Cell const &cell, int movementX, int movementY)
{
    /*
            0 1 2 3              0 1 2 3              0 1 2
    4x4 : 0 x x x x      4x3 : 0 x x x x      3x4 : 0 x x x
          1 x x x x            1 x x x x            1 x x x
          2 x x x x            2 x x x x            2 x x x
          3 x x x x                                 3 x x x

    - tableau 4x4 -> i = 4y+x, x = i%4, y = i/4, n = 4 = xMax+1
    - tableau 4x3 -> i = 4y+x, x = i%4, y = i/4, n = 4 = xMax+1
    - tableau 3x4 -> i = 3y+x, x = i%3, y = i/3, n = 3 = xMax+1
    - tableau mxn -> i = ny+x, x = i%n, y = i/n, n = xMax+1
    */

    size_t const n{static_cast<size_t>(map.back().positions.x + 1)};

    int const newX{static_cast<int>(cell.positions.x + movementX)};
    int const newY{static_cast<int>(cell.positions.y + movementY)};

    size_t cellIndex{n * newY + newX}; // cellIndex if it exists

    // if cellIndex supposed x and y doesn't match newX and newY it's invalid and shouldn't be returned
    if (cellIndex < map.size() &&
        cellIndex % n == static_cast<size_t>(newX) &&
        cellIndex / n == static_cast<size_t>(newY))
    {
        return map[cellIndex];
    }

    return std::nullopt;
};

std::vector<Cell> adjacentCells(std::vector<Cell> const &map, Cell const &cell) // return vect of adjacent cells
{
    std::vector<Cell> neighbours{};

    // std::cout << "actuel : \n \tx : " << cell.positions.x << ", y : " << cell.positions.y << ", value : " << cell.value << std::endl;

    // std::cout << "voisins : " << std::endl;
    for (int y{-1}; y <= 1; y++)
    {
        for (int x{1}; x >= -1; x--)
        {
            std::optional<Cell> adjacentCell{findNeighbour(map, cell, x, y)};
            if ((x != 0 || y != 0) && adjacentCell.has_value())
            {
                neighbours.push_back(adjacentCell.value());
                // std::cout << "\tx : " << adjacentCell.value().positions.x << ", y : " << adjacentCell.value().positions.y
                //           << ", value : " << adjacentCell.value().value << std::endl;
            }
        }
    }
    return neighbours;
}
std::vector<Cell> cellularAutomata(std::vector<Cell> const &map)
{
    std::vector<Cell> newMap{};
    for (Cell const cell : map)
    {
        std::vector<Cell> neighbours{adjacentCells(map, cell)};
        Cell newCell{cell};
        if (neighbours.size() >= 4)
        {
            neighbours.push_back(cell);
            int nbPlain{std::accumulate(neighbours.begin(), neighbours.end(), 0,
                                        [](int sum, const Cell &cell)
                                        { return sum + cell.value; })}; // add to sum value of each cell based on the fact that 0 is empty
            newCell.value = nbPlain >= 5 ? 1 : 0;                       // 5 plain cells around (+itself)
        }
        else
        {
            newCell.value = 0;
        }
        newMap.push_back(newCell);
    }
    return newMap;
}

std::vector<Cell> generateCellularMap(std::vector<Cell> const &map, int nbIterations)
{
    if (nbIterations < 0)
    {
        return {};
    }
    std::vector<Cell> newMap{};
    for (int i{0}; i < nbIterations; i++)
    {
        newMap = cellularAutomata(map);
    }
    return newMap;
}

void displayValuedMap(std::vector<Cell> const &map)
{
    std::cout << "\n"
              << std::endl;

    for (Cell cell : map)
    {
        if (cell.positions.x == 0)
        {
            std::cout << std::endl;
        };
        if (cell.value < INT_MAX)
        {
            std::cout << cell.value << " ";
        }
        else
        {
            std::cout << "X ";
        }
    }
}

void displayDirectedMap(std::vector<CellDirection> const &map)
{
    std::cout << "\n"
              << std::endl;

    for (CellDirection cell : map)
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
