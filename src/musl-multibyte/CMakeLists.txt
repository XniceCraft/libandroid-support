file(GLOB_RECURSE MUSL_MULTIBYTE_SRCS "*.c" "*.h")

add_library(musl-multibyte OBJECT ${MUSL_MULTIBYTE_SRCS})
list(APPEND LIBANDROID_SUPPORT_OBJECTS $<TARGET_OBJECTS:musl-multibyte>)
set(LIBANDROID_SUPPORT_OBJECTS ${LIBANDROID_SUPPORT_OBJECTS} PARENT_SCOPE)
set(musl-multibyte PROPERTIES COMPILE_FLAGS " -std=c99 -DNULL=0 ")
