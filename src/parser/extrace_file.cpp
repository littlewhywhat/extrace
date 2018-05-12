#include "processrecordfile.h"
#include "simpleprocesschangefile.h"

#include <string>

#include <iostream>

using namespace std;

int main(int argc, char ** argv) {
  if (argc != 2) {
    printf("specify only filename\n");
    return 1;
  }
  SimpleProcessRecordFile file(string(argv[1]), new SimpleProcessChangeFileCreator());
  vector<ProcessRecord*> records;
  file.parseTo(records);
  for (auto * record : records) {
    cout << *record << endl;
  }
  return 0;
}