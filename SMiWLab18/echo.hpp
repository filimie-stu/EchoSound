#pragma once

#include <cstdio>

void apply_echo_to_file(
	std::FILE & baseFile, 
	int echoLayers, 
	double attenuation, 
	std::FILE & outFile
);
