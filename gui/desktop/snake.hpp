#include "../../brick_game/models/render.h"
#include <ncurses.h>
#include <map>

namespace S21 {

template <size_t height, size_t width, int start_y, int start_x>
class Snake_Render : public Render{
    private:
        enum State{
            ST_WAITING,
            ST_MOVE,
            ST_PAUSE,
            ST_WIN,
            ST_LOSE
        };
        typedef bool (*InputHandler)(int);
        typedef void (*ScreenHandler)();
        const int HEIGHT = height;
        const int WIDTH = width;
        const int START_Y = start_y;
        const int START_X = start_x;
        State cur_state;
        WINDOW *window;
        std::map<State, InputHandler> input_handlers= {
            {ST_WAITING, this->Move_Handler}, {ST_MOVE, this->Move_Handler}, {ST_PAUSE, this->Pause_Handler}, {ST_WIN, this->Win_Handler}, {ST_LOSE, this->Lose_Handler};
        };
        std::map<State, ScreenHandler> screen_handlers= {
            {ST_MOVE, this->MoveScreen_Handler}, {ST_PAUSE, this->PauseScreen_Handler}, {ST_WIN, this->WinScreen_Handler}, {ST_LOSE, this->LoseScreen_Handler}; 
        };
        InputHandler GetInputHandler();

        bool Waiting_Handler(int key){
            if (key == 's'){
                this->cur_state = ST_MOVE;
                return true;
            }
            return 0;
        }
        
        bool Move_Handler(int key){
            if (key == 'p'){
                this->cur_state = ST_PAUSE;
                return true;
            }
            return 0;
        }

        bool Pause_Handler(int key){
            if (key == 'p'){
                this->cur_state = ST_MOVE;
                return true;
            }
            return 0;
        }

        bool Win_Handler(int key){
            return 0;
        }

        bool Lose_Handler(int key){
            return 0;
        }

        void WaitingScreen_Handler(){
            mvwprintw(this->window, 2, 2, "WAITING");
        }


        void MoveScreen_Handler(){
            mvwprintw(this->window, 2, 2, "MOVE");
        }

        void PauseScreen_Handler(){
            mvwprintw(this->window, 2, 2, "PAUSE");
        }

        void WinScreen_Handler(){
            mvwprintw(this->window, 2, 2, "WIN");
        }

        void LoseScreen_Handler(){
            mvwprintw(this->window, 2, 2, "LOSE");
        }


    public:
        Snake_Render(GameModel *mdl){
            this->model = mdl;
            this->cur_state = ST_WAITING;
            this->window = newwin(this->HEIGHT, this->WIDTH, this->START_Y, this->START_X);
            if (initscr() == nullptr) {
                cbreak();               // отключаем буферизацию строк
                noecho();               // не отображать вводимые символы
                keypad(stdscr, TRUE);   // включаем функциональные клавиши
                nodelay(stdscr, TRUE);  // неблокирующий ввод (опционально)
            }
        };

        InputHandler GetInputHandler(){
            return input_handlers[this->cur_state];

        }

        UserAction_t GetAction() override {
            int ch = getch();
            UserAction_t action = NoAction;

            InputHandler handle = this->GetInputHandler();
            if handle && handle(ch) {return action;}    //если клавиша перехвачена интерфесом - игнорируем


            if (ch != ERR) {
                switch (ch) {
                    case 's': action = Start;     break;
                    case 'q': action = Terminate; break;
                    case KEY_LEFT:  action = Left;  break;
                    case KEY_RIGHT: action = Right; break;
                    case KEY_DOWN:  action = Down;  break;
                    case ' ':       action = Action; break;
                    case 'p':       action = Pause;  break;
                }
            }
            else if Tick(){
                action = Tick
            }

            return action;
        }

        void UpdateState(GameModel *model) override{
            this->model = model;
            ScreenHandler dramF = this->screen_handlers[this->cur_state]
            if dramF{
                drawF();
                wrefresh(this->window);
            } 
        }

        ~Snake_Render() override{
            delwin(this->window);
            endwin();
        }

}

}  // namespace S21
