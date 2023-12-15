set(LIBC_SRCS
    __set_errno.cpp
    libc_logging.cpp
    include/LocalArray.h
    include/private/bionic_asm.h
    include/private/bionic_futex.h
    include/private/bionic_lock.h
    include/private/bionic_macros.h
    include/private/ErrnoRestorer.h
    include/private/get_cpu_count_from_string.h
    include/private/kernel_sigset_t.h
    include/private/libc_events.h
    include/private/libc_logging.h
    include/private/ScopedPthreadMutexLocker.h
    include/private/ScopedReaddir.h
    include/private/thread_private.h
    include/ScopedFd.h
    include/sys/_errdefs.h
    include/sys/_sigdefs.h
    stdio/glue.h
    stdio/local.h
    stdio/wcio.h
    stdlib/rand48.h
)

# Android 6.0+ functionality
if(__ANDROID_API__ LESS 23)
    set(LIBC_SRCS
        ${LIBC_SRCS}
        clock_getcpuclockid.cpp
        dirent-23.cpp
        lfs64_support.cpp
        locale.cpp
        mempcpy.cpp
        mkfifoat.c
        pty.c
        posix_madvise.cpp
        sigqueue.cpp
        sigtimedwait.cpp
        sigwaitinfo.cpp
        strerror_r.cpp
        sysinfo.cpp
        stdio/flags.c
        stdio/fmemopen.cpp
        stdio/mktemp.c
        stdio/open_memstream.c
        stdio/open_wmemstream.c
        stdio/stdio.cpp
        stdio/stdio_ext-23.cpp
        stdlib/lcong48.c
        wmempcpy.cpp
        __gnu_basename.cpp
    )

    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-arm64/___rt_sigqueueinfo.S
            arch-arm64/__rt_sigtimedwait.S
            arch-arm64/process_vm_readv.S
            arch-arm64/process_vm_writev.S
            arch-arm64/sethostname.S
            arch-arm64/machine/asm.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-arm/___rt_sigqueueinfo.S
            arch-arm/__rt_sigtimedwait.S
            arch-arm/process_vm_readv.S
            arch-arm/process_vm_writev.S
            arch-arm/sethostname.S
            arch-arm/machine/asm.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-mips/___rt_sigqueueinfo.S
            arch-mips/__rt_sigtimedwait.S
            arch-mips/process_vm_readv.S
            arch-mips/process_vm_writev.S
            arch-mips/sethostname.S
            arch-mips/machine/asm.h
            arch-mips/machine/regdef.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips64")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-mips64/___rt_sigqueueinfo.S
            arch-mips64/__rt_sigtimedwait.S
            arch-mips64/process_vm_readv.S
            arch-mips64/process_vm_writev.S
            arch-mips64/sethostname.S
            arch-mips64/machine/asm.h
            arch-mips64/machine/regdef.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-x86/___rt_sigqueueinfo.S
            arch-x86/__rt_sigtimedwait.S
            arch-x86/process_vm_readv.S
            arch-x86/process_vm_writev.S
            arch-x86/sethostname.S
            arch-x86/machine/asm.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-x86_64/___rt_sigqueueinfo.S
            arch-x86_64/__rt_sigtimedwait.S
            arch-x86_64/process_vm_readv.S
            arch-x86_64/process_vm_writev.S
            arch-x86_64/sethostname.S
            arch-x86_64/machine/asm.h
        )
    else()
        message(FATAL_ERROR "Unsupported CPU Architecture")
    endif()
endif()

# Android 7.0+ functionality
if(__ANDROID_API__ LESS 24)
    set(LIBC_SRCS
        ${LIBC_SRCS}
        bionic_netlink.cpp
        bionic_netlink.h
        lockf.cpp
        ifaddrs.cpp
        net_if.cpp
        pthread_barrier.cpp
        pthread_spinlock.cpp
        scandirat.cpp
        stdio/stdio_ext-24.cpp
        strchrnul.cpp
    )
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-arm64/adjtimex.S
            arch-arm64/clock_adjtime.S
            arch-arm64/preadv.S
            arch-arm64/pwritev.S
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-arm/adjtimex.S
            arch-arm/clock_adjtime.S
            arch-arm/__preadv64.S
            arch-arm/__pwritev64.S
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-mips/adjtimex.S
            arch-mips/clock_adjtime.S
            arch-mips/__preadv64.S
            arch-mips/__pwritev64.S
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips64")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-mips64/adjtimex.S
            arch-mips64/clock_adjtime.S
            arch-mips64/preadv.S
            arch-mips64/pwritev.S
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-x86/adjtimex.S
            arch-x86/clock_adjtime.S
            arch-x86/__preadv64.S
            arch-x86/__pwritev64.S
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-x86_64/adjtimex.S
            arch-x86_64/clock_adjtime.S
            arch-x86_64/preadv.S
            arch-x86_64/pwritev.S
        )
    else()
        message(FATAL_ERROR "Unsupported CPU Architecture")
    endif()
endif()

# Android 10+ functionality
if(__ANDROID_API__ LESS 29)
    set(LIBC_SRCS
        ${LIBC_SRCS}
        reallocarray.c
    )
endif()

add_library(libc OBJECT ${LIBC_SRCS})
target_include_directories(libc PRIVATE include)
target_include_directories(libc PRIVATE include/private)
list(APPEND LIBANDROID_SUPPORT_OBJECTS $<TARGET_OBJECTS:libc>)
set(LIBANDROID_SUPPORT_OBJECTS ${LIBANDROID_SUPPORT_OBJECTS} PARENT_SCOPE)

# Include dir (architecture specific)
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
    target_include_directories(libc PRIVATE arch-arm64)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
    target_include_directories(libc PRIVATE arch-arm)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips")
    target_include_directories(libc PRIVATE arch-mips)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips64")
    target_include_directories(libc PRIVATE arch-mips64)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386")
    target_include_directories(libc PRIVATE arch-x86)
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    target_include_directories(libc PRIVATE arch-x86_64)
endif()
