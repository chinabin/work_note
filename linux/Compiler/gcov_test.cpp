#include <stdlib.h>
#include <signal.h>

static void sigCtrlcHandle(int sig)
{
    exit(sig);
}

static int setSigHandle(void)
{
    struct sigaction act;

    sigfillset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = sigCtrlcHandle; /* Ctrl C  */
    sigaction(SIGINT, &act, 0);

}

int m = 50200000;
char arr[4] = {'1', '2', '3', 0};

long test() {
  long sum = 0;
  int a = 0;
  for (a = 0; a < m; ++a) {
    sum += atoi(arr+(a%2));
  }
}

int main(int argc, const char *argv[])
{
    setSigHandle();     // 这个调用很关键，哭了
    while (1)
        test();
  return 0;
}