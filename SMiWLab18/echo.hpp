#pragma once

#include <cstdio>

//
// For the function to work correctly, 
// base file must be sampled at 44100
// with signed 16-bit integer.
//
void apply_echo_to_file(
	std::FILE & baseFile, 
	int echoLayers, 
	double attenuation, 
	std::FILE & outFile
);
