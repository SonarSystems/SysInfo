#include "SysInfo.hpp"

namespace Sonar
{

	SysInfo::SysInfo( const MemoryFormat &memoryFormat )
	{
		switch ( memoryFormat )
		{
			case MemoryFormat::Bits:
				_memoryFormatMultiplier = 8.0f;

				break;

			case MemoryFormat::Bytes:
				_memoryFormatMultiplier = 1.0f;

				break;

			case MemoryFormat::Kilobytes:
				_memoryFormatMultiplier = MEMORY_MULTIPLIER;

				break;

			case MemoryFormat::Megabytes:
				_memoryFormatMultiplier =  pow( MEMORY_MULTIPLIER, 2.0f );

				break;

				/*
				* NOT NEEDED BUT LEFT INCASE DEFAULT VALUE CHANGED IN HEADER
			case MemoryFormat::Gigabytes:
				_memoryFormatMultiplier = 1024.0f * 1024.0f * 1024.0f;

				break;
				*/
		}
	}

	const Sonar::SystemInformation &SysInfo::GetSystemInformation( )
	{
		SYSTEMTIME sysTime;
		MEMORYSTATUSEX memStat;

		memStat.dwLength = sizeof( memStat );

		GlobalMemoryStatusEx( &memStat );
		GetSystemTime( &sysTime );

		// Timestamp
		_systemInformation._timestamp._hour = sysTime.wHour;
		_systemInformation._timestamp._minutes = sysTime.wMinute;
		_systemInformation._timestamp._seconds = sysTime.wSecond;
		_systemInformation._timestamp._milliseconds = sysTime.wMilliseconds;
		_systemInformation._timestamp._day = sysTime.wDay;
		_systemInformation._timestamp._month = sysTime.wMonth;
		_systemInformation._timestamp._year = sysTime.wYear;
		_systemInformation._timestamp._dayOfTheWeek = sysTime.wDayOfWeek;

		// Load (in percentage)
		_systemInformation._cpuLoad = GetCPULoad( ) * 100.0f;
		_systemInformation._memoryLoad = memStat.dwMemoryLoad;

		// Memory stats
		_systemInformation._physicalAvailableMemory = memStat.ullAvailPhys / _memoryFormatMultiplier;
		_systemInformation._physicalTotalMemory = memStat.ullTotalPhys / _memoryFormatMultiplier;
		_systemInformation._pageFileAvailableMemory = memStat.ullAvailPageFile /  _memoryFormatMultiplier;
		_systemInformation._pageFileTotalMemory = memStat.ullTotalPageFile / _memoryFormatMultiplier;
		_systemInformation._virtualAvailableMemory = memStat.ullAvailVirtual / _memoryFormatMultiplier;
		_systemInformation._virtualTotalMemory = memStat.ullTotalVirtual / _memoryFormatMultiplier;
		_systemInformation._virtualExtendedAvailableMemory = memStat.ullAvailExtendedVirtual / _memoryFormatMultiplier;

		return _systemInformation;
	}

	float SysInfo::CalculateCPULoad( unsigned long long idleTicks, unsigned long long totalTicks )
	{
		static unsigned long long _previousTotalTicks = 0;
		static unsigned long long _previousIdleTicks = 0;

		unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
		unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;

		float ret = 1.0f - ( ( totalTicksSinceLastTime > 0 ) ? ( ( float )idleTicksSinceLastTime ) / totalTicksSinceLastTime : 0 );

		_previousTotalTicks = totalTicks;
		_previousIdleTicks = idleTicks;

		return ret;
	}

	unsigned long long SysInfo::FileTimeToInt64( const FILETIME &ft )
	{ return ( ( ( unsigned long long )( ft.dwHighDateTime ) ) << 32 ) | ( ( unsigned long long )ft.dwLowDateTime ); }

	// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
	// You'll need to call this at regular intervals, since it measures the load between
	// the previous call and the current one.  Returns -1.0 on error.
	float SysInfo::GetCPULoad( )
	{
		FILETIME idleTime, kernelTime, userTime;

		return GetSystemTimes( &idleTime, &kernelTime, &userTime ) ? CalculateCPULoad( FileTimeToInt64( idleTime ), FileTimeToInt64( kernelTime ) + FileTimeToInt64( userTime ) ) : -1.0f;
	}
}