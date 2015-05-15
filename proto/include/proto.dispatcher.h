#ifndef _proto_dispatch_h_
#define _proto_dispatch_h_


/*

implementation thoughts

task types

simple task: non mutable data
streaming tasks: non mutable data. Input and output streams
reduce / expand tasks: ??? non-mutable data non 1:1 ( calculate average every 32 elements and output single value for each of them )

a task is a simple kernel which acts specifically on the data given
non mutable data ( kernel data )



*/

#include "proto.details.h"

#include <thread>

namespace proto {

	using task_t = std::function < void() >;

	class idispatcher {
	private:

	public:

		virtual void enqueue (task_t && t) = 0;

	};
	
}

#endif