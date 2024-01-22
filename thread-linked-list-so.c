#include <stdio.h>
#include <stdlib.h> //dung cho exit() va free()
//#include<pthread>
// Dinh nghia 1 Node trong danh sach lien ket
struct Node {
    int data;
    struct Node *next;
};

// tao Node voi 1 gia tri cho truoc
struct Node *TaoNode(int value) {
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Khong the cap phat bo nho cho nut moi.\n");
        exit(1);
    }
    newNode->data = value;
    newNode->next = NULL; // phan next tro vo NULL
    return newNode;
}

//Them Node vao dau linked list
void ThemDau(struct Node** head, int value) {
    
    // Kiểm tra xem con trỏ head trỏ đến vùng nhớ hợp lệ không
    if (*head == NULL) {
        printf("Con tro head tro den vung nho khong hop le. Khong the them node.\n");
        exit(0);
    }
    if (head == NULL) {
        printf("Con tro head la NULL. Khong the them node.\n");
        return;
    }
    struct Node* newNode = TaoNode(value);
    newNode->next = *head;
    *head = newNode;
}
//Them Node vao cuoi linked list
void ThemCuoi(struct Node **head, int value) {
    struct Node* newNode = TaoNode(value);

    // Nếu danh sách rỗng, thì nút mới trở thành đầu danh sách
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    // Duyệt qua danh sách để đến nút cuối cùng
    struct Node *lastNode = *head;
    while (lastNode->next != NULL) {
        lastNode = lastNode->next;
    }

    // Thay đổi con trỏ next của nút cuối cùng để trỏ đến nút mới
    lastNode->next = newNode;
}
// Xoa 1 Node trong linked lis
void Xoa(struct Node** head, int key) {
    // Lưu trữ nút hiện tại và nút trước nút hiện tại
    struct Node *current = *head, *prev = NULL;

    // Nếu nút cần xóa là nút đầu tiên
    if (current != NULL && current->data == key) {
        *head = current->next; // Thay đổi con trỏ đầu danh sách
        free(current); // Giải phóng bộ nhớ của nút cần xóa
        return;
    }

    // Duyệt qua danh sách để tìm nút cần xóa và nút trước nút cần xóa
    while (current != NULL && current->data != key) {
        prev = current;
        current = current->next;
    }

    // Nếu không tìm thấy nút cần xóa
    if (current == NULL) {
        printf("Khong tim thay nut co gia tri %d trong danh sach.\n", key);
        return;
    }

    // Thay đổi con trỏ next của nút trước nút cần xóa để bỏ qua nút cần xóa
    prev->next = current->next;

    // Giải phóng bộ nhớ của nút cần xóa
    free(current);
}
// Xoa node cuoi cung trong linked list
void XoaCuoiCung(struct Node** head) {
    // Neu danh sach roong
    if (*head == NULL) {
        printf("Danh sach rong, khong co gi de xoa.\n");
        return;
    }

    // Meu chi co 1 node
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }

    // Lưu trữ nút hiện tại và nút trước nút hiện tại
    struct Node *current = *head, *prev = NULL;

    // Duyệt qua danh sách để đến nút cuối cùng
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    // Nếu đã tìm thấy nút cuối cùng
    prev->next = NULL;
    free(current);
}

// tim kiem 1 Node trong linnked list
struct Node* TimNode(struct Node* head, int key) {
    while (head != NULL) {
        if (head->data == key) {
            return head; // Trả về con trỏ đến nút chứa giá trị cần tìm
        }
        head = head->next;
    }
    return NULL; // Trả về NULL nếu không tìm thấy
}
// In cac Node trong liked list
void InNode(struct Node* head) {
    //printf("Danh sach lien ket: ");
    while (head != NULL) {
        printf("%d -> ", head->data);
        head = head->next;
    }
    printf("NULL\n");
}
// Hàm để in kích thước của linked list
int KichThuoc(struct Node* head) {
    int size = 0;
    struct Node* current = head;

    while (current != NULL) {
        size++;
        current = current->next;
    }

    return size;
}

// Tra ve gia tri cuoi cung trong linked list khi khac NULL
int GiaTriCuoiCung( const struct Node* head) {
    // Neu danh sach rong
    if (head == NULL) {
        printf("Danh sach rong.\n");
        return -1; // Hoac gia tri khac tuy thuoc vao yeu cau cua ban
    }

    // Duyet qua danh sach den khi tim thay nut cuoi cung
    while (head->next != NULL) {
        head = head->next;
    }

    return head->data;
}

// Định nghĩa cấu trúc ThreadData
/*struct ThreadData {
    const char* threadName;
    void (*func1)(void*);
    void (*func2)(void*);
    void (*func3)(void*);
};

// Hàm thực hiện công việc của thread
void* threadFunction(void* data) {
    struct ThreadData* threadData = (struct ThreadData*)data;
    printf("%s bat dau.\n", threadData->threadName);

    if (threadData->func1 != NULL) 
        threadData->func1(NULL);

    if (threadData->func2 != NULL) 
        threadData->func2(NULL);

    if (threadData->func3 != NULL) 
        threadData->func3(NULL);
   
    printf("%s ket thuc.\n", threadData->threadName);
    pthread_exit(NULL);
}

// cau truc thread data
struct ThreadData {
    char* threadName;
    void (*ThemDau)(struct Node**, int);
    void (*InNode)(struct Node*);
    void (*Xoa)(struct Node**, int);
};

// tao va RUN thread
void TaoVaRun(){
 pthread_t themThread1, themThread2, layThread1, layThread2;
    struct ThreadData themData1 = {"Thread 1", &ThemDau, NULL, NULL};
    struct ThreadData themData2 = {"Thread 2", &ThemDau, NULL, NULL};
    struct ThreadData layData1 = {"Thread 3", NULL, &InNode, &Xoa};
    struct ThreadData layData2 = {"Thread 4", NULL, &InNode, &Xoa};


    	pthread_create(&layThread1, NULL, LayDuLieu, &layData1);
        pthread_create(&layThread2, NULL, LayDuLieu, &layData2);
        pthread_create(&themThread1, NULL, ThemDuLieu, &themData1);
        pthread_create(&themThread2, NULL, ThemDuLieu, &themData2);

        

        pthread_join(themThread1, NULL);
        pthread_join(themThread2, NULL);
        pthread_join(layThread1, NULL);
        pthread_join(layThread2, NULL);
}

//giai phong vung nho
void freeLinkedList(struct Node* head) {
 while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp);
    }
}
*/
