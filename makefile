all : main1

flag = O2

basic.o : basic.h basic.cpp const.h
	g++ -c basic.cpp -o basic.o -$(flag) -std=c++11 -Wall

entity.o : entity.h entity.cpp basic.o
	g++ -c entity.cpp -o entity.o -$(flag) -std=c++11 -Wall

scene.o : scene.h scene.cpp entity.o
	g++ -c scene.cpp -o scene.o -$(flag) -std=c++11 -Wall

main.o : main.cpp scene.o
	g++ -c main.cpp -o main.o -$(flag) -std=c++11 -Wall

main : scene.o entity.o basic.o main.o
	g++ scene.o entity.o basic.o main.o -o main -$(flag) -std=c++11 -Wall
