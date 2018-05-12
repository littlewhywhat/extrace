#ifndef LTTLWHWHT_SIMPLE_ENTRY_FILE_H
#define LTTLWHWHT_SIMPLE_ENTRY_FILE_H

#include "entryfile.h"

#include <errno.h>  // errno
#include <string.h> // strerror
#include <unistd.h> // access
#include <fcntl.h>  // creat, open
#include <cstdio>

#include <string>

using namespace std;

//! I am an Entry creator. I create it based on name.
class EntryByNameCreator {
  public:
    virtual ~EntryByNameCreator() {}
    virtual Entry * create(int pid, long timeLow, long timeHigh,
                           const char * entryName,
                           const char * content) const = 0;
};

//! idea is to update process stats with entries until stats are ready
//! NOTE some events - like switch could be relevant to two process - switched from and switched to
//! this should create two entries - SwitchFrom and SwitchTo
class SimpleEntryFile : public EntryFile {
  public:
    SimpleEntryFile(const string & filename,
                    const EntryByNameCreator * entryCreator):
                    myFilename(filename), myEntryCreator(entryCreator) {}
    void parseTo(vector<Entry*> & entries) const override;
  private:
    const string & myFilename;
    const EntryByNameCreator * myEntryCreator = NULL;
};

void SimpleEntryFile::parseTo(vector<Entry*> & entries) const {
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
      Entry * entry = myEntryCreator->create(pid, timeLow, timeHigh, entryName, content);
      if (!entry) {
        fprintf(stderr, "error creating entry - wrong content?\n");
        continue;
      }
      entries.push_back(entry);
    }
  }
  fclose(file);
  
}

#endif // LTTLWHWHT_SIMPLE_ENTRY_FILE_H