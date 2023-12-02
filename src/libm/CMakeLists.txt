include(CheckSymbolExists)

set(LIBM_SRCS
    cephes_subrl.c
    cephes_subrl.h
    fpmath.h
    invtrig.h
    invtrig.c
    math_private.h
)

check_symbol_exists(cacos complex.h HAVE_CACOS)
check_symbol_exists(clog complex.h HAVE_CLOG)
check_symbol_exists(cabsl complex.h HAVE_CABSL)

if(NOT HAVE_CACOS)
    set(LIBM_SRCS
        ${LIBM_SRCS}
        catrig.c
        catrigf.c
        s_ccosh.c
        s_ccoshf.c
        s_csinh.c
        s_csinhf.c
        s_ctanh.c
        s_ctanhf.c
        k_exp.c
        s_cexp.c
        s_cexpf.c
        w_cabs.c
        w_cabsf.c
        s_csqrt.c
        s_csqrtf.c
        s_carg.c
        s_cargf.c
        s_cargl.c
        s_cimag.c
        s_cimagf.c
        s_cimagl.c
        s_conj.c
        s_conjf.c
        s_conjl.c
        s_cproj.c
        s_cprojf.c
        s_creal.c
        s_crealf.c
        s_creall.c
    )
endif()

if(NOT HAVE_CLOG)
     set(LIBM_SRCS
        ${LIBM_SRCS}
        catrigl.c
        ccosl.c
        csinhl.c
        ctanhl.c
        cexpl.c
        s_clog.c
        s_clogf.c
        s_clogl.c
        s_cpow.c
        s_cpowl.c
        s_cpowf.c
    )
endif()

if(NOT HAVE_CABSL)
     set(LIBM_SRCS
        ${LIBM_SRCS}
        w_cabsl.c
        s_csqrtl.c
        s_cprojl.c
    )
endif()

add_library(libm OBJECT ${LIBM_SRCS})
list(APPEND LIBANDROID_SUPPORT_OBJECTS $<TARGET_OBJECTS:libm>)
set(LIBANDROID_SUPPORT_OBJECTS ${LIBANDROID_SUPPORT_OBJECTS} PARENT_SCOPE)
target_link_libraries(libm m)
