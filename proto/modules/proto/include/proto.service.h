#pragma once
#ifndef	_proto_service_h_
#define _proto_service_h_

#include <memory>
#include <thread>
#include <type_traits>
#include <vector>

#include "proto.details.h"
#include "proto.id.h"

namespace proto {

	class service_base {
	public:

		virtual id_t id() const = 0;

		virtual expected < void > initialize() = 0;

		virtual void update() = 0;

	};

	template < class _service_t >
	class async_service : public service_base {
	public:

		static_assert (std::is_base_of < service_base, _service_t >::value, "service_t type must be derived from service_base");

		virtual id_t id() const override {
			return _service->id();
		}

		virtual expected < void > initialize() override {
			auto init_result = _service->initialize();

			if (init_result) {
				_runner = std::thread([this]() {
					_running = true;

					for (;_running;)
						_service->update();
				});
			}

			return init_result;
		}

		virtual void update() override {}

		virtual ~async_service() {
			if (_runner.joinable()) {
				_running = false;
				_runner.join();
			}
		}

	private:
		std::unique_ptr < _service_t >	_service;
		std::atomic < bool >			_running = false;
		std::thread						_runner;
	};

	class service_manager {
	public:

		expected < void > initialize();
		void update();

		service_base * find_service(id_t const & id) const;

		template < 
			class _service_t, 
			class = std::enable_if <std::is_base_of < service_base, _service_t >::value>,
			class ... _ctor_args 
		>
		inline service_base * create_service(_ctor_args && ... args) {
			_service_t * service = new _service_t(args...);
			_services.push_back(service);

			return service;
		}

	private:
		using service_vector = std::vector < std::unique_ptr < service_base > >;

		service_vector _services;
	};

}

#endif