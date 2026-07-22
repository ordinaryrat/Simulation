Make:
	g++ -c Main.cpp Simplex.cpp World.cpp Map.cpp -I/home/frank/Documents/SFML-3.1.0/include && g++ Main.o Simplex.o World.o Map.o -o Simulation -L/home/frank/Documents/SFML-3.1.0/lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system && export LD_LIBRARY_PATH=/home/frank/Documents/SFML-3.1.0/lib && ./Simulation
