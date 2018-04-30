/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2018 Roman Vaivod
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

#include "androidtoolbox.h"


#include <stdlib.h> // free
#include <utils/String8.h>
#include <utils/Tokenizer.h>

using namespace android;

bool AndroidToolBox::parseFileToTokens(const char * filename, const char * delims,
                                       std::set<std::string> & tokens) const
{
  Tokenizer* tokenizer = NULL;
  if (Tokenizer::open(String8(filename), &tokenizer) != NO_ERROR) {
      return false;
  }
  while (!tokenizer->isEol()) {
      String8 token = tokenizer->nextToken(delims);
      if (token.isEmpty()) {
          tokenizer->skipDelimiters(delims);
          continue;
      }
      tokens.insert(token.string());
  }
  delete tokenizer;
  return true;
}

void AndroidToolBox::parseToTokens(const char * delimseplist, const char * delims,
                                   std::set<std::string> & tokens) const
{
  char* delimseplist_dup = strdup(delimseplist);
  char* token = strtok(delimseplist_dup, delims);
  while (token) {
      tokens.insert(token);
      token = strtok(NULL, delims);
  }
  free(delimseplist_dup);
}