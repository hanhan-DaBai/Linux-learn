#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

volatile sig_atomic_t count = 0; // 计数值
pid_t child_pid; // 子进程的 PID

// 父进程的信号处理函数
void parent_signal_handler(int sig) {
    if (sig == SIGUSR1) {
        count++;
        printf("Parent: %d\n", count);
        kill(child_pid, SIGUSR2); // 发送信号给子进程
    }
}

// 子进程的信号处理函数
void child_signal_handler(int sig) {
    if (sig == SIGUSR2) {
        count++;
        printf("Child: %d\n", count);
        kill(getppid(), SIGUSR1); // 发送信号给父进程
    }
}

int main() {
    struct sigaction sa;

    // 设置信号处理函数
    sa.sa_handler = parent_signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_handler = child_signal_handler;
    sigaction(SIGUSR2, &sa, NULL);

    // 创建子进程
    child_pid = fork();
    if (child_pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // 子进程
        // 子进程等待父进程的信号
        pause(); // 等待父进程的初始信号
        while (1) {
            pause(); // 等待信号到来
        }
        exit(EXIT_SUCCESS);
    } else {
        // 父进程
        // 父进程开始时发送信号给子进程
        kill(child_pid, SIGUSR2);

        while (1) {
            pause(); // 等待信号到来
        }
    }

    return 0;
}

