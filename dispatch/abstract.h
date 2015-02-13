#ifndef _dispatch_abstract_h_
#define	_dispatch_abstract_h_

#include <cinttypes>

namespace dispatch {

	enum struct command_type : uint8_t {
		resource = 0,
		render = 1
	};

	enum struct resource_command : uint8_t {	
		create = 16,
		update = 17,
		erase = 18
	};

	enum struct render_target_command : uint8_t {
		clear = 0,
		present = 255
	};

	enum struct render_opacity : uint8_t {
		opaque = 0,
		transparent = 128
	};

	union command_key {

		uint32_t full;
		uint16_t	ushort_v[2];
		uint8_t		uchar_v [4];

		inline operator uint64_t() const {
			return full;
		}

		inline static command_key resource ( resource_command action ) {
			command_key k = { 0 };
			
			k.uchar_v[0] =
				(uint8_t)command_type::resource + (uint8_t)action;

			return k;
		}

		inline static command_key render_target_clear (uint8_t render_target_id) {
			command_key k = { 0 };

			k.uchar_v[0] = (uint8_t)command_type::render + render_target_id;
			k.uchar_v[1] = (uint8_t)render_target_command::clear;

			return k;
		}

		inline static command_key render_target_present (uint8_t render_target_id) {
			command_key k = { 0 };

			k.uchar_v[0] = (uint8_t)command_type::render + render_target_id;
			k.uchar_v[1] = (uint8_t)render_target_command::present;
			k.ushort_v[1] = 0xFFFF; // make sure its the last one

			return k;
		}

		inline static command_key render(uint8_t render_target_id, uint8_t layer, uint16_t mesh_id, render_opacity opacity, uint16_t depth) {
			command_key k = { 0 };

			k.uchar_v[0] = (uint8_t)command_type::render + render_target_id;
			k.uchar_v[1] = (uint8_t)opacity + layer;


			switch (opacity) {
			case (render_opacity::opaque) : {
					k.ushort_v[1] = mesh_id;
					break;
				}
			case (render_opacity::transparent) : {
					k.ushort_v[1] = depth;
					break;
				}
			}

			return k;
		}

	};



}

#endif