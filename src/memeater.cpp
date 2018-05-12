#include <time.h> // nanosleep, struct timespec
#include <errno.h>

#include <unistd.h> // sleep

#define size 100000
#define period 10000000

bool signaled = false;

void handleSignal(int /*signo*/)
{
  signaled = true;
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

int main(void) {
  char ** memory = NULL;
  sleep(5);
  memory = new char*[size];
  for (int i = 0; i < size; i++) {
    delete memory[i];
  }

  struct timespec timeLeft;
  timeLeft.tv_sec = 0;
  timeLeft.tv_nsec = period;
  registerSignalHandler();
  for (int i = 0; i < size; i++) {
    do {
      memory[i] = new char[size];
      if (signaled) {
        return 1;
      }
    } while (nanosleep(&timeLeft, &timeLeft) == -1 && errno == EINTR);
  }
  for (int i = 0; i < size; i++) {
    delete memory[i];
  }
  delete memory;
  return 0;
}