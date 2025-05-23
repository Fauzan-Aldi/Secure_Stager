# Stardust

An modern 64-bit position independent implant template. 

- raw strings 
- global instance 
- compile time hashing

```c
#include <Common.h>
#include <Constexpr.h>

FUNC VOID Main(
    _In_ PVOID Param
) {
    STARDUST_INSTANCE

    PVOID Message = { 0 };

    if ( ( Instance()->Modules.Kernel32 = LdrModulePeb( H_MODULE_KERNEL32 ) ) ) {
        if ( ! ( Instance()->Win32.LoadLibraryW = LdrFunction( Instance()->Modules.Kernel32, HASH_STR( "LoadLibraryW" ) ) ) ) {
            return;
        }
    }

    if ( ( Instance()->Modules.User32 = Instance()->Win32.LoadLibraryW( L"User32" ) ) ) {
        if ( ! ( Instance()->Win32.MessageBoxW = LdrFunction( Instance()->Modules.User32, HASH_STR( "MessageBoxW" ) ) ) ) {
            return;
        }
    }

    Message = NtCurrentPeb()->ProcessParameters->ImagePathName.Buffer;

    Instance()->Win32.MessageBoxW( NULL, Message, L"Stardust MessageBox", MB_OK );
}

```

## How does it work ?
I have written a [Blog post](https://5pider.net/blog/2024/01/27/modern-shellcode-implant-design/) about how it fully works and the reason behind it.

![Stardust messagebox](https://5pider.net/assets/images/MessagePop-4e72bc8a03044463b6afa71d8881646a.png)
