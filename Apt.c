#include "Apt.h"

extern WINDBG_EXTENSION_APIS   ExtensionApis;

#define OLE_STA_MASK   0x080	// Bugslayer, MSJ 10/99
#define OLE_MTA_MASK   0x140	// Bugslayer, MSJ 10/99
#define OLE_TNA_MASK   0x800	// http://members.tripod.com/IUnknwn

#define JPDBGEXT_E_DEBUGEE_ERROR MAKE_HRESULT( 1, FACILITY_ITF, 0x200 );
#define JPDBGEXT_E_UNKNOWN_APT	 MAKE_HRESULT( 1, FACILITY_ITF, 0x201 );

typedef struct _OLE_INFORMATION
{
	CHAR Padding[ 0xC ];
	DWORD Apartment;
} OLE_INFORMATION;

HRESULT JpDbgExtpGetThreadTebBaseAddress( 
	__in HANDLE hThread,
	__out DWORD *pdwBaseAddress 
	)
{
	THREAD_BASIC_INFORMATION threadInfo;
	DWORD retLen;
	NTSTATUS status;
	
	_ASSERTE( hThread );
	_ASSERTE( pdwBaseAddress );

	status = NtQueryInformationThread(
		hThread,
		ThreadBasicInformation,
		&threadInfo,
		sizeof( THREAD_BASIC_INFORMATION ),
		&retLen );
	if ( STATUS_SUCCESS != status )
	{
		return HRESULT_FROM_NT( status );
	}

	*pdwBaseAddress = * ( DWORD* ) &threadInfo.TebBaseAddress;
	return S_OK;
}

HRESULT JpDbgExtpGetApartmentType( 
	__in HANDLE hThread,
	__out APARTMENT_TYPE *pApt
	)
{
	DWORD dwTebBaseAddress = 0;
	PVOID pOleAddress = 0;
	OLE_INFORMATION oleInfo;
	HRESULT hr = E_UNEXPECTED;
	TEB debugeeTeb;

	_ASSERTE( hThread );
	_ASSERTE( pApt );

	//
	// Get the debugee thread's TEB.
	//
	hr = JpDbgExtpGetThreadTebBaseAddress( hThread, &dwTebBaseAddress );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	if ( ! ReadMemory( 
		dwTebBaseAddress, 
		&debugeeTeb,
		sizeof( TEB ),
		NULL ) )
	{
		return JPDBGEXT_E_DEBUGEE_ERROR;
	}

	//
	// Reach into the TEB and read OLE information.
	//
	pOleAddress = debugeeTeb.ReservedForOle;

	if ( pOleAddress == NULL )
	{
		//
		// Not a COM thread.
		//
		*pApt = APARTMENT_TYPE_NONE;
		hr = S_OK;
	}
	else
	{
		DWORD dwOleAddress = * ( DWORD* ) &pOleAddress;

		//
		// COM thread, get apartment
		//
		if ( ! ReadMemory( 
			dwOleAddress, 
			&oleInfo,
			sizeof( OLE_INFORMATION ),
			NULL ) )
		{
			return JPDBGEXT_E_DEBUGEE_ERROR;
		}

		if ( oleInfo.Apartment & OLE_STA_MASK )
		{
			*pApt = APARTMENT_TYPE_STA;
			hr = S_OK;
		}
		else if ( oleInfo.Apartment & OLE_MTA_MASK )
		{
			*pApt = APARTMENT_TYPE_MTA;
			hr = S_OK;
		}
		else if ( oleInfo.Apartment & OLE_TNA_MASK )
		{
			*pApt = APARTMENT_TYPE_TNA;
			hr = S_OK;
		}
		else
		{
#ifdef _DEBUG
			dprintf( "Unknown apartment type: 0x%08X\n", oleInfo.Apartment );
#endif
			*pApt = APARTMENT_TYPE_UNKNOWN;
			hr = S_OK;
		}
	}

	return hr;
}
