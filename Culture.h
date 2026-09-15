#ifndef CULTURE_H
#define CULTURE_H

#include <string>

struct NameGenerator {
	std::string s;
	std::string generateName() {
		return s;
	}

	NameGenerator() {
		s = "dog";
	}
};

struct Culture {
	std::string name;
	NameGenerator* name_generator;

	Culture(std::string set_name) {
		name = set_name;
		name_generator = new NameGenerator();
	}
};

#endif
