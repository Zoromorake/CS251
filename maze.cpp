#include "maze.hpp"
#include "cyclic_double_queue.hpp"
#include "iostream"
using namespace cs251;

void maze::initialize(const size_t dimX, const size_t dimY, const size_t dimZ, unsigned seed)
{
	//TODO: Remove following line and add your implementation here.
    m_dimX = dimX;
	m_dimY = dimY;
	m_dimZ = dimZ;
	m_map = new unsigned[dimX*dimY*dimZ];
	cyclic_double_queue<size_t> stack(dimX * dimY * dimZ);
    stack.enqueue_back(get_index(0, 0, 0));
    unsigned localSeed = seed;

    while (!stack.empty()) {
        size_t currentCoord = stack.back();
        stack.pop_back();
        m_map[currentCoord] |= static_cast<unsigned>(Direction::Mark);
        Direction testDirection = get_next_direction(localSeed);

        for (int i = 0; i < 6; ++i) {
            size_t neighborCoord = get_neighbor_coordinate(currentCoord, testDirection);

            if (is_valid_coordinate(neighborCoord) && !is_visited(neighborCoord)) {
                m_map[currentCoord] &= ~static_cast<unsigned>(testDirection);
                stack.enqueue_back(neighborCoord);
                m_map[neighborCoord] |= static_cast<unsigned>(Direction::Mark);
                m_map[neighborCoord] &= ~static_cast<unsigned>(1 << static_cast<unsigned>(get_opposite_direction(testDirection)));
            }
            testDirection = get_next_direction(localSeed);
        }
    }
    
}


bool maze::solve(const size_t startX, const size_t startY, const size_t startZ, const  size_t endX, const size_t endY, const size_t endZ)
{
	//TODO: Remove following line and add your implementation here.
    cyclic_double_queue<size_t> queue(m_dimX * m_dimY * m_dimZ);
    cyclic_double_queue<size_t> previous(m_dimX * m_dimY * m_dimZ);

    // Enqueue the start coordinate
    queue.enqueue_back(get_index(startX, startY, startZ));

    // Loop until the queue is empty
    while (!queue.empty()) {
        size_t currentIndex = queue.front();
        queue.pop_front();

        // If current index is the end point, break the loop
        if (currentIndex == get_index(endX, endY, endZ)) {
            break;
        }

        // For each valid, unvisited neighbor of the current index
        for (int i = 0; i < 6; ++i) {
            Direction testDirection = static_cast<Direction>(1 << i);
            size_t neighborIndex = get_neighbor_coordinate(currentIndex, testDirection);

            // If the neighbor coordinate is valid and unvisited
            if (is_valid_coordinate(neighborIndex) && !is_visited(neighborIndex)) {
                // Enqueue the neighbor's index
                queue.enqueue_back(neighborIndex);

                // Set the neighbor's "previous" index to the current index
                previous.enqueue_back(currentIndex);

                // Mark the neighbor coordinate as visited
                m_map[neighborIndex] |= static_cast<unsigned>(Direction::Mark);
            }
        }
    }
    if (!queue.empty()) { // If the end point was found
        size_t currentIndex = get_index(endX, endY, endZ);
        while (currentIndex != get_index(startX, startY, startZ)) { // Loop until the current index is the start point
            m_map[currentIndex] |= static_cast<unsigned>(Direction::Mark); // Mark the current index as part of the path using Direction::Mark
            currentIndex = previous.back(); // Set the current index to its "previous" index
            previous.pop_back();
        }
        m_map[currentIndex] |= static_cast<unsigned>(Direction::Mark); // Mark the start index as part of the path using Direction::Mark
        return true; // Maze is solvable
    }
    return false; // Maze is not solvable
}

std::string maze::print_walls(const size_t z) const
{
	//TODO: Remove following line and add your implementation here.
    std::stringstream walls;

    for (size_t y = m_dimY; y > 0; --y) {
        for (size_t x = 0; x < m_dimX; ++x) {
            size_t index = get_index(x, y - 1, z);
            walls << '+';

            if (is_wall(index, Direction::Front) && is_wall(index, Direction::Back)) {
                walls << 'X';
            } else if (is_wall(index, Direction::Back)) {
                walls << 'B';
            } else if (is_wall(index, Direction::Front)) {
                walls << 'F';
            } else {
                walls << ' ';
            }

            std::string leftPad = is_wall(index, Direction::Left) ? "---" : "   ";
            walls << leftPad;

            if (x == m_dimX - 1) {
                walls << "+";
            }
        }

        walls << '\n';

        for (size_t x = 0; x < m_dimX; ++x) {
            size_t index = get_index(x, y - 1, z);

            std::string bottomPad = is_wall(index, Direction::Bottom) ? "---" : "   ";
            walls << bottomPad;

            if (is_wall(index, Direction::Mark)) {
                walls << '#';
            } else {
                walls << ' ';
            }

            std::string rightPad = is_wall(index, Direction::Right) ? "|" : " ";
            walls << rightPad;
        }

        walls << '\n';
    }

    return walls.str();
}

std::string maze::print_marks(const size_t z) const
{
	//TODO: Remove following line and add your implementation here.
    std::stringstream marks;

    for (size_t y = m_dimY; y > 0; --y) {
        for (size_t x = 0; x < m_dimX; ++x) {
            size_t index = get_index(x, y - 1, z);
            if (is_marked(index)) {
                marks << "#";
            } else {
                marks << ' ';
            }
            if (x == m_dimX - 1) {
                marks << " ";
            }
        }
        marks << '\n';
    }
    return marks.str();
}

void maze::get_dim(size_t& dimX, size_t& dimY, size_t& dimZ) const
{
	//TODO: Remove following line and add your implementation here.
    dimX = m_dimX;
	dimY = m_dimY;
	dimZ = m_dimZ;
}

Direction maze::get_next_direction(unsigned& seed)
{
	//Please do not modify this function! 
	//Any modification of this function may result in a zero for all grading cases.
	std::mt19937 gen(seed);
	seed = gen();
	return static_cast<Direction>(1 << seed % 6);
}

size_t maze::get_index(const size_t x, const size_t y, const size_t z) const
{
	//TODO: Remove following line and add your implementation here.
    return x + m_dimX*(y + m_dimY*z);
}

bool maze::is_valid_coordinate(size_t index) const
{
    size_t x = index % m_dimX;
    size_t y = (index / m_dimX) % m_dimY;
    size_t z = index / (m_dimX * m_dimY);

    return x < m_dimX && y < m_dimY && z < m_dimZ;
}

bool maze::is_visited(size_t index) const
{
    return (m_map[index] & static_cast<unsigned>(Direction::Mark)) != 0;
}

size_t maze::get_neighbor_coordinate(size_t index, Direction direction) const
{
    size_t x = index % m_dimX;
    size_t y = (index / m_dimX) % m_dimY;
    size_t z = index / (m_dimX * m_dimY);

    switch (direction) {
        case Direction::Right:
            return is_valid_coordinate(index + 1) ? index + 1 : index;
        case Direction::Left:
            return is_valid_coordinate(index - 1) ? index - 1 : index;
        case Direction::Top:
            return is_valid_coordinate(index + m_dimX) ? index + m_dimX : index;
        case Direction::Bottom:
            return is_valid_coordinate(index - m_dimX) ? index - m_dimX : index;
        case Direction::Back:
            return is_valid_coordinate(index + m_dimX * m_dimY) ? index + m_dimX * m_dimY : index;
        case Direction::Front:
            return is_valid_coordinate(index - m_dimX * m_dimY) ? index - m_dimX * m_dimY : index;
        default:
            return index;
    }
}

Direction maze::get_opposite_direction(Direction direction) const
{
    switch (direction) {
        case Direction::Right:
            return Direction::Left;
        case Direction::Left:
            return Direction::Right;
        case Direction::Top:
            return Direction::Bottom;
        case Direction::Bottom:
            return Direction::Top;
        case Direction::Back:
            return Direction::Front;
        case Direction::Front:
            return Direction::Back;
        default:
            return direction;
    }
}

bool maze::is_wall(size_t index, Direction direction) const
{
    return (m_map[index] & static_cast<unsigned>(direction)) != 0;
}

bool maze::is_marked(size_t index) const
{
    return (m_map[index] & static_cast<unsigned>(Direction::Mark)) != 0;
}
