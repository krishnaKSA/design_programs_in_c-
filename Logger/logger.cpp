
//Header file declarations
#include "logger.h"

//namespace declarations
using namespace systemLogger;

// static variables declarations
shared_ptr<logger> logger::loggerInstance = 0;

/************ Function definitions of logger class ***************/


//static function to get the object
shared_ptr<logger> logger::getInstance()
{
    //if the object hasn't created yet
    if(0 == loggerInstance)
    {
        //create the shared pointer object
        loggerInstance = shared_ptr<logger> (new logger);
    }
    return loggerInstance;
}

//set prefernce , if the preference doesn't set then logger will not dump the logs
void logger::setPreferences(logOutputPath outputPath, string filePath, uint32_t logFilters)        
{
    //set the log filter
    logFilter = logFilters;

    //Output path is file, then set the file path.
    if(FILETYPE == outputPath)
    {
        logPathType = outputPath;
        if(!filePath.empty())
        {
            outputFile.open(filePath);//create the file if the file is not present
            if(!outputFile.good())
            {
                cout<<"logger::setPreferences:: File open is not successfull -  "<<filePath.c_str()<<endl;
            }
        }
        else
        {
            //if path doesn't spectified , create the default path
            outputFile.open(DEFAULT_LOG_DUMP_FILE); 
            if(!outputFile.good())
            {
                cout<<"logger::setPreferences:: File open is not successfull on default path (logsdump.txt)"<<endl;
            }
        }
        
    }
    else
    {
        logPathType = outputPath; // user request or by default
    }
}

//check whether log level enabled 
bool logger::isLogLevelEnabled(uint8_t level)
{
    bool ret = false;
    if(0 == logFilter) //no log level enabled
    {
        ret = false;
    }
    else if(ALLLOGS == logFilter) // all levels enabled
    {
        ret = true;
    }
    else
    {
        uint32_t value = logFilter&level;
        //Info enabled
        if((value == (uint8_t)INFO) || (value == (uint8_t)DEBUG) || (value == (uint8_t)WARNING) || (value == (uint8_t)ERROR) )
        {
            ret = true;
        }
    }

    return ret;
}

//get the string name of the log level
string logger::getLogLevelName(logLevels level)
{
    string logLevelName = "";
    switch(level)
    {
        case NONE:  { 
            logLevelName = "NONE";
            break;
        }
        case INFO:  {
            logLevelName = "INFO";
            break;
        }
        case WARNING: {
            logLevelName = "WARNING";
            break;
        }
        case DEBUG:  {
            logLevelName = "DEBUG";
            break;
        }
        case ERROR:  {
            logLevelName = "ERROR";
            break;
        }
        default:{
            //do nothing 
            break;
        }
    }

    return logLevelName;
}
