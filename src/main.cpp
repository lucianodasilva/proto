#include <iostream>

#include "harness.h"
#include "imp_reference.h"
#include "imp_datadriven.h"

int main() {

	harness::tester tester{
		10000,
		5.0
	};

	tester.exec < imp_reference::factory > ("Reference");
	tester.exec < imp_datadriven::factory > ("Data Driven");

	int temp;
	std::cin >> temp;
	
	return 0;
}