#ifndef STARDUST_COMMON_H
#define STARDUST_COMMON_H

#include <windows.h>
#include <wininet.h>
#include <wincrypt.h>
#include <cstdio>

#include <Native.h>
#include <Macros.h>
#include <Ldr.h>
#include <Defs.h>
#include <Utils.h>
#include <Config.h>

EXTERN_C ULONG __Instance_offset;
EXTERN_C PVOID __Instance;

typedef struct _INSTANCE {

    BUFFER Base;

    struct {

        D_API( RtlAllocateHeap        )
        D_API( NtProtectVirtualMemory )

        D_API( LoadLibraryW )
        D_API( VirtualAlloc )
        D_API( VirtualProtect )
        D_API( VirtualFree )
        D_API( GetLastError )

        D_API( MessageBoxA )

        D_API ( strlen );
        D_API ( strcmp)
        D_API ( sprintf );
        D_API ( calloc );
        D_API ( memset );
        D_API ( free );

        D_API( InternetOpenA );
        D_API( InternetConnectA );
        D_API( HttpOpenRequestA );
        D_API( HttpSendRequestA );
        D_API( HttpQueryInfoA );
        D_API( InternetQueryOptionA );
        D_API( InternetSetOptionA );
        D_API( InternetReadFile );
        D_API( InternetCloseHandle );

        D_API( CryptAcquireContextA );
        D_API( CryptCreateHash );
        D_API( CryptHashData );
        D_API( CryptGetHashParam );
        D_API( CryptDestroyHash );
        D_API( CryptReleaseContext );

    } Win32;

    struct {
        PVOID Ntdll;
        PVOID Kernel32;
        PVOID User32;
        PVOID Msvcrt;
        PVOID Wininet;
        PVOID Advapi32;
    } Modules;

} INSTANCE, *PINSTANCE;

EXTERN_C PVOID StRipStart();
EXTERN_C PVOID StRipEnd();

VOID Main(
    _In_ PVOID Param
);

#define MD5LEN  16

#endif 
