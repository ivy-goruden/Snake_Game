TEST_SRCS := $(wildcard tests/*.cpp tests/snake/*.cpp tests/tetris/*.cpp)
FLAGS := -Werror -Wextra -Wall -I ./
BUILD = ../build
GTEST_LIBS = -lgtest -lgtest_main -pthread

SNAKE_GUI := $(wildcard gui/cli/snake/*.cpp)
SNAKE_GAME := $(wildcard brick_game/snake/*.cpp)
SNAKE_GAME_OBJS := $(notdir $(SNAKE_GAME:.cpp=.o))
TETRIS_GUI := $(wildcard gui/cli/tetris/*.cpp)
TETRIS_GAME := $(wildcard brick_game/tetris/*.cpp)
TETRIS_GAME_OBJS := $(notdir $(TETRIS_GAME:.cpp=.o))
MODELS := $(wildcard brick_game/models/*.cpp)
MODELS_OBJS := $(notdir $(MODELS:.cpp=.o))
CLI_CONTROLLER := gui/cli/cli_controller.cpp

MAIN_GAME := gui/game.cpp
GLOBALS := $(wildcard brick_game/*.cpp)
GLOBALS_OBJS = $(notdir $(GLOBALS:.cpp=.o))
all: clean brick_game.a install run

test: clean $(SNAKE_GAME_OBJS) $(TETRIS_GAME_OBJS) $(GLOBALS_OBJS)
	@mkdir -p $(BUILD)
	g++ $(FLAGS) --coverage -g -O0 $(TEST_SRCS) $(SNAKE_GAME_OBJS) $(TETRIS_GAME_OBJS) $(GLOBALS_OBJS) -o $(BUILD)/test $(GTEST_LIBS)
	./$(BUILD)/test

brick_game.a: $(SNAKE_GAME_OBJS) $(TETRIS_GAME_OBJS) $(GLOBALS_OBJS) $(MODELS_OBJS)
	ar rcs brick_game.a $(SNAKE_GAME_OBJS) $(TETRIS_GAME_OBJS) $(GLOBALS_OBJS) $(MODELS_OBJS)
	
compile_desktop:
	qmake qt_test.pro -o Makefile_qt
	make -f Makefile_qt

%.o: brick_game/snake/%.cpp
	g++ $(FLAGS) --coverage -g -O0  -c $< -o $@

%.o: brick_game/tetris/%.cpp
	g++ $(FLAGS) --coverage -g -O0  -c $< -o $@

%.o: brick_game/%.cpp
	g++ $(FLAGS) --coverage -g -O0  -c $< -o $@
%.o: tests/%.cpp
		g++ $(FLAGS) --coverage -g -O0 -c $< -o $@
%.o: tests/snake/%.cpp
		g++ $(FLAGS) --coverage -g -O0 -c $< -o $@
%.o: tests/tetris/%.cpp
		g++ $(FLAGS) --coverage -g -O0 -c $< -o $@
%.o: brick_game/models/%.cpp
	g++ $(FLAGS) --coverage -g -O0 -c $< -o $@


gcov_report: clean test
	lcov --directory . --capture --output-file $(BUILD)/coverage.info --no-external --ignore-errors mismatch
	lcov --remove $(BUILD)/coverage.info '*/tests/*' --output-file $(BUILD)/filtered_coverage.info --ignore-errors mismatch --ignore-errors unused
	genhtml $(BUILD)/filtered_coverage.info --output-directory $(BUILD)/out --ignore-errors mismatch
	open $(BUILD)/out/index.html

install: brick_game.a
	g++ $(FLAGS) --coverage $(SNAKE_GUI) $(TETRIS_GUI) $(CLI_CONTROLLER) $(MAIN_GAME) -L. brick_game.a -o game -lncurses -lm
	install -d $(BUILD)
	# Устанавливаем исполняемый файл
	install -m 755 game $(BUILD)
	
	# Устанавливаем библиотеку
	install -m 644 brick_game.a $(BUILD)
	make dvi

uninstall:
	rm -rf $(BUILD)

dvi:
	install -d $(BUILD)
	install -m 644 images/* $(BUILD)/

dist:
	tar -czf ../game.tar.gz $(BUILD)/*


clean:
	rm -f -r *.o *.a *.gcno *.gcda *.out *.info game  $(BUILD)/*

run:
	valgrind --suppressions=ncurses.supp --leak-check=full $(BUILD)/game

snake-desktop:
	./gui/desktop/Snake/Snake.app/Contents/MacOS/Snake
	
tetris-desktop:
	./gui/desktop/Tetris/build/Qt_6_10_2_for_macOS-Debug/Tetris
	