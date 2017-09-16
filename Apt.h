#include "stdafx.h"
#include "Native.h"

#pragma once

typedef enum
{
	APARTMENT_TYPE_STA = 0,
	APARTMENT_TYPE_MTA = 1,
	APARTMENT_TYPE_TNA = 2,
	APARTMENT_TYPE_NONE = 3,
	APARTMENT_TYPE_UNKNOWN = 4
} APARTMENT_TYPE;

/*++
  Description:
	Returns the VA of the TEB type of the given thread

  Parameters:
	hThread - Thread to query
	pdwBaseAddress  - VA
--*/
HRESULT JpDbgExtpGetThreadTebBaseAddress( 
	__in HANDLE hThread,
	__out DWORD *pdwBaseAddress 
	);

/*++
  Description:
	Returns the Apartment type of the given thread

  Parameters:
	hThread - Thread to query
	pApt    - Apartment type
--*/
HRESULT JpDbgExtpGetApartmentType( 
	__in HANDLE hThread,
	__out APARTMENT_TYPE *pApt
	);
