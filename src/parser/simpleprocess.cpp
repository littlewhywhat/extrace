#include "simpleprocess.h"

void SimpleProcess::updateTo(long time) {
  if (myState == ProcessState::RUNNING) {
    myTotalRun += time - myLastUpdateTime;
  }
  myLastUpdateTime = time;
}