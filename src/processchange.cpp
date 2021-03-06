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

#include "processchange.h"

void ProcessChange::applyTo(Process & process) const {
  process.updateTo(myTimeStamp);
  applyFurtherTo(process);
}

void MemoryChange::applyFurtherTo(Process & process) const {
  process.setVSS(myVSS)
         ->setRSS(myRSS)
         ->setPSS(myPSS)
         ->setUSS(myUSS);
}

void StateChange::applyFurtherTo(Process & process) const {
  process.setState(myState);
}

void NotificationChange::applyFurtherTo(Process & process) const {
}