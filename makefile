Make:
	g++ -O2 -c Main.cpp Simplex.cpp World.cpp Map.cpp UIElements.cpp World.cpp Civilization.cpp -I/home/frank/Documents/SFML-3.1.0/include && g++ Main.o Simplex.o World.o Map.o UIElements.o Civilization.o -o Simulation -L/home/frank/Documents/SFML-3.1.0/lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system && export LD_LIBRARY_PATH=/home/frank/Documents/SFML-3.1.0/lib && ./Simulation
