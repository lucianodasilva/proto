#include "proto.service.h"

#include <algorithm>

namespace proto {

	expected < void > service_manager::initialize() {
		for (auto & service : _services) {
			auto r = service->initialize();
			if (!r)
				return r;
		}

		return {};
	}

	void service_manager::update() {
		for (auto & service : _services)
			service->update();
	}

	service_base * service_manager::find_service(id_t const & id) const {
		service_base * item = nullptr;

		for (auto & service : _services) {
			if (service->id() == id) {
				item = service.get();
				break;
			}
		}

		return item;
	}

}