TEST_SRCS := $(wildcard tests/*.c)
FLAGS := -Werror -Wextra -Wall -std=c11 -D_DEFAULT_SOURCE
BUILD = ../build/
all:
	make clean;
	make brick_game.a
#	gcc $(FLAGS) -fsanitize=address -g gui/cli/game.c brick_game.a brick_game/tetris/s21_matrix.a -o game -lncurses -lm
#	./game 
	gcc $(FLAGS) gui/cli/game.c brick_game.a brick_game/tetris/s21_matrix.a -o game -lncursesw -lm
	valgrind --suppressions=ncurses.supp --leak-check=full ./game
	make install
test:
	make clean
	make brick_game.a
#	gcc -fsanitize=address -g $(FLAGS) $(TEST_SRCS) -Iinclude -lcheck -lm -lpthread -L. brick_game.a brick_game/tetris/s21_matrix.a -o main_test.out -I/opt/homebrew/include -L/opt/homebrew/lib
#	./main_test.out
	gcc $(FLAGS) $(TEST_SRCS) -Iinclude -lcheck -lm -lpthread -lsubunit -L. brick_game.a brick_game/tetris/s21_matrix.a -o main_test.out 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main_test.out
brick_game.a:
	gcc -c brick_game/tetris/brick_game.c -o brick_game.o
	ar rcs brick_game.a brick_game.o

gcov_report:
	mkdir -p $(BUILD)
	gcc -fprofile-arcs -ftest-coverage brick_game/tetris/brick_game.c $(TEST_SRCS) -o $(BUILD)/gcov_report -Iinclude brick_game.a brick_game/tetris/s21_matrix.a -lcheck -lm -lpthread -lsubunit
#	gcc -fprofile-arcs -ftest-coverage brick_game/tetris/brick_game.c $(TEST_SRCS) -o $(BUILD)/gcov_report -Iinclude brick_game.a brick_game/tetris/s21_matrix.a -lcheck -lm -lpthread -I/opt/homebrew/include -L/opt/homebrew/lib
	$(BUILD)/gcov_report
	lcov -c -d $(BUILD) --base-directory . --ignore-errors source -o $(BUILD)/coverage.info
	genhtml -o $(BUILD)/report $(BUILD)/coverage.info
	open $(BUILD)/report/index.html

install:
	install -d $(BUILD)
	# Устанавливаем исполняемый файл
	install -m 755 game $(BUILD)
	
	# Устанавливаем библиотеку
	install -m 644 brick_game.a $(BUILD)
	
	# Устанавливаем данные (GUI файлы и другие ресурсы)
	install -m 644 gui/cli/* $(BUILD)
	make dvi

uninstall:
	rm -rf $(BUILD)

dvi:
	install -d $(BUILD)
	install -m 644 scheme.png $(BUILD)/

dist:
	tar -czf ../game.tar.gz $(BUILD)/*


clean:
	rm -f *.o *.a  *.out game
