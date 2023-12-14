libandroid-support
------------------
A copy of libandroid-support as built from NDK r14 in Termux.

The purpose of `libandroid-support` in Termux is to add functionality missing
the system libc to ease building packages.

Not implemented yet
-------------------
`Introduced in API 23`
• <malloc.h> malloc_info
• <netdb.h> gethostbyaddr_r, gethostbyname2_r
• <pthread.h> pthread_rwlockattr_getkind_np/pthread_rwlockattr_setkind_np
• <stdlib.h> mkostemp/mkostemps, lcong48
• <resolv.h> functions: ns_format_ttl, ns_get16, ns_get32, ns_initparse, ns_makecanon, ns_msg_getflag, ns_name_compress, ns_name_ntol, ns_name_ntop, ns_name_pack, ns_name_pton, ns_name_rollback, ns_name_skip, ns_name_uncompress, ns_name_unpack, ns_parserr, ns_put16, ns_put32, ns_samename, ns_skiprr, ns_sprintrr, and ns_sprintrrf.

What is still necessary?
------------------------
Some functionality can probably be removed in favour of functionality in bionic:

- [bionic in Android 5.0](https://android.googlesource.com/platform/bionic.git/+/lollipop-release/libc/bionic/)
- [bionic in Android 6.0](https://android.googlesource.com/platform/bionic.git/+/marshmallow-release/libc/bionic/)
- [bionic in Android 7.0](https://android.googlesource.com/platform/bionic.git/+/nougat-release/libc/bionic/)

Test cases
----------
- Findutils: `touch åäö && find . -name åäö`.
