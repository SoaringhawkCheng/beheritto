#ifndef mylist_h
#define mylist_h

#include <iostream>

class MemList;
class MemBlock;

template<typename T> class MyList;

template<typename T>
class ListNode{
public:
    ListNode();
//    ListNode(const ListNode<T> &node);
//    ListNode(const T &t);
    ListNode<T> *prev;
    ListNode<T> *next;
    T data;
};

template<typename T>
class ListIterator{
    friend class MyList<T>;
public:
    bool operator ==(const ListIterator &iter) const;
    bool operator !=(const ListIterator &iter) const;
    T &operator *() const;
    T *operator ->() const;
    ListIterator &operator ++();
    ListIterator &operator --();
    ListIterator operator ++(int);
    ListIterator operator --(int);
private:
    ListNode<T> *ptr;
};

template<typename T>
class MyList{
public:
    MyList();
    ~MyList();
    bool empty();
    int size();
    void push_back(const T &t);
    void pop_back();
    const T& front();
    const T& back();
    ListIterator<T> begin();
    ListIterator<T> end();
    void erase(ListIterator<T> &iterator);
    void clear();
private:
    ListNode<T> *head;
    ListNode<T> *tail;
    ListIterator<T> *iterator;
};







template<typename T>
ListNode<T>::ListNode():prev(NULL),next(NULL){}

template<typename T>
bool ListIterator<T>::operator ==(const ListIterator &iter) const{
    return ptr==iter.ptr;
}

template<typename T>
bool ListIterator<T>::operator !=(const ListIterator &iter) const{
    return ptr!=iter.ptr;
}

template<typename T>
T &ListIterator<T>::operator *() const {
    return (*ptr).data;
}

template<typename T>
T *ListIterator<T>::operator ->() const{
    return &(operator*());
}

template<typename T>
ListIterator<T> &ListIterator<T>::operator ++(){
    ptr=ptr->next;
    return *this;
}

template<typename T>
ListIterator<T> &ListIterator<T>::operator --(){
    ptr=ptr->prev;
    return *this;
}

template<typename T>
ListIterator<T> ListIterator<T>::operator ++(int){
    ListNode<T> *tmp=*this;
    ptr=ptr->next;
    return tmp;
}

template<typename T>
ListIterator<T> ListIterator<T>::operator --(int){
    ListNode<T> *tmp=*this;
    ptr=ptr->prev;
    return tmp;
}

template<typename T>
MyList<T>::MyList(){
    head=NULL;
    tail=NULL;
}

template<typename T>
MyList<T>::~MyList(){
    ListNode<T> *ptr=head;
    while(ptr){
        ListNode<T> *tmp=ptr;
        ptr=ptr->next;
        free(tmp);
    }
}

template<typename T>
bool MyList<T>::empty(){
    return head==NULL;
}

template<typename T>
int MyList<T>::size(){
    if(head==NULL) return 0;
    int count=1;
    ListNode<T> *ptr=head;
    while(ptr!=tail){
        ++count;
        ptr=ptr->next;
    }
    return count;
}

template<typename T>
void MyList<T>::push_back(const T &t){
    ListNode<T> *ptr=(ListNode<T> *)malloc(sizeof(ListNode<T>));
    ptr->data=t;
    if(head==NULL){
        head=tail=ptr;
    }
    else{
        tail->next=ptr;
        ptr->prev=tail;
        tail=tail->next;
    }
    cout<<"after push list now is "<<empty()<<" "<<size()<<endl;
}

template<typename T>
void MyList<T>::pop_back(){
    if(head==NULL) return;
    if(head==tail){
        free(tail);
        head=tail=NULL;
    }
    else{
        ListNode<T> *ptr=tail;
        tail=tail->prev;
        free(ptr);
    }
    cout<<"after push list now is"<<empty()<<" "<<size()<<endl;
}

template<typename T>
void MyList<T>::erase(ListIterator<T> &iterator){
    if(head==NULL) return;
    if(head==tail){
        if(iterator.ptr==head){
            head=tail=NULL;
            free(iterator.ptr);
            iterator.ptr=NULL;
        }
    }
    else{
        if(iterator.ptr==head){
            head=head->next;
            head->prev=NULL;
            free(iterator.ptr);
            iterator.ptr=head;
        }
        else if(iterator.ptr==tail){
            tail=tail->prev;
            tail->next=NULL;
            free(iterator.ptr);
            iterator.ptr=NULL;
        }
        else{
            ListNode<T> *ptr=iterator.ptr;
            ptr->prev->next=ptr->next;
            ptr->next->prev=ptr->prev;
            iterator.ptr=ptr->next;
            free(ptr);
        }
    }
}

template<typename T>
void MyList<T>::clear(){
    for(auto iterator=begin();iterator!=end();++iterator){
        ListNode<T> *ptr=iterator.ptr;
        iterator.ptr=ptr->next;
        free(ptr);
    }
    head=tail=NULL;
}

template<typename T>
const T& MyList<T>::front(){
    return head->data;
}

template<typename T>
const T& MyList<T>::back(){
    return tail->data;
}

template<typename T>
ListIterator<T> MyList<T>::begin(){
    ListIterator<T> *iter_pointer=(ListIterator<T> *)malloc(sizeof(ListIterator<T>));
    iter_pointer->ptr=head;
    return *iter_pointer;
}

template<typename T>
ListIterator<T> MyList<T>::end(){
    ListIterator<T> *iter_pointer=(ListIterator<T> *)malloc(sizeof(ListIterator<T>));
    iter_pointer->ptr=NULL;
    return *iter_pointer;
}




#endif
