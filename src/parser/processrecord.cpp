#include "processrecord.h"

ProcessRecord::ProcessRecord(const Process * process) {
  myVss   = process->getVSS();
  myRss   = process->getRSS();
  myPss   = process->getPSS();
  myUss   = process->getUSS();
  myTimeStamp = process->getTotalRun();
  myState = process->getState();

}