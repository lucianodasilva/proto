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

		virtual dispatcher_base * dispatcher() = 0;
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

		dispatcher_base * dispatcher() override;
		std::atomic < bool > const & is_running() const override;

		expected < void > run() override;
		void exit() override;

	private:

		proto::dispatcher <>	_dispatcher;
		std::atomic < bool >	_running;

	};

	namespace details {

		template < class _application_t >
		using application_update_callback_t = std::function < void(_application_t &) >;

		template < class _application_t >
		class callback_application : public _application_t {
		public:

			using callback_type = application_update_callback_t < _application_t >;

			inline callback_application(callback_type callback)
				: _callback(std::move(callback)) {}

		protected:

			void update() override {
				if (_callback)
					_callback(*this);
			}

		private:
			callback_type _callback;
		};

	}

	template < class _application_t = proto::application >
	inline expected < void > run_application(
		typename details::callback_application < _application_t >::callback_type && update_callback
	) {
		static_assert (
			std::is_base_of<application_base, _application_t>::value, 
			"run_application type must be derived from application_base"
		);

		details::callback_application < _application_t > app(
			std::forward < typename details::callback_application < _application_t >::callback_type > (
				update_callback
			)
		);

		return app.run();
	}

}

#endif
