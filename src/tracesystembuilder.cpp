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

#include "tracesystembuilder.h"

TraceSystem * TraceSystemBuilder::build(const Wire & wire, const ExtraceArguments & traceArguments) const {
  FileSystem * fileSystem = NULL;
  auto * kernelSystem  = m_KernelSystemBuilder->build(wire, fileSystem);
  auto * androidSystem = m_AndroidSystemBuilder->build(wire);
  auto * trace         = m_TraceBuilder->build(wire, kernelSystem, androidSystem,
                                               traceArguments);
  return new TraceSystem(fileSystem, kernelSystem, androidSystem, trace);
};
