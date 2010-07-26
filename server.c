#include <stdio.h>
#include <string.h>
#include <zmq.h>

char reply[] = "Your message was received!";

int main()
{
    void *ctx, *s;
    zmq_msg_t query, result;

    ctx = zmq_init(1);

    s = zmq_socket(ctx, ZMQ_REP);

    zmq_bind(s, "tcp://lo:5555");

    printf("serving...");
    while (1) {
        zmq_msg_init(&query);
        zmq_recv(s, &query, 0);

        printf("%s", (char *)zmq_msg_data(&query));
        zmq_msg_close(&query);

        zmq_msg_init_data(&result, reply, strlen(reply), NULL, NULL);
        zmq_send(s, &result, 0);
        zmq_msg_close(&result);
    }
}
