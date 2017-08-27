//#pragma once
//
//#ifndef	_proto_event_scheduler_h_
//#define _proto_event_scheduler_h_
//
//#include "proto.scheduler.h"
//
//namespace proto {
//
//	class event_scheduler : public scheduler_base, public singleton_base < event_scheduler > {
//	private:
//		thread _thread;
//	protected:
//		friend class singleton_base < event_scheduler >;
//		event_scheduler();
//	public:
//
//		virtual bool contains_thread(const thread::id & id) const;
//
//		virtual ~event_scheduler();
//
//		template < class _ft_t, class ... _args_t >
//		static inline auto enqueue(_ft_t && f, _args_t && ... args)
//			-> future < typename result_of < _ft_t(_args_t ...)>::type >
//		{
//			auto & s = instance();
//			return static_cast <scheduler_base &> (s).enqueue(f, args...);
//		}
//	};
//
//}
//
//#endif