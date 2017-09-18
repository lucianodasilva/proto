#pragma once
#ifndef	_proto_application_h_
#define _proto_application_h_

#include "proto.details.h"
#include "proto.dispatcher.h"

#include <memory>

namespace proto {

	class application_base : public non_copyable  {
	public:

		virtual ~application_base() = default;

		virtual dispatcher_base & dispatcher() = 0;
		virtual std::atomic < bool > const & is_running() const = 0;

		virtual expected < void > run() = 0;
		virtual void exit() = 0;


	protected:

		virtual expected < void > initialize();
		virtual void update() = 0;

	};

	class application : public application_base {
	public:

		application();
		virtual ~application();

		dispatcher_base & dispatcher() override;
		std::atomic < bool > const & is_running() const override;

		expected < void > run() override;
		void exit() override;

	private:

		proto::dispatcher <>	_dispatcher;
		std::atomic < bool >	_running;

	};

	namespace details {

		using application_update_callback_t = std::function < void(application_base &) >;

		class callback_application : public application {
		public:
			callback_application(application_update_callback_t && callback);
		protected:
			void update() override;
		private:
			application_update_callback_t _callback;
		};

	}

	inline expected < void > run_application(details::application_update_callback_t && update_callback) {
		details::callback_application app(std::move(update_callback));
		return app.run();
	}

}

#endif
