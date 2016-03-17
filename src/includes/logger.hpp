#ifndef LOGGER_HPP
#define LOGGER_HPP
/**
  * @file logger.hpp
  * @brief Logger class to write messages
  * @date 19-10-14
  * @author Calcul Québec
  */
#include <string>
#include <iostream>
#include <fstream>

  /** @class Logger
  * @brief Logger class to write messages
  *
  * Downloaded from  https://wiki.calculquebec.ca/w/C%2B%2B_:_classe_Logger/en as
  * of 19-10-14 and modified / enhanced (choose one...) by Jean-Gabriel Young.
  *
  */
class Logger {
public:
    // log priorities
    enum Priority {
      DEBUG, 
      INFO,
      WARNING, 
      CONFIG, 
      ERROR};
 
    // start/stop logging
    // - messages with priority >= minPriority will be written in log
    // - set logFile = "" to write to standard output
    static void Start(Priority minPriority, const std::string& logFile);
    static void Stop();
 
    // write message
    static void Write(Priority priority, const std::string& message);

private:
    // Logger adheres to the singleton design pattern, hence the private
    // constructor, copy constructor and assignment operator.
    Logger();
    Logger(const Logger& logger) = delete;
    Logger& operator = (const Logger& logger) = delete;
 
    // private instance data
    bool            active;
    std::ofstream   fileStream;
    Priority        minPriority;
 
    // names describing the items in enum Priority
    static const std::string PRIORITY_NAMES[];
    // the sole Logger instance (singleton)
    static Logger instance;
};

// helpers:
template <typename T> std::string var_print(std::string name, T value) {
    return "[["+name+"=="+std::to_string(value)+"]]";
}
std::string var_print(std::string name, std::string value);
std::string var_print(std::string name, const char * value);

template <typename T> std::string var_eval(T value) {return std::to_string(value);}
std::string var_eval(std::string value);
std::string var_eval(const char * value);

// These macros are always defined to avoid compilation errors
#define VAR_PRINT(NAME) var_print(#NAME,(NAME))
#define VAR_EVAL(NAME)  var_eval((NAME))

// Level dependent macro
// LOGGER_LEVEL == 0 : DEBUG+
// LOGGER_LEVEL == 1 : INFO+
// LOGGER_LEVEL == 2 : COFIG+
// LOGGER_LEVEL == 3 : NO LOGS

#if LOGGER_LEVEL < 3
  #define LOGGER_STOP() Logger::Stop();
  #define LOGGER_WRITE(PRIORITY, MESSAGE) Logger::Write(PRIORITY, MESSAGE);
  #define LOGGER_LINE(PRIORITY) Logger::Line(PRIORITY);
#else
  // high performance: no logging. 
  #define LOGGER_START(FILE) 
  #define LOGGER_STOP()
  #define LOGGER_WRITE(PRIORITY, MESSAGE)
  #define LOGGER_LINE(MIN_PRIORITY)
#endif

#if LOGGER_LEVEL == 0
  #define LOGGER_START(FILE) Logger::Start(Logger::DEBUG, FILE);
#elif LOGGER_LEVEL == 1
  #define LOGGER_START(FILE) Logger::Start(Logger::INFO, FILE);
#elif LOGGER_LEVEL == 2
  #define LOGGER_START(FILE) Logger::Start(Logger::CONFIG, FILE);
#endif

#endif // LOGGER_HPP