#pragma once
#ifndef _abstract_h_
#define _abstract_h_

#include <vector>
#include <string>
#include <xhash>
#include <cinttypes>

namespace proto {

	using id_t = uint32_t;

	inline id_t text_to_id(const char * id_str) {
		return std::hash < std::string >() (id_str);
	}

#	define declare_id(x) const proto::id_t x = proto::text_to_id (#x)

	namespace id {

		namespace types {
			declare_id(static_visual_entity);
			declare_id(dynamic_visual_entity);
		}

		namespace properties {

			declare_id(position);
			declare_id(scale);
			declare_id(transform);

			declare_id(material_id);
			declare_id(mesh_id);

		}

		namespace components {
			declare_id(visual);
			declare_id(transformer);
		}

		namespace system {
			declare_id(transform);
			declare_id(graphics);
		}
	}

	namespace abstract {

		namespace details {

			struct value_desc {

				struct ivalue {
					virtual ~ivalue() {}
					virtual ivalue * clone() const = 0;
				};

				template < class _t >
				struct value : public ivalue {

					_t data;

					inline value(const _t & v) : data(v) {};

					inline virtual ivalue * clone() const {
						return new value < _t >(data);
					}

				};

				ivalue * data;

				inline value_desc(const value_desc & v) : data(v.data->clone()) {}

				template < class _t >
				inline value_desc(const _t & v) : data(new value <_t>(v)) {}

				inline ~value_desc(){ delete data; }

			};

		}

		struct property_desc {
			id_t id;
			details::value_desc value;
		};

		using property_desc_list = std::vector < property_desc > ;

		struct component_desc {
			id_t type;
			property_desc_list arguments;
		};

		using component_desc_list = std::vector < component_desc > ;

		struct entity_desc {
			id_t type;
			component_desc_list components;
			property_desc_list properties;
		};

		using entity_desc_list = std::vector < entity_desc > ;

		struct world_desc {
			entity_desc_list entity_descriptions;
		};

	}
}

#endif //_abstract_h_