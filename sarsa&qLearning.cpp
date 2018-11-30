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
    ~Action(){}
};

class State{
public:
    std::vector<Action> availableActions;
    Direction policy = NONE;

    State(){}
    ~State(){}
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

bool explore(){
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

Action& getAction(State& state){
    if (!explore() && state.policy != NONE){
        int index = 0;
        for (int j = 0; j < state.availableActions.size(); j++){
            if (state.availableActions.at(j).direction == state.policy){
                index = j;
                break;
            }
        }

        return state.availableActions.at(index);
    } else {   
        double highestValue = -DBL_MAX;
        int index = 0;
        for (int j = 0; j < state.availableActions.size(); j++){
            if (state.availableActions.at(j).value > highestValue){
                highestValue = state.availableActions.at(j).value;
                index = j; 
            }
        }

        //Action& action = state.availableActions.at(rand()%state.availableActions.size());
        Action& action = state.availableActions.at(index);  
        state.policy = action.direction;
        return action;
    }
}


int main(){
    std::srand(std::time(nullptr));
    double alpha = .1, eGreedy = .1; 
    int row, column;
    int score = 0;


    State states[rows][columns];

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

    for (int i =0; i < 500; i++){
        //std::cout << i << std::endl;
        row = rows-1;
        column = 0;
        
        while(true){
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

    for (int y = 0; y < rows; y++){
        for (int x = 0; x < columns; x++){
            switch (states[y][x].policy){
                case NORTH:
                    std::cout << "^";
                    break;
                case SOUTH:
                    std::cout << "v";
                    break;
                case EAST:
                    std::cout << ">";
                    break;
                case WEST:
                    std::cout << "<";
                    break;
                default:
                    std::cout<< "0";
                    break;
            } 
        }
        std::cout<<'\n';
    }
    std::cout << score << std::endl;


    /*---------------------------------------------------------------*/
    /*--------------SARSA LEARNING IMPLEMENTATION--------------------*/
    /*---------------------------------------------------------------*/

    State sStates[rows][columns];

    // initialize sStates w/ available actions
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            if (i < rows-1 || (i == rows-1 && j == 0)){
                //up
                if (i > 0){
                    Action action(NORTH);
                    sStates[i][j].availableActions.push_back(std::move(action));
                }
                //down
                if (i < rows-1){
                    Action action(SOUTH);
                    sStates[i][j].availableActions.push_back(std::move(action));
                }
                //left
                if (j > 0){
                    Action action(WEST);
                    sStates[i][j].availableActions.push_back(std::move(action));
                }
                //right
                if (j < columns-1){
                    Action action(EAST);
                    sStates[i][j].availableActions.push_back(std::move(action));
                }
            }
        }
    }


    score = 0;
    State* currentState;
    Action* action;
    for (int i =0; i < 500; i++){
        //std::cout << i << std::endl;
        row = rows-1;
        column = 0;

        currentState = &sStates[row][column];
        action = &getAction(*currentState);

        
        while(true){
            int location = detReward(row, column);
            if (location == -101){
                break;
            }else if (location == 1){
                score++;
                break;
            }

            switch (action->direction){
                case NORTH:
                    row-=1;
                    break;
                case SOUTH:
                    row+=1;
                    break;
                case EAST:
                    column+=1;
                    break;
                case WEST:
                    column-=1;
                    break;
                default:
                    return -1;
            }
            State& nextState = sStates[row][column];
            
            if (nextState.availableActions.size() > 0){
                
                Action& nextAction = getAction(nextState);
                action->value = redefineAction(action->value, alpha, -1, nextAction.value);

                currentState = &nextState;
                action = &nextAction;
            }else {
                action->value = redefineAction(action->value, alpha, detReward(row, column), 0);
            } 

        }
    }

    currentState = nullptr;
    action = nullptr;


    for (int y = 0; y < rows; y++){
        for (int x = 0; x < columns; x++){
            switch (sStates[y][x].policy){
                case NORTH:
                    std::cout << "^";
                    break;
                case SOUTH:
                    std::cout << "v";
                    break;
                case EAST:
                    std::cout << ">";
                    break;
                case WEST:
                    std::cout << "<";
                    break;
                default:
                    std::cout<< "0";
                    break;
            } 
        }
        std::cout<<'\n';
    }
    std::cout << score<< std::endl;
    

    return 0;
}