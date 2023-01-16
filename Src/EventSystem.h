#pragma once
#include "pch.h"

//--------------------------------------------
	namespace ES
	{
		inline int						m_needs_update = 0;
		struct slotPair {
			slotPair(int ievent_id, Slot islt)
			{
				event_id = ievent_id;
				slot = islt;
			}
			Slot slot;
			int  event_id;
		};
	//============================================================================================
		// this is how to bind
		// #include <functional>    #for std::placeholders::_1
		// HS::Dispatcher.subscribe(char[8], std::bind( &HS::CLASS::handle_function, this));
		// HS::Dispatcher.subscribe(char[8], std::bind( &HS::CLASS::handle_function, this, std::placeholders::_1 ));
		// handle_function signature is:
		//                         void handle()  }
		//                         void handle(const Event& e) { }
		
		slotPair *subscribe(std::string event_name, Slot&& slot, int event_id = -1);
		slotPair *subscribe(ET			event_type, Slot&& slot, int event_id = -1);

		// dispatch event right away
		void dispatch(ET			event_type, int event_id = -1, const CEvent* ievent = nullptr);
		void dispatch(std::string	event_name, int event_id = -1, const CEvent* ievent = nullptr);
		void dispatch(const CEvent & event);
		void dispatch(int event_id);
		void post(std::string	event_name, int event_id = -1, const CEvent* ievent = nullptr);
		void post(ET			event_type, int event_id = -1, const CEvent* ievent = nullptr);
		void post(ET event_type,packet &data);
		void post				(const CEvent& event);
		void postSkipRepeat		(const CEvent& event);
		void postSkipRepeat		(ET event_type, int event_id= -1);
		void postReplaceRepeat	(const CEvent& event);
		void postReplaceRepeat	(ET event_type, int event_id = -1);

		void flush();

		void clear();
		
		void needsUpdate();
		
		int isUpdateNeeded();
		
		void updated();

	
	//-------------------------------------------- Global
}// end namespace
