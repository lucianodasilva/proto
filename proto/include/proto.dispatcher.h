#ifndef _proto_dispatch_h_
#define _proto_dispatch_h_

#include <thread>

/*

implementation thoughts

task types

simple task: non mutable data
streaming tasks: non mutable data. Input and output streams
reduce / expand tasks: ??? non-mutable data non 1:1 ( calculate average every 32 elements and output single value for each of them )

a task is a simple kernel which acts specifically on the data given
non mutable data ( kernel data )



*/

namespace proto {

	struct dispatch {
		
		static inline dispatch & instance () {
			static dispatch inst;
			return inst;
		}
		
		// register fixed threads
		// initialize for calculated number of threads
		
		// fixed thread task ...
		// ( Render, Main ( Logic )
		// static inline void add_task
		
		// variable dispatch task ( worker threads )
		// static inline void add_task
		
	};
	
}

#endif