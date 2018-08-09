#include "stdafx.h"
#include "Timestamp.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdio.h>

namespace FacileLib
{
	Timestamp::Timestamp(int64_t microSecondsSinceEpoch)
		: microSecondsSinceEpoch_(microSecondsSinceEpoch)
	{

	}

	Timestamp::~Timestamp()
	{
	}

	void Timestamp::swap(Timestamp& that)
	{
		std::swap(microSecondsSinceEpoch_, that.microSecondsSinceEpoch_);
	}

	std::string Timestamp::toString() const
	{
		char buf[32] = { 0 };
		int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
		int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
	#ifdef WIN32
		_snprintf_s(buf,32,_TRUNCATE,"%ld.%06ld", seconds, microseconds);
	#else
		sprintf(buf, "%ld.%06ld", seconds, microseconds);
	#endif
		
		return buf;
	}

	std::string Timestamp::toFormattedString(bool showMicroseconds /*= true*/) const
	{
		time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
		struct tm tm_time;

#ifdef WIN32
		localtime_s(&tm_time, &seconds);
#else
		struct tm *ptm;
		ptm = localtime(&seconds);
		tm_time = *ptm;
#endif
		//ss << std::put_time(&ptm, "%F %T");
		//<<		((double)(microSecondsSinceEpoch_ % kMicroSecondsPerSecond)) / kMicroSecondsPerSecond;

		char buf[32] = { 0 };

		if (showMicroseconds)
		{
			int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
#ifdef WIN32
			_snprintf_s(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
				microseconds);
#else
			snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
				microseconds);
#endif
		}
		else
		{
#ifdef WIN32
			_snprintf_s(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
#else
			snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
				tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
				tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
#endif
		}
		return buf;
	}

	Timestamp Timestamp::now()
	{
		std::chrono::time_point<std::chrono::system_clock, std::chrono::microseconds> now = std::chrono::time_point_cast<std::chrono::microseconds>(
			std::chrono::system_clock::now());

		int64_t microSeconds = now.time_since_epoch().count();
		Timestamp time(microSeconds);
		return time;
	}

	Timestamp Timestamp::invalid()
	{
		return Timestamp();
	}
}