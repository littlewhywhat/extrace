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
#include "sleepaction.h"
#include "startaction.h"
#include "streamaction.h"
#include "dumpaction.h"
#include "stopaction.h"
#include "cleanupaction.h"
#include "showhelpaction.h"
#include "memorysampleaction.h"
#include "interpretdumpfileaction.h"

ExtraceActionsRunnerBuilder::~ExtraceActionsRunnerBuilder() {
  delete m_EnvironmentBuilder;
}

void ExtraceActionsRunnerBuilder::addStopActions(ActionsRunner & actionsRunner,
                                            const Wire & wire,
                                            const ExtraceArguments & extraceArguments,
                                            shared_ptr<Environment> & environment) const {
  actionsRunner.addAction(new StopAction(wire, environment));
  actionsRunner.addAction(DumpAction::Builder()
                             .buildFrom(wire, environment, extraceArguments));
  actionsRunner.addAction(new CleanUpAction(wire, environment));
  if (extraceArguments.hasOutputFilename() && !extraceArguments.compressionEnabled()
      && extraceArguments.hasPIDs()) {
    actionsRunner.addAction(new InterpretDumpFileAction(wire, environment, extraceArguments.getOutputFilename(),
                                                        extraceArguments.getPIDs()));
  }
}

ActionsRunner * ExtraceActionsRunnerBuilder::build(const Wire & wire,
                                                   Signal & signal,
                                                   const ExtraceArguments & extraceArguments) const{
  auto environment = shared_ptr<Environment>(m_EnvironmentBuilder->build(wire, extraceArguments));  
  auto * actionsRunner = new ActionsRunner(wire);
  if (extraceArguments.hasHelpMessage()) {
    actionsRunner->addAction(new ShowHelpAction(wire, extraceArguments.getAppName(),
                                                extraceArguments.getHelpMessage()));
    return actionsRunner;
  }
  if (extraceArguments.listCategoriesEnabled()) {
    actionsRunner->addAction(new ListSupportedCategories(wire, environment));
    return actionsRunner;
  } 
  if (extraceArguments.ignoreSignalsEnabled()) {
    signal.turnOff();
  }
  if (extraceArguments.coreServicesEnabled()) {
    actionsRunner->addAction(new AddAndroidCoreToTrace(wire, environment));
  }
  if (extraceArguments.hasInitSleepDuration()) {
    actionsRunner->addAction(new SleepAction(wire, signal,
                                             extraceArguments.getInitSleepDuration()));
  }
  if (extraceArguments.asyncStartEnabled()) {
    actionsRunner->addAction(new StartAction(wire, environment));
    if (extraceArguments.streamEnabled()) {
      actionsRunner->addAction(new StreamAction(wire, environment, signal));
      signal.turnOn();
    }
  }
  else if (extraceArguments.asyncStopEnabled()) {
    addStopActions(*actionsRunner, wire, extraceArguments, environment);
  }
  else if (extraceArguments.asyncDumpEnabled()) {
    actionsRunner->addAction(DumpAction::Builder()
                               .buildFrom(wire, environment, extraceArguments));
    if (extraceArguments.hasOutputFilename() && !extraceArguments.compressionEnabled()
        && extraceArguments.hasPIDs()) {
      actionsRunner->addAction(new InterpretDumpFileAction(wire, environment,
                                   extraceArguments.getOutputFilename(), extraceArguments.getPIDs()));
    }
  }
  else {
    actionsRunner->addAction(new StartAction(wire, environment));
    actionsRunner->addAction(new SleepAction(wire, signal,
                                           extraceArguments.getInitSleepDuration()));
    if (extraceArguments.streamEnabled()) {
      actionsRunner->addAction(new StreamAction(wire, environment, signal));
      signal.turnOn();
    } 
    else if (extraceArguments.hasPeriod() && extraceArguments.hasTimes()
             && extraceArguments.hasPIDs()) {
      actionsRunner->addAction(new MemorySampleAction(wire, environment,
                                                      signal,
                                                      extraceArguments.getPeriod(),
                                                      extraceArguments.getTimes(),
                                                      extraceArguments.getPIDs()));
    }
    else {
      actionsRunner->addAction(new SleepAction(wire, signal,
                                           extraceArguments.getMidSleepDuration()));
    }
    addStopActions(*actionsRunner, wire, extraceArguments, environment);
  }
  return actionsRunner;
}
