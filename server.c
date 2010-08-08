#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zmq.h>

char reply[] = "Your message was received!";

int main()
{
    void *ctx, *s;
    zmq_msg_t query, result;

    ctx = zmq_init(1);

    s = zmq_socket(ctx, ZMQ_REP);

    if (zmq_bind(s, "tcp://127.0.0.1:5554") < 0) {
        printf("could not bind\n");
        exit(-1);
    }

    while (1) {
        printf("listening...\n");
        zmq_msg_init(&query);
        zmq_recv(s, &query, 0);

        int msg_len = zmq_msg_size(&query) + 1;
        char *out = (char *) malloc(msg_len);
        snprintf(out, msg_len, "%s", (char *) zmq_msg_data(&query));
        out[msg_len-1]='\0';
        printf("%s\n", out);
        zmq_msg_close(&query);
        free(out);

        zmq_msg_init_data(&result, reply, strlen(reply), NULL, NULL);
        zmq_send(s, &result, 0);
        zmq_msg_close(&result);
    }
}
