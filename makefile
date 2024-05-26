all: cy_yonder

rooms.o: rooms.c main.h
	gcc -c rooms.c -o rooms.o -lncursesw

affichage.o: affichage.c main.h
	gcc -c affichage.c -o affichage.o -lncursesw

save.o: save.c main.h
	gcc -c save.c -o save.o -lncursesw

game.o: game.c main.h
	gcc -c game.c -o game.o -lncursesw

main.o: game.c main.h
	gcc -c main.c -o main.o -lncursesw

exec: rooms.o affichage.o save.o game.o main.o main.h
	gcc rooms.o affichage.o save.o game.o main.o -lncursesw -o cy-yonder

clean:
	rm -f *.o
	rm cy yonder
