#include "echo.hpp"
#include "circular_iterator.hpp"

#include <cmath>
#include <cstdint>
#include <vector>

using buffer_t = std::vector<std::int_least16_t>;
using circular_buffer_iterator_t = circular_iterator<buffer_t>;

std::vector<buffer_t>
initialize_echo_buffers(int echoLayers, std::size_t samplingFreq) {
	auto echoBuffers = std::vector<std::vector<std::int_least16_t>>();
	for (int i = 0; i < echoLayers; i++) {
		echoBuffers.emplace_back((i + 1) * samplingFreq);
	}
	return echoBuffers;
}

std::vector<circular_buffer_iterator_t>
initialize_circular_iterators(std::vector<buffer_t>& echoBuffers) {
	auto circIterators = std::vector<circular_buffer_iterator_t>();
	for (int i = 0; i < echoBuffers.size(); i++) {
		circIterators.emplace_back(echoBuffers[i]);
	}
	return circIterators;
}

template<class InputIterator, class OutputIterator, class CircularIterator>
void apply_echo_to_sequence(
	InputIterator begin,
	InputIterator end,
	OutputIterator out,
	CircularIterator& circIt,
	double attenuation)
{
	for (auto it = begin; it != end; ++it) {
		*circIt = *it;
		++circIt;
		*out += *circIt * attenuation;
		++out;
	}
}

void apply_echo_to_file(std::FILE& baseFile, int echoLayers, double attenuation, std::FILE& outFile) {

	constexpr std::size_t SAMPLING_FREQUENCY_HZ = 44100;
	constexpr std::size_t IO_BUFFER_SIZE_BYTES = 1 << 12;	// Arbitrary choice of chunk size.

	auto echoBuffers = initialize_echo_buffers(echoLayers, SAMPLING_FREQUENCY_HZ);
	auto circularIterators = initialize_circular_iterators(echoBuffers);

	auto inBuffer = buffer_t(IO_BUFFER_SIZE_BYTES);
	auto samplesRead = std::size_t(0);
	do {
		samplesRead = std::fread(inBuffer.data(), 2, inBuffer.size(), &baseFile);
		auto outBuffer = inBuffer;

		for (int i = 0; i < echoLayers; i++) {

			apply_echo_to_sequence(
				inBuffer.begin(),
				inBuffer.begin() + samplesRead,
				outBuffer.begin(),
				circularIterators[i],
				std::pow(attenuation, i + 1)
			);

		}

		std::fwrite(outBuffer.data(), 2, samplesRead, &outFile);
	} while (samplesRead == inBuffer.size());
}
