/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <string.h>
#include <locale.h>
#include <wchar.h>

#include "private/bionic_macros.h"

static bool __bionic_current_locale_is_utf8 = true;

struct __locale_t {
  size_t mb_cur_max;

  __locale_t(size_t mb_cur_max) : mb_cur_max(mb_cur_max) {
  }

  __locale_t(const __locale_t* other) {
    if (other == LC_GLOBAL_LOCALE) {
      mb_cur_max = __bionic_current_locale_is_utf8 ? 4 : 1;
    } else {
      mb_cur_max = other->mb_cur_max;
    }
  }

  DISALLOW_COPY_AND_ASSIGN(__locale_t);
};

extern "C" {

char* strerror_l(int error, locale_t) {
  return strerror(error);
}

int strncasecmp_l(const char* s1, const char* s2, size_t n, locale_t) {
  return strncasecmp(s1, s2, n);
}

int wcscasecmp_l(const wchar_t* ws1, const wchar_t* ws2, locale_t) {
  return wcscasecmp(ws1, ws2);
}

int wcsncasecmp_l(const wchar_t* ws1, const wchar_t* ws2, size_t n, locale_t) {
  return wcsncasecmp(ws1, ws2, n);
}

}
