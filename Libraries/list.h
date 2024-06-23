#include <stdlib.h>

#define GLUE_HELPER(x, y) x##y
#define GLUE(x, y) GLUE_HELPER(x, y)

#define T(x) typeof(x)

// defining malloc with 1 len
#define MA(type) \
    (type *) malloc(1 * sizeof(type))

// defining calloc with 1 len
#define CA(type) \
    (type *) calloc(1, sizeof(type))

// defining realloc
#define RA(type, ptr, size) \
    (type *) realloc(ptr, size * sizeof(type))

#define foreach_node(type, head) \
    for(type current = head; current != NULL; current = current->next)

#define DLinkedList(typeName, type) typedef struct GLUE(Node_, typeName) { type value; struct GLUE(Node_, typeName) *next; } GLUE(Node_, typeName); typedef struct GLUE(Linked_List_, typeName) { int length; GLUE(Node_, typeName) *items; } GLUE(Linked_List_, typeName);
#define NewNode(newItem, type) \
({                             \
    type* newNode = CA(type);  \
    newNode->value = newItem;  \
    newNode->next = NULL;      \
    newNode;                   \
})
#define LList(typeName, name) GLUE(Linked_List_, typeName) *name = MA(GLUE(Linked_List_, typeName)); name->length = 0; name->items = CA(GLUE(Node_, typeName));
#define Append(newItem, lls)                                                  \
({                                                                            \
    T(lls->items) head = lls->items;                                          \
    T(lls->items) newNode = NewNode(newItem, T(*(lls->items)));               \
    if (lls->length == 0) {                                                   \
        head->value = newNode->value;                                         \
        head->next = NULL;                                                    \
    }                                                                         \
    else{                                                                     \
        T(lls->items) last = head;                                            \
        while (last->next != NULL) last = last->next;                         \
        last->next = newNode;                                                 \
    }                                                                         \
    lls->length += 1;                                                         \
})

#define At(index, lls)                \
({                                    \
    T(lls->items) head = lls->items;  \
    T(lls->items) answer = head;      \
    if (lls->length == 0) {           \
        answer = NULL;                \
    } else {                          \
        int i = 0;                    \
        while (head != NULL) {        \
            if (i == index) {         \
                answer = head;        \
                break;                \
            }                         \
            i += 1;                   \
            head = head->next;        \
        }                             \
    }                                 \
    answer;                           \
})

#define Search(searchValue, lls, keyFunc)                               \
({                                                                      \
    T(lls->items) answer = NULL;                                        \
    if (lls->length != 0) {                                             \
        T(lls->items) head = lls->items;                                \
        while (head != NULL) {                                          \
            if (keyFunc(searchValue, head)) {                           \
                answer = head;                                          \
                break;                                                  \
            }                                                           \
            head = head->next;                                          \
        }                                                               \
    }                                                                   \
    answer;                                                             \
})                                                                      

#define DList(typeName, type) typedef struct GLUE(List_, typeName) { int length; type *items; } GLUE(List_, typeName);
#define List(typeName, name) GLUE(List_, typeName) *name = MA(GLUE(List_, typeName)); name->length = 0; name->items = CA(T(*(name->items)));
#define LsAppend(newItem, parent) *(parent->items + parent->length) = newItem; parent->items = RA(T(newItem), parent->items, (parent->length + 2)); parent->length += 1;