# Set default target as link
.DEFAULT_GOAL := link

# Directories
MAIN_SRCDIR = ./src
SECONDARY_SRCDIR = $(MAIN_SRCDIR)/classes
TERCIARY_SRCDIR = $(MAIN_SRCDIR)/misc
LIBDIR = ./lib
OBJDIR = ./build
INCDIR = ./include
THIRD_PARTY_INCDIR = ./thirdParty

# Compiler
CC = g++

# Output
EXEC = minesweeper

# Change .cpp to .o
SRC = $(wildcard $(TERCIARY_SRCDIR)/*.cpp $(SECONDARY_SRCDIR)/*.cpp $(MAIN_SRCDIR)/*.cpp)
OBJ = $(addprefix $(OBJDIR)/,$(notdir $(SRC:.cpp=.o)))

# Flags
CFLAGS = -I $(THIRD_PARTY_INCDIR) -I $(INCDIR) -Wall -Wextra -pedantic -std=c++11
DBGFLAGS = -g -fno-inline
LFLAGS = -lrayliblinux -lpthread -ldl

# Ignore these files
.PHONY : compile link run clean valgrind

# Compile source to outputs .o 
compile: $(OBJ)

ifeq ($(DEBUG),YES)
CFLAGS := $(CFLAGS) $(DBGFLAGS)
endif

ifeq ($(OS),Windows_NT)
LFLAGS := -lraylibwin -lopengl32 -lgdi32 -lwinmm -mwindows
endif

$(OBJDIR)/%.o: $(TERCIARY_SRCDIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(SECONDARY_SRCDIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(MAIN_SRCDIR)/%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

# Link everything together
link: compile
	$(CC) -o $(EXEC) $(OBJDIR)/*.o -L $(LIBDIR) $(LFLAGS)

# Run the program
run:
	./$(EXEC)

# Delete the program and build files
clean:
	rm -f $(EXEC)
	rm -f $(EXEC).*
	rm -f $(OBJDIR)/*.o
	
# Run valgrind to search for memory leaks
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(EXEC)