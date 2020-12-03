#pragma once

#include <random>
#include <vector>
#include <utility>

class Engine
{
    int rows;
    int cols;
    int density;
    std::vector<std::vector<bool>> FIELD;
public:
    Engine();

    int GetRows();

    int GetCols();

    Engine(int rows_,int cols_,int density);

    void update(std::vector<std::pair<int,int>>& changed);

    int getNeighboursCount(int row,int col);

    const std::vector<std::vector<bool>>& getFIELD();
};
