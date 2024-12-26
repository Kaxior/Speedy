CXX = g++
CXXFLAGS = -Wall -std=c++11 -Iinclude

# Liste des fichiers objets
OBJS = obj/carte.o obj/main.o

# Nom de l'exécutable
TARGET = jeu_cartes

# Règle par défaut
all: obj $(TARGET)

# Création du dossier obj si inexistant
obj:
	mkdir -p obj

# Compilation du programme
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation des fichiers sources en fichiers objets
obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et de l'exécutable
clean:
	rm -rf obj $(TARGET)

.PHONY: all clean