SEARCHCOUNT=32

COMPILEFLAG=-std=c++17 -pthread -DSEARCHPROCESS=$(SEARCHCOUNT) -g -DLAYERLOCK -DONEINSERT -DGLOBALLOCK
OUTPUT_DIR=build
test:test.o fileread.o config.o skiplist.hpp
	make -C build
fileread.o: fileread.cpp fileread.hpp skiplist.hpp
	g++ $< -c -o $(OUTPUT_DIR)/$@ $(COMPILEFLAG)
config.o: config.cpp config.hpp skiplist.hpp
	g++ $< -c -o $(OUTPUT_DIR)/$@ $(COMPILEFLAG)
test.o: test.cpp
	g++ $< -c -o $(OUTPUT_DIR)/$@ $(COMPILEFLAG)
clean:
	-rm build/*.o