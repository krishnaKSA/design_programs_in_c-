#ifndef PUBLISHERS_H
#define PUBLISHERS_H

#include <iostream>
#include "CEventNotifier.h"

using namespace std;
using namespace EventNotifier;


/*Defined sample publisher class */

/* Timer class */
class CTimer
{
	/* Get the instance of CEventNotifier */
	shared_ptr<CEventNotifier> notifier;

public:
	/*timer data */
	struct timerdata
	{
		int timerId;
		bool timerExpired;
		timerdata(int id, bool status)
		{
			timerId = id;
			timerExpired = status;
		}
	};

	//constructor
	CTimer()
	{
		notifier = CEventNotifier::getEventNotifier();
	}

	//Created for testing purpose.Sample funtion to show how to send events.
	void executeAllCases()
	{
		//sending events to EventNotifier
		timerdata *data = new timerdata(10, false);
		CEventdata<timerdata>* pRet = new CEventdata<timerdata>(data); 
		notifier->notifyEvent(EVENT_TIMER_FIRED, static_cast<IEventdata*>(pRet));

		data = new timerdata(10, true);
		pRet = new CEventdata<timerdata>(data);
		notifier->notifyEvent(EVENT_TIMER_EXPIRED, static_cast<IEventdata*>(pRet));
	}
};

/* Alarm class */

class CAlarm
{
	/* Get the instance of CEventNotifier */
	shared_ptr<CEventNotifier> notifier;
public:
	//constructor
	CAlarm()
	{
		notifier = CEventNotifier::getEventNotifier();
	}

	//Created for testing purpose.Sample funtion to show how to send events.
	void executeAllCases()
	{
		//sending events to EventNotifier
		bool* data = new bool( false);
		CEventdata<bool>* pRet = new CEventdata<bool>(data);
		notifier->notifyEvent(EVENT_ALARM_OFF, static_cast<IEventdata*>(pRet));

		data = new bool(true);
		pRet = new CEventdata<bool>(data);
		notifier->notifyEvent(EVENT_ALARM_ON, static_cast<IEventdata*>(pRet));
	}
};

#endif