/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdlib.h>

// Delegation will work in these cases because all the transitive dependencies
// are already 64-bit ready. In particular, we don't have non-O_LARGEFILE
// open (our open is actually open64) and stat and stat64 are the same.

extern "C" {

int mkostemp64(char* path, int flags) {
  return mkostemp(path, flags);
}

int mkostemps64(char* path, int suffix_length, int flags) {
  return mkostemps(path, suffix_length, flags);
}

}
