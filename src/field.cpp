#include "field.h"

#include <cstring>
#include <iostream>
#include <algorithm>

Field* Field::instance {nullptr};

Field* Field::getInstance()
{
    if(!instance)
    {
        instance = new Field;
    }
    return instance;
}

void Field::init()
{
    std::for_each(std::begin(m_eField), std::end(m_eField), [](Cell& cell) { cell = Cell::EMPTY; });
}

void Field::print() const
{
    std::system("clear");
    std::cout << "        _ _ _" << std::endl;
    for(auto i = 0; i < SIZE; ++i)
    {
        std::cout << "       |";
        for(auto j = 0; j < SIZE; ++j)
        {
            std::cout << (char)m_eField[i * SIZE + j] << '|';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Field::isCellEmpty(const int i, const int j) const
{
    return m_eField[i * SIZE + j] == Cell::EMPTY;
}

bool Field::setCell(const Cell cell, const int i, const int j)
{
    if(isCellEmpty(i, j))
    {
        m_eField[i * SIZE + j] = cell;
        return true;
    }
    return false;
}

Field::Cell Field::getCell(const int i, const int j) const
{
    return m_eField[i * SIZE + j];
}

bool Field::areFreeCells() const
{
    return std::end(m_eField) != std::find_if(std::begin(m_eField), std::end(m_eField), [](const Cell& cell) { return cell == Cell::EMPTY; });
}
