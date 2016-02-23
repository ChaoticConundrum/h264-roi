/*******************************************************************************
**                                  LibChaos                                  **
**                                   zlog.h                                   **
**                          (c) 2015 Charlie Waters                           **
*******************************************************************************/
#ifndef ZLOG_H
#define ZLOG_H

#include "zlogworker.h"
#include "zexception.h"
#include <atomic>

#define ZLOG_DEBUG_DEPTH 100

#define ZLOG_PREFILE LibChaos::ZPath(__FILE__).last()
#define ZLOG_PRELINE LibChaos::ZString(__LINE__)
#define ZLOG_PREFUNC LibChaos::ZString(__FUNCTION__)

#if LIBCHAOS_BUILD == LIBCHAOS_RELEASE
    #define LOG(A)  LibChaos::ZLog(LibChaos::ZLog::NORMAL) << A
    #define DLOG(A) LibChaos::ZLog(LibChaos::ZLog::DEBUG) << A
    #define ELOG(A) LibChaos::ZLog(LibChaos::ZLog::ERRORS) << A
    #define RLOG(A) LibChaos::ZLog(LibChaos::ZLog::NORMAL) << LibChaos::ZLog::raw << A
    #define TLOG(A) LibChaos::ZLog(LibChaos::ZLog::NORMAL) << LibChaos::ZLog::this_thread << A
    #define OLOG(A) LibChaos::ZLog(LibChaos::ZLog::NORMAL) << LibChaos::ZLog::stdio << A
#else
    #define LOG(A)  LibChaos::ZLog(LibChaos::ZLog::NORMAL, ZLOG_PREFILE, ZLOG_PRELINE, ZLOG_PREFUNC) << A
    #define DLOG(A) LibChaos::ZLog(LibChaos::ZLog::DEBUG, ZLOG_PREFILE, ZLOG_PRELINE, ZLOG_PREFUNC) << A
    #define ELOG(A) LibChaos::ZLog(LibChaos::ZLog::ERRORS, ZLOG_PREFILE, ZLOG_PRELINE, ZLOG_PREFUNC) << A
    #define RLOG(A) LibChaos::ZLog(LibChaos::ZLog::NORMAL, ZLOG_PREFILE, ZLOG_PRELINE, ZLOG_PREFUNC) << LibChaos::ZLog::raw << A
    #define TLOG(A) LibChaos::ZLog(LibChaos::ZLog::NORMAL, ZLOG_PREFILE, ZLOG_PRELINE, ZLOG_PREFUNC) << LibChaos::ZLog::this_thread << A
    #define OLOG(A) LibChaos::ZLog(LibChaos::ZLog::NORMAL, ZLOG_PREFILE, ZLOG_PRELINE, ZLOG_PREFUNC) << LibChaos::ZLog::stdio << A
#endif
#define ORLOG(A) OLOG(LibChaos::ZLog::raw << A)

#define IF_LOG(A, B, C, D) if(A){ LOG( B << C ); } else { LOG( B << D ); }
#define IF_DLOG(A, B, C, D) if(A){ DLOG( B << C ); } else { DLOG( B << D ); }

namespace LibChaos {

struct ZLogInfo {
    enum info_type {
        FILE        = 1,
        LINE        = 2,
        FUNCTION    = 3,
        CLOCK       = 4,
        DATE        = 5,
        TIME        = 6,
        THREAD      = 7,
    };

    ZLogInfo(info_type logtype, ZString loginfo) : type(logtype), info(loginfo){}

    info_type type;
    ZString info;
};

//! Logging provider.
class ZLog {
public:
    typedef ZLogWorker::LogJob LogJob;

    enum zlog_source {
        NORMAL  = 1,
        DEBUG   = 2,
        ERRORS  = 3,
        ALL     = 4,
    };

    enum zlog_flag {
        // Actions
        flush = 1,          // Flush log immediately
        newln = 2,          // Append newline to buffer
        flushln = 3,        // Append newline and flush log

        // Format Modifers
        noln = 4,           // Disable automatic newlines for this object
        raw = 5,            // This object will log without formatting

        // Target Modifiers
        stdio = 9,          // Current instance outputs only to stdout

        // Sequence Modifiers
        this_thread = 12    // Log immediately from this thread, block until done
    };

    ZLog(zlog_source source = ZLog::NORMAL);
    ZLog(zlog_source source, ZString prefile, ZString preline, ZString prefunc);

    ~ZLog();

    ZLog &operator<<(zlog_flag);
    ZLog &log(ZString logtext);

    ZLog &operator<<(ZLogInfo info);

    inline ZLog &operator<<(ZString text){ return log(text); } // Base overload
    inline ZLog &operator<<(const char *text){ return log(text); }
    inline ZLog &operator<<(ZPath text){ return log(text.str()); }
    inline ZLog &operator<<(ZException e){ return log(ZString(e.code()) + ": " + e.what()); }
    ZLog &operator<<(ZBinary text);

    inline ZLog &operator<<(char text){ return log(ZString(text)); }
    inline ZLog &operator<<(unsigned char text){ return log(ZString(text)); }
    inline ZLog &operator<<(zus num){ return log(ZString(num)); }
    inline ZLog &operator<<(zss num){ return log(ZString(num)); }
    inline ZLog &operator<<(zuint num){ return log(ZString(num)); }
    inline ZLog &operator<<(zint num){ return log(ZString(num)); }
    inline ZLog &operator<<(zul num){ return log(ZString(num)); }
    inline ZLog &operator<<(zsl num){ return log(ZString(num)); }
    inline ZLog &operator<<(zull num){ return log(ZString(num)); }
    inline ZLog &operator<<(zsll num){ return log(ZString(num)); }
    inline ZLog &operator<<(double num){ return log(ZString(num)); }
    inline ZLog &operator<<(bool tf){ return log(tf ? "true" : "false"); }

    inline ZLog &operator,(ZString text){ return log(" ").log(text); } // Base overload

    ZString pullBuffer();

    static ZString getThread();
    static ZString genLogFileName(ZString prefix);

    static void init();
    static void deInit();

    static void formatStdout(zlog_source type, ZString fmt);
    static void formatStderr(zlog_source type, ZString fmt);
    static void addLogFile(ZPath, zlog_source type, ZString fmt);

private:
    void flushLog(bool final);

    static std::atomic<bool> _init;
    static ZLogWorker *worker;

    LogJob *job;
    bool stdiolog;
    bool newline;
    bool rawlog;
    bool noqueue;
};

} // namespace LibChaos

#endif // ZLOG_H
