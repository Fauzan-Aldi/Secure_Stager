#include <Common.h>
#include <Constexpr.h>

ST_GLOBAL PVOID __Instance = C_PTR( 'rdp5' );

EXTERN_C FUNC VOID PreMain(
    PVOID Param
) {
    INSTANCE Stardust = { 0 };
    PVOID    Heap     = { 0 };
    PVOID    MmAddr   = { 0 };
    SIZE_T   MmSize   = { 0 };
    ULONG    Protect  = { 0 };

    MmZero( & Stardust, sizeof( Stardust ) );
    Heap = NtCurrentPeb()->ProcessHeap;

   
    Stardust.Base.Buffer = StRipStart();
    Stardust.Base.Length = U_PTR( StRipEnd() ) - U_PTR( Stardust.Base.Buffer );

    MmAddr = Stardust.Base.Buffer + InstanceOffset();
    MmSize = sizeof( PVOID );

    if ( ( Stardust.Modules.Ntdll = LdrModulePeb( H_MODULE_NTDLL ) ) ) {
        if ( ! ( Stardust.Win32.RtlAllocateHeap        = LdrFunction( Stardust.Modules.Ntdll, HASH_STR( "RtlAllocateHeap"        ) ) ) ||
             ! ( Stardust.Win32.NtProtectVirtualMemory = LdrFunction( Stardust.Modules.Ntdll, HASH_STR( "NtProtectVirtualMemory" ) ) )
        ) {
            return;
        }
    }

    if ( ! NT_SUCCESS( Stardust.Win32.NtProtectVirtualMemory(
        NtCurrentProcess(),
        & MmAddr,
        & MmSize,
        PAGE_READWRITE,
        & Protect
    ) ) ) {
        return;
    }

    
    if ( ! ( C_DEF( MmAddr ) = Stardust.Win32.RtlAllocateHeap( Heap, HEAP_ZERO_MEMORY, sizeof( INSTANCE ) ) ) ) {
        return;
    }

    MmCopy( C_DEF( MmAddr ), &Stardust, sizeof( INSTANCE ) );
    MmZero( & Stardust, sizeof( INSTANCE ) );
    MmZero( C_PTR( U_PTR( MmAddr ) + sizeof( PVOID ) ), 0x18 );

    
    Main( Param );
}