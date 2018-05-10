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

#include "tracebuilder.h"

#include "filesystem_impl.h"
#include "trace_impl.h"
#include "androidtoolbox.h"
#include "simplefiledatamaker.h"
#include "compressedfiledatamaker.h"

TraceBuilder::~TraceBuilder() {
  delete m_AndroidTraceSystemBuilder;
}

Trace * TraceBuilder::build(const Wire & wire, const ExtraceArguments & traceArguments) const {
  FileDataMaker * fileDataMaker;
  if (traceArguments.compressionEnabled()) {
    fileDataMaker = new CompressedFileDataMaker();
  }
  else {
    fileDataMaker = new SimpleFileDataMaker();
  }
  FileSystem * fileSystem = new FileSystemImpl(wire);
  auto * ftraceBufferFile = new FTraceBufferFile(wire, new FTrace(wire, new FileSystemImpl(wire), new AndroidToolBox()),
                                                 fileDataMaker);
  auto * androidSystem = m_AndroidTraceSystemBuilder->build(wire);
  auto * kernelTraceSystem = new KernelTraceSystem(wire, new FTrace(wire, new FileSystemImpl(wire), 
                                                         new AndroidToolBox()));
  auto * traceImpl = new TraceImpl(wire, androidSystem, new FTrace(wire, new FileSystemImpl(wire), 
                                                         new AndroidToolBox()), fileSystem, kernelTraceSystem, ftraceBufferFile);
  if (traceArguments.circleBufferEnabled()) {
    traceImpl->enableTraceOverwrite();
  }
  traceImpl->setTraceBufferSizeKB(traceArguments.getBufferSize());
  if (traceArguments.hasKernelCategories()) {
    for (auto & category : traceArguments.getKernelCategories()) {
      traceImpl->addKernelCategory(category.c_str());
    }
  }
  if (traceArguments.hasAndroidCategories()) {
    for (auto & category: traceArguments.getAndroidCategories()) {
      traceImpl->addAndroidCategory(category.c_str());
    }
  }
  if (traceArguments.hasApps()) {
    for (auto & app: traceArguments.getApps()) {
      traceImpl->addApp(app.c_str());
    }
  }
  if (traceArguments.hasKernelFunctions()) {
    for (auto & function: traceArguments.getKernelFunctions()) {
      traceImpl->addFunction(function.c_str());
    }
  }
  return traceImpl;
};
