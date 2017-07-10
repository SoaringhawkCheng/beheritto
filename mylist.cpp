//#include "mylist.h"
//#include "mempool.h"

//template<typename T>
//ListNode<T>::ListNode():prev(NULL),next(NULL){}
//
//template<typename T>
//bool ListIterator<T>::operator ==(const ListIterator &iter) const{
//    return ptr==iter->ptr;
//}
//
//template<typename T>
//bool ListIterator<T>::operator !=(const ListIterator &iter) const{
//    return ptr!=iter->ptr;
//}
//
//template<typename T>
//T &ListIterator<T>::operator *() const {
//    return (*ptr).data;
//}
//
//template<typename T>
//T *ListIterator<T>::operator ->() const{
//    return &(operator*());
//}
//
//template<typename T>
//ListIterator<T> &ListIterator<T>::operator ++(){
//    ptr=ptr->next;
//    return *this;
//}
//
//template<typename T>
//ListIterator<T> &ListIterator<T>::operator --(){
//    ptr=ptr->prev;
//    return *this;
//}
//
//template<typename T>
//ListIterator<T> ListIterator<T>::operator ++(int){
//    ListNode<T> *tmp=*this;
//    ptr=ptr->next;
//    return tmp;
//}
//
//template<typename T>
//ListIterator<T> ListIterator<T>::operator --(int){
//    ListNode<T> *tmp=*this;
//    ptr=ptr->prev;
//    return tmp;
//}
//
//template<typename T>
//MyList<T>::MyList(){
//    head=NULL;
//    tail=NULL;
//}
//
//template<typename T>
//MyList<T>::~MyList(){
//    ListNode<T> *ptr=head;
//    while(ptr){
//        ListNode<T> *tmp=ptr;
//        ptr=ptr->next;
//        free(tmp);
//    }
//}
//
//template<typename T>
//bool MyList<T>::empty(){
//    return head==NULL;
//}
//
//template<typename T>
//void MyList<T>::push_back(const T &t){
//    ListNode<T> *ptr=(ListNode<T> *)malloc(sizeof(ListNode<T>));
//    ptr->data=t;
//    if(head==NULL){
//        head->ptr;
//    }
//    else{
//        tail->next=ptr;
//        ptr->prev=tail;
//        tail=tail->next;
//    }
//}
//
//template<typename T>
//void MyList<T>::pop_back(){
//    if(head==NULL) return;
//    if(head==tail){
//        free(tail);
//        head=tail=NULL;
//    }
//    else{
//        ListNode<T> *ptr=tail;
//        tail=tail->prev;
//        free(ptr);
//    }
//}
//
//template<typename T>
//void MyList<T>::erase(ListIterator<T> &iterator){
//    if(head==NULL) return;
//    if(head==tail){
//        if(iterator.ptr==head){
//            head=tail=NULL;
//            free(iterator.ptr);
//            iterator.ptr=NULL;
//        }
//    }
//    else{
//        if(iterator.ptr==head){
//            head=head->next;
//            head->prev=NULL;
//            free(iterator.ptr);
//            iterator.ptr=head;
//        }
//        else if(iterator.ptr==tail){
//            tail=tail->prev;
//            tail->next=NULL;
//            free(iterator.ptr);
//            iterator.ptr=NULL;
//        }
//        else{
//            ListNode<T> *ptr=iterator.ptr;
//            ptr->prev->next=ptr->next;
//            ptr->next->prev=ptr->prev;
//            iterator.ptr=ptr->next;
//            free(ptr);
//        }
//    }
//}
//
//template<typename T>
//void MyList<T>::clear(){
//    for(auto iterator=begin();iterator!=end();++iterator){
//        ListNode<T> *ptr=iterator.ptr;
//        iterator.ptr=ptr->next;
//        free(ptr);
//    }
//    head=tail=NULL;
//}
//
//template<typename T>
//const T& MyList<T>::front(){
//    return head->data;
//}
//
//template<typename T>
//const T& MyList<T>::back(){
//    return tail->data;
//}
//
//template<typename T>
//ListIterator<T> MyList<T>::begin(){
//    ListIterator<T> *iterator=(ListIterator<T> *)malloc(sizeof(ListIterator<T>));
//    iterator.ptr=head;
//    return *iterator;
//}
//
//template<typename T>
//ListIterator<T> MyList<T>::end(){
//    ListIterator<T> *iterator=(ListIterator<T> *)malloc(sizeof(ListIterator<T>));
//    iterator.ptr=NULL;
//    return *iterator;
//}
