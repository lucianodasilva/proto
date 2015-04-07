#ifndef _dispatch_abstract_h_
#define	_dispatch_abstract_h_

#include <cinttypes>

/*	==== | 64 bit key | ====
 *
 *  | 2 bit			| 2 bit	| 4 nul | 2 byte		| 2 bit 				| 6 bit | 		< too much waste
 *
 * 	| key type		|
 * 		resource	| < function pointer >
 * 		render		| full screen	| render target	| command type			| layer | opaque 	| material ( texture / colors and stuffs )
 *													| ( default )			|		|			| depth
 *													| ( pre_render_custom )	| < function pointer >
 */

namespace dispatch {
	namespace commands {

		enum struct key_t : uint8_t {
			resource 	= 0x00, // 0000 0000
			render 		= 0x40	// 0100 0000
		};

		enum struct fullscreen_t : uint8_t {
			scene 		= 0x00, // 0000 0000
			postprocess = 0x10, // 0001 0000
			hud			= 0x20  // 0010 0000
		};

		using render_target_t = uint16_t;

		enum struct command_t : uint8_t {

		};
	}

	enum struct command_type : uint8_t {
		resource 	= 0x00,
		render 		= 0x01
	};

	enum struct resource_command : uint8_t {
		create 		= 0x10,
		update		= 0x11,
		erase 		= 0x12
	};

	enum struct render_target_command : uint8_t {
		clear 		= 0x00,
		present 	= 0xFF
	};

	enum struct render_opacity : uint8_t {
		opaque 		= 0x00,
		transparent = 0xF0
	};

	union command_key {

		uint32_t 	full;
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
