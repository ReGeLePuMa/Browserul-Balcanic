build: task1 task2 task3 task4

task1: task1.c
	gcc -o task1 task1.c -g -Wall -std=c99

task2: task2.c
	gcc -o task2 task2.c -g -Wall -std=c99

task3: task3.c
	gcc -o task3 task3.c -g -Wall -std=c99

task4: task4.c
	gcc -o task4 task4.c -g -Wall -std=c99

browser: browser.c
	gcc -o browser browser.c -g -Wall -std=c99 -lncurses -lmenu

.PHONY: clean
clean :
	rm -f task1 task2 task3 task4 browser *.out
