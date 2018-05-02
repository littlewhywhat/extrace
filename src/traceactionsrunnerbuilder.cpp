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

#include "traceactionsrunnerbuilder.h"

#include "listsupportedcategories.h"
#include "addandroidcoretotrace.h"
#include "addkernelcategoriesfromfiletotrace.h"
#include "sleepaction.h"

TraceActionsRunner * TraceActionsRunnerBuilder::build(const Wire & wire, TraceSystem * traceSystem,
                                                      const TraceArguments & traceArguments) const {
  auto * traceActionsRunner = new TraceActionsRunner(wire, traceSystem, traceArguments);

  if (traceArguments.listCategoriesEnabled()) {
    traceActionsRunner->addTraceAction(new ListSupportedCategories(wire));
  } 
  else {
    if (traceArguments.coreServicesEnabled()) {
      traceActionsRunner->addTraceAction(new AddAndroidCoreToTrace(wire));
    }
    if (traceArguments.hasKernelCategoryFilename()) {
      traceActionsRunner->addTraceAction(new AddKernelCategoriesFromFileToTrace(wire, 
                                             traceArguments.getKernelCategoryFilename()));
    }
    if (traceArguments.hasInitSleepDuration()) {
      traceActionsRunner->addTraceAction(new SleepAction(wire, 
                                             traceArguments.getInitSleepDuration()));
    }
    // if (traceArguments.asyncStartEnabled()) {
    //   traceActionsRunner->addTraceAction(new StartAction(wire));
    //   if (traceArguments.streamEnabled()) {
    //     traceActionsRunner->addTraceAction(new StreamAction(wire));
    //   }
    // }
  }
  //   else if (traceArguments.enableAsyncStop()) {
  //     traceActionsRunner->addTraceAction(new StopAction(wire));
  //     traceActionsRunner->addTraceAction(new DumpAction(wire, traceArguments));
  //     traceActionsRunner->addTraceAction(new CleanUpAction(wire));
  //   }
  //   else if (traceArguments.enableAsyncDump()) {
  //     traceActionsRunner->addTraceAction(new DumpAction(wire, traceArguments));
  //   }
  //   else {
  //     traceActionsRunner->addTraceAction(new StartAction(wire));
  //     traceActionsRunner->addTraceAction(SleepAction::createMid(traceArguments));
  //     if (traceArguments.enableStream()) {
  //       traceActionsRunner->addTraceAction(new StreamAction(wire));
  //       traceActionsRunner->enableInterrupts();
  //     }
  //     traceActionsRunner->addTraceAction(new StopAction(wire));
  //     traceActionsRunner->addTraceAction(new DumpAction(wire, traceArguments));
  //     traceActionsRunner->addTraceAction(new CleanUpAction(wire));
  //   }
  // }
  return traceActionsRunner;
}
