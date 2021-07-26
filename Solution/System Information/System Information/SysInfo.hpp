#pragma once

#include <iostream>
#include <fstream>
#include <windows.h>

/**
* \brief creates a static variable to convert Bytes to Megabytes
*/
#define MB 1048576

namespace Sonar
{
	/**
	* \brief Timestamp
	*/
	struct Timestamp
	{
		unsigned int _hour;
		unsigned int _minutes;
		unsigned int _seconds;
		unsigned int _milliseconds;
	};

	/**
	* \brief System information (all memory is in MB)
	*/
	struct SystemInformation
	{
		Timestamp _timestamp;
		float _cpuLoad;
		unsigned long _memoryLoad;
		unsigned long long _physicalTotalMemory;
		unsigned long long _physicalAvailableMemory;
		unsigned long long _pageFileTotalMemory;
		unsigned long long _pageFileAvailableMemory;
		unsigned long long _virtualTotalMemory;
		unsigned long long _virtualAvailableMemory;
		unsigned long long _virtualExtendedAvailableMemory;
	};

	class SysInfo
	{
	public:
		SysInfo( );
		~SysInfo( );

		static const SystemInformation &GetSystemInformation( );

		static float CalculateCPULoad( unsigned long long idleTicks, unsigned long long totalTicks );
		static unsigned long long FileTimeToInt64( const FILETIME &ft );
		static float GetCPULoad( );

	private:
		

	};
}
