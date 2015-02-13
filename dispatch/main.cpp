#include <iostream>

#include "command_buffer.h"

int main() {

	dispatch::command_buffer < int > buffer;

	buffer.reserve(200);
	buffer.sort();

	int temp;
	std::cin >> temp;
	
	return 0;
}