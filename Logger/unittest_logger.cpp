#include <iostream>
#include "logger.h"
#include <chrono>
#include <iostream>

using namespace systemLogger;
using std::chrono::system_clock;

int main()
{
    //Unit Testing
    shared_ptr<logger> obj = logger::getInstance();
    obj->setPreferences(CONSOLETYPE, "",ALLLOGS);
    obj->Log(DEBUG, __FUNCTION__, time(NULL),"Debug print ");
    obj->Log(ERROR, __FUNCTION__, time(NULL),"ERROR print %d",1);

    return 0;
}