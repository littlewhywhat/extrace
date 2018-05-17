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

#include "environmentbuilder.h"

#include "androidtoolbox.h"
#include "filesystem_impl.h"
#include "ftrace.h"
#include "android_impl.h"
#include "compressedfiledatamaker.h"
#include "simplefiledatamaker.h"
#include "ftracebufferfile.h"
#include "kerneltracesystem.h"
#include "simpleandroidtracesystem.h"
#include "trace_impl.h"

Environment * EnvironmentBuilder::build(const Wire & wire, const ExtraceArguments & traceArguments) const {
  auto toolBox = shared_ptr<ToolBox>(new AndroidToolBox());
  auto fileSystem = shared_ptr<FileSystem>(new FileSystemImpl(wire));
  auto ftrace = shared_ptr<FTrace>(new FTrace(wire, fileSystem, toolBox, traceArguments.getMountPoint()));
  auto android = shared_ptr<Android>(new AndroidImpl(wire));
  FileDataMaker * fileDataMaker;
  if (traceArguments.compressionEnabled()) {
    fileDataMaker = new CompressedFileDataMaker();
  }
  else {
    fileDataMaker = new SimpleFileDataMaker();
  }
  auto traceBuffer = shared_ptr<TraceBuffer>(new FTraceBufferFile(wire, ftrace, fileDataMaker));
  auto kernelTraceSystem = shared_ptr<KernelTraceSystem>(new KernelTraceSystem(wire, ftrace));
  for (const auto & category: traceArguments.getKernelCategories()) {
    kernelTraceSystem->rememberToTrace(category);    
  }
  auto androidTraceSystem = shared_ptr<AndroidTraceSystem>(new SimpleAndroidTraceSystem(wire, android, toolBox));
  for (const auto & category: traceArguments.getAndroidCategories()) {
    androidTraceSystem->rememberToTrace(category);    
  }
  auto * trace = new TraceImpl(wire, ftrace, androidTraceSystem, kernelTraceSystem);
  trace->setTraceBufferSizeKB(traceArguments.getBufferSize());
  if (traceArguments.circleBufferEnabled()) {
    trace->enableCircleBuffer();
  }
  return new Environment(shared_ptr<Trace>(trace), kernelTraceSystem, androidTraceSystem, traceBuffer, fileSystem, toolBox);
}