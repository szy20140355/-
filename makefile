all : main main_debug

#flag = O2
flag = -O3 -fopenmp

basic.o : basic.h basic.cpp const.h
	g++ -c basic.cpp -o basic.o $(flag) -std=c++11

entity.o : entity.h entity.cpp basic.o
	g++ -c entity.cpp -o entity.o $(flag) -std=c++11

mesh.o : mesh.h mesh.cpp entity.o
	g++ -c mesh.cpp -o mesh.o $(flag) -std=c++11

beizer.o : beizer.h beizer.cpp entity.o
	g++ -c beizer.cpp -o beizer.o $(flag) -std=c++11

scene.o : scene.h scene.cpp entity.o mesh.o beizer.o
	g++ -c scene.cpp -o scene.o $(flag) -std=c++11

main.o : main.cpp scene.o
	g++ -c main.cpp -o main.o $(flag) -std=c++11

main : scene.o entity.o basic.o main.o
	g++ scene.o entity.o basic.o mesh.o beizer.o main.o -o main $(flag) -std=c++11

main_debug : basic.cpp entity.cpp scene.cpp main.cpp mesh.cpp
	g++ basic.cpp entity.cpp beizer.cpp scene.cpp main.cpp mesh.cpp -o main_debug -g
