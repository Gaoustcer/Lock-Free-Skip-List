COMPILEFLAG=-std=c++17 -g
OUTPUT_DIR=build
test:test.cpp
	@echo $(COMPILEFLAG)
	g++ $< -o $(OUTPUT_DIR)/$@ $(COMPILEFLAG)