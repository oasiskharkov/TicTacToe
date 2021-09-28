#pragma once

#include "field.h"
#include <memory>

class Logic
{
public:
    enum Status {PLAY, WIN, LOSS, DRAW};
public:
    Logic(const Logic&) = delete;
    Logic& operator = (const Logic&) = delete;
    static Logic* getInstance();

    bool gameOver() const;
    Status gameStatus() const;
    void setStatus(const Status status);
    void playerMove(std::unique_ptr<Field>& field);
    void aiMove(std::unique_ptr<Field>& field);
private:
    Logic(): m_eStatus {Status::PLAY} {}
    bool checkWin(std::unique_ptr<Field>& field) const;
    bool checkLine(std::unique_ptr<Field>& field, int i, int j, const int vx, const int vy) const;
    bool aiTryWin(const std::unique_ptr<Field>& field, std::tuple<int,int,int>& move);
    bool aiTryIntercept(const std::unique_ptr<Field>& field, std::tuple<int,int,int>& move);
    bool tryFinishLine(const std::unique_ptr<Field>& field, int i, int j, const int vx, const int vy, const Field::Cell cell, const int length, std::tuple<int,int,int>& move);
    bool findLine(const std::unique_ptr<Field> &field, const Field::Cell cell, std::tuple<int,int,int>& move);
private:
    Status m_eStatus;
    static Logic* instance;
};
