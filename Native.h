#pragma once

/*++
 Description:
   Native API declarations,  mostly taken from ReactOS/Wine

 Author: 
   J. Passing
--*/

typedef enum _THREAD_INFORMATION_CLASS {
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger
} THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;

typedef DWORD NTSTATUS;
#define STATUS_SUCCESS 0

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _THREAD_BASIC_INFORMATION
{
	NTSTATUS  ExitStatus;
	PVOID     TebBaseAddress;
	CLIENT_ID ClientId;
	ULONG     AffinityMask;
	LONG      Priority;
	LONG      BasePriority;
} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;

typedef struct _GDI_TEB_BATCH
{
    ULONG  Offset;
    HANDLE HDC;
    ULONG  Buffer[0x136];
} GDI_TEB_BATCH;

typedef struct _UNICODE_STRING {
	USHORT Length;        /* bytes */
	USHORT MaximumLength; /* bytes */
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _TEB
{
    NT_TIB          Tib;                        /* 000 */
    PVOID           EnvironmentPointer;         /* 01c */
    CLIENT_ID       ClientId;                   /* 020 */
    PVOID           ActiveRpcHandle;            /* 028 */
    PVOID           ThreadLocalStoragePointer;  /* 02c */
    PVOID           Peb;                        /* 030 */
    ULONG           LastErrorValue;             /* 034 */
    ULONG           CountOfOwnedCriticalSections;/* 038 */
    PVOID           CsrClientThread;            /* 03c */
    PVOID           Win32ThreadInfo;            /* 040 */
    ULONG           Win32ClientInfo[31];        /* 044 used for user32 private data in Wine */
    PVOID           WOW32Reserved;              /* 0c0 */
    ULONG           CurrentLocale;              /* 0c4 */
    ULONG           FpSoftwareStatusRegister;   /* 0c8 */
    PVOID           SystemReserved1[54];        /* 0cc used for kernel32 private data in Wine */
    PVOID           Spare1;                     /* 1a4 */
    LONG            ExceptionCode;              /* 1a8 */
    BYTE            SpareBytes1[40];            /* 1ac */
    PVOID           SystemReserved2[10];        /* 1d4 used for ntdll private data in Wine */
    GDI_TEB_BATCH   GdiTebBatch;                /* 1fc */
    ULONG           gdiRgn;                     /* 6dc */
    ULONG           gdiPen;                     /* 6e0 */
    ULONG           gdiBrush;                   /* 6e4 */
    CLIENT_ID       RealClientId;               /* 6e8 */
    HANDLE          GdiCachedProcessHandle;     /* 6f0 */
    ULONG           GdiClientPID;               /* 6f4 */
    ULONG           GdiClientTID;               /* 6f8 */
    PVOID           GdiThreadLocaleInfo;        /* 6fc */
    PVOID           UserReserved[5];            /* 700 */
    PVOID           glDispachTable[280];        /* 714 */
    ULONG           glReserved1[26];            /* b74 */
    PVOID           glReserved2;                /* bdc */
    PVOID           glSectionInfo;              /* be0 */
    PVOID           glSection;                  /* be4 */
    PVOID           glTable;                    /* be8 */
    PVOID           glCurrentRC;                /* bec */
    PVOID           glContext;                  /* bf0 */
    ULONG           LastStatusValue;            /* bf4 */
    UNICODE_STRING  StaticUnicodeString;        /* bf8 used by advapi32 */
    WCHAR           StaticUnicodeBuffer[261];   /* c00 used by advapi32 */
    PVOID           DeallocationStack;          /* e0c */
    PVOID           TlsSlots[64];               /* e10 */
    LIST_ENTRY      TlsLinks;                   /* f10 */
    PVOID           Vdm;                        /* f18 */
    PVOID           ReservedForNtRpc;           /* f1c */
    PVOID           DbgSsReserved[2];           /* f20 */
    ULONG           HardErrorDisabled;          /* f28 */
    PVOID           Instrumentation[16];        /* f2c */
    PVOID           WinSockData;                /* f6c */
    ULONG           GdiBatchCount;              /* f70 */
    ULONG           Spare2;                     /* f74 */
    ULONG           Spare3;                     /* f78 */
    ULONG           Spare4;                     /* f7c */
    PVOID           ReservedForOle;             /* f80 */
    ULONG           WaitingOnLoaderLock;        /* f84 */
    PVOID           Reserved5[3];               /* f88 */
    PVOID          *TlsExpansionSlots;          /* f94 */
} TEB, *PTEB;

NTSYSAPI 
NTSTATUS
NTAPI
NtQueryInformationThread(
	IN HANDLE					ThreadHandle,
	IN THREAD_INFORMATION_CLASS ThreadInformationClass,
	OUT PVOID					ThreadInformation,
	IN ULONG					ThreadInformationLength,
	OUT PULONG					ReturnLength OPTIONAL );

