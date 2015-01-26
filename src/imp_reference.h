#pragma once
#ifndef _imp_reference_h_
#define _imp_reference_h_

#include <map>
#include <vector>
#include <random>

#include "test_math.h"
#include "abstract.h"

namespace proto {
	namespace imp_reference {


		struct iproperty {
			virtual iproperty * clone() const = 0;
		};

		//	PROPERTY LOGIC

		template < class _t >
		struct property : public iproperty{

			_t data;

			inline property(const _t & v) : data(v) {}
			virtual ~property() {}

			inline void operator = (const _t & v) { data = v; }
			inline operator _t () { return data; }

			virtual iproperty * clone() const {
				return new property < _t >(data);
			}

		};

		namespace details {

			struct property_guard {
			private:
				iproperty * _instance;
			public:

				inline property_guard(iproperty * instance) : _instance(instance) {}

				template < class _t >
				inline operator _t () {

					if (_instance) {
						property < _t > * p = static_cast <iproperty *> (_instance);
						return p->data;
					}

					return _t();
				}

				template < class _t >
				inline void operator = (const _t & v) {
					if (_instance) {
						property < _t > * p = static_cast <iproperty *> (_instance);
						p->data = v;
					}
				}

			};

		}

		struct property_container {
		private:
			std::map < id_t, iproperty * > _properties;
		public:

			template < class _t >
			inline property_container & require(const id_t & id, const _t & default_value, property < _t > *& prop_inst) {

				auto it = _properties.find(id);
				if (it == _properties.end()) {
					auto p_inst = new property < _t >(default_value);
					_properties[id] = p_inst;
					prop_inst = p_inst;
				}
				else {
					prop_inst = static_cast <property < _t > *> (it->second);
				}

				return *this;
			}

			template < class _t >
			inline property_container & require(const id_t & id, const _t & default_value) {
				property < _t > * p;
				return require(id, default_value, p);
			}

			template < class _t >
			inline property_container & require(const id_t & id) {
				return require(id, _t());
			}

			inline details::property_guard operator [](const id_t & id) {
				auto it = _properties.find(id);
				if (it == _properties.end())
					return nullptr;
				return it->second;
			}

		};

		// COMPONENT LOGIC

		namespace events {
			const id_t start = text_to_id("events_start");
			const id_t update = text_to_id("events_update");
			const id_t render = text_to_id("events_render");
		}

		struct entity;

		struct icomponent {

			inline virtual ~icomponent() {}

			virtual void initialize(entity & entity_inst) = 0;
			virtual void update(const id_t & event_id) = 0;
		};

		struct entity {
			property_container properties;
			std::vector < icomponent * > components;

			void initialize() {
				for (auto c : components)
					c->initialize(*this);
			}

			void update(const id_t & event_id) {
				for (auto c : components)
					c->update(event_id);
			}

		};

		struct entity_manager;

		struct isystem {
			virtual void initialize(entity_manager & manager) = 0;
			virtual void update(const id_t & event_id) = 0;
		};

		struct entity_manager {

			std::map < id_t, entity > entities;
			std::vector < isystem * > systems;

			void initialize() {
				for (auto i : systems)
					i->initialize(*this);

				for (auto i : entities)
					i.second.initialize();
			}

			inline void frame() {
				update(events::update);
			}

			inline void update(const id_t & event_id) {
				for (auto i : entities)
					i.second.update(event_id);

				for (auto i : systems)
					i->update(event_id);
			}
		};

		// SPECIFICS

		struct transformer : public icomponent {

			property < math::vec3 >
				* _position,
				*_scale,
				*_rotation;

			property < math::mat4 > *
				_transform;

			inline virtual void initialize(entity & entity_inst) {
				entity_inst.properties
					.require(text_to_id("position"), math::vec3(), _position)
					.require(text_to_id("scale"), math::vec3(), _scale)
					.require(text_to_id("rotation"), math::vec3(), _rotation)
					.require(text_to_id("transform"), math::mat4(), _transform);
			}

			inline virtual void update(const id_t & event_id) {

				if (events::update != event_id)
					return;

				math::mat4 m = math::mat4::make_translation(*_position);
				m = math::mat4::make_scale(*_scale) * m;

				(*_transform) = m;
			}

		};

		struct visual_bucket {
			id_t material_id;
			id_t mesh_id;
			math::mat4 transform;
		};

		struct visual_system : public isystem {

			entity_manager * e_manager;

			std::vector < visual_bucket > render_items;

			inline void reg(const visual_bucket & bucket) {
				render_items.push_back(bucket);
			}

			inline virtual void initialize(entity_manager & manager) {
				e_manager = &manager;
			}

			inline virtual void update(const id_t & event_id) {
				if (events::update != event_id)
					return;

				e_manager->update(events::render);

				for (auto vb : render_items) {
					math::mat4 id = math::mat4::identity;
					math::mat4 t = id * vb.transform;
					vb.transform = t.invert();
				}

				render_items.clear();
			}
		};

		struct visual : public icomponent {

			property < id_t >
				* _material_id,
				*_mesh_id;

			property < math::mat4 > *
				_transform;

			visual_system * _system;

			inline virtual void initialize(entity & entity_inst) {
				entity_inst.properties
					.require(text_to_id("material_id"), text_to_id("SOME_MATERIAL_ID"), _material_id)
					.require(text_to_id("mesh_id"), text_to_id("SOME_MESH_ID"), _mesh_id)
					.require(text_to_id("transform"), math::mat4(), _transform);
			}

			inline virtual void update(const id_t & event_id) {

				if (events::render != event_id)
					return;

				_system->reg({
					*_material_id,
					*_mesh_id,
					*_transform
				});
			}

		};

		struct factory {

			inline static entity_manager create_manager() {
				entity_manager e_manager;

				e_manager.systems.push_back(new visual_system());

				return e_manager;
			}

			inline static void create_entity(entity_manager & manager, abstract::entity_desc & desc ) {

				//entity new_entity;
				//
				//if (desc.type == id::types::dynamic_visual_entity) {
				//
				//
				//}
				//else if (desc.type == id::types::static_visual_entity) {
				//
				//}
				//
				//

				uint32_t ent_type = std::rand() % 4;

				entity new_entity;

				switch (ent_type) {
				case (1) : {
					new_entity.components.push_back(new transformer());
					break;
				}
				case (2) : {
					visual * v = new visual();
					v->_system = (visual_system *)manager.systems[0];
					new_entity.components.push_back(v);
					break;
				}
				case (3) : {
					new_entity.components.push_back(new transformer());
					visual * v = new visual();
					v->_system = (visual_system *)manager.systems[0];
					new_entity.components.push_back(v);
					break;
				}
				default: {
					// do nothing
				}
				}

				manager.entities[std::rand()] = new_entity;
			};

		};

	}

}

#endif