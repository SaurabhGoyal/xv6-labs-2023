#include "kernel/types.h"
#include "user/user.h"

// read from in_pipe, write ACK to STDOUT and message to out_pipe.
void _recv_and_rebound(int in_pipe[], int out_pipe[], char *rebound_msg) {
    char recv_msg[100];
    read(in_pipe[0], &recv_msg, 4);
    fprintf(1, "Pid: %d, Msg: %s\n", getpid(), recv_msg);
    sleep(1);
    write(out_pipe[1], rebound_msg, strlen(rebound_msg));
}

// Runs an infinite loop of ping-pong. Two achieve actual ping pong, it uses two pipes.
// The STDOUT message is currently not thread-safe, i.e. both processes keep writing to STDOUT together.
// $ pingpong
// Pid: 4, Msg: ping
// Pid: 3, Msg: pong
// Pid: 4, Msg: ping
// PPiidd::  43,,  MMssgg::  ppoinngg

// PPiidd::  43,,  MMssgg::  ppinog
// ng
// PPiid:d 3:,  4M,s gM:sg :p opnign
// g
// PiPidd: :4,  M3s,g :M spgi:n gp
// ong
// PPiidd:: 4 ,3 ,M sMgs:g :p inpgo
// ng
// PPiidd::  43,,  MMssgg:: p onpgi
// ng
// PPiidd:: 4 ,3 ,M sMgs:g :p ipngo
// ng
// PPiidd::  43,,  MMssgg::  ppionngg
int loop_impl(int argc, char *args[]) {
    int p1[2], p2[2];
    pipe(p1);
    pipe(p2);
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Some error in fork\n");
        exit(0);
    }
    for(int i = 0;;) {
        if (pid == 0) {
            // child
            _recv_and_rebound(p1, p2, "pong");
        } else {
            // parent
            if(i==0){
              write(p1[1], "ping", strlen("ping"));
            }
            _recv_and_rebound(p2, p1, "ping");
        }
    }
    exit(1);
}

// Runs a single instance of ping-pong. Uses one pipe to communicate for a single message.
// $ pingpong
// Pid: 4, Msg: ping
// Pid: 3, Msg: pong
int single_impl(int argc, char *args[]) {
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Some error in fork\n");
        exit(0);
    }
    char buf[8];
    if (pid == 0) {
        // child
        fprintf(1, "Pid: %d, Msg: %s\n", getpid(), "ping");
        write(p[1], "pong", 4);
    } else {
        // parent
        wait(0);
        read(p[0], buf, 4);
        fprintf(1, "Pid: %d, Msg: %s\n", getpid(), buf);
    }
    exit(1);
}


int main(int argc, char *args[]) {
    return single_impl(argc, args);
    // return loop_impl(argc, args);
}
