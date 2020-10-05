SOURCE_DIR = .
OUTPUT_BIN = ./abdalevg
CXX = g++
CXXFLAGS = -Wall -pedantic -Wno-long-long -O0 -std=c++11 -g
CXXLOCALFLAGS = -Wall

all: compile run doc
	
compile: main.o game_manager.o card.o effect.o gamer.o attack.o game.o console.o file.o data_game.o person.o ai.o
	$(CXX) -o $(OUTPUT_BIN) main.o game_manager.o card.o effect.o gamer.o attack.o game.o console.o file.o data_game.o person.o ai.o

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

game_manager.o: game_manager.cpp game_manager.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

card.o: card.cpp card.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

effect.o: effect.cpp effect.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

gamer.o: gamer.cpp gamer.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

attack.o: attack.cpp attack.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

game.o: game.cpp game.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

console.o: console.cpp console.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

file.o: file.cpp file.h
	$(CXX) -c $(CXXFLAGS) $(SOURCE_DIR)/$^

data_game.o: data_game.cpp data_game.h
	$(CXX) -c $^

person.o: person.cpp person.h
	$(CXX) -c $^

ai.o: ai.cpp ai.h
	$(CXX) -c $^

clean: 
	$(RM) *.o bot
	$(RM) *.gch bot

run:
	$(OUTPUT_BIN)

doc:
	doxygen -g Doxyfile.conf
	doxygen Doxyfile.conf

.PHONY: all run compile
