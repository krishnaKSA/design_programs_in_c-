Design an event notification system in C++
Introduction:

The intention of the design to provide  the simple generic EventNotifier class which is responsible for handling the event notification functionality for the complete system.  EventNotifier object notifies the events between different objects. 
This design provides the communication mechanism between the objects when one object is depending on the state information from another object (or) object is expecting the data from other object (like timer expire notification).
This design provides the lose coupling between the listener object and notifier object. Both objects doesn't need to know each other and doesn't hold the instance of it. EventNotifier object is the mediator of these two objects. 
Listener/Observer objects will register for the events which it is interested into. EventNotifier object holds the data of event list and corresponding registered listeners. Notifier object sends the notification to the EventNotifier object, and EventNotifier notify the events to corresponding listener objects.

Note: Listener class means class which needs data from other class , and registered for those events. Publisher class means the class which sends the events to eventNotifier class. A class can be both publisher and listener.

Functionality:
This design has the below classes.

 1. EventNotifier class 
 2. Interface class for observer/listener.
 3. Eventdata class
 4. Observer/Listener class
 5. Notifier/Publisher class

Implementation:
Event list enumeration. All the signals/events added here.

enum eEventsList{

    EVENT_INVALID = 0,
    EVENT_TIMER_EXPIRED,
    EVENT_TIMER_FIRED
};
Listener id enumeration. Each listener class assigned with unique ID value which is used in the EventNotifier class to map the function callback.

/* Listener ID*/
enum ListenersId
{
	LISTENER_INVALID = 0,
	LISTENER_TIMER //Timer class
};


1.EventNotifier class:
EventNotifier class contains two maps. 
This class contains the list of listenerID and corresponding function callbacks.           
This class contains mapping data between list of events and corresponding listeners.                
Listener/Observer class has unique ID as defined in ListenersID enum. During the observer class object instantiation , it will provide the list of interested events and listenerID . IEventConsumer class parameterized constructor takes these info's as argument , and invoke registerListener funtion of EventNotifier class (1.ListenerId 2. Callback function pointer 3.list of events which it is interested into).

In registerListener function, EventNotifier object first creates the map between listenerId and callback function pointer, and second it creates map between events and ListenerID. When the event comes, first it will get the list of listenerID's which are registered for those events.Second, it gets the callback function for the listenerID and invokes the callback function.

This class provides notifyEvent function. Publisher class will invoke this function when it wants to send an events.
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


2. Interface class:
Any class can be a listener class, and each listener object is different data type. To provide the generic callback function, defined IEventConsumer interface class. 

The purpose of this interface to provide the generic callback function to process the events. All the listener classes should inherit IEventConsumer and provide override function of processEvents (callback function) . This class invokes the registerListener function in the constructor.

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


3. EventData class:
Each publisher object sends different type of data while sending the event. To provide the generic function which accepts different data type, defined the interface class for event data transmission.

/* Interface class for event data*/
class IEventdata
{
public:
	virtual void* getdata() = 0;
	virtual void release() = 0;
};

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


4. Listener/Observer Class:
Observer class inherit from IEventConsumer class. This class should provide the override function for void processEvents(eEventsList id, IEventdata* eventData) override. In the constructor function, it provides the list of events to listen and, listenerID. These listener classes has unique listenerID which is defined in the enum listenerID.

//Sample listener class:
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


5. Publisher/Notifier class:
Publisher class has the instance of  EventNotifier object. 
If this object wants to notify any data to other objects, it will invoke the notifyEvent(0 function from  EventNotifier object along with the eventID and event data. EventNotifier will send the event to corresponding listener by invoking processEvents() function.

//Sample notifier class:

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


Unit testing:
/*Unit Testing */
int main()
{
    //Create listener class objects
    CWatchDog* watchDog = new CWatchDog();
    CSensor* sensor = new CSensor();

    //Create publisher class objects
    CAlarm* alramPublisher = new CAlarm();
    alramPublisher->executeAllCases();

    CTimer* timerPublisher = new CTimer();
    timerPublisher->executeAllCases();

    return 0;
}