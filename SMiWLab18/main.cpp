#define _CRT_SECURE_NO_WARNINGS // for std::fopen portability

#include "echo.hpp"

#include <cstdio>

int main(int argc, char* argv[]) {

	auto inFile = std::fopen(argv[1], "rb");
	if (!inFile) {
		std::fprintf(stderr, "Could not open input file.");
		return 1;
	}

	auto outFile = std::fopen(argv[2], "wb");
	if (!outFile) {
		std::fprintf(stderr, "Could not open output file.");
		return 2;
	}

	auto echoLayers = 1;
	auto attenunationPerLayer = 0.5;
	
	apply_echo_to_file(*inFile, echoLayers, attenunationPerLayer, *outFile);


	std::fclose(inFile);
	std::fclose(outFile);

	return 0;
}