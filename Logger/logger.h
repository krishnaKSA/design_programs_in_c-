#ifndef LOGGER_H
#define LOGGER_H

//Header file declartions
#include <iostream>
#include <memory> //smart pointers
#include <string>
#include <cstring>
#include <fstream> //file operations
#include <time.h> //print time info

//namespace declarations
using namespace std;

namespace systemLogger
{
    enum logLevels
    {
        NONE = 0,
        INFO = 1,
        DEBUG = 2,
        WARNING = 4,
        ERROR = 8,
        ALLLOGS = 16
    };

    enum logOutputPath
    {
        INVALIDPATHTYPE  = 0,
        CONSOLETYPE,
        FILETYPE
    };

    const string DEFAULT_LOG_DUMP_FILE = "logsdump.txt";
    const uint16_t MAX_NUMBER_CHAR = 500;

    //Class declation
    class logger
    {
        private:
        //static instance , we need to have static instance so that only once object get created, and all the clients
        //access the same object.
        //singleton pattern
        static shared_ptr<logger> loggerInstance; //single object accessed by different users as different object

        //log path
        logOutputPath logPathType;
        ofstream outputFile; //file to dump the logs
        uint32_t logFilter;

        public:

        ~logger()
        {
            if(outputFile.is_open())
            {
                outputFile.close(); //closing file if its opened
            }
        }
        
        logger()
        {
            logPathType = INVALIDPATHTYPE;
            logFilter = 0;
        }

        //static function to get the static instance of the logger obj
        static shared_ptr<logger> getInstance();

        //set prefernce , if the preference doesn't set then logger will not dump the logs
        void setPreferences(logOutputPath outputPath, string filePath, uint32_t logFilters);

        //log function
        template<typename... Args>
        void Log(logLevels logLevel, string functionname, time_t time, string log, Args... args)
        {
            char buffer[MAX_NUMBER_CHAR];
            auto fomattedText = [=, &buffer]()
            {
                string logname = string("[ ")+strtok(ctime(&time), "\n")+ string(" ] [")+getLogLevelName(logLevel)+"] "+functionname+ ": "+log+" ";
                sprintf(buffer, logname.c_str(), args...);
                return buffer;
            };
            
            //print the log on desired location
            if((NONE != logLevel) && (isLogLevelEnabled(logLevel)))
            {
                if(FILETYPE == logPathType) //dump the logs into the file
                {
                    if(outputFile.good())
                    {
                        outputFile<<fomattedText()<<endl;
                    }
                }
                else if(CONSOLETYPE == logPathType) //dump the logs to console
                {
                    cout<<fomattedText()<<endl;
                }
                else
                {
                    cout<<"error in logpathtype. no logs printed !!!!!!!"<<endl;
                }
            }
            else
            {
                //cout<<"Log level not enabled. no logs printed !!!!!!!"<<endl;
            }
        }

        //utility functions
        inline logOutputPath getlogPathType()
        {
            return logPathType;
        }
        bool isLogLevelEnabled(uint8_t level);
        string getLogLevelName(logLevels level);
    };

}


#endif