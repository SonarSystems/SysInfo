#pragma once

#include <iostream>
#include <fstream>
#include <windows.h>

/**
* \brief creates a static variable to convert Bytes to Megabytes
*/
#define MB 1048576

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
		unsigned int _hour = 0;
		unsigned int _minutes = 0;
		unsigned int _seconds = 0;
		unsigned int _milliseconds = 0;
		unsigned int _day = 0;
		unsigned int _month = 0;
		unsigned short _year = 0;
		unsigned short _dayOfTheWeek = 0;
	};

	/**
	* \brief System information (all memory is in Bytes)
	*/
	struct SystemInformation
	{
		Timestamp _timestamp;
		float _cpuLoad = 0.0f;
		unsigned long _memoryLoad = 0;
		float _physicalTotalMemory = 0;
		float _physicalAvailableMemory = 0;
		float _pageFileTotalMemory = 0;
		float _pageFileAvailableMemory = 0;
		float _virtualTotalMemory = 0;
		float _virtualAvailableMemory = 0;
		float _virtualExtendedAvailableMemory = 0;
	};

	class SysInfo
	{
	public:
		/**
		* \brief Default constructor
		*/
		SysInfo( const MemoryFormat &memoryFormat = MemoryFormat::Gigabytes );

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
