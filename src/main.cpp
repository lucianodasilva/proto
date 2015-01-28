#include <iostream>

#include "harness.h"
#include "imp_reference.h"
#include "imp_datadriven.h"

int main() {

	using namespace proto;

	abstract::world_desc source = harness::generator::run(10000);

	harness::tester tester = {
		5.0
	};

	tester.exec < imp_reference::factory > ("Reference", source);
	tester.exec < imp_datadriven::factory > ("Data Driven", source);

	int temp;
	std::cin >> temp;
	
	return 0;
}