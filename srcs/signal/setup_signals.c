#include "minibash.h"

void sighandler(int signo) {
    (void)signo;

    if (RL_ISSTATE(RL_STATE_READCMD)) {
        ioctl(1, TIOCSTI, "\n");
        g_status_code = 1;
    } else
        write(1, "\n", 1);

    rl_on_new_line();
    rl_replace_line("", 0);
}

/*
 * @brief Sets up the signals
 * @return 0 on success, 1 on failure
 **/
int setup_signals(void) {
    struct sigaction act;

    act.sa_handler = &sighandler;
    if (sigfillset(&act.sa_mask))
        return 1;
    act.sa_flags = 0;

    if (sigaction(SIGINT, &act, NULL) == -1)
        return 1;

    act.sa_handler = SIG_IGN;

    if (sigaction(SIGQUIT, &act, NULL) == -1)
        return 1;

    return 0;
}
