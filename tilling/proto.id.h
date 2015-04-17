#pragma once

#ifndef	 _proto_id_h_
#define  _proto_id_h_

#include <cinttypes>
#include <ostream>
#include <string>

namespace proto {

	namespace details {

		static constexpr uint64_t hash_basis (14695981039346656037);
		static constexpr uint64_t hash_prime (1099511628211);

		constexpr uint64_t hash_one (char c, const char * remain, uint64_t value) {
			return c == 0 ? value : hash_one (remain[0], remain + 1, (value ^ c) * hash_prime);
		}

		constexpr uint64_t hash (const char * v) {
			return hash_one (v[0], v + 1, hash_basis);
		}

	}

#	ifdef _DEBUG

	struct id_t {
		uint32_t		hash;
		const char *	text;
	};

	inline id_t create_id (const char * v) {
		return id_t { (uint32_t)details::hash (v), v };
	}


	constexpr id_t operator "" _id (const char * v) {
		return id_t{ (uint32_t)details::hash (v), v };
	}

	inline bool operator < (const id_t & v1, const id_t & v2) {
		return v1.hash < v2.hash;
	}

	inline std::ostream & operator << (std::ostream & s, const id_t & id) {
		s << "[" << std::to_string (id.hash) << " | " << id.text << "]";
		return s;
	}

#	else

	using id_t = uint32_t;

	inline id_t operator "" _id (const char * v) {
		return (id_t)details::hash (v);
	}

	constexpr id_t operator "" _id (const char * v) {
		return (id_t)details::hash (v);
	}

#	endif

}

#endif