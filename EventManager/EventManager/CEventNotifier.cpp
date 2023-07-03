#include <iostream>
#include "CEventNotifier.h"

using namespace std;
using namespace EventNotifier;

/* Static variable declarations*/
shared_ptr<CEventNotifier> CEventNotifier::pEventNotifier = 0;

//static method to get instance of eventNotifier object
shared_ptr<CEventNotifier> CEventNotifier::getEventNotifier()
{
    if (0 == pEventNotifier)
    {  
        pEventNotifier = shared_ptr<CEventNotifier>(new CEventNotifier());
    }
    return pEventNotifier;
}

//Register listener function
void CEventNotifier::registerListener(ListenersId listenerId, IEventConsumer* fn, vector< eEventsList> eventsList)
{
    if (LISTENER_INVALID != listenerId)
    {
        //create map between listenerID and callbck function
        listenersList.insert({ listenerId , fn });

        if (!eventsList.empty())
        {
            for (auto event : eventsList)
            {
                //create map between interested events and listenerID
                eventListeners.insert({ event, listenerId });
            }
        }
    }
    
}

//Notify event
void CEventNotifier::notifyEvent(eEventsList eventId, IEventdata* eventData)
{
    //get the regsistered listenerId's for the notified event
    if (eventListeners.find(eventId) != eventListeners.end())
    {
        if (nullptr != eventData)
        {
            auto range = eventListeners.equal_range(eventId);
            //iterate through all the listenerID's
            for (auto it = range.first; it != range.second; ++it)
            {
                if (listenersList.find(it->second) != listenersList.end())
                {
                    auto ret = listenersList.find(it->second);

                    //Get the callback function of the listenerID
                    IEventConsumer* consumer = listenersList[ret->first];

                    //Invoke the corresponding object callback
                    consumer->processEvents(eventId, eventData);
                }
            }
        }
    }
}

//Unregistered the listenerID
void CEventNotifier::unregisterListener(ListenersId listenerId)
{
    if (LISTENER_INVALID != listenerId)
    {
        if (listenersList.find(listenerId) != listenersList.end())
        {
            listenersList.erase(listenerId);
        }
    }
}