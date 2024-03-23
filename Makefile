CC = g++
# CFLAGS = -Wall -pedantic -g -I src/include -L src/lib
CFLAGS = -Wall -pedantic -g
# LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf
SRC = $(wildcard src/*.cpp) 
OBJ = $(SRC:.cpp=.o)
# EXEC = lovetond.exe
EXEC = lovetond

.PHONY: all compile run clean doc

all: compile doc

compile: $(EXEC)

run: $(EXEC)
	./$(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(LIBS)

clean:
	rm -f $(OBJ) $(EXEC)
	rm -rf doc

doc:
	doxygen Doxyfile