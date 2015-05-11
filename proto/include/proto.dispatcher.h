#ifndef _proto_dispatch_h_
#define _proto_dispatch_h_

#include <thread>

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