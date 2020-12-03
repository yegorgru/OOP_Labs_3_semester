#include "engine.h"

Engine::Engine(int rows_,int cols_,int density_):rows(rows_),cols(cols_),density(density_){

    std::random_device rd;
    std::mt19937 mersenne(rd());

    FIELD.assign(rows,std::vector<bool>(cols,0));
    for(auto& i:FIELD){
        for(size_t j=0;j<(size_t)cols;j++){
            i[j] = (int)mersenne()%100<density_ ? 1 : 0;
        }
    }
}

Engine::Engine(){
    cols = 0;
    rows = 0;
    density = 0;
}

int Engine::GetRows(){
    return this->rows;
}

int Engine::GetCols(){
    return this->cols;
}

void Engine::update(std::vector<std::pair<int,int>>& changed){
    std::vector<std::vector<bool>> new_field;
    changed.clear();
    new_field.assign(rows,std::vector<bool>(cols));
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            int Neighbours = getNeighboursCount(i,j);
            if(Neighbours==3 || (Neighbours==2 && FIELD[i][j])){
                new_field[i][j] = 1;
            }
            else {
                new_field[i][j] = 0;
            }
            if(FIELD[i][j] != new_field[i][j]){
                changed.push_back(std::make_pair(i,j));
            }
        }
    }

    FIELD = new_field;
}

int Engine::getNeighboursCount(int row, int col){
    int topN = row==0? this->rows-1:row-1;
    int bottomN = row==this->rows-1? 0:row+1;
    int leftN = col==0? this->cols-1:col-1;
    int rightN = col==this->cols-1? 0:col+1;
    int counter = 0;
    counter+=(int)FIELD[topN][leftN];
    counter+=(int)FIELD[topN][col];
    counter+=(int)FIELD[topN][rightN];
    counter+=(int)FIELD[row][leftN];
    counter+=(int)FIELD[row][rightN];
    counter+=(int)FIELD[bottomN][leftN];
    counter+=(int)FIELD[bottomN][col];
    counter+=(int)FIELD[bottomN][rightN];
    return counter;
}

const std::vector<std::vector<bool>>& Engine::getFIELD(){
    return FIELD;
}
