#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
	// system("gcc -pthread -o 1 ../obj/1.o -lm");
	// system("gcc -pthread -o 2 ../obj/2.o -lm");
	// system("gcc -o graph ../obj/graph.o");

	char program1[1000] = "./bin/1 ";
	char program2[1000] = "./bin/2 ";
	strcat(program1,argv[1]);
	strcat(program1," ");
	strcat(program1,argv[2]);

	strcat(program2,argv[1]);
	strcat(program2," ");
	strcat(program2,argv[2]);
	
	system(program1);
	system(program2);
	system("./bin/graph");
}
