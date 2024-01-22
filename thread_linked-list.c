#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>

#define SoLan  10
typedef struct Node* (*TaoNode)(int);
typedef void (*ThemDau)(struct Node**, int);
typedef void (*ThemCuoi)(struct Node**, int);
typedef void (*Xoa)(struct Node**, int);
typedef void (*InRa)(struct Node*);
typedef struct Node* (*Tim)(struct Node*, int);

struct Node {
    int data;
    struct Node* next;
};


struct ThreadData {
    char* threadName;
    ThemDau themDau;
    InRa inRa;
};

int main() {
void* lib = dlopen("./liblinked-list.so", RTLD_LAZY);
TaoNode TaoNode = dlsym(lib, "createNode");   // lay dia chi cac ham trong thu vien
Tim Tim = dlsym(lib, "searchNode");
ThemDau themDau = dlsym(lib, "insertAtBeginning");
ThemCuoi themCuoi = dlsym(lib, "insertAtEnd");
Xoa xoa = dlsym(lib, "deleteNode");
InRa inRa = dlsym(lib, "printLinkedList");
    
  // kiem ra co loi khi lay dia chi ca ham khong
char *dlError = dlerror();
if (dlError != NULL) {
        fprintf(stderr, "Loi khi lay dia chi cua cac ham: %s\n", dlError);
        dlclose(lib);
        return 1;
    }
    

struct Node* head = NULL;
    
void*ThemDuLieu(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    ThemDau themDau = data->themDau;

   // for (int i = 0; i < SoLan; ++i) {
        int threadID = (int)pthread_self();
        int dataValue = rand() % 100;
        
        if (themDau != NULL) {
            themDau(&head, dataValue);
            printf("%s: Thread ID %d added data %d\n", data->threadName, threadID, dataValue);
        }

        sleep(1);
    //}
    pthread_exit(NULL);
}

void*LayDuLieu(void* arg) {
    struct ThreadData* data = (struct ThreadData*)arg;
    InRa inRa = data->inRa;


    int threadID = (int)pthread_self();
    printf("%s: Thread ID %d printing data\n", data->threadName, threadID);
    inRa(head);
    sleep(1);
    
    pthread_exit(NULL);
}


      
/*    void*ThemDuLieu(void *arg) {
    	ThemDau themDau = (ThemDau)arg;
    	for (int i = 0; i < SoLan; ++i) {
        	int data = rand() % 100;

        	if (themDau!=NULL){
        		themDau(&head, data);
        		sleep(1);
    		}
    	}
    	pthread_exit(NULL);
	}
	

    void*LayDuLieu(void *arg) {
    	InRa inRa = (InRa)arg;
        inRa(head);
        sleep(1);
    	pthread_exit(NULL);
	}
*/
  
    pthread_t themThread1, themThread2, layThread1, layThread2;
    struct ThreadData themData1 = {"Thread 1", themDau, NULL};
    struct ThreadData themData2 = {"Thread 2", themDau, NULL};
    struct ThreadData layData1 = {"Thread 3", NULL, inRa};
    struct ThreadData layData2 = {"Thread 4", NULL, inRa};
 for (int i = 0; i < SoLan; ++i) {
    pthread_create(&themThread1, NULL, ThemDuLieu, &themData1);
    pthread_create(&themThread2, NULL, ThemDuLieu, &themData2);

   

    pthread_create(&layThread1, NULL, LayDuLieu, &layData1);
    pthread_create(&layThread2, NULL, LayDuLieu, &layData2);
}    
    pthread_join(layThread1, NULL);
    pthread_join(layThread2, NULL);
    pthread_join(themThread1, NULL);
    pthread_join(themThread2, NULL);
 
    /////////////////////////
/*   

   pthread_create(&themThread1, NULL, ThemDuLieu, themDau);
   pthread_create(&themThread2, NULL, ThemDuLieu, themDau);


   pthread_join(themThread1, NULL);
   pthread_join(themThread2, NULL);
  
    
   pthread_create(&layThread1, NULL, LayDuLieu, inRa);
   pthread_create(&layThread2, NULL, LayDuLieu, inRa);


   pthread_join(layThread1, NULL);
   pthread_join(layThread2, NULL);
*/
//giai phong bo nho
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
    dlclose(lib);

    return 0;
}







