#include "../include/SysInfo.hpp"

namespace Sonar
{

	SysInfo::SysInfo( const MemoryFormat &memoryFormat, const bool loadSystemInfo )
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

		if ( loadSystemInfo )
		{ LoadSystemInformation( ); }
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

	void SysInfo::LoadSystemInformation( )
	{
		//First make a delegate object that handles the cases where the computer has multiple CPUs
		std::unique_ptr<CPUInfoDelegate> cpuInfo = std::make_unique<CPUInfoDelegate>( );

		//Then extract the separate CPUs into a vector (of CPUInfo objects)
		std::vector<CPUInfo> cpuInfoVector{ cpuInfo->cpuInfoVector( ) };

		//Print out the number of CPUs, directory from the delegate object
		_systemInformation._numberofPhysicalCPUs = cpuInfo->numberOfCPUInfoItems( );

		for ( std::vector<CPUInfo>::const_iterator iter = cpuInfoVector.begin( ); iter != cpuInfoVector.end( ); iter++ )
		{
			CPU cpu;

			cpu._cpuNumber = iter->cpuNumber( ) + 1;
			cpu._name = iter->name( );
			cpu._manufacturer = iter->manufacturer( );
			cpu._coreCount = iter->numberOfCores( );
			cpu._clockSpeed = iter->currentClockSpeed( );
			cpu._architecture = iter->architecture( );
			cpu._l2CacheSize = iter->L2CacheSize( );
			cpu._l3CacheSize = iter->L3CacheSize( );
			cpu._temperature = iter->currentTemperature( );

			_systemInformation._cpus.push_back( cpu );
		}

		std::unique_ptr<MotherboardInfoDelegate> moboInfo = std::make_unique<MotherboardInfoDelegate>( );
		std::vector<MotherboardInfo> moboInfoVector = moboInfo->motherboardInfoVector( );
		std::cout << "This computer has " << moboInfo->numberOfMotherboardInfoItems( ) << " motherboard(s) installed" << std::endl;

		for ( std::vector<MotherboardInfo>::const_iterator iter = moboInfoVector.begin( ); iter != moboInfoVector.end( ); iter++ )
		{
			std::cout << "Information for motherboard #" << iter->motherboardNumber( ) + 1 << ": " << std::endl;
			std::cout << "Motherboard Name = " << iter->name( ) << std::endl;
			std::cout << "Motherboard Manufacturer = " << iter->manufacturer( ) << std::endl;
			std::cout << "Motherboard Chipset = " << iter->chipset( ) << std::endl;
			std::cout << "Motherboard Serial Number = " << iter->serialNumber( ) << std::endl;
			std::cout << "Motherboard Version = " << iter->version( ) << std::endl;
			std::cout << std::endl;
		}

		std::unique_ptr<RAMInfoDelegate> ramInfo = std::make_unique<RAMInfoDelegate>( );
		std::vector<RAMInfo> ramInfoVector = ramInfo->ramInfoVector( );
		std::cout << "This computer has " << ramInfo->numberOfRAMInfoItems( ) << " RAM stick(s) installed" << std::endl;

		for ( std::vector<RAMInfo>::const_iterator iter = ramInfoVector.begin( ); iter != ramInfoVector.end( ); iter++ )
		{
			std::cout << "Information for RAM stick #" << iter->ramNumber( ) + 1 << ": " << std::endl;
			std::cout << "RAM Name = " << iter->name( ) << std::endl;
			std::cout << "RAM Manufacturer = " << iter->manufacturer( ) << std::endl;
			std::cout << "RAM Capacity = " << iter->capacity( ) << std::endl;
			std::cout << "RAM Serial Number = " << iter->serialNumber( ) << std::endl;
			std::cout << "RAM Form Factor = " << iter->formFactor( ) << std::endl;
			std::cout << "RAM Part Number = " << iter->partNumber( ) << std::endl;
			std::cout << "RAM Memory Type = " << iter->memoryType( ) << std::endl;
			std::cout << "RAM Clock Speed = " << iter->clockSpeed( ) << std::endl;
			std::cout << std::endl;
		}

		std::unique_ptr<GPUInfoDelegate> gpuInfo = std::make_unique<GPUInfoDelegate>( );
		std::vector<GPUInfo> gpuInfoVector = gpuInfo->gpuInfoVector( );
		std::cout << "This computer has " << gpuInfo->numberOfGPUInfoItems( ) << " GPU(s) installed" << std::endl;
		for ( std::vector<GPUInfo>::const_iterator iter = gpuInfoVector.begin( ); iter != gpuInfoVector.end( ); iter++ )
		{
			std::cout << "Information for GPU #" << iter->gpuNumber( ) + 1 << ": " << std::endl;
			std::cout << "GPU Name = " << iter->name( ) << std::endl;
			std::cout << "GPU Manufacturer = " << iter->manufacturer( ) << std::endl;
			std::cout << "GPU Caption = " << iter->caption( ) << std::endl;
			std::cout << "GPU Adapter RAM = " << iter->adapterRAM( ) << std::endl;
			std::cout << "GPU Refresh Rate = " << iter->refreshRate( ) << std::endl;
			std::cout << "GPU Driver Version = " << iter->driverVersion( ) << std::endl;
			std::cout << "GPU Video Architecture = " << iter->videoArchitecture( ) << std::endl;
			std::cout << "GPU Video Mode Description = " << iter->videoModeDescription( ) << std::endl;
			std::cout << "GPU Video Processor = " << iter->videoProcessor( ) << std::endl;
			std::cout << std::endl;
		}

		std::unique_ptr<OSInfoDelegate> osInfo = std::make_unique<OSInfoDelegate>( );
		std::vector<OSInfo> osInfoVector = osInfo->osInfoVector( );
		std::cout << "This computer has " << osInfo->numberOfOSInfoItems( ) << " OS(s) installed" << std::endl;
		for ( std::vector<OSInfo>::const_iterator iter = osInfoVector.begin( ); iter != osInfoVector.end( ); iter++ )
		{
			std::cout << "Information for OS #" << iter->osNumber( ) + 1 << ": " << std::endl;
			std::cout << "OS Name = " << iter->name( ) << std::endl;
			std::cout << "OS Manufacturer = " << iter->manufacturer( ) << std::endl;
			std::cout << "OS Caption = " << iter->caption( ) << std::endl;
			std::cout << "OS Version = " << iter->version( ) << std::endl;
			std::cout << "Current User = " << iter->currentUser( ) << std::endl;
			std::cout << "Install Date = " << iter->installDate( ) << std::endl;
			std::cout << "Build Number = " << iter->buildNumber( ) << std::endl;
			std::cout << "Last Boot Up Time = " << iter->lastBootUpTime( ) << std::endl;
			std::cout << "Boot Device = " << iter->bootDevice( ) << std::endl;
			std::cout << "Total Virtual Memory = " << iter->totalVirtualMemory( ) << std::endl;
			std::cout << "Total Visible Memory = " << iter->totalVisibleMemory( ) << std::endl;
			std::cout << "Total Swap Size = " << iter->totalSwapSize( ) << std::endl;
			std::cout << "Serial Number = " << iter->serialNumber( ) << std::endl;
			std::cout << "Free Physical Memory = " << iter->freePhysicalMemory( ) << std::endl;
			std::cout << "Free Virtual Memory = " << iter->freeVirtualMemory( ) << std::endl;
			std::cout << "Free Paging File Space = " << iter->freePagingFileSpace( ) << std::endl;
			std::cout << "Used Paging File Space = " << iter->usedPagingFileSpace( ) << std::endl;
			std::cout << "Current Date Time = " << iter->currentDateTime( ) << std::endl;
			std::cout << std::endl;
		}
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