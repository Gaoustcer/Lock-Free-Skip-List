COMPILEFLAG=-std=c++17 -pthread
OUTPUT_DIR=build
test:test.o fileread.o config.o
	make -C build
fileread.o: fileread.cpp fileread.hpp
	g++ $< -c -o $(OUTPUT_DIR)/$@ $(COMPILEFLAG)
config.o: config.cpp config.hpp
	g++ $< -c -o $(OUTPUT_DIR)/$@ $(COMPILEFLAG)
test.o: test.cpp
	g++ $< -c -o $(OUTPUT_DIR)/$@ $(COMPILEFLAG)