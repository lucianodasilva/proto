#pragma once
#ifndef _harness_h_
#define _harness_h_

#include <cinttypes>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <iomanip>

#include "abstract.h"
#include "harness_timer.h"
#include "test_math.h"

namespace proto {

	namespace harness {


		struct generator {
		private:

			inline static float rand_f (float min, float max) {
				return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
			}

			inline static float rand_f (float max) {
				return rand_f (.0, max);
			}

			inline static math::vec3 rand_vec3 () {
				return {
					rand_f (1.),
					rand_f (1.),
					rand_f (1.)
				};
			}

			inline static abstract::entity_desc generate_static_visual_entity(){
				return{
					id::types::static_visual_entity,
					{
						{ 
							id::components::visual,	// id
							{ // properties
								{ id::properties::material_id, text_to_id("xxx.material") },
								{ id::properties::mesh_id, text_to_id("xxx.mesh") }
							}
						}
					},
					{
						{ id::properties::position, rand_vec3() },
						{ id::properties::scale, rand_vec3() }
					}
				};
			}

			inline static abstract::entity_desc generate_dynamic_visual_entity() {
				return{
					id::types::static_visual_entity,
					{
						{
							id::components::visual,	// id
							{ // properties
								{ id::properties::material_id, text_to_id("xxx.material") },
								{ id::properties::mesh_id, text_to_id("xxx.mesh") }
							}
						},
						{ id::components::transformer, {} }
					},
					{
						{ id::properties::position, rand_vec3() },
						{ id::properties::scale, rand_vec3() }
					}
				};
			}

		public:

			inline static abstract::world_desc run(uint32_t entity_count) {

				srand(
					(uint32_t)std::chrono::duration_cast <
					std::chrono::milliseconds
					> (
					std::chrono::high_resolution_clock::now().time_since_epoch()
					)
					.count());

				abstract::world_desc world;

				for (uint32_t i = 0; i < entity_count; ++i) {

					uint32_t entity_type = rand() % 100;

					// static visible world object ( 7 / 10 )
					if (entity_type > 0 && entity_type <= 70) {
						world.entity_descriptions.push_back(
							generate_static_visual_entity()
							);
					}

					// dynamic visible world objects ( 3 / 10 )
					if (entity_type > 70 && entity_type < 100) {
						world.entity_descriptions.push_back(
							generate_dynamic_visual_entity()
							);
					}
				}

				return world;
			}

		};

		struct test_result {
			std::string name;
			double average_fps;
		};

		struct tester {

			double		cycle_time;

			template <
				class _entity_factory_t
			>
			inline void exec(const std::string & description, abstract::world_desc & world_desc ) {

					test_result	r = {
						description,
						.0
					};

					std::cout << "[ " << r.name << " ]" << std::endl;

					// create instances
					auto manager = _entity_factory_t::create_manager();

					for (auto & e_desc : world_desc.entity_descriptions) {
						_entity_factory_t::create_entity(manager, e_desc);
					}

					manager.initialize();

					uint32_t sum_frames = 0;
					auto start_time = harness::time_now();
					double elapsed;

					do {
						manager.frame();
						++sum_frames;
						elapsed = harness::time_elapsed_now(start_time);
					} while (elapsed < cycle_time);

					// report
					r.average_fps = (double(sum_frames) / double(cycle_time));

					std::cout << " - fps: " << std::right << std::setw(10) << r.average_fps << std::endl;
				}

		};


	}
}

#endif //_harness_h_