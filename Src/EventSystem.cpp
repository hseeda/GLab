#include "pch.h"
#include "EventSystem.h"
#include "WindowSystem.h"
#include "ImguiSystem.h"
//#include "global.h"

namespace ES
{
#define EVENTBYNAMSTART 22222
	CEvent	dummy_event;

	inline int		eventCounter = 0;
	inline int		event_store_count = 0;
	inline int      event_by_name = EVENTBYNAMSTART;

	std::unordered_map <std::string, int /*event_type*/> m_event_by_name_n;
	std::unordered_map <int /*event_type*/, std::string> m_event_by_name_i;

	std::unordered_map <int /*event_type*/, std::list<slotPair>> m_slots;

	//<int/*event_type*/, int/*event_id*/, int =-1 if no CEvent is specified, 1 otherwise>	
	inline i3		m_id_store_id[EVENT_SYSTEM_MAX_EVENTS];

	inline CEvent	m_event_store[EVENT_SYSTEM_MAX_EVENTS];

	ET getEventType(std::string event_name)
	{
		if (m_event_by_name_n.find(event_name) == m_event_by_name_n.end())
		{
			return ET::none;
		}
		return static_cast<ET> (m_event_by_name_n[event_name]);
	}
	std::string getEventName(int event_type_by_name)
	{
		if (m_event_by_name_i.find(event_type_by_name) == m_event_by_name_i.end())
		{
			return "not found\n";
		}
		return m_event_by_name_i[event_type_by_name];
	}

	
	slotPair* subscribe(std::string event_name, Slot&& slot, int event_id /*= -1*/) {
		int event_type = 0;
		try {
			event_type = m_event_by_name_n.at(event_name);
		}
		catch (std::out_of_range)
		{
			event_type = event_by_name++;
			m_event_by_name_n[event_name] = event_type;
			m_event_by_name_i[event_type] = event_name;
		}
		return subscribe(static_cast<ET> (event_type), std::move(slot), event_id);
	}
	
	slotPair* subscribe(ET event_type, Slot&& slot, int event_id /*= -1*/)
{
		std::list<slotPair>* pslots;

		try {
			pslots = &m_slots.at(event_type);
		}
		catch (std::out_of_range)
		{
			m_slots.insert({ event_type, std::list<slotPair> ()});
			pslots = &m_slots.at(event_type);
		}
		pslots->push_back(slotPair(event_id, slot));
		return &pslots->back();
	}

	void dispatch(ET event_type, int event_id /*= -1*/, const CEvent* ievent /*= nullptr*/)
	{

		std::list<slotPair>* pslots;
		try {
			pslots = &m_slots.at(event_type);
		}
		catch (std::out_of_range)
		{
			return;
		}

		int cc = 0;
		for (auto i : *pslots) {
			int slot_id = i.event_id;
			auto func = i.slot;
			if (event_id < 0 || event_id == slot_id)
			{
				if (ievent) func(*ievent);
				else func(dummy_event);
				cc++;
			}
		}
		if(static_cast<int> (event_type) >= EVENTBYNAMSTART)
			IS::log(4, "[dsptc][%8i][%2i]{%i} %s\n", eventCounter, static_cast<int> (event_type), 
				cc, getEventName(event_type).c_str());
		else
			
		IS::log(4, "[dsptc][%8i][%2i]{%i} %s\n", eventCounter, static_cast<int> (event_type), 
			cc, eventTypeName((ET)event_type).c_str());
		eventCounter++;
	}

	void dispatch(const CEvent& ievent)
	{
		ET event_type = ievent.typeID;
		int event_id = ievent.eventID;
		dispatch(event_type, event_id, &ievent);
	}

	void dispatch(int event_id)
	{
		dispatch(ET::none, event_id,nullptr);
	}

	void dispatch(std::string event_name, int event_id /*= -1*/, const CEvent* ievent /*= nullptr*/)
	{
		dispatch(getEventType(event_name), event_id, ievent);
	}

	void post(ET event_type, int event_id /*= -1*/, const CEvent* ievent /*= nullptr*/) {
		if (event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		if (ievent == nullptr) {
			m_id_store_id[event_store_count].i[0] = event_type;
			m_id_store_id[event_store_count].i[1] = event_id;
			m_id_store_id[event_store_count].i[2] = -1;
		}
		else {
			m_id_store_id[event_store_count].i[0] = event_type;
			m_id_store_id[event_store_count].i[1] = event_id;
			m_id_store_id[event_store_count].i[2] = 1;
			m_event_store[event_store_count] = *ievent;
		}
		event_store_count++; 
	}
	
	void post(const CEvent& ievent)
	{
		if (event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		
		int event_type = ievent.typeID;
		int event_id   = ievent.eventID;
		m_id_store_id[event_store_count].i[0] = event_type;
		m_id_store_id[event_store_count].i[1] = event_id;
		m_id_store_id[event_store_count].i[2] = 1;
		m_event_store[event_store_count] = ievent;
		event_store_count++;
	}

	void post(ET event_type, packet& data)
	{
		post(CEvent(event_type, data));
	}

	void post(int event_id)
	{
		post(ET::none, event_id, nullptr);
	}

	void post(std::string event_name, int event_id /*= -1*/, const CEvent* ievent /*= nullptr*/)
	{
		post(getEventType(event_name), event_id, ievent);
	}

	void postSkipRepeat(const CEvent& ievent)
	{
		if (event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		if (event_store_count > 0) {
			if (m_event_store[event_store_count - 1].typeID != ievent.typeID) {
				m_id_store_id[event_store_count].i[0] = ievent.typeID;
				m_id_store_id[event_store_count].i[1] = ievent.eventID;
				m_id_store_id[event_store_count].i[2] = 1;
				m_event_store[event_store_count] = ievent;
				event_store_count++;
			}
		}
		else {
			m_id_store_id[event_store_count].i[0] = ievent.typeID;
			m_id_store_id[event_store_count].i[1] = ievent.eventID;
			m_id_store_id[event_store_count].i[2] = 1;
			m_event_store[event_store_count] = ievent;
			event_store_count++;
		}
	}

	void postSkipRepeat(ET event_type, int event_id/*= -1*/)
	{
		if (event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		if (event_store_count > 0) {
			if (m_event_store[event_store_count - 1].typeID != event_type) {
				m_id_store_id[event_store_count].i[0] = event_type;
				m_id_store_id[event_store_count].i[1] = event_id;
				m_id_store_id[event_store_count].i[2] = -1;
				event_store_count++;
			}
		}
		else {
			m_id_store_id[event_store_count].i[0] = event_type;
			m_id_store_id[event_store_count].i[1] = event_id;
			m_id_store_id[event_store_count].i[2] = -1;
			event_store_count++;
		}
	}

	void postReplaceRepeat(const CEvent& ievent)
	{
		if (event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		if (event_store_count > 0) {
			if (m_event_store[event_store_count - 1].typeID != ievent.typeID) {
				m_id_store_id[event_store_count].i[0] = ievent.typeID;
				m_id_store_id[event_store_count].i[1] = ievent.eventID;
				m_id_store_id[event_store_count].i[2] = 1;
				m_event_store[event_store_count] = ievent;
				event_store_count++;
			}
			else {
				m_id_store_id[event_store_count - 1].i[0] = ievent.typeID;
				m_id_store_id[event_store_count - 1].i[1] = ievent.eventID;
				m_id_store_id[event_store_count - 1].i[2] = 1;
				m_event_store[event_store_count - 1] = ievent;
			}
		}
		else {
			m_id_store_id[event_store_count].i[0] = ievent.typeID;
			m_id_store_id[event_store_count].i[1] = ievent.eventID;
			m_id_store_id[event_store_count].i[2] = 1;
			m_event_store[event_store_count] = ievent;
			event_store_count++;
		}
	}

	void postReplaceRepeat(ET event_type, int event_id/*= -1*/)
	{
		if (event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		if (event_store_count > 0) {
			if (m_event_store[event_store_count - 1].typeID != event_id) {
				m_id_store_id[event_store_count].i[0] = event_id;
				m_id_store_id[event_store_count].i[1] = event_type;
				m_id_store_id[event_store_count].i[2] = -1;
				event_store_count++;
			}
			else {
				m_id_store_id[event_store_count - 1].i[0] = event_id;
				m_id_store_id[event_store_count - 1].i[1] = event_type;
				m_id_store_id[event_store_count - 1].i[2] = -1;
			}
		}
		else {
			m_id_store_id[event_store_count].i[0] = event_id;
			m_id_store_id[event_store_count].i[1] = event_type;
			m_id_store_id[event_store_count].i[2] = -1;
			event_store_count++;
		}
	}

	void flush()
	{
		if (event_store_count == 0) return;
		for (int i = 0; i < event_store_count; i++) {
			ET event_type = (ET) m_id_store_id[i].i[0];
			int event_id = m_id_store_id[i].i[1];
			int slot_type = m_id_store_id[i].i[2];
			if(slot_type == -1)
				dispatch(event_type, event_id,nullptr);
			else
				dispatch(m_event_store[i]);
		}
		event_store_count = 0;
	}

	void clear()
	{
		event_store_count = 0;
	}

	void needsUpdate()
	{
		m_needs_update++;
	}

	int isUpdateNeeded()
	{
		return m_needs_update;
	}

	void updated()
	{
		m_needs_update = 0;
	}

}