import pefile
import argparse

STARDUST_END : bytes = b'STARDUST-END'
PAGE_SIZE    : int   = 0x1000

def size_to_pages( size: int ) -> int:
    PAGE_MASK       : int = 0xfff
    BASE_PAGE_SHIFT : int = 12

    return ( size >> BASE_PAGE_SHIFT ) + ( ( size & PAGE_MASK ) != 0 )
def main() -> None:
    parser = argparse.ArgumentParser( description = 'Extracts shellcode from a PE.' )
    parser.add_argument( '-f', required = True,  help = 'Path to the source executable', type = str )
    parser.add_argument( '-o', required = True,  help = 'Path to store the output raw binary', type = str )
    option = parser.parse_args()

    executable  = pefile.PE( option.f )
    shellcode   = bytearray( executable.sections[ 0 ].get_data() )
    shellcode   = shellcode[ : shellcode.find( STARDUST_END ) ]
    size        = len( shellcode )

    pages    = size_to_pages( size )
    padding  = ( ( pages * PAGE_SIZE ) - size )

    size = len( shellcode )
    print( f"[*] payload len : { size - padding } bytes" )
    print( f"[*] size        : { size } bytes" )
    print( f"[*] padding     : { padding } bytes" )
    print( f"[*] page count  : { size / PAGE_SIZE } pages" )

    file = open( option.o, 'wb+' )

    file.write( shellcode )
    file.close()

    return

if __name__ in '__main__':
    main()