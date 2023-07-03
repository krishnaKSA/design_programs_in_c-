#ifndef LISTENERS_H
#define LISTENERS_H

#include <iostream>
#include "CEventNotifier.h"
#include "publishers.h"

using namespace std;
using namespace EventNotifier;

/* Define sample listener class */

/* Watchdog class */
class CWatchDog :public IEventConsumer
{
public:
	//Constructor. 
	//Provide the list of interested events while calling the IEventConsumer constructor.
	CWatchDog() : IEventConsumer(LISTENER_WATCHDOG, { EVENT_TIMER_EXPIRED,  EVENT_TIMER_FIRED })
	{
		
	}
	
	/* defined the callback function to process incoming events */
	void processEvents(eEventsList id, IEventdata* eventData) override
	{
		cout << "callback triggered for " << id << endl;
		CTimer::timerdata* data = static_cast<CTimer::timerdata*> (eventData->getdata());
		cout << " timer data expired = " << data->timerExpired << " handle = " << data->timerId << endl;
	}
};

/* Sensor class */
class CSensor :public IEventConsumer
{
public:
	//Constructor. 
	//Provide the list of interested events while calling the IEventConsumer constructor.
	CSensor() : IEventConsumer(LISTENER_SENSOR, { EVENT_ALARM_OFF, EVENT_ALARM_ON })
	{

	}
	
	void processEvents(eEventsList id, IEventdata* eventData) override
	{
		cout << "callback triggered for " << id << endl;
		bool* data = static_cast<bool*> (eventData->getdata());
		cout << "Alarm ON " << *data << endl;
	}
};

#endif