#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

static void *ctx, *s;
static char q_open, r_open;
static zmq_msg_t query, result;

void cleanup() {
    if (q_open) zmq_msg_close(&query);
    if (r_open) zmq_msg_close(&result);
    zmq_close(s);
    zmq_term(ctx);
}

void check(int rc, char *msg) {
    if (rc < 0) {
        printf("error: %s\n", msg);
        cleanup();
        exit(-1);
    }
}

int main(int argc, char *argv[]) {

    q_open = r_open = 0;

    if (argc != 2) {
        printf("usage: %s <message>\n", argv[0]);
        exit(-1);
    }

    ctx = zmq_init(1);
    if (ctx < 0) {
        printf("could not create context\n");
        exit(-1);
    }

    s = zmq_socket(ctx, ZMQ_REQ);
    check((long)s,"could not initialize socket");

    printf("connecting...\n");
    check(zmq_connect(s, "tcp://127.0.0.1:5554"),
        "could not connect to server");

    check(zmq_msg_init_data(&query, argv[1], strlen(argv[1]), NULL, NULL),
        "could not initialize message");
    q_open = 1;

    printf("sending...\n");
    check(zmq_send(s, &query, 0), "could not send message\n");

    printf("waiting for result...\n");
    check(zmq_msg_init(&result), "could not initialize message");
    r_open = 1;
    check(zmq_recv(s, &result, 0),
        "could not receive reply");

    printf("got response from server: %s\n", (char *)zmq_msg_data(&result));

    cleanup();
}
