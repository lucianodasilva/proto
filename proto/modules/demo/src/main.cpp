
#include <proto.h>

#include <iostream>
#include <string>

using namespace proto;


int main(int arg_c, char * arg_v[]) {

	proto::run_application([](application_base & app) {

		std::cout << "this cenas heres" << std::endl;

	});

	return 0;
}