#include "minibash.h"

void sighandler(int signo) {
    (void)signo;

    if (RL_ISSTATE(RL_STATE_READCMD)) {
        g_status_code = 1;
        if (ioctl(1, TIOCSTI, "\n") != 0)
            perror("minibash: ioctl");
    } else {
        if (write(1, "\n", 1)) {
          perror("minibash: write");
          return;
        }
    }

    rl_replace_line("", 0);
    rl_on_new_line();
}

/*
 * @brief Sets up the signals
 * @return 0 on success, 1 on failure
 **/
int setup_signals(void) {
    struct sigaction act;

    act.sa_flags = 0;
    if (sigfillset(&act.sa_mask) != 0) {
        perror("minibash: sigfillset");
        return 1;
    }

    act.sa_handler = &sighandler;

    if (sigaction(SIGINT, &act, NULL) != 0) {
        perror("minibash: sigaction");
        return 1;
    }

    act.sa_handler = SIG_IGN;

    if (sigaction(SIGQUIT, &act, NULL) != 0) {
        perror("minibash: sigaction");
        return 1;
    }

    return 0;
}
