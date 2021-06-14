#include "logic.h"
#include "field.h"
#include "utils.h"

#include <fstream>
#include <vector>
#include <algorithm>

static const std::vector<std::pair<int, int>> vecs = {
    {1, 1},
    {1, 0},
    {0, 1},
    {1, -1},
    {-1, 1},
    {0, -1},
    {-1, 0},
    {-1, -1}
};

static std::vector<int> indices {0, 1, 2, 3, 4, 5, 6, 7};

Logic* Logic::instance {nullptr};

Logic *Logic::getInstance()
{
    if (!instance)
    {
        instance = new Logic;
    }
    return instance;
}

bool Logic::gameOver() const
{
    return m_eStatus == Status::DRAW or m_eStatus == Status::LOSS or m_eStatus == Status::WIN;
}

Logic::Status Logic::gameStatus() const
{
    return m_eStatus;
}

void Logic::setStatus(const Status status)
{
    m_eStatus = status;
}

void Logic::playerMove(std::unique_ptr<Field>& field)
{
    if (!field or m_eStatus != Status::PLAY)
    {
        return;
    }

    auto result {false};
    int x, y;
    do
    {
        char xc = {0}, yc = {0};
        do
        {
            std::cout << "Input x (1-3): ";
            xc = Utils::getch();
            std::cout << "Input y (1-3): ";
            yc = Utils::getch();

            if (xc < '1' or xc > '0' + SIZE or yc < '1' or yc > '0' + SIZE)
            {
                std::cout << "Incorrect coordinates! Try again!" << std::endl;
                continue;
            }
        } while (xc < '1' or xc > '0' + SIZE or yc < '1' or yc > '0' + SIZE);

        x = xc - '0';
        y = yc - '0';

        result = field->setCell(Field::Cell::PLAYER, y - 1, x - 1);
        if (!result)
        {
            std::cout << "Cell is busy! Try another cell!" << std::endl;
        }
    } while (!result);


    if (result and checkWin(field))
    {
        m_eStatus = Status::WIN;
        return;
    }

    if (!field->areFreeCells())
    {
        m_eStatus = Status::DRAW;
    }
}

void Logic::aiMove(std::unique_ptr<Field>& field)
{
    if (!field or m_eStatus != Status::PLAY)
    {
        return;
    }

    auto result {false};

    std::tuple<int,int,int> winMove;
    auto tryWin = aiTryWin(field, winMove);

    std::tuple<int,int,int> safeMove;
    auto trySafe = aiTrySafe(field, safeMove);

    if(trySafe and tryWin)
    {
        if(std::get<2>(winMove) >= std::get<2>(safeMove))
        {
            field->setCell(Field::Cell::AI, std::get<0>(winMove), std::get<1>(winMove));
            result = tryWin;
        }
        else
        {
            field->setCell(Field::Cell::AI, std::get<0>(safeMove), std::get<1>(safeMove));
            result = trySafe;
        }
    }
    else if(tryWin)
    {
        field->setCell(Field::Cell::AI, std::get<0>(winMove), std::get<1>(winMove));
        result = tryWin;
    }
    else if(trySafe)
    {
        field->setCell(Field::Cell::AI, std::get<0>(safeMove), std::get<1>(safeMove));
        result = trySafe;
    }

    if (!result)
    {
        int i, j;
        do
        {
            i = rand() % SIZE;
            j = rand() % SIZE;
        } while (!field->isCellEmpty(i, j));
        result = field->setCell(Field::Cell::AI, i, j);
    }

    if (result and checkWin(field))
    {
        m_eStatus = Status::LOSS;
    }

    if (!field->areFreeCells())
    {
        m_eStatus = Status::DRAW;
    }
}

bool Logic::checkWin(std::unique_ptr<Field>& field) const
{
    for(auto i = 0; i < SIZE; ++i)
    {
        for(auto j = 0; j < SIZE; ++j)
        {
            if(checkLine(field, i, j, 1, 1) or
               checkLine(field, i, j, 1, 0) or
               checkLine(field, i, j, 0, 1) or
               checkLine(field, i, j, 1, -1))
            {
                return true;
            }
        }
    }
    return false;
}

bool Logic::checkLine(std::unique_ptr<Field>& field, int i, int j, const int vx, const int vy) const
{
    const auto start = field->getCell(i, j);
    auto count {1};
    while (i += vy, j += vx, start != Field::Cell::EMPTY and i >= 0 and j >= 0 and i < SIZE and j < SIZE)
    {
        auto next = field->getCell(i, j);
        if (next == start)
        {
            count++;
        }
    }
    return count == SIZE;
}

bool Logic::aiTryWin(const std::unique_ptr<Field>& field, std::tuple<int,int,int>& move)
{
    return findLine(field, Field::Cell::AI, move);
}

bool Logic::aiTrySafe(const std::unique_ptr<Field>& field, std::tuple<int,int,int>& move)
{
    return findLine(field, Field::Cell::PLAYER, move);
}

bool Logic::findLine(const std::unique_ptr<Field>& field, Field::Cell cell, std::tuple<int,int,int>& move)
{
    for(auto cnt = SIZE - 1; cnt >= 1; --cnt)
    {
        std::random_shuffle(indices.begin(), indices.end());
        for(auto i = 0; i < SIZE; ++i)
        {
            for(auto j = 0; j < SIZE; ++j)
            {
                for(size_t k = 0; k < indices.size(); ++k)
                {
                    if(tryFinishLine(field, i, j, vecs[indices[k]].first, vecs[indices[k]].second, cell, cnt, move))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Logic::tryFinishLine(const std::unique_ptr<Field> &field, int i, int j, const int vx, const int vy, Field::Cell cell, const int length, std::tuple<int,int,int>& move)
{
    const auto start = field->getCell(i, j);
    auto count {1};
    while (i += vy, j += vx, start != Field::Cell::EMPTY and start == cell and i >= 0 and j >= 0 and i < SIZE and j < SIZE)
    {
        auto next = field->getCell(i, j);
        if (next == start)
        {
            count++;
        }
        if(count == length and next == Field::Cell::EMPTY)
        {
            move = {i, j, length};
            return true;
        }
        if(next == Field::Cell::EMPTY)
        {
            int ni = i + vy;
            int nj = j + vx;
            if(ni >= 0 and nj >= 0 and ni < SIZE and nj < SIZE)
            {
                auto nextnext = field->getCell(ni, nj);
                if (nextnext == cell)
                {
                    move = {i, j, SIZE};
                    return true;
                }
            }
        }
    }
    return false;
}
