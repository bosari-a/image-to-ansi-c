CC=gcc

toansi: toansi.c
	$(CC) toansi.c -o toansi

check_ansi:
	valgrind --leak-check=full ./toansi ./assets/kirby.png ansi

clean:
	rm toansi
