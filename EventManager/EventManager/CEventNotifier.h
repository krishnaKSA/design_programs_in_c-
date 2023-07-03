#ifndef CEVENTNOTIFIER_H
#define CEVENTNOTIFIER_H

#include <iostream>
#include <unordered_map>
#include <map>
#include <functional>
#include <memory>

using namespace std;
/*
* Event Notification System in C++
* 
* Design a event notification system in C++.

* 
* The intention of this design to provide central EventNotifier class which is responsible for handling the event notification
functionality for the complete system.

This EventNotifier object notifys the events between different objects.
Objects will register the events which it is interested in , EventNotifier will notify the events to the objects when it occurs.

, and 

This design has three systems.

1. EventNotifier Class:
	Central system to manage all the events and listener lists
	EventNotifer class maintains the list of listerners and corresponding function callbacks
	list of events and registered listeners.
2. Observer class - The class which 
If any object is depending on the another object's state change (or) signals/events , then 
*/
namespace EventNotifier
{
	//forward declaration
	class IEventConsumer;

	/* List of events */
	enum eEventsList
	{
		EVENT_INVALID = 0,
		//Timer class events
		EVENT_TIMER_EXPIRED,
		EVENT_TIMER_FIRED,
		//Alarm class events
		EVENT_ALARM_OFF,
		EVENT_ALARM_ON
	};

	/* Listener ID*/
	enum ListenersId
	{
		LISTENER_INVALID = 0,
		LISTENER_WATCHDOG, //WatchDog class
		LISTENER_SENSOR //Sensor class
	};

	/* Interface class for event data*/
	class IEventdata
	{
	public:
		virtual void* getdata() = 0;
		virtual void release() = 0;
	};

	/* Defined class to hold the event data. While sending the event from one onject to another object, 
	there may be some data to send along with the event. Since we desgined the generic event manager,
	defined the template class to hold the event data. So that the objects can access it, and transfer the data 
	with different data types (int, struct, ectc.,) */
	template<typename T>
	class CEventdata: public IEventdata
	{
		T* data;
	public:
		explicit CEventdata(T* eventInfo)
		{
			data = eventInfo;
		}
		void* getdata() override
		{
			return static_cast<void*>(data);
		}
		void setdata(T* newdata)
		{
			data = newdata;
		}
		void release() override
		{
			if (data)
				delete data;
		}
	};

	/* Event Notifier */
	class CEventNotifier
	{
	private:
		//static instance
		static shared_ptr<CEventNotifier> pEventNotifier;

		//List of listeners, this listerner map contains the listener id and corresponding function callback
		std::map<ListenersId, IEventConsumer*> listenersList;

		//Create list which has map tp events and corresponding listerners
		std::unordered_multimap<eEventsList, ListenersId> eventListeners;

	public:

		static shared_ptr<CEventNotifier> getEventNotifier();
		void registerListener(ListenersId listenerId, IEventConsumer* fn, vector< eEventsList> eventsList);		
		void notifyEvent(eEventsList eventId, IEventdata* eventData);
		void unregisterListener(ListenersId listenerId);
	};


	/*Event consumer interface */
	class IEventConsumer
	{
	public:
		IEventConsumer(ListenersId listernerId, vector< eEventsList> eventsList)
		{
			shared_ptr<CEventNotifier> pNotifier = CEventNotifier::getEventNotifier();
			//Call registerListener function to store the interested events and callback function
			pNotifier->registerListener(listernerId, (this), eventsList);
		}
		
		virtual void processEvents(eEventsList id, IEventdata* eventData) = 0;
	};
}
#endif