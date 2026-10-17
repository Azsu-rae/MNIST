

.PHONY: run

run: ./serial/main.c
	gcc ./serial/main.c -o main && ./main
