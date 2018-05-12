#include "process.h"

void Process::updateTo(long time) {
  if (myState == ProcessState::RUNNING) {
    myTotalRun += time - myLastUpdateTime;
  }
  myLastUpdateTime = time;
}