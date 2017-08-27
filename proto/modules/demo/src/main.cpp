
#include <proto.h>

#include <iostream>
#include <string>

using namespace proto;

void other_stuff(std::string const & xxx) {
	std::cout << xxx << std::endl;
}

void update (run_scheduler & scheduler) {
	std::cout << "updatings" << std::endl;

	auto rnd = rand() % 100;

	if (rnd < 10) {
		for (decltype(rnd) i = 0; i < rnd; ++i) {
			scheduler.enqueue(&other_stuff, std::to_string(rnd));
		}
	}
}

int main(int arg_c, char * arg_v[]) {

	run_scheduler main_runner;

	main_runner.run(&update);

	return 0;
}