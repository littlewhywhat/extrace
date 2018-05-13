#include "cmdlineapp.h"
#include "cmdlineargs.h"
#include "signal.h"
#include "extraceactioncmdlinebuilder.h"
#include "extraceactionsrunnerbuilder.h"
#include "extraceargumentsbuilder.h"
#include "environmentbuilder.h"

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
                                         new ExtraceActionsRunnerBuilder(
                                             new EnvironmentBuilder()
                                         )
                                    ));
  registerSignalHandler();
  return cmdLineApp.run();
}
