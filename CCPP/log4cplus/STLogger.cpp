/*
 * =========================================================================
 *
 *       Filename:  STLogger.cpp
 *
 *    Description:  Logger Management and Configuration
 *
 *        Version:  1.0
 *        Created:  03/05/2014 09:58:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bian Jiang (Kvbian)
 *
 * =========================================================================
 */

#include "STLogger.h"

#include <boost/scoped_array.hpp>

#include <iostream>
#include <fstream>

static bool isInited = false;

static void initLogger() 
{
    log4cplus::initialize();
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("./log4cplus.properties"));
    isInited = true; 
    std::cout << __func__ << " STLogger " << std::endl;
}

STLogger::STLogger(const std::string tag)
{
    if (!isInited) {
        initLogger();
    }
    m_logger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(tag));
}

STLogger::~STLogger()
{
}

/// Internal: Translate to log4cplus log level.
static log4cplus::LogLevel Log4CPlusLevelFromLevel(STLogger::Level inLevel) {
    switch (inLevel) {
        case STLogger::kTrace: return log4cplus::TRACE_LOG_LEVEL;
        case STLogger::kDebug: return log4cplus::DEBUG_LOG_LEVEL;
        case STLogger::kInfo:  return log4cplus::INFO_LOG_LEVEL;
        case STLogger::kWarn:  return log4cplus::WARN_LOG_LEVEL;
        case STLogger::kError: return log4cplus::ERROR_LOG_LEVEL;
        case STLogger::kFatal: return log4cplus::FATAL_LOG_LEVEL;
        default:
            return log4cplus::INFO_LOG_LEVEL;
    }
}

// Allow big log messages, just in case there are stack traces involved.
enum { LOG_BUFFER_SIZE = 10240 };

void STLogger::nLog(STLogger logger, STLogger::Level inLevel, const char *inFormat, const char *file, int line, ...)
{
    va_list args;
    va_start(args, inFormat);
    boost::scoped_array<char> buffer(new char[LOG_BUFFER_SIZE]);
    vsnprintf(buffer.get(), LOG_BUFFER_SIZE, inFormat, args);
    buffer[LOG_BUFFER_SIZE-1] = '\0';
    std::string message(buffer.get());
    buffer.reset();
    va_end(args);

    logger.m_logger.log(Log4CPlusLevelFromLevel(inLevel), message, file, line);
}

