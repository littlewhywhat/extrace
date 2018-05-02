#include "cmdlineapp.h"
#include "cmdlineargs.h"
#include "signal.h"
#include "extraceactioncmdlinebuilder.h"
#include "extraceargumentsbuilder.h"
#include "tracesystembuilder.h"
#include "androidsystembuilder.h"
#include "kernelsystembuilder.h"
#include "tracebuilder.h"
#include "traceactionsrunnerbuilder.h"

#include <signal.h>

using namespace std;

CmdLineApp cmdLineApp;

void handleSignal(int /*signo*/)
{
  cmdLineApp.handleSignal();
}

void registerSignalHandler()
{
  struct sigaction sa;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handleSignal;
  sigaction(SIGHUP, &sa, NULL);
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);
  sigaction(SIGTERM, &sa, NULL);
}

int main(int argc, const char ** argv) {
  cmdLineApp.setArgs(new CmdLineArgs(argc, argv));
  cmdLineApp.setActionCmdLineBuilder(new ExtraceActionCmdLineBuilder(
                                         new ExtraceArgumentsBuilder(),
                                         new TraceActionsRunnerBuilder(
                                             new TraceSystemBuilder(
                                                 new KernelSystemBuilder(),
                                                 new AndroidSystemBuilder(),
                                                 new TraceBuilder()
                                             )
                                         )
                                    ));
  registerSignalHandler();
  return cmdLineApp.run();
}
