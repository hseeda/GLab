#include "pch.h"
#include "EventSystem.h"

namespace ES
{
	//inline EventSystem es;
	std::map <et, std::vector<std::pair <int,SlotWithEvent>>>	m_observers_with_event;
	ES::event									m_event_store[EVENT_SYSTEM_MAX_EVENTS];
	int											m_event_count = 0;
	/*int											m_needs_update = 0;*/
	
	void subscribe(et event_type, SlotWithEvent&& slot, int eventID /*= -1*/)
	{
		m_observers_with_event[event_type].push_back(std::make_pair(eventID, slot));
	}

	void dispatch(const ES::event& event)
	{
		if (m_observers_with_event.find(event.tID) != m_observers_with_event.end())	// if found
		{
			//_plcb("event function dispatched");
			auto slot_pair = m_observers_with_event.at(event.tID);
			size_t n = slot_pair.size();
			for (auto i = 0; i < n; i++) {
				int id = slot_pair[i].first;
				auto func = slot_pair[i].second;
				if (id < 0 || event.data.ID == id)
				{
					func(event);
				}
				//_pl("event skipped");
			}
		}
	}

	void dispatch(et event_type)
	{
		dispatch(event(event_type));
	}

	void post(const ES::event& event)
	{
		if (m_event_count == EVENT_SYSTEM_MAX_EVENTS)
		{
			_pl("EventSystem::post: event queue full");
			return;
		}
			m_event_store[m_event_count] = event;
			m_event_count++;
			return;
	}

	void post(et event_type)
	{
		post(event(event_type));
	}

	void post(et event_type, packet& data)
	{
		post(event(event_type, data));
	}

	void postSkipRepeat(const event& event)
	{
		if (m_event_count == EVENT_SYSTEM_MAX_EVENTS)
		{
			_pl("EventSystem::post: event queue full");
			return;
		}
		if (m_event_count > 0) {
			if (m_event_store[m_event_count - 1].tID != event.tID) {
				m_event_store[m_event_count] = event;
				m_event_count++;
			}
		}
		else {
			m_event_store[m_event_count] = event;
			m_event_count++;
		}
	}

	void postReplaceRepeat(const event& event)
	{
		if (m_event_count == EVENT_SYSTEM_MAX_EVENTS)
		{
			_pl("EventSystem::post: event queue full");
			return;
		}

		if (m_event_count > 0) {
			if (m_event_store[m_event_count - 1].tID != event.tID) {
				m_event_store[m_event_count] = event;
				m_event_count++;
			}
			else {
				m_event_store[m_event_count - 1] = event;
			}
		}
		else {
			m_event_store[m_event_count] = event;
			m_event_count++;
		}
	}

	void flush()
	{
		if (m_event_count == 0) return;
		for (int i = 0; i < m_event_count; i++)
			dispatch(m_event_store[i]);
		m_event_count = 0;
	}

	void clear()
	{
		m_event_count = 0;
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