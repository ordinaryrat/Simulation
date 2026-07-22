#ifndef SIMPLEX_H
#define SIMPLEX_H

#include <vector>
#include <string>

class Simplex {
	public:
		int grid_width;
		int grid_height;

		float scale;
		float* grid;

		Simplex(int input_width, int input_height, float input_scale = 0.01) {
			grid_width = input_width;
			grid_height = input_height;
			scale = input_scale;
			
			grid = new float[grid_width * grid_height];
		}

		void simplexNoise();
		int fastFloor(float x);
	
	private:
		float dotProduct(int8_t grad[], float x, float y);
};
#endif
