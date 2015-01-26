#pragma once
#ifndef _imp_datadriven_h_
#define _imp_datadriven_h_

#include <map>
#include <vector>
#include <random>

#include "test_math.h"
#include "abstract.h"

namespace proto {

	namespace imp_datadriven {

		struct property_handle {
			id_t		id;
			uint32_t	index;
		};

		struct handle_container {
		private:
			int _index;
		public:

			property_handle _properties[10];

			static property_handle none;

			inline handle_container() : _index(0){}

			inline handle_container & add(const id_t & id, uint32_t index) {
				_properties[_index] = {
					id,
					index
				};

				++_index;

				return *this;
			}

			inline property_handle & operator [](const id_t & id) {
				for (auto & v : _properties)
				{
					if (v.id == id)
						return v;
				}

				return none;
			}

		};

		property_handle handle_container::none = { text_to_id("none"), 0 };

		// COMPONENT LOGIC

		namespace events {
			const id_t start = text_to_id("events_start");
			const id_t update = text_to_id("events_update");
			const id_t render = text_to_id("events_render");
		}

		enum class component_id : uint32_t {
			transformer = 0x01,
			visual = 0x02
		};

		inline component_id operator | (component_id origin, component_id other) {
			return component_id((uint32_t)origin | (uint32_t)other);
		}

		inline bool has_flag(component_id origin, component_id v) {
			return ((uint32_t)origin & (uint32_t)v) == (uint32_t)v;
		}

		namespace handles {
			const id_t position_handle = text_to_id("position");
			const id_t visual_handle = text_to_id("visual");
		}

		struct entity {
			component_id components;
			handle_container handles;
		};

		struct entity_manager;

		struct isystem {
			virtual void initialize(entity_manager & manager) = 0;
			virtual void update(const id_t & event_id) = 0;
		};

		struct entity_manager {

			std::vector < entity > entities;
			std::vector < isystem * > systems;

			void initialize() {
				for (auto i : systems)
					i->initialize(*this);
			}

			inline void frame() {
				update(events::update);
			}

			inline void update(const id_t & event_id) {
				for (auto i : systems)
					i->update(event_id);
			}
		};

		// SPECIFICS
		struct transformer_system : public isystem {

			struct transformer_properties {
				math::vec3
					position,
					scale;

				math::mat4
					transform;
			};

			std::vector < transformer_properties > properties;


			inline virtual void initialize(entity_manager & manager) {}

			inline virtual void update(const id_t & event_id) {
				if (events::update != event_id)
					return;

				for (auto v : properties) {
					v.transform = math::mat4::make_translation(v.position) * math::mat4::make_scale(v.scale);
				}
			}
		};

		struct visual_system : public isystem {


			struct visual_bucket {
				id_t material_id;
				id_t mesh_id;
				math::mat4 transform;
			};

			struct visual_properties {
				id_t
					material_id,
					mesh_id;

				uint32_t transform_index;
			};

			entity_manager * e_manager;
			transformer_system * trans_system;

			std::vector < visual_bucket > render_items;
			std::vector < visual_properties > properties;

			inline void reg(const visual_bucket & bucket) {
				render_items.push_back(bucket);
			}

			inline virtual void initialize(entity_manager & manager) {
				e_manager = &manager;

				trans_system = (transformer_system*)manager.systems[0];
			}

			inline virtual void update(const id_t & event_id) {
				if (events::update != event_id)
					return;

				for (auto v : properties) {

					auto tp = trans_system->properties[v.transform_index];
					reg({
						v.material_id,
						v.mesh_id,
						tp.transform
					});
				}

				for (auto vb : render_items) {
					math::mat4 id = math::mat4::identity;
					math::mat4 t = id * vb.transform;
					vb.transform = t.invert();
				}

				render_items.clear();
			}
		};

		struct factory {

			inline static entity_manager create_manager() {
				entity_manager e_manager;

				e_manager.systems.push_back(new transformer_system());
				e_manager.systems.push_back(new visual_system());

				return e_manager;
			}

			inline static void create_entity(entity_manager & manager, abstract::entity_desc & desc) {
				
				uint32_t ent_type = std::rand() % 4;

				entity new_entity;

				transformer_system * ts = (transformer_system*)manager.systems[0];
				visual_system * vs = (visual_system*)manager.systems[1];

				switch (ent_type) {
				case (1) : {
					new_entity.components = component_id::transformer;
					new_entity.handles.add(handles::position_handle, ts->properties.size());
					ts->properties.push_back(transformer_system::transformer_properties());
					break;
				}
				case (2) : {
					new_entity.components = component_id::visual;

					new_entity.handles.add(handles::visual_handle, vs->properties.size());
					vs->properties.push_back(visual_system::visual_properties{
						text_to_id("some id"),
						text_to_id("some other id"),
						0
					});
					break;
				}
				case (3) : {
					new_entity.components = component_id::visual | component_id::transformer;

					uint32_t transf_i = ts->properties.size();
					new_entity.handles.add(handles::position_handle, transf_i);
					new_entity.handles.add(handles::visual_handle, vs->properties.size());

					ts->properties.push_back(transformer_system::transformer_properties());
					vs->properties.push_back(visual_system::visual_properties{
						text_to_id("some id"),
						text_to_id("some other id"),
						transf_i
					});

					break;
				}
				default: {
					// do nothing
				}
				}

				manager.entities.push_back(new_entity);
			};

		};

	}

}

#endif