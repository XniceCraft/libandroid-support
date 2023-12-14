/*	$OpenBSD: local.h,v 1.12 2005/10/10 17:37:44 espie Exp $	*/
/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * Information local to this implementation of stdio,
 * in particular, macros and private variables.
 */

#ifndef __BIONIC_STDIO_LOCAL_H__
#define __BIONIC_STDIO_LOCAL_H__

#include <pthread.h>
#include <stdbool.h>
#include <wchar.h>
#include "wcio.h"

__BEGIN_DECLS

struct __sfileext {
  // ungetc buffer.
  struct __sbuf _ub;

  // Wide char io status.
  struct wchar_io_data _wcio;

  // File lock.
  pthread_mutex_t _lock;

  // __fsetlocking support.
  bool _caller_handles_locking;

  // Equivalent to `_seek` but for _FILE_OFFSET_BITS=64.
  // Callers should use this but fall back to `__sFILE::_seek`.
  off64_t (*_seek64)(void*, off64_t, int);
};

#if defined(__cplusplus)
#define _EXT(fp) reinterpret_cast<__sfileext*>((fp)->_ext._base)
#else
#define _EXT(fp) ((struct __sfileext *)((fp)->_ext._base))
#endif

#define _UB(fp) _EXT(fp)->_ub
#define _FLOCK(fp)  _EXT(fp)->_lock

#define _FILEEXT_INIT(fp) \
do { \
	_UB(fp)._base = NULL; \
	_UB(fp)._size = 0; \
	WCIO_INIT(fp); \
	pthread_mutexattr_t attr; \
	pthread_mutexattr_init(&attr); \
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); \
	pthread_mutex_init(&_FLOCK(fp), &attr); \
	pthread_mutexattr_destroy(&attr); \
	_EXT(fp)->_caller_handles_locking = false; \
} while (0)

#define _FILEEXT_SETUP(f, fext) \
do { \
	(f)->_ext._base = (unsigned char *)(fext); \
	_FILEEXT_INIT(f); \
} while (0)

/*
 * Android <= KitKat had getc/putc macros in <stdio.h> that referred
 * to __srget/__swbuf, so those symbols need to be public for LP32
 * but can be hidden for LP64.
 */
__LIBC32_LEGACY_PUBLIC__ int __srget(FILE*);
__LIBC32_LEGACY_PUBLIC__ int __swbuf(int, FILE*);
__LIBC32_LEGACY_PUBLIC__ int __srefill(FILE*);

/* This was referenced by the apportable middleware for LP32. */
__LIBC32_LEGACY_PUBLIC__ int __swsetup(FILE*);

/* These were referenced by a couple of different pieces of middleware and the Crystax NDK. */
__LIBC32_LEGACY_PUBLIC__ int __sflags(const char*, int*);
__LIBC32_LEGACY_PUBLIC__ FILE* __sfp(void);
__LIBC32_LEGACY_PUBLIC__ void __smakebuf(FILE*);

/* These are referenced by the Greed for Glory franchise. */
__LIBC32_LEGACY_PUBLIC__ int __sflush(FILE *);
__LIBC32_LEGACY_PUBLIC__ int __sread(void *, char *, int);
__LIBC32_LEGACY_PUBLIC__ int __swrite(void *, const char *, int);
__LIBC32_LEGACY_PUBLIC__ fpos_t __sseek(void *, fpos_t, int);
__LIBC32_LEGACY_PUBLIC__ int __sclose(void *);
__LIBC32_LEGACY_PUBLIC__ int _fwalk(int (*)(FILE *));

#pragma GCC visibility push(hidden)

off64_t __sseek64(void*, off64_t, int);
int	__sflush_locked(FILE *);
int	__swhatbuf(FILE *, size_t *, int *);
wint_t __fgetwc_unlock(FILE *);
wint_t	__ungetwc(wint_t, FILE *);
int	__vfprintf(FILE *, const char *, __va_list);
int	__svfscanf(FILE * __restrict, const char * __restrict, __va_list);
int	__vfwprintf(FILE * __restrict, const wchar_t * __restrict, __va_list);
int	__vfwscanf(FILE * __restrict, const wchar_t * __restrict, __va_list);

/*
 * Return true if the given FILE cannot be written now.
 */
#define	cantwrite(fp) \
	((((fp)->_flags & __SWR) == 0 || (fp)->_bf._base == NULL) && \
	 __swsetup(fp))
/*
 * Test whether the given stdio file has an active ungetc buffer;
 * release such a buffer, without restoring ordinary unread data.
 */
#define	HASUB(fp) (_UB(fp)._base != NULL)
#define	FREEUB(fp) { \
	if (_UB(fp)._base != (fp)->_ubuf) \
		free(_UB(fp)._base); \
	_UB(fp)._base = NULL; \
}
/*
 * test for an fgetln() buffer.
 */
#define	HASLB(fp) ((fp)->_lb._base != NULL)
#define	FREELB(fp) { \
	free((char *)(fp)->_lb._base); \
	(fp)->_lb._base = NULL; \
}

#define FLOCKFILE(fp)   if (!_EXT(fp)->_caller_handles_locking) flockfile(fp)
#define FUNLOCKFILE(fp) if (!_EXT(fp)->_caller_handles_locking) funlockfile(fp)

#define FLOATING_POINT
#define PRINTF_WIDE_CHAR
#define SCANF_WIDE_CHAR
#define NO_PRINTF_PERCENT_N

/* OpenBSD exposes these in <stdio.h>, but we only want them exposed to the implementation. */
#define __sfeof(p)     (((p)->_flags & __SEOF) != 0)
#define __sferror(p)   (((p)->_flags & __SERR) != 0)
#define __sclearerr(p) ((void)((p)->_flags &= ~(__SERR|__SEOF)))
#if !defined(__cplusplus)
#define __sgetc(p) (--(p)->_r < 0 ? __srget(p) : (int)(*(p)->_p++))
static __inline int __sputc(int _c, FILE* _p) {
  if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n')) {
    return (*_p->_p++ = _c);
  } else {
    return (__swbuf(_c, _p));
  }
}
#endif

#pragma GCC visibility pop

__END_DECLS

#endif
