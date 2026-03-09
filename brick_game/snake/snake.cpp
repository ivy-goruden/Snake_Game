#include "snake.hpp"


void s21::Snake_Game::updateCurrentState(UserAction_t action) override{
    const auto&states = this->FSM_States;
    const auto& triggers = this->FSM_Triggers[this->cur_state];
    if (this->cur_state < states.size()){
        s21::Snake_Game::Handler main_func = this->FSM_Handlers[cur_state];
        main_func(action); //выполнение функции состояния
        for (std::size_t i = 0; i < triggers.size(); ++i){
            if (triggers[i] && triggers[i]()){
                this->cur_state = s21::Snake_Game::State(i);
                return;
            }
        }
    }
}

void s21::Snake_Game::Move_Handler(UserAction_t action){
    switch(action){
        case Left:
            this->direction = std::abs(this->direction -1)%3;
        case Right:
            this->direction = std::abs(this->direction +1)%3;
        case Action:
            this->Forward();
        case Tick:
            this->Forward()
    }
}

void s21::Snake_Game::Over_Handler(UserAction_t action){
    
}

void s21::Snake_Game::Eating_Handler(UserAction_t action){
    this->ateApple = true;
    this->NewApple();
}

bool s21::Snake_Game::FoundApple(){
    return this->body[0] == this->apple;
}

bool s21::Snake_Game::Smashed(){
    Position headPos = this->body[0];
    bool SmashedIntoSelf = false;
    bool SmashedIntoBorder = (headPos.x <0 || headPos.x >=WIDTH) || (headPos.y <0 || headPos.y >=HEIGHT);
    for (std::size_t i = 0; i < body.size(); ++i){
        if (body[i] == headPos){
            SmashedIntoSelf = true;
            break;
        }
    }
    return SmashedIntoBorder || SmashedIntoSelf;
}

bool s21::Snake_Game::isWin(){
    return body.lenght >= s21::Snake_Game::HEIGHT*s21::Snake_Game::WIDTH;
}

void s21::Snake_Game::Forward(){
    Position headPos = getNextPos();
    
    this->body.push_front(headPos);
    if (!this->ateApple){
        this->body.pop_back();
    }
    this->ateApple = false;
}

Position getNextPos(){
    Position headPos;
    Position prevPos = this->body[0];
    switch(this->direction){
        case s21::Snake_Game::LEFT:
            headPos = Position{
                x: prevPos.x-1,
                y: prevPos.y
            };
        case s21::Snake_Game::RIGHT:
            headPos = Position{
                x: prevPos.x-1,
                y: prevPos.y
            };
        case s21::Snake_Game::UP:
            headPos = Position{
                x: prevPos.x,
                y: prevPos.y -1
            };
        case s21::Snake_Game::DOWN:
            headPos = Position{
                x: prevPos.x,
                y: prevPos.y +1
            };
    }
    return headPos;
}

bool s21::Snake_Game::isOver(){
    return isWin() || Smashed();
}

void s21::Snake_Game::NewApple(){
    std::list<Position> availableSpots;
    for (std::size_t i = 0; i < this->HEIGHT; ++i){
        for (std::size_t x = 0; x < this->WIDTH; ++x){
            Position cur = Position{ x: i, y: x};
            bool available = true;
            for (std::size_t i = 0; i < this->body.size(); ++i){
                if body[i] == cur{
                    available = false;
                    break;
                }
            }
            if(available){
                availableSpots.insert(availableSpots.end(), cur);
            }
        }
    }
    int randomN = random(0, availableSpots.size()-1);
    this->apple = availableSpots[randomN];
}