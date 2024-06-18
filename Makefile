all : final

final: bin_files
	./bin/main $(N) $(t)
# 	mv *.txt ./output/
# 	mv *.png ./output/

bin_files : object_files
	gcc -pthread -o ./bin/1 ./obj/1.o -lm
	gcc -pthread -o ./bin/2 ./obj/2.o -lm
	gcc -o ./bin/graph ./obj/graph.o
	gcc -o ./bin/main ./obj/main.o

object_files : #make_directories
	gcc -g -O -c ./src/1.c
	gcc -g -O -c ./src/2.c
	gcc -g -O -c ./src/graph.c
	gcc -g -O -c ./src/main.c
	mv *.o ./obj/ 

# make_directories:
# 	mkdir obj
# 	mkdir output
# 	mkdir bin

clean :
	rm -rf *.o
	rm -rf obj/*.o
	rm -rf bin/*