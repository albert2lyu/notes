/*
 * =========================================================================
 *
 *       Filename:  STLogger.h
 *
 *    Description:  Logger Management and Configuration
 *
 *        Version:  1.0
 *        Created:  03/05/2014 09:59:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Bian Jiang (Kvbian)
 *
 * =========================================================================
 */

#ifndef __STLOGGER_H__INCLUDED_
#define __STLOGGER_H__INCLUDED_

#include <string>
#include <iostream>
#include <fstream>
#include <cstdarg>

#include <stdio.h>
#include <stdlib.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>

#include <log4cplus/helpers/loglog.h>

class STLogger
{
public:
	STLogger(const std::string tag);
	~STLogger();

public:
    log4cplus::Logger m_logger;
    // These log levels correspond to the log levels used by log4j.
    enum Level {
        kTrace, //< Primitive calls and other low-level trace messages
        kDebug, //< Script-level debugging information
        kInfo,  //< Information about the system (use sparingly)
        kWarn,  //< Warnings for developers
        kError, //< Serious errors (fatal when not in developer mode)
        kFatal  //< Fatal errors
    };

public:
    static void nLog(STLogger logger, Level inLevel, const char *inFormat, const char *file, int line, ...);


////////////////////////////////////////////////////////////////////////////////
#define VLOG_WITH_LEVEL_NEW(logger, LEVEL, inFormat, arg...) \
    STLogger::nLog(logger, (LEVEL), inFormat, __FILE__, __LINE__, arg); 

#define STLOG_TRACE(logger, inFormat, ...) \
    VLOG_WITH_LEVEL_NEW(logger, STLogger::kTrace, inFormat, __VA_ARGS__);

#define STLOG_DEBUG(logger, inFormat, ...) \
    VLOG_WITH_LEVEL_NEW(logger, STLogger::kDebug, inFormat, __VA_ARGS__);

#define STLOG_INFO(logger, inFormat, ...) \
    VLOG_WITH_LEVEL_NEW(logger, STLogger::kInfo, inFormat, __VA_ARGS__);

#define STLOG_WARN(logger, inFormat, ...) \
    VLOG_WITH_LEVEL_NEW(logger, STLogger::kWarn, inFormat, __VA_ARGS__);

#define STLOG_ERROR(logger, inFormat, ...) \
    VLOG_WITH_LEVEL_NEW(logger, STLogger::kError, inFormat, __VA_ARGS__);

#define STLOG_FATAL(logger, inFormat, ...) \
    VLOG_WITH_LEVEL_NEW(logger, STLogger::kFatal, inFormat, __VA_ARGS__);

////////////////////////////////////////////////////////////////////////////////

};


#endif // __STLOGGER_H__INCLUDED_
