# Makefile pour Speedy

COMPILER = g++
TARGET = speedy
SRC = source/main.cpp source/jeu.cpp source/joueur.cpp source/carte.cpp source/utilitaires.cpp
INC = include/jeu.h include/joueur.h include/carte.h include/utilitaires.h
CFLAGS = -Wall -O2 -std=c++17 -Iinclude

$(TARGET): $(SRC) $(INC)
	$(COMPILER) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -r $(TARGET)
