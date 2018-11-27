#include <iostream>

int main(){
    int rows = 4, columns = 12;

    int map[rows][columns];


    double alpha = .9, eGreedy = .1, discount = -.5;


    // Generate Cliff map
    for (int i = rows-1; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if (j != 0 && j != columns -1){
                map[i][j] = -100;
            }else if (j == columns-1){
                map[i][j] = 1;
            }
        }
    }



    return 0;
}