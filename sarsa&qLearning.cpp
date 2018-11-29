#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <float.h>

int rows = 4, columns = 12;

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
        return -101;
    }else if (row == rows-1 && column == columns-1){
        return 1;
    }

    return -1;
};

bool usePolicy(){
    return (rand()%10 == 0);
}

double redefineAction(double currentActionValue, double alpha, int reward, double nextStateAction){
    return currentActionValue + alpha*(reward + nextStateAction - currentActionValue);
}

double sPrimeMax(State& s){
    double highestValue = -DBL_MAX;

    for (auto & x : s.availableActions){
        if (x.value > highestValue)
            highestValue = x.value;
    }

    if (highestValue == -DBL_MAX)
        return 0;

    return highestValue;
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

    int row, column;

    int score = 0;
    for (int i =0; i < 10000; i++){
        //std::cout << i << std::endl;
        row = rows-1;
        column = 0;
        
        while(true){
            // std::cout << "ROW: "<< row << " Column: " << column << std::endl;
            // for (int y = 0; y < rows; y++){
            //     for (int x = 0; x < columns; x++){
            //         switch (states[y][x].policy){
            //             case NORTH:
            //                 std::cout << "^";
            //                 break;
            //             case SOUTH:
            //                 std::cout << "v";
            //                 break;
            //             case EAST:
            //                 std::cout << ">";
            //                 break;
            //             case WEST:
            //                 std::cout << "<";
            //                 break;
            //             default:
            //                 std::cout<< "0";
            //                 break;
            //         } 
            //     }
            //     std::cout<<'\n';
            // }

            

            int location = detReward(row, column);
            if (location == -101){
                break;
            }else if (location == 1){
                score++;
                break;
            }

            State& currentState = states[row][column];
            if (!usePolicy() || currentState.policy == NONE){
                double highestValue = -DBL_MAX;
                int index = 0;

                for (int j = 0; j < currentState.availableActions.size(); j++){
                    if (currentState.availableActions.at(j).value > highestValue){
                        highestValue = currentState.availableActions.at(j).value;
                        index = j;
                    }
                }

                Action& chosenAction = currentState.availableActions.at(index);
                
                currentState.policy = chosenAction.direction;
                switch (chosenAction.direction){
                    case NORTH:
                        chosenAction.value = redefineAction(chosenAction.value, alpha, detReward(row-1, column), sPrimeMax(states[row-1][column]));
                        row-=1;
                        break;
                    case SOUTH:
                        chosenAction.value = redefineAction(chosenAction.value, alpha, detReward(row+1, column), sPrimeMax(states[row+1][column]));
                        row+=1;
                        break;
                    case EAST:
                        chosenAction.value = redefineAction(chosenAction.value, alpha, detReward(row, column+1), sPrimeMax(states[row][column+1]));
                        column+=1;
                        break;
                    case WEST:
                        chosenAction.value = redefineAction(chosenAction.value, alpha, detReward(row, column-1), sPrimeMax(states[row][column-1]));
                        column-=1;
                        break;
                }

            } else {
                int index = 0;
                for (int j = 0; j < currentState.availableActions.size(); j++){
                    if (currentState.availableActions.at(j).direction == currentState.policy){
                        index = j;
                        break;
                    }
                }
                
                Action& chosenAction = currentState.availableActions.at(index);
                switch (currentState.policy){
                    case NORTH:
                        chosenAction.value = redefineAction(chosenAction.value, alpha, detReward(row-1, column), sPrimeMax(states[row-1][column]));
                        row-=1;
                        break;
                    case SOUTH:
                        chosenAction.value = redefineAction(chosenAction.value, alpha, detReward(row+1, column), sPrimeMax(states[row+1][column]));
                        row+=1;
                        break;
                    case EAST:
                        chosenAction.value = redefineAction(chosenAction.value, alpha, detReward(row, column+1), sPrimeMax(states[row][column+1]));
                        column+=1;
                        break;
                    case WEST:
                        chosenAction.value = redefineAction(chosenAction.value, alpha, detReward(row, column-1), sPrimeMax(states[row][column-1]));
                        column-=1;
                        break;
                } 
            }
        }
    }

    row = rows-1; 
    column = 0;

    while (row != rows-1 || column != columns-1){
        State& currentState = states[row][column];
        switch (currentState.policy){
            case NORTH:
                row-=1;
                std::cout << "NORTH" << std::endl;
                break;
            case SOUTH:
                row+=1;
                std::cout << "SOUTH" << std::endl;
                break;
            case EAST:
                column+=1;
                std::cout << "EAST" << std::endl;
                break;
            case WEST:
                column-=1;
                std::cout << "WEST" << std::endl;
                break;
        } 
    }
    std::cout << score << std::endl;

    return 0;
}