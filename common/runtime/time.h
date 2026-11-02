// BismIllahIRRahmaanIRRaheem
/* time system functionality */

#ifndef RUNTIME_TIME_H
#define RUNTIME_TIME_H
#include <runtime/engineCore_C.h>
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
largefloat sc_get_time();
largefloat sc_get_time_not_since_engine_startup();
largefloat sc_get_delta_and_update_time(largefloat* apLastTime, largefloat* apUpdateTime);
void sc_sleep_seconds(largefloat aSeconds);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif // !RUNTIME_TIME_H