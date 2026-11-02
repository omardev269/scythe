// BismIllahIRRahmaanIRRaheem
/* win32 time functionality implementation */

#include <runtime/time.h>
#ifdef SC_WINDOWS
#include <timeint.h>
#include <float.h>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef SC_X64
#define SC_TIME_MAX_SECONDS DBL_MAX
#else
#define SC_TIME_MAX_SECONDS FLT_MAX
#endif // SC_X64

static uint32 sc_time_start_low;
static uint32 sc_time_start_high;
static largefloat sc_time_seconds_per_tick;

SC_SPECIAL_STATIC largefloat sc_time_ticks_to_float(uint32 aHigh, uint32 aLow) {
	return (largefloat)aHigh * (largefloat)4294967296.0f + (largefloat)aLow;
}

void sc_init_time() {
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_Start;
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_Start);
	sc_time_seconds_per_tick = (largefloat)1.0f /
		sc_time_ticks_to_float((uint32)m_Frequency.HighPart, (uint32)m_Frequency.LowPart);
	sc_time_start_low = (uint32)m_Start.LowPart;
	sc_time_start_high = (uint32)m_Start.HighPart;
}

void sc_deinit_time() {

}

largefloat sc_get_time() {
	LARGE_INTEGER m_Now;
	uint32 m_Low;
	uint32 m_High;
	QueryPerformanceCounter(&m_Now);
	m_Low = (uint32)m_Now.LowPart - sc_time_start_low;
	m_High = (uint32)m_Now.HighPart - sc_time_start_high -
		((uint32)m_Now.LowPart < sc_time_start_low);
	return sc_time_ticks_to_float(m_High, m_Low) * sc_time_seconds_per_tick;
}

largefloat sc_get_time_not_since_engine_startup() {
	LARGE_INTEGER m_Now;
	QueryPerformanceCounter(&m_Now);
	return sc_time_ticks_to_float((uint32)m_Now.HighPart, (uint32)m_Now.LowPart) *
		sc_time_seconds_per_tick;
}

largefloat sc_get_delta_and_update_time(largefloat* apLastTime, largefloat* apUpdateTime) {
	largefloat m_CurrentTime = *apUpdateTime;
	largefloat m_Delta = m_CurrentTime - *apLastTime;
	*apLastTime = m_CurrentTime;
	*apUpdateTime = sc_get_time();
	return m_Delta;
}

void sc_sleep_seconds(largefloat aSeconds) {
	largefloat m_Milliseconds;
	largeint m_Duration;
	if (!(aSeconds > (largefloat)0.0f) || aSeconds > SC_TIME_MAX_SECONDS) {
		return;
	}
	while (aSeconds >= (largefloat)4096.0f) {
		Sleep(4096000);
		aSeconds -= (largefloat)4096.0f;
	}
	m_Milliseconds = aSeconds * (largefloat)1000.0f;
	m_Duration = (largeint)m_Milliseconds;
	if ((largefloat)m_Duration < m_Milliseconds) {
		++m_Duration;
	}
	if (m_Duration != 0) {
		Sleep((DWORD)m_Duration);
	}
}

#endif // SC_WINDOWS