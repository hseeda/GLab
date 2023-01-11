#include "pch.h"
#include "EventSystem.h"
#include "WindowSystem.h"

namespace ES
{
	event dummy_event;
	std::unordered_map <int /*event_type*/, std::vector<std::pair<int /*event_id*/, Slot>>> m_slots;
	ES::event				m_event_store[EVENT_SYSTEM_MAX_EVENTS];
	int						m_event_store_count = 0;
	//<int/*event_type*/, int/*event_id*/, int =-1 if no event is specified, 1 otherwise>	
	i3	m_id_store_id[EVENT_SYSTEM_MAX_EVENTS];

	std::pair<int  /*event_id*/, Slot>* subscribe(ET event_type, Slot&& slot, int event_id /* = -1*/)
	{
		std::vector<std::pair<int, Slot>>* pslots;

		try {
			pslots = &m_slots.at(event_type);
		}
		catch (std::out_of_range)
		{
			std::vector<std::pair<int, Slot>> vslots;
			m_slots.insert({ event_type, std::move (vslots)});
			pslots = &m_slots.at(event_type);
		}
		pslots->push_back(std::make_pair(event_id, slot));
		return &pslots->back();
	}

	void dispatch(ET event_type, int event_id , const event* ievent)
	{
		std::vector<std::pair<int, Slot>>* pslots;
		try {
			pslots = &m_slots.at(event_type);
		}
		catch (std::out_of_range)
		{
			return;
		}
		size_t n = pslots->size();
		for (auto i = 0; i < n; i++) {
			std::pair<int, Slot>& p = pslots->at(i);
			int slot_id = p.first;
			auto func = p.second;
			if (event_id < 0 || event_id == slot_id)
			{
				WS::log.AddLog(5, "[event][%i] %s\n", (int)event_type, eventTypeName((ET) event_type).c_str());
				if(ievent) func(*ievent);
				else func(dummy_event);
			}
		}
	}

	void dispatch(const ES::event& ievent)
	{
		ET event_type = ievent.typeID;
		int event_id = ievent.eventID;
		dispatch(event_type, event_id, &ievent);
	}

	void dispatch(ET event_type, int event_id /*= -1*/)
	{
		dispatch(event_type, event_id, nullptr);
	}

	void dispatch(int event_id)
	{
		dispatch(ET::none, event_id,nullptr);
	}

	void post(ET event_type, int event_id, const event* ievent /*= nullptr*/) {
		if (m_event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		if (ievent == nullptr) {
			m_id_store_id[m_event_store_count].i[0] = event_type;
			m_id_store_id[m_event_store_count].i[1] = event_id;
			m_id_store_id[m_event_store_count].i[2] = -1;
		}
		else {
			m_id_store_id[m_event_store_count].i[0] = event_type;
			m_id_store_id[m_event_store_count].i[1] = event_id;
			m_id_store_id[m_event_store_count].i[2] = 1;
			m_event_store[m_event_store_count] = *ievent;
		}
		m_event_store_count++;
	}
	
	void post(const ES::event& ievent)
	{
		if (m_event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		
		int event_type = ievent.typeID;
		int event_id   = ievent.eventID;
		m_id_store_id[m_event_store_count].i[0] = event_type;
		m_id_store_id[m_event_store_count].i[1] = event_id;
		m_id_store_id[m_event_store_count].i[2] = 1;
		m_event_store[m_event_store_count] = ievent;
		m_event_store_count++;
	}

	void post(ET event_type, int event_id /*= -1*/)
	{
		post(event_type, event_id,nullptr);
	}

	void post(ET event_type, packet& data)
	{
		post(event(event_type, data));
	}

	void post(int event_id)
	{
		post(ET::none, event_id, nullptr);
	}

	void postSkipRepeat(const event& ievent)
	{
		if (m_event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		if (m_event_store_count > 0) {
			if (m_event_store[m_event_store_count - 1].typeID != ievent.typeID) {
				m_id_store_id[m_event_store_count].i[0] = ievent.typeID;
				m_id_store_id[m_event_store_count].i[1] = ievent.eventID;
				m_id_store_id[m_event_store_count].i[2] = 1;
				m_event_store[m_event_store_count] = ievent;
				m_event_store_count++;
			}
		}
		else {
			m_id_store_id[m_event_store_count].i[0] = ievent.typeID;
			m_id_store_id[m_event_store_count].i[1] = ievent.eventID;
			m_id_store_id[m_event_store_count].i[2] = 1;
			m_event_store[m_event_store_count] = ievent;
			m_event_store_count++;
		}
	}

	void postReplaceRepeat(const event& ievent)
	{
		if (m_event_store_count == EVENT_SYSTEM_MAX_EVENTS) { _pl("EventSystem::post: event queue full");	return; }
		if (m_event_store_count > 0) {
			if (m_event_store[m_event_store_count - 1].typeID != ievent.typeID) {
				m_id_store_id[m_event_store_count].i[0] = ievent.typeID;
				m_id_store_id[m_event_store_count].i[1] = ievent.eventID;
				m_id_store_id[m_event_store_count].i[2] = 1;
				m_event_store[m_event_store_count] = ievent;
				m_event_store_count++;
			}
			else {
				m_id_store_id[m_event_store_count- 1].i[0] = ievent.typeID;
				m_id_store_id[m_event_store_count- 1].i[1] = ievent.eventID;
				m_id_store_id[m_event_store_count- 1].i[2] = 1;
				m_event_store[m_event_store_count- 1] = ievent;
			}
		}
		else {
			m_id_store_id[m_event_store_count].i[0] = ievent.typeID;
			m_id_store_id[m_event_store_count].i[1] = ievent.eventID;
			m_id_store_id[m_event_store_count].i[2] = 1;
			m_event_store[m_event_store_count] = ievent;
			m_event_store_count++;
		}
	}

	void flush()
	{
		if (m_event_store_count == 0) return;
		for (int i = 0; i < m_event_store_count; i++) {
			ET event_type = (ET) m_id_store_id[i].i[0];
			int event_id = m_id_store_id[i].i[1];
			int slot_type = m_id_store_id[i].i[2];
			if(slot_type == -1)
				dispatch(event_type, event_id,nullptr);
			else
				dispatch(m_event_store[i]);
		}
		m_event_store_count = 0;
	}

	void clear()
	{
		m_event_store_count = 0;
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