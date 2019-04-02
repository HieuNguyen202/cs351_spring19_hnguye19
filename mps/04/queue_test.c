//
// Created by HIEU NGUYEN on 2019-04-02.
//
#include "queue.c"

int main(int argc, char** argv) {
    queuep_t q = queue_create();
    enqueue(q, 1);
    queue_print(q);
    enqueue(q, 2);
    queue_print(q);
    enqueue(q, 3);
    queue_print(q);
    enqueue(q, 4);
    queue_print(q);
    dequeue(q);
    queue_print(q);
    dequeue(q);
    queue_print(q);
    dequeue(q);
    queue_print(q);
    dequeue(q);
    dequeue(q);
    queue_print(q);
}
