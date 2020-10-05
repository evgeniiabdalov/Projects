SOURCE_DIR = ./src
OUTPUT_BIN = ./abdalevg
CXX = g++
CXXFLAGS = -Wall -pedantic -Wno-long-long -O0 -std=c++11 -g
CXXLOCALFLAGS = -Wall

all: compile run
	
compile: $(SOURCE_DIR)/main.o $(SOURCE_DIR)/game_manager.o $(SOURCE_DIR)/card.o $(SOURCE_DIR)/effect.o $(SOURCE_DIR)/gamer.o $(SOURCE_DIR)/attack.o $(SOURCE_DIR)/game.o $(SOURCE_DIR)/console.o $(SOURCE_DIR)/file.o $(SOURCE_DIR)/data_game.o $(SOURCE_DIR)/person.o $(SOURCE_DIR)/ai.o
	$(CXX) -o $(OUTPUT_BIN) $(SOURCE_DIR)/main.o $(SOURCE_DIR)/game_manager.o $(SOURCE_DIR)/card.o $(SOURCE_DIR)/effect.o $(SOURCE_DIR)/gamer.o $(SOURCE_DIR)/attack.o $(SOURCE_DIR)/game.o $(SOURCE_DIR)/console.o $(SOURCE_DIR)/file.o $(SOURCE_DIR)/data_game.o $(SOURCE_DIR)/person.o $(SOURCE_DIR)/ai.o

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

game_manager.o: $(SOURCE_DIR)/game_manager.cpp $(SOURCE_DIR)/game_manager.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

card.o: $(SOURCE_DIR)/card.cpp $(SOURCE_DIR)/card.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

effect.o: $(SOURCE_DIR)/effect.cpp $(SOURCE_DIR)/effect.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

gamer.o: $(SOURCE_DIR)/gamer.cpp $(SOURCE_DIR)/gamer.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

attack.o: $(SOURCE_DIR)/attack.cpp $(SOURCE_DIR)/attack.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

game.o: $(SOURCE_DIR)/game.cpp $(SOURCE_DIR)/game.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

console.o: $(SOURCE_DIR)/console.cpp $(SOURCE_DIR)/console.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

file.o: $(SOURCE_DIR)/file.cpp $(SOURCE_DIR)/file.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

data_game.o: $(SOURCE_DIR)/data_game.cpp $(SOURCE_DIR)/data_game.h
	$(CXX) -c $^

person.o: $(SOURCE_DIR)/person.cpp $(SOURCE_DIR)/person.h
	$(CXX) -c $^

ai.o: $(SOURCE_DIR)/ai.cpp $(SOURCE_DIR)/ai.h
	$(CXX) -c $^

clean: 
	$(RM) $(SOURCE_DIR)/*.o bot
	$(RM) $(SOURCE_DIR)/*.gch bot
	$(RM) $(OUTPUT_BIN) bot


run:
	$(OUTPUT_BIN)



.PHONY: all run compile
