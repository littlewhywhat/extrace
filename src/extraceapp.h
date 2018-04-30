/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include "filesystem_impl.h"
#include "systemtime_impl.h"
#include "androidsystem_impl.h"
#include "kernelsystem_impl.h"
#include "androidtoolbox.h"
#include "trace_impl.h"
#include "signal.h"
#include "startaction.h"
#include "stopaction.h"
#include "sleepaction.h"
#include "dumpaction.h"
#include "streamaction.h"
#include "cleanupaction.h"
#include "actionrunner_impl.h"

using namespace std;

class ExtraceApp {
  public:
    ~ExtraceApp();
    void handleSignal();
    bool run(int argc, const char ** argv);

    void setErrorStream(FILE * errorStream);
    void setOutputStream(FILE * outputStream);
    void setAndroidToolBox(AndroidToolBox * androidToolBox);
    void setSystemTimeImpl(SystemTimeImpl * systemTimeImpl);
    void setFileSystemImpl(FileSystemImpl * fileSystemImpl);
    void setKernelSystemImpl(KernelSystemImpl * kernelSystemImpl);
    void setAndroidSystemImpl(AndroidSystemImpl * androidSystemImpl);
    void setTraceImpl(TraceImpl * traceImpl);
    void setInitSleepAction(SleepAction * initSleep);
    void setStartAction(StartAction * startAction);
    void setMidSleepAction(SleepAction * midSleep);
    void setStreamAction(StreamAction * streamAction);
    void setStopAction(StopAction * stopAction);
    void setDumpAction(DumpAction * dumpAction);
    void setCleanUpAction(CleanUpAction * cleanUpAction);
    void setSignal(Signal * signal);
  private:
    bool m_HandleSignals = true;
    FILE * m_ErrorStream;
    FILE * m_OutputStream;
    AndroidToolBox * m_AndroidToolBox;
    SystemTimeImpl * m_SystemTimeImpl;
    FileSystemImpl * m_FileSystemImpl;
    KernelSystemImpl * m_KernelSystemImpl;
    AndroidSystemImpl * m_AndroidSystemImpl;
    TraceImpl * m_TraceImpl;
    SleepAction * m_InitSleep;
    StartAction * m_StartAction;
    SleepAction * m_MidSleep;
    StreamAction * m_StreamAction;
    StopAction * m_StopAction;
    DumpAction * m_DumpAction;
    CleanUpAction * m_CleanUpAction;
    Signal * m_Signal;
    void listSupportedCategories() const;
    // Print the command usage help to errorStream.
    void showHelp(const char *cmd) const;
    void setupDependencies();
    void setupAndroidSystemImpl();
    void setupKernelSystemImpl();
    bool addCoreServicesToTrace();
    
};