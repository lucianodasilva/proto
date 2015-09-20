#pragma once

#ifndef	_proto_color_h_
#define _proto_color_h_

#include "proto.math.h"

namespace proto {

	struct color_int {

		static const uint32_t size = 3;

		union {
			uint8_t data[size];
			struct {
				uint8_t r, g, b;
			};
		};

		static color_int from_color (const color & c) {
			return {
				(uint8_t)(c.r * 255),
				(uint8_t)(c.g * 255),
				(uint8_t)(c.b * 255)
			};
		}
	};

	struct color4_int {

		static const uint32_t size = 4;

		union {
			uint8_t data[size];
			struct {
				uint8_t r, g, b, a;
			};

			uint32_t uint32;
		};

		static color4_int from_color (const color4 & c) {
			return {
				(uint8_t)(c.r * 255),
				(uint8_t)(c.g * 255),
				(uint8_t)(c.b * 255),
				(uint8_t)(c.a * 255)
			};
		}

		inline operator uint32_t () const {
			return uint32;
		}

	};
}

#endif