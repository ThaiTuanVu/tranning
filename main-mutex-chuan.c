#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "thread-linked-list-so.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_producer = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumer = PTHREAD_COND_INITIALIZER;

int dataValue, temp = 1;
int can_produce = 1;  // condition variable 

struct ThreadData {
    char* threadName;
    void (*ThemDau)(struct Node**, int);
    void (*InNode)(struct Node*);
    void (*XoaCuoiCung)(struct Node**);
};

struct Node* head1 = NULL;
struct Node* head2 = NULL; // Luu tu 1 -> 1000

void* ThemDuLieu(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    int threadID = (int)pthread_self();

    while (1) {
        dataValue = rand() % 101;
        pthread_mutex_lock(&mutex);

        while (!can_produce) {
            pthread_cond_wait(&cond_producer, &mutex);
        }

        printf("%s: Thread ID %d added data %d\n", data->threadName, threadID, dataValue);
        data->ThemDau(&head1, dataValue);
        printf("Size Linked List First: %d\n", KichThuoc(head1));

        can_produce = 0;  // Producers stop producing
        pthread_cond_signal(&cond_consumer);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void* LayDuLieu(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    int threadID = (int)pthread_self();

    while (1) {
        pthread_mutex_lock(&mutex);

        while (can_produce) {
            pthread_cond_wait(&cond_consumer, &mutex);
        }

        printf("%s: Thread ID %d Delete data %d\n", data->threadName, threadID, GiaTriCuoiCung(head1));

        if (head1->data == temp) {
            data->ThemDau(&head2, head1->data);
            printf("Linked List Out:  ");
            data->InNode(head2);
            temp++;
        }

        data->XoaCuoiCung(&head1);
        printf("Size Linked List Frist: %d\n", KichThuoc(head1));

        if (temp > 1000) {
            printf("Data from 1 to 1000 has been processed. Exiting...\n");
            data->InNode(head2);
            exit(0);
        }

        can_produce = 1;  // Consumers stop consuming
        pthread_cond_signal(&cond_producer);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

// tao va RUN thread
void TaoVaRun() {
    pthread_t themThread1, themThread2, layThread1, layThread2;
    struct ThreadData themData1 = {"Thread 1", &ThemDau, NULL, NULL};
    struct ThreadData themData2 = {"Thread 2", &ThemDau, NULL, NULL};
    struct ThreadData layData1 = {"Thread 3", &ThemDau, &InNode, &XoaCuoiCung};
    struct ThreadData layData2 = {"Thread 4", &ThemDau, &InNode, &XoaCuoiCung};

    pthread_create(&layThread1, NULL, LayDuLieu, &layData1);
    pthread_create(&layThread2, NULL, LayDuLieu, &layData2);
    pthread_create(&themThread1, NULL, ThemDuLieu, &themData1);
    pthread_create(&themThread2, NULL, ThemDuLieu, &themData2);

    pthread_join(themThread1, NULL);
    pthread_join(themThread2, NULL);
    pthread_join(layThread1, NULL);
    pthread_join(layThread2, NULL);
}

// giai phong vung nho
void freeLinkedList(struct Node* head) {
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(){
      TaoVaRun();
    freeLinkedList(head1);
    freeLinkedList(head2);
    return 0;  
    
    
}
