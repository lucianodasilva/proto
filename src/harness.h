#pragma once
#ifndef _harness_h_
#define _harness_h_

#include <cinttypes>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <iomanip>

#include "harness_timer.h"

namespace harness {

	struct test_result {
		std::string name;
		double average_fps;
	};

    struct tester {

        uint32_t	entity_count;
        double		cycle_time;

        template <
            class _entity_factory_t
        >
        inline void exec ( const std::string & description ) {

			test_result	r = {
				description,
				.0
			};

			std::cout << "[ " << r.name << " ]" << std::endl;

			srand((uint32_t)std::chrono::duration_cast <std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
			auto manager = _entity_factory_t::create_manager();

			for (uint32_t i = 0; i < entity_count; ++i) {
				_entity_factory_t::create_entity(manager);
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

#endif //_harness_h_