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

#include "extraceactionsrunnerbuilder.h"

#include <memory>

#include "environment.h"

#include "listsupportedcategories.h"
#include "addandroidcoretotrace.h"
#include "addkernelcategoriesfromfiletotrace.h"
#include "sleepaction.h"
#include "startaction.h"
#include "streamaction.h"
#include "dumpaction.h"
#include "stopaction.h"
#include "cleanupaction.h"
#include "showhelpaction.h"

ExtraceActionsRunnerBuilder::~ExtraceActionsRunnerBuilder() {
  delete m_TraceSystemBuilder;
}

ActionsRunner * ExtraceActionsRunnerBuilder::build(const Wire & wire,
                                                   Signal & signal,
                                                   const ExtraceArguments & extraceArguments) const{
  auto traceSystem = shared_ptr<TraceSystem>(m_TraceSystemBuilder->build(wire, extraceArguments));
  auto environment = shared_ptr<Environment>(new Environment(extraceArguments.getAppName()));  
  auto * actionsRunner = new ActionsRunner(wire);
  if (extraceArguments.hasHelpMessage()) {
    actionsRunner->addAction(new ShowHelpAction(wire, environment,
                                                extraceArguments.getHelpMessage()));
    return actionsRunner;
  }
  if (extraceArguments.listCategoriesEnabled()) {
    actionsRunner->addAction(new ListSupportedCategories(wire, traceSystem));
    return actionsRunner;
  } 
  if (extraceArguments.ignoreSignalsEnabled()) {
    signal.turnOff();
  }
  if (extraceArguments.coreServicesEnabled()) {
    actionsRunner->addAction(new AddAndroidCoreToTrace(wire, traceSystem));
  }
  if (extraceArguments.hasKernelCategoryFilename()) {
    actionsRunner->addAction(new AddKernelCategoriesFromFileToTrace(wire, traceSystem, 
                                             extraceArguments.getKernelCategoryFilename()));
  }
  if (extraceArguments.hasInitSleepDuration()) {
    actionsRunner->addAction(new SleepAction(wire, signal,
                                             extraceArguments.getInitSleepDuration()));
  }
  if (extraceArguments.asyncStartEnabled()) {
    actionsRunner->addAction(new StartAction(wire, traceSystem));
    if (extraceArguments.streamEnabled()) {
      actionsRunner->addAction(new StreamAction(wire, traceSystem, signal));
      signal.turnOn();
    }
  }
  else if (extraceArguments.asyncStopEnabled()) {
    actionsRunner->addAction(new StopAction(wire, traceSystem));
    actionsRunner->addAction(DumpAction::Builder()
                               .buildFrom(wire, traceSystem, extraceArguments));
    actionsRunner->addAction(new CleanUpAction(wire, traceSystem));
  }
  else if (extraceArguments.asyncDumpEnabled()) {
    actionsRunner->addAction(DumpAction::Builder()
                               .buildFrom(wire, traceSystem, extraceArguments));
  }
  else {
    actionsRunner->addAction(new StartAction(wire, traceSystem));
    actionsRunner->addAction(new SleepAction(wire, signal,
                                           extraceArguments.getInitSleepDuration()));
    if (extraceArguments.streamEnabled()) {
      actionsRunner->addAction(new StreamAction(wire, traceSystem, signal));
      signal.turnOn();
    }
    actionsRunner->addAction(new SleepAction(wire, signal,
                                           extraceArguments.getMidSleepDuration()));
    actionsRunner->addAction(new StopAction(wire, traceSystem));
    actionsRunner->addAction(DumpAction::Builder()
                               .buildFrom(wire, traceSystem, extraceArguments));
    actionsRunner->addAction(new CleanUpAction(wire, traceSystem));
  }
  return actionsRunner;
}
