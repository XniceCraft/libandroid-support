/*	$OpenBSD: findfp.c,v 1.15 2013/12/17 16:33:27 deraadt Exp $ */
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

#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>

#include "local.h"
#include "glue.h"
#include "private/thread_private.h"

#include <android/api-level.h>

#define ALIGNBYTES (sizeof(uintptr_t) - 1)
#define ALIGN(p) (((uintptr_t)(p) + ALIGNBYTES) &~ ALIGNBYTES)

#define	NDYNAMIC 10

_THREAD_PRIVATE_MUTEX(__sfp_mutex);

// TODO: when we no longer have to support both clang and GCC, we can simplify all this.
#define SBUF_INIT {0,0}
#if defined(__LP64__)
#define MBSTATE_T_INIT {{0},{0}}
#else
#define MBSTATE_T_INIT {{0}}
#endif
#define WCHAR_IO_DATA_INIT {MBSTATE_T_INIT,MBSTATE_T_INIT,{0},0,0}

struct glue __sglue = { NULL, 3, __sF };
static struct glue* lastglue = &__sglue;

static glue* moreglue(int n) {
  static FILE empty;

  char* data = new char[sizeof(glue) + ALIGNBYTES + n * sizeof(FILE) + n * sizeof(__sfileext)];
  if (data == nullptr) return nullptr;

  glue* g = reinterpret_cast<glue*>(data);
  FILE* p = reinterpret_cast<FILE*>(ALIGN(data + sizeof(*g)));
  __sfileext* pext = reinterpret_cast<__sfileext*>(ALIGN(data + sizeof(*g)) + n * sizeof(FILE));
  g->next = NULL;
  g->niobs = n;
  g->iobs = p;
  while (--n >= 0) {
    *p = empty;
    _FILEEXT_SETUP(p, pext);
    p++;
    pext++;
  }
  return g;
}


/*
 * Find a free FILE for fopen et al.
 */
FILE* __sfp(void) {
	FILE *fp;
	int n;
	struct glue *g;
	_THREAD_PRIVATE_MUTEX_LOCK(__sfp_mutex);
	for (g = &__sglue; g != NULL; g = g->next) {
		for (fp = g->iobs, n = g->niobs; --n >= 0; fp++)
			if (fp->_flags == 0)
				goto found;
	}
	/* release lock while mallocing */
	_THREAD_PRIVATE_MUTEX_UNLOCK(__sfp_mutex);
	if ((g = moreglue(NDYNAMIC)) == NULL)
		return (NULL);
	_THREAD_PRIVATE_MUTEX_LOCK(__sfp_mutex);
	lastglue->next = g;
	lastglue = g;
	fp = g->iobs;
found:
	fp->_flags = 1;		/* reserve this slot; caller sets real flags */
	_THREAD_PRIVATE_MUTEX_UNLOCK(__sfp_mutex);
	fp->_p = NULL;		/* no current pointer */
	fp->_w = 0;		/* nothing to read or write */
	fp->_r = 0;
	fp->_bf._base = NULL;	/* no buffer */
	fp->_bf._size = 0;
	fp->_lbfsize = 0;	/* not line buffered */
	fp->_file = -1;		/* no file */
	fp->_lb._base = NULL;	/* no line buffer */
	fp->_lb._size = 0;
	_FILEEXT_INIT(fp);
	// Caller sets cookie, _read/_write etc.
	// We explicitly clear _seek and _seek64 to prevent subtle bugs.
	fp->_seek = nullptr;
	_EXT(fp)->_seek64 = nullptr;
	return fp;
}
