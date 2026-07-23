#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <vector>
#include <string>

class Simplex {
	private:
		void simplexNoise(); // Outputs all values between 0 - 1
		float dotProduct(int8_t grad[], float x, float y);
	
	public:
		int grid_width;
		int grid_height;

		float scale;
		float* grid;
		int seed_effect;

		Simplex(int input_width, int input_height, float input_scale = 0.01, int input_seed_effect = 0) {
			grid_width = input_width;
			grid_height = input_height;
			scale = input_scale;
			seed_effect = input_seed_effect;
			
			grid = new float[grid_width * grid_height];
			simplexNoise();
		}

		int fastFloor(float x);
};
#endif
