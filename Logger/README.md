
Logger program in C++

Designing the logger system which logs the various system states during run time. 
We can configure the logging level (warning, debug, error,etc ,.. ), also we can configure the output platform (file or console).
Default output path is console. If the user doesn't provide the file path when it selects the output path as file, the default
path will be taken. Default path is predefined. 

Requirements:

1. Design Logger system in c++
2. This logger should provide the configurable output platform (file or console).
3. This logger should provide method to filter the logs during run time.

Programming decisions:

1. Preferred signleton pattern since we need to create only one object , and all the client access the same.
2. Instance defined as the shared ptr. So if all the client objects dereferenced then reference count becomes zero and destructor will get invoked for the shared ptr.
3. Provided default file incase client missed to provide the file for logs dumping.


Note: Simple logger without mutithread
