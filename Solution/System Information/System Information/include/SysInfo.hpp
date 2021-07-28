/**
* \brief Using code from https://github.com/tlewiscpp/SystemInfo
*/

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

#include "systeminfo.h"

/**
* \brief creates a static variable to convert Bytes to Megabytes
*/
#define MB 1048576

/**
* \brief Used when converting between different memory units
*/
#define MEMORY_MULTIPLIER 1024.0f

namespace Sonar
{
	/**
	* \brief Memory format
	*/
	enum MemoryFormat { Bits, Bytes, Kilobytes, Megabytes, Gigabytes };

	/**
	* \brief Timestamp
	*/
	struct Timestamp
	{
		unsigned int _hour;
		unsigned int _minutes;
		unsigned int _seconds;
		unsigned int _milliseconds;
		unsigned int _day;
		unsigned int _month;
		unsigned short _year;
		unsigned short _dayOfTheWeek;
	};

	/**
	* \brief CPU stats
	*/
	struct CPU
	{
		int _cpuNumber = 0; // Which physical CPU it is (starts at 1)
		std::string _name;
		std::string _manufacturer;
		std::string _coreCount;
		std::string _clockSpeed;
		std::string _architecture;
		std::string _l2CacheSize;
		std::string _l3CacheSize;
		std::string _temperature;
	};

	/**
	* \brief System information (all memory is in Bytes)
	*/
	struct SystemInformation
	{
		Timestamp _timestamp;
		float _cpuLoad;
		unsigned long _memoryLoad;
		float _physicalTotalMemory;
		float _physicalAvailableMemory;
		float _pageFileTotalMemory;
		float _pageFileAvailableMemory;
		float _virtualTotalMemory;
		float _virtualAvailableMemory;
		float _virtualExtendedAvailableMemory;

		int _numberofPhysicalCPUs;
		std::vector<CPU> _cpus; // Will most likely only be one for most systems
	};

	class SysInfo
	{
	public:
		/**
		* \brief Constructor
		* 
		* \param memoryFormat Memory units to store memory stats in
		* \param loadSystemInfo Should the system info be loaded at the start (can be a bit slow so false by default)
		*/
		SysInfo( const MemoryFormat &memoryFormat = MemoryFormat::Gigabytes, const bool loadSystemInfo = false );

		/**
		* \brief Default destructor
		*/
		~SysInfo( ) = default;

		/**
		* \brief Get the current system stats
		*
		* \return Output returns the system's statistics
		*/
		const SystemInformation &GetSystemInformation( );

		/**
		* \brief Loads the system information (only needs to be run once)
		*/
		void LoadSystemInformation( );

	private:
		/**
		* \brief Calculate the CPU's load (as a percentage)
		*
		* \param idleTicks Number of ticks whilst idle
		* \param totalTicks Number of total ticks
		*
		* \return Output returns the CPU load
		*/
		float CalculateCPULoad( unsigned long long idleTicks, unsigned long long totalTicks );

		/**
		* \brief Convert file time to int 64
		*
		* \param ft File time
		*
		* \return Output returns the converted file time
		*/
		unsigned long long FileTimeToInt64( const FILETIME &ft );

		/**
		* \brief Get the CPU load
		*
		* \return Output returns the CPU's load
		*/
		float GetCPULoad( );

		/**
		* \brief System information struct
		*/
		SystemInformation _systemInformation{ };

		/**
		* \brief Multipler to convert memory units
		*/
		float _memoryFormatMultiplier = pow( MEMORY_MULTIPLIER, 3.0f );

	};
}
