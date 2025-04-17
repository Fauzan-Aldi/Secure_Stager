#include <Common.h>


 @brief


 @param Buffer

  @param Hashed
 
  @return
 
FUNC PVOID LdrModulePeb(
    _In_ ULONG Hash
) {
    PLDR_DATA_TABLE_ENTRY Data  = { 0 };
    PLIST_ENTRY           Head  = { 0 };
    PLIST_ENTRY           Entry = { 0 };

    Head  = & NtCurrentPeb()->Ldr->InLoadOrderModuleList;
    Entry = Head->Flink;

    for ( ; Head != Entry ; Entry = Entry->Flink ) {
        Data = C_PTR( Entry );

        if ( HashString( Data->BaseDllName.Buffer, Data->BaseDllName.Length ) == Hash ) {
            return Data->DllBase;
        }
    }

    return NULL;
}


 @brief
 
 
  @param Image
 
 
  @return
 

FUNC PIMAGE_NT_HEADERS LdrpImageHeader(
        _In_ PVOID Image
) {
    PIMAGE_DOS_HEADER DosHeader = { 0 };
    PIMAGE_NT_HEADERS NtHeader  = { 0 };

    DosHeader = C_PTR( Image );

    if ( DosHeader->e_magic != IMAGE_DOS_SIGNATURE ) {
        return NULL;
    }

    NtHeader = C_PTR( U_PTR( Image ) + DosHeader->e_lfanew );

    if ( NtHeader->Signature != IMAGE_NT_SIGNATURE ) {
        return NULL;
    }

    return NtHeader;
}

FUNC PVOID LdrFunction(
    _In_ PVOID Library,
    _In_ ULONG Function
) {
    PVOID                   Address    = { 0 };
    PIMAGE_NT_HEADERS       NtHeader   = { 0 };
    PIMAGE_EXPORT_DIRECTORY ExpDir     = { 0 };
    SIZE_T                  ExpDirSize = { 0 };
    PDWORD                  AddrNames  = { 0 };
    PDWORD                  AddrFuncs  = { 0 };
    PWORD                   AddrOrdns  = { 0 };
    PCHAR                   FuncName   = { 0 };


    if ( ! Library || ! Function ) {
        return NULL;
    }
    if ( ! ( NtHeader = LdrpImageHeader( Library ) ) ) {
        return NULL;
    }

    ExpDir     = C_PTR( Library + NtHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress );
    ExpDirSize = NtHeader->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size;
    AddrNames  = C_PTR( Library + ExpDir->AddressOfNames );
    AddrFuncs  = C_PTR( Library + ExpDir->AddressOfFunctions );
    AddrOrdns  = C_PTR( Library + ExpDir->AddressOfNameOrdinals );

    
    for ( DWORD i = 0; i < ExpDir->NumberOfNames; i++ ) {
        
        FuncName = C_PTR( U_PTR( Library ) + AddrNames[ i ] );

        if ( HashString( FuncName, 0 ) != Function ) {
            continue;
        }

        Address = C_PTR( U_PTR( Library ) + AddrFuncs[ AddrOrdns[ i ] ] );

        if ( ( U_PTR( Address ) >= U_PTR( ExpDir ) ) &&
             ( U_PTR( Address ) <  U_PTR( ExpDir ) + ExpDirSize )
        ) {
           
            __debugbreak();
        }

        break;
    }

    return Address;
}