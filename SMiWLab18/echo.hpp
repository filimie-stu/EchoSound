#pragma once

#include "circular_iterator.hpp"

#include <cstdio>
#include <vector>
#include <cmath>

template<class InputIterator, class OutputIterator, class CircularIterator>
void make_echo(InputIterator begin, InputIterator end, OutputIterator out, CircularIterator& circIt, double attenuation)
{
	for (auto it = begin; it != end; ++it) {
		*circIt = *it;
		++circIt;
		*out += *circIt * attenuation;
		++out;
	}
}

void apply_echo(std::FILE& inFile, int echoLayers, double attenuation, std::FILE& outFile) {

	constexpr std::size_t IO_BUFFER_SIZE_BYTES = 1 << 12; 
	constexpr std::size_t SAMPLING_FREQUENCY_HZ = 44100;

	auto inBuffer = std::vector<std::int_least16_t>(IO_BUFFER_SIZE_BYTES);
	auto samplesRead = std::size_t(0);

	auto echoBuffers = std::vector<std::vector<std::int_least16_t>>();
	for (int i = 0; i < echoLayers; i++) {
		echoBuffers.emplace_back((i + 1) * SAMPLING_FREQUENCY_HZ);
	}
	auto circIterators = std::vector<circular_iterator<std::vector<std::int_least16_t>>>();
	for (int i = 0; i < echoLayers; i++) {
		circIterators.emplace_back(echoBuffers[i]);
	}

	do {
		samplesRead = std::fread(inBuffer.data(), 2, inBuffer.size(), &inFile);
		auto outBuffer = inBuffer;

		for (int i = 0; i < echoLayers; i++) {
			make_echo(
				inBuffer.begin(),
				inBuffer.begin() + samplesRead,
				outBuffer.begin(),
				circIterators[i],
				std::pow(attenuation, i + 1)
			);
		}
		std::fwrite(outBuffer.data(), 2, samplesRead, &outFile);
	} while (samplesRead == inBuffer.size());

}
