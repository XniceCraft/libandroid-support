include(CheckSymbolExists)

set(LIBC_SRCS
    lockf.cpp
    error.cpp
    getgrnam_r.c
    ifaddrs.cpp
    include/LocalArray.h
    include/ScopedFd.h
    include/bionic_asm.h
    include/fileext.h
    include/local.h
    include/wcio.h
    stdio_ext.cpp
    strchrnul.cpp
    __set_errno.cpp
)

check_symbol_exists(sigqueue signal.h HAVE_SIGQUEUE)

if(NOT ${HAVE_SIGQUEUE})
    set(LIBC_SRCS
        ${LIBC_SRCS}
        sigqueue.cpp
    )
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-arm64/___rt_sigqueueinfo.S
            arch-arm64/machine/asm.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-arm/___rt_sigqueueinfo.S
            arch-arm/machine/asm.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-mips/___rt_sigqueueinfo.S
            arch-mips/machine/asm.h
            arch-mips/machine/regdef.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "mips64")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-mips64/___rt_sigqueueinfo.S
            arch-mips64/machine/asm.h
            arch-mips64/machine/regdef.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "i386")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-x86/___rt_sigqueueinfo.S
            arch-x86/machine/asm.h
        )
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(LIBC_SRCS
            ${LIBC_SRCS}
            arch-x86_64/___rt_sigqueueinfo.S
            arch-x86_64/machine/asm.h
        )
    else()
        message(FATAL_ERROR "Unsupported CPU Architecture")
    endif()
endif()

add_library(libc OBJECT ${LIBC_SRCS})
list(APPEND LIBANDROID_SUPPORT_OBJECTS $<TARGET_OBJECTS:libc>)
set(LIBANDROID_SUPPORT_OBJECTS ${LIBANDROID_SUPPORT_OBJECTS} PARENT_SCOPE)

