// thread-linked-list-so.h
#ifndef THREAD_LINKED_LIST_SO_H
#define THREAD_LINKED_LIST_SO_H

struct Node {
    int data;
    struct Node* next;
};
void ThemDau(struct Node** head, int value);
void ThemCuoi(struct Node** head, int value);
void Xoa(struct Node** head, int key);
void InNode(struct Node* head);
struct Node* TimNode(struct Node* head, int key);
void XoaCuoiCung(struct Node** head);
int KichThuoc(struct Node* head);
int GiaTriCuoiCung( struct Node* head); 
#endif // THREAD_LINKED_LIST_SO_H

