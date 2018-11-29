#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>

const int rows = 4, columns = 12;

enum Direction{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NONE
};


class Action{
public:
    double value = 0;
    Direction direction = NONE;

    Action(Direction d): direction{d}{}
    Action(Action&& a): value{a.value}, direction{a.direction} {}

};

class State{
public:
    std::vector<Action> availableActions;
    Direction policy = NONE;

    State(){}
};

int detReward(int row, int column){
    if (row == rows-1 && column > 0 && column < columns-1){
        return -100;
    }else if (row == rows-1 && column == columns-1){
        return 1;
    }

    return -1;
};

bool usePolicy(){
    return (rand()%10 == 0);
}



int main(){
    std::srand(std::time(nullptr));
    
    State states[rows][columns];
    double alpha = .1, eGreedy = .1; 

    // initialize states w/ available actions
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if (i < rows-1 || (i == rows-1 && j == 0)){
                //up
                if (i > 0){
                    Action action(NORTH);
                    states[i][j].availableActions.push_back(std::move(action));
                }
                //down
                if (i < rows-1){
                    Action action(SOUTH);
                    states[i][j].availableActions.push_back(std::move(action));
                }
                //left
                if (j > 0){
                    Action action(WEST);
                    states[i][j].availableActions.push_back(std::move(action));
                }
                //right
                if (j < columns-1){
                    Action action(EAST);
                    states[i][j].availableActions.push_back(std::move(action));
                }
            }
        }
    }

    int row = rows-1;
    int column = 0;

    int score = 0;
    for (int i =0; i < 10000; i++){
        while(true){
            State& currentState = states[row][column];
            
            

            for (Action& action: currentState.availableActions){
                if (action.value > highestValue){
                    highestValue = action.value
                }
            }


        }
    }


    return 0;
}