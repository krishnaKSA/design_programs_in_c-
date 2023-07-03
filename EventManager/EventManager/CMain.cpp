
#include <iostream>
#include "publishers.h"
#include "listeners.h"

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