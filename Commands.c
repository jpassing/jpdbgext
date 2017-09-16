#include "stdafx.h"
#include "Native.h"
#include "Apt.h"

EXT_API_VERSION         ApiVersion = { 1, 0, EXT_API_VERSION_NUMBER32, 0 };
WINDBG_EXTENSION_APIS   ExtensionApis;
ULONG SavedMajorVersion;
ULONG SavedMinorVersion;




VOID
WinDbgExtensionDllInit(
    PWINDBG_EXTENSION_APIS lpExtensionApis,
    USHORT MajorVersion,
    USHORT MinorVersion
    )
{
    ExtensionApis = *lpExtensionApis;

    SavedMajorVersion = MajorVersion;
    SavedMinorVersion = MinorVersion;

    return;
}

LPEXT_API_VERSION
ExtensionApiVersion(
    VOID
    )
{
    return &ApiVersion;
}

DECLARE_API( help )
/*
	HANDLE                 hCurrentProcess
	HANDLE                 hCurrentThread
	ULONG64                dwCurrentPc
	ULONG                  dwProcessor
	PCSTR                  args
*/
{
	UNREFERENCED_PARAMETER( hCurrentProcess );
	UNREFERENCED_PARAMETER( hCurrentThread );
	UNREFERENCED_PARAMETER( dwCurrentPc );
	UNREFERENCED_PARAMETER( dwProcessor );
	UNREFERENCED_PARAMETER( args );

	dprintf( "Johannes Passing's Debugging Extension\n"
			 "(C) 2007, All rights reserved\n\n"
			 
			 "apt    Show apartment type of current thread\n" );
}

DECLARE_API( apt )
/*
	HANDLE                 hCurrentProcess
	HANDLE                 hCurrentThread
	ULONG64                dwCurrentPc
	ULONG                  dwProcessor
	PCSTR                  args
*/
{
	APARTMENT_TYPE aptType;
	HRESULT hr = E_UNEXPECTED;

	UNREFERENCED_PARAMETER( hCurrentProcess );
	UNREFERENCED_PARAMETER( dwCurrentPc );
	UNREFERENCED_PARAMETER( dwProcessor );
	UNREFERENCED_PARAMETER( args );

	hr = JpDbgExtpGetApartmentType( hCurrentThread, &aptType );
	if ( FAILED( hr ) )
	{
		dprintf( "Apartment determination failed: HRESULT 0x%08X\n", hr );
		return;
	}

	dprintf( "Thread 0x%08X Apartment: ", hCurrentThread );

	switch ( aptType )
	{
	case APARTMENT_TYPE_MTA:
		dprintf( "MTA\n" );
		break;

	case APARTMENT_TYPE_STA:
		dprintf( "STA\n" );
		break;

	case APARTMENT_TYPE_TNA:
		dprintf( "TNA\n" );
		break;

	case APARTMENT_TYPE_NONE:
		dprintf( "Not a COM thread\n" );
		break;

	case APARTMENT_TYPE_UNKNOWN:
		dprintf( "Unknown (Unrecognized flags)\n" );
		break;

	}
}
