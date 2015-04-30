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

	std::cout << tester.exec < imp_reference::factory > ("Reference", source).to_string () << std::endl;
	std::cout << tester.exec < imp_datadriven::factory > ("Data Driven", source).to_string() << std::endl;
 
	int temp;
	std::cin >> temp;
	
	return 0;
}