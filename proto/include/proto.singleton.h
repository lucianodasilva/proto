#ifndef _proto_singleton_h_
#define _proto_singleton_h_

namespace proto {

	template < class _declared_t > 
	class singleton_base {
	protected:

		inline singleton_base() {}

	public:

		singleton_base(const singleton_base &) = delete;
		singleton_base(singleton_base &&) = delete;

		singleton_base & operator = (const singleton_base &) = delete;
		singleton_base & operator = (singleton_base &&) = delete;

		static inline _declared_t & instance() {
			static _declared_t singleton_inst;
			return singleton_inst;
		}

	};

}

#endif