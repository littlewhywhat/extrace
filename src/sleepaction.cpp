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
#include "sleepaction.h"

#include <time.h> // nanosleep, struct timespec
#include <errno.h>

void SleepAction::setDurationSeconds(uint32_t durationSeconds) {
  m_DurationSeconds = durationSeconds;
}

void SleepAction::setErrorStream(FILE * errorStream) {
  m_ErrorStream = errorStream;
}

bool SleepAction::tryRun() {
  bool ok = true;
  struct timespec timeLeft;
  timeLeft.tv_sec = m_DurationSeconds;
  timeLeft.tv_nsec = 0;
  do {
    if (m_Interrupted) {
      ok = false;
      break;
    }
  } while (nanosleep(&timeLeft, &timeLeft) == -1 && errno == EINTR);
  if (!ok) {
    fprintf(m_ErrorStream, "error SleepAction::tryRun - sleep aborted\n");
  }
  return ok;
}

InterruptableAction * SleepAction::InitSleepBuilder::buildFrom(const ExtraceArguments & arguments) const {
  auto * sleepAction = new SleepAction();
  if (arguments.specifyInitSleepDuration()) {
    sleepAction->setDurationSeconds(arguments.getInitSleepDuration());
  }
  return sleepAction;
}

InterruptableAction * SleepAction::MidSleepBuilder::buildFrom(const ExtraceArguments & arguments) const {
  auto * sleepAction = new SleepAction();
  if (arguments.specifyMidSleepDuration()) {
    sleepAction->setDurationSeconds(arguments.getMidSleepDuration());
  }
  return sleepAction;
}