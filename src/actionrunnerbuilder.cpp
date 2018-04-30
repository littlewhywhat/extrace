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

#include "actionrunnerbuilder.h"

#include "actionrunner_impl.h"
#include "listsupportedcategories.h"
#include "sleepaction.h"
#include "startaction.h"
#include "addandroidcoretotrace.h"
#include "addkernelcategoriesfromfiletotrace.h"
#include "streamaction.h"
#include "stopaction.h"
#include "dumpaction.h"
#include "cleanupaction.h"

void ActionRunnerBuilder::setSystemCoreBuilder(SystemCore::Builder * systemCoreBuilder)
{
  m_SystemCoreBuilder = systemCoreBuilder;
}

ActionRunner * ActionRunnerBuilder::buildFrom(const ExtraceArguments & arguments) const {
  auto * systemCore       = m_SystemCoreBuilder->build(arguments);
  auto * actionRunnerImpl = new ActionRunnerImpl();
  actionRunnerImpl->setSystemCore(systemCore);
  if (arguments.enableInterrupts()) {
    actionRunnerImpl->enableInterrupts();
  }
  if (arguments.haveListCategoriesOption()) {
    actionRunnerImpl->addAction(ListSupportedCategories::Builder()
                                    .buildFrom(*systemCore));
  } else {
    if (arguments.enableCoreServices()) {
      actionRunnerImpl->addAction(AddAndroidCoreToTrace::Builder()
                                    .buildFrom(*systemCore));
    }
    if (arguments.haveKernelCategoryFilename()) {
      actionRunnerImpl->addAction(AddKernelCategoriesFromFileToTrace::Builder().buildFrom(*systemCore, arguments));
    }
    if (arguments.specifyInitSleepDuration()) {
      actionRunnerImpl->addInterruptableAction(SleepAction::InitSleepBuilder()
                                               .buildFrom(arguments));
    }
    if (arguments.enableAsyncStart()) {
      actionRunnerImpl->addAction(StartAction::Builder()
                                    .buildFrom(*systemCore));
      if (arguments.enableStream()) {
        actionRunnerImpl->addInterruptableAction(StreamAction::Builder()
                                                 .buildFrom(*systemCore));
        actionRunnerImpl->enableInterrupts();
      }
    }
    else if (arguments.enableAsyncStop()) {
      actionRunnerImpl->addAction(StopAction::Builder()
                                    .buildFrom(*systemCore));
      actionRunnerImpl->addAction(DumpAction::Builder()
                                    .buildFrom(*systemCore, arguments));
      actionRunnerImpl->addAction(CleanUpAction::Builder()
                                    .buildFrom(*systemCore));
    }
    else if (arguments.enableAsyncDump()) {
      actionRunnerImpl->addAction(DumpAction::Builder()
                                    .buildFrom(*systemCore, arguments));
    }
    else {
      actionRunnerImpl->addAction(StartAction::Builder()
                                    .buildFrom(*systemCore));
      actionRunnerImpl->addInterruptableAction(SleepAction::MidSleepBuilder()
                                    .buildFrom(arguments));
      if (arguments.enableStream()) {
        actionRunnerImpl->addInterruptableAction(StreamAction::Builder()
                                    .buildFrom(*systemCore));
        actionRunnerImpl->enableInterrupts();
      }
      actionRunnerImpl->addAction(StopAction::Builder()
                                    .buildFrom(*systemCore));
      actionRunnerImpl->addAction(DumpAction::Builder()
                                    .buildFrom(*systemCore, arguments));
      actionRunnerImpl->addAction(CleanUpAction::Builder()
                                    .buildFrom(*systemCore));
    }
  }
  return actionRunnerImpl;
}