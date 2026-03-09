#include "snake.h"


void Snake_Game::updateCurrentState(UserAction_t action) override{
    const auto&states = this->FSM_States;
    if (cur_state < states.size()){
        auto triggers = states[this->cur_state];
        Handler main_func = this->FSM_Handlers[cur_state];
        main_func(action); //выполнение функции состояния
        for (std::size_t i = 0; i < triggers.size(); ++i){
            if (triggers[i] && triggers[i]()){
                this->cur_state = State(i);
                return;
            }
        }
    }
}