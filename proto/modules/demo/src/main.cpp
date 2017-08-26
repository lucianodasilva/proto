
#include <proto.h>

#include <iostream>

using namespace proto;

using test_event = event <>;

test_event on_mouse_move;

void handler() {
	std::cout << "nothin";
}

int main(int arg_c, char * arg_v[]) {

	on_mouse_move += &handler;

	on_mouse_move.invoke();

	return 0;
}