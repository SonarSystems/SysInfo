#include "SysInfo.hpp"

namespace Sonar
{
	SysInfo::SysInfo( )
	{
		//Code block initialization for the memory referenced in the Kernel
		
		//memStat.dwLength = sizeof( memStat );
		//GlobalMemoryStatusEx( &memStat );

		//
		////Retrieves data so that we have a way to Get it to output when using the pointers
		//GetSystemTime( &sysTime );
	}

	SysInfo::~SysInfo( )
	{

	}

	const Sonar::SystemInformation &SysInfo::GetSystemInformation( )
	{
		SYSTEMTIME sysTime;

		MEMORYSTATUSEX memStat;

		SystemInformation systemInformation;

		memStat.dwLength = sizeof( memStat );
		GlobalMemoryStatusEx( &memStat );

		GetSystemTime( &sysTime );
		
		systemInformation._timestamp._hour = sysTime.wHour;
		systemInformation._timestamp._minutes = sysTime.wMinute;
		systemInformation._timestamp._seconds = sysTime.wSecond;
		systemInformation._timestamp._milliseconds = sysTime.wMilliseconds;
		systemInformation._cpuLoad = GetCPULoad( ) * 100.0f;
		systemInformation._memoryLoad = memStat.dwMemoryLoad;
		systemInformation._physicalAvailableMemory = memStat.ullTotalPhys;
		systemInformation._physicalTotalMemory = memStat.ullAvailPhys;
		systemInformation._pageFileAvailableMemory = memStat.ullTotalPageFile;
		systemInformation._pageFileTotalMemory = memStat.ullAvailPageFile;
		systemInformation._virtualAvailableMemory = memStat.ullTotalVirtual;
		systemInformation._virtualTotalMemory = memStat.ullAvailVirtual;
		systemInformation._virtualExtendedAvailableMemory = memStat.ullAvailExtendedVirtual;

		return systemInformation;

		/*std::cout <<
			sysTime.wHour << "." <<
			sysTime.wMinute << "." <<
			sysTime.wSecond << ", " << */
			//GetCPULoad( ) * 100 << "%, " << 
			//memStat.dwMemoryLoad << "%, " << 
			//memStat.ullTotalPhys / MB << ", " << 
			//memStat.ullAvailPhys / MB << ", " << 
			//memStat.ullTotalPageFile / MB << ", " << 
			//memStat.ullAvailPageFile / MB << ", " << 
			//memStat.ullTotalVirtual / MB << ", " << 
			//memStat.ullAvailVirtual / MB << ", " << 
			//memStat.ullAvailExtendedVirtual / MB << "\n";
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
	{
		return ( ( ( unsigned long long )( ft.dwHighDateTime ) ) << 32 ) | ( ( unsigned long long )ft.dwLowDateTime );
	}

	// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
	// You'll need to call this at regular intervals, since it measures the load between
	// the previous call and the current one.  Returns -1.0 on error.
	float SysInfo::GetCPULoad( )
	{
		FILETIME idleTime, kernelTime, userTime;
		return GetSystemTimes( &idleTime, &kernelTime, &userTime ) ? CalculateCPULoad( FileTimeToInt64( idleTime ), FileTimeToInt64( kernelTime ) + FileTimeToInt64( userTime ) ) : -1.0f;
	}
}