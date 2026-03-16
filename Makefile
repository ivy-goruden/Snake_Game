TEST_SRCS := $(wildcard tests/*.cpp tests/snake/*.cpp tests/tetris/*.cpp)
FLAGS := -Werror -Wextra -Wall -I ./
BUILD = ../build
GTEST_LIBS = -lgtest -lgtest_main -pthread

SNAKE_GUI := $(wildcard gui/cli/snake/*.cpp)
SNAKE_GAME := $(wildcard brick_game/snake/*.cpp)
SNAKE_GAME_OBJS := $(SNAKE_GAME:.cpp=.o)
TETRIS_GUI := $(wildcard gui/cli/tetris/*.cpp)
TETRIS_GAME := $(wildcard brick_game/tetris/*.cpp)
TETRIS_GAME_OBJS := $(TETRIS_GAME:.cpp=.o)

MAIN_GAME := gui/game.cpp
GLOBALS := $(wildcard brick_game/*.cpp)
all: clean brick_game.a install run

test: clean brick_game.a
	@mkdir -p $(BUILD)
	g++ $(FLAGS) $(TEST_SRCS) brick_game.a -o $(BUILD)/test $(GTEST_LIBS)
	./$(BUILD)/test

brick_game.a: $(SNAKE_GAME_OBJS) $(TETRIS_GAME_OBJS) $(GLOBALS:.cpp=.o)
	ar rcs brick_game.a $(SNAKE_GAME_OBJS) $(TETRIS_GAME_OBJS) $(GLOBALS:.cpp=.o)
	

%.o: %.cpp
	 g++ $(FLAGS) -c $< -o $@

gcov_report: test
	mkdir -p $(BUILD)
	gcc -fprofile-arcs -ftest-coverage $(TEST_SRCS) -o $(BUILD)/gcov_report -Iinclude brick_game.a brick_game/tetris/s21_matrix.a -lcheck -lm -lpthread -lsubunit
	$(BUILD)/gcov_report
	lcov -c -d $(BUILD) --base-directory . --ignore-errors source -o $(BUILD)/coverage.info
	genhtml -o $(BUILD)/report $(BUILD)/coverage.info
	open $(BUILD)/report/index.html

install:
	g++ $(FLAGS) $(SNAKE_GUI) $(SNAKE_GAME) $(TETRIS_GUI) $(TETRIS_GAME) $(MAIN_GAME) $(GLOBALS) -o $(BUILD)/game -lncursesw -lm
	install -d $(BUILD)
	# Устанавливаем исполняемый файл
	install -m 755 $(BUILD)/game $(BUILD)
	
	# Устанавливаем библиотеку
	install -m 644 $(BUILD)/brick_game.a $(BUILD)
	
	# Устанавливаем данные (GUI файлы и другие ресурсы)
	install -m 644 gui/cli/* $(BUILD)
	make dvi

uninstall:
	rm -rf $(BUILD)

dvi:
	install -d $(BUILD)
	install -m 644 images/* $(BUILD)/

dist:
	tar -czf ../game.tar.gz $(BUILD)/*


clean:
	rm -f *.o *.a  *.out $(BUILD)/game

run:
	valgrind --suppressions=ncurses.supp --leak-check=full $(BUILD)/game
