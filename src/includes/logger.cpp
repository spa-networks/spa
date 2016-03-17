#include "logger.hpp"

// --------------------------------------
// static members initialization
// --------------------------------------
 
const std::string Logger::PRIORITY_NAMES[] =
{
    "DEBUG",
    "INFO",
    "WARNING",
    "CONFIG",
    "ERROR"
};
 
// the sole Logger instance (singleton)
Logger Logger::instance; 
 
// --------------------------------------
// function implementations
// --------------------------------------
 
Logger::Logger() : active(false) {}
 
void Logger::Start(Priority minPriority, const std::string& logFile)
{
    instance.active = true;
    instance.minPriority = minPriority;
    if (logFile != "")
    {
        instance.fileStream.open(logFile.c_str());
    }
}
 
void Logger::Stop()
{
    instance.active = false;
    if (instance.fileStream.is_open())
    {
        instance.fileStream.close();
    }
}
 
void Logger::Write(Priority priority, const std::string& message)
{
    if (instance.active && priority >= instance.minPriority)
    {
        // identify current output stream
        std::ostream& stream
            = instance.fileStream.is_open() ? instance.fileStream : std::clog;
 
        stream  << PRIORITY_NAMES[priority]
                << ": "
                << message
                << std::endl;
    }
}

std::string var_print(std::string name, std::string value) {
    return "[["+name+"=="+value+"]]";
}
std::string var_print(std::string name, const char * value) {
     return "[["+name+"=="+value+"]]";
}
std::string var_eval(std::string value) {return value;}
std::string var_eval(const char * value) {return value;}
