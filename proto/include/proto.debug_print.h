#ifndef _proto_debug_message_h_
#define _proto_debug_message_h_

namespace proto {
	namespace debug {

#	ifdef _DEBUG

	class debug_message {
	private:
	
		std::stringstream	_stream;
		bool				_active;
	
	public:
	
		// remove copy
		debug_message () = delete;
		debug_message (const debug_message &) = delete;
		debug_message operator = (const debug_message  &) = delete;
	
		// new instance
		template < class _t >
		inline debug_message (const _t & v) :
			_active (true),
			_stream ()
		{
			_stream << v;
		}
	
		// move operator
		inline debug_message (debug_message && o) :
		_active (o._active),
		_stream (std::move (o._stream))
		{
			o.dismiss ();
		}
	
		template < class _t >
		inline debug_message & operator << (const _t & v) {
			_stream << v;
			return *this;
		}
	
		inline ~debug_message () {
			if (_active) {
				std::cerr << _stream.str () << std::endl;
			}
		}
	
		inline void dismiss () { _active = false; }
	
	};

#		define debug_print proto::debug::debug_message ("[ " __FUNCTION__ " ] ")

#else

	class debug_message {
	public:
		template < class _t >
		inline debug_message & operator << (const _t & v) { return *this; }
	
	};

#	define debug_print proto::debug::debug_message ()
		
#endif
	}
}

#endif