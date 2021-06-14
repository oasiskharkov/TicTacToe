#pragma once

constexpr int SIZE {3};

class Field
{
public:
    enum class Cell {AI = 'O', PLAYER = 'X', EMPTY = '_'};
public:
    Field(const Field&) = delete;
    Field& operator = (const Field&) = delete;
    static Field* getInstance();

    void init();
    void print() const;
    bool isCellEmpty(const int i, const int j) const;
    bool setCell(const Cell cell, const int i, const int j);
    Cell getCell(const int i, const int j) const;
    bool areFreeCells() const;
private:
    Field() = default;
    static Field* instance;
private:
    Cell m_eField [SIZE * SIZE];
};
