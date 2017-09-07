#pragma once
#ifndef _proto_details_mask_h_
#define _proto_details_mask_h_

#include <type_traits>

namespace proto {

	template < class _enum_t >
	struct mask {
	public:
		static_assert (std::is_enum < _enum_t >::value, "masks must be of enum types");

		using underlying_type = std::underlying_type_t < _enum_t >;

		underlying_type value;

		inline mask(mask const &) = default;

		constexpr inline mask() noexcept
			: value(underlying_type())
		{}

		constexpr inline mask(_enum_t const & e) noexcept
			: value(static_cast <std::underlying_type_t < _enum_t >> (e))
		{}

		constexpr inline mask operator & (mask const & m) const noexcept {
			return { value & m.value };
		}

		constexpr inline mask & operator &= (mask const & m) noexcept {
			value &= m.value;
			return *this;
		}

		constexpr inline mask operator | (mask const & m) const noexcept {
			return { value | m.value };
		}

		constexpr inline mask & operator |= (mask const & m) noexcept {
			value |= m.value;
			return *this;
		}

		constexpr inline mask operator ! () const noexcept {
			return { !value };
		}

		constexpr inline mask & operator != (mask const & m) noexcept {
			value = !m.value;
			return *this;
		}

		constexpr inline bool has(_enum_t v) const noexcept {
			return (value & mask(v).value) == mask(v).value;
		}
	};

}

#endif