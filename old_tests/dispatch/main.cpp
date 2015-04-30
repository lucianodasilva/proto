#include <iostream>
#include <random>
#include <cinttypes>

#include "command_buffer.h"
#include "stopwatch.h"

int main() {

	dispatch::command_buffer < uint32_t > buffer;

	std::random_device rd;
	std::default_random_engine re(rd());

	std::uniform_int_distribution < uint32_t > item_dist (5000, 1000000);
	std::uniform_int_distribution < uint32_t > key_dist (0, 70000000);
	uint32_t count = 1000000;// item_dist(re);

	std::cout << "Generating " << count << " items" << std::endl;

	buffer.reserve(count);
	for (uint32_t i = 0; i < count; ++i) {
		uint32_t key_value = key_dist(re);
		dispatch::command_key k;
		k.full = key_value;
		buffer.push_back(k, key_value);
	}

	std::cout << "Sorting..." << std::endl;

	double elapsed_sum = 0.0;
	int rep = 100;

	for (int i = 0; i < rep; ++i) {

		auto time_start = dispatch::stopwatch::time_now();

		buffer.sort();

		auto elapsed_time = dispatch::stopwatch::time_elapsed_now(time_start);
		elapsed_sum += elapsed_time;
	}

	std::cout << "Sorted in avg: " << (elapsed_sum / double(rep)) << std::endl;

	//for (uint32_t i = 0; i < count; ++i) {
	//	uint32_t v = buffer[i];
	//	std::cout << v << std::endl;
	//}

	int temp;
	std::cin >> temp;

	return 0;
}
