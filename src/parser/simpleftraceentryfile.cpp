/*
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

#include "simpleftraceentryfile.h"

SimpleFTraceEntryFile::~SimpleFTraceEntryFile() {
  delete myEntryCreator;
}

void SimpleFTraceEntryFile::parseTo(vector<FTraceEntry*> & entries) const {
  FILE * file = fopen(myFilename.c_str(), "r+");
  if (!file) {
    fprintf(stderr, "error opening %s: %s (%d)\n", myFilename.c_str(),
              strerror(errno), errno);
  }
  char line[256];
  char name[17];
  int pid;
  char tgid[6];
  char skip1[4];
  char skip2[5];
  int timeHigh;
  int timeLow; 
  char entryName[64];
  char content[128];
  int readArgs, readChars;
  while (fgets(line, 256, file)) {
    readArgs = sscanf(line, "%16[^-]-%5d (%5[^)]) [%3s] %4s %5d.%6d: %[^:]: %n",
                name, &pid, tgid, skip1, skip2, &timeHigh, &timeLow,
                entryName, &readChars);
    if (readArgs != 8) {
      fprintf(stderr, "error format read %d\n", readArgs);
      continue;
    }
    else {
      int couldWrite = snprintf(content, 128,"%s", line + readChars);
      if (couldWrite >= 128) { 
        fprintf(stderr, "error didn't read %d characters from end\n", couldWrite - 128 + 1);
        continue;
      }
      content[couldWrite-1] = '\0';
      FTraceEntry * entry = myEntryCreator->create(pid, timeLow, timeHigh, entryName, content);
      if (!entry) {
        fprintf(stderr, "error creating entry - wrong content?\n");
        continue;
      }
      entries.push_back(entry);
    }
  }
  fclose(file);
}
