all: cy_yonder

rooms.o: rooms.c main.h
	gcc -c rooms.c -o rooms.o -lncursesw

display.o: display.c main.h
	gcc -c display.c -o display.o -lncursesw

save.o: save.c main.h
	gcc -c save.c -o save.o -lncursesw

game.o: game.c main.h
	gcc -c game.c -o game.o -lncursesw

main.o: game.c main.h
	gcc -c main.c -o main.o -lncursesw

cy_yonder: rooms.o display.o save.o game.o main.o main.h
	gcc rooms.o display.o save.o game.o main.o -lncursesw -o cy_yonder

clean:
	rm -f *.o
	rm cy_yonder
