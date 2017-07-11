//#ifndef mylist_h
//#define mylist_h
//
//#include <iostream>
//
//class MemList;
//class MemBlock;
//
//template<typename T> class MyList;
//
//template<typename T>
//class ListNode{
//public:
//    void init();
//    ListNode<T> *prev;
//    ListNode<T> *next;
//    T data;
//};
//
////template<typename T>
////class ListIterator{
////    friend class MyList<T>;
////public:
////    void init();
////    bool operator ==(const ListIterator &iter) const;
////    bool operator !=(const ListIterator &iter) const;
////    T &operator *() const;
////    T *operator ->() const;
////    ListIterator &operator ++();
////    ListIterator &operator --();
//////    ListIterator operator ++(int);
//////    ListIterator operator --(int);
//////private:
////    ListNode<T> *ptr;
////};
//
//template<typename T>
//class MyList{
//public:
//    ~MyList();
//    void init();
//    bool empty();
//    int size();
//    const T &front();
//    const T &back();
//    ListNode<T> * const begin();
//    ListNode<T> * const end();
//    void push_back(const T &t);
//    void pop_back();
//    void erase(ListNode<T> *ptr);
////    const ListIterator<T> &begin();
////    const ListIterator<T> &end();
////    void erase(ListIterator<T> &iterator);
//    void clear();
////private:
//    ListNode<T> dummy;
////    ListIterator<T> listiterator;
//};
//
//template<typename T>
//void ListNode<T>::init(){
//    prev=this;
//    next=this;
//}
//
////template<typename T>
////void ListIterator<T>::init(){
////    ptr=NULL;
////}
//
////template<typename T>
////bool ListIterator<T>::operator ==(const ListIterator &iter) const{
////    return ptr==iter.ptr;
////}
////
////template<typename T>
////bool ListIterator<T>::operator !=(const ListIterator &iter) const{
////    return ptr!=iter.ptr;
////}
//
////template<typename T>
////T &ListIterator<T>::operator *() const {
////    return ptr->data;
////}
////
////template<typename T>
////T *ListIterator<T>::operator ->() const{
////    return &(ptr->data);
////}
//
////template<typename T>
////ListIterator<T> &ListIterator<T>::operator ++(){
////    ptr=ptr->next;
////    return *this;
////}
////
////template<typename T>
////ListIterator<T> &ListIterator<T>::operator --(){
////    ptr=ptr->prev;
////    return *this;
////}
////
////template<typename T>
////ListIterator<T> ListIterator<T>::operator ++(int){
////    ListNode<T> *tmp=*this;
////    ptr=ptr->next;
////    return tmp;
////}
////
////template<typename T>
////ListIterator<T> ListIterator<T>::operator --(int){
////    ListNode<T> *tmp=*this;
////    ptr=ptr->prev;
////    return tmp;
////}
//
//template<typename T>
//void MyList<T>::init(){
//    dummy.init();
////    listiterator.init();
//}
//
//template<typename T>
//MyList<T>::~MyList(){
//    ListNode<T> *ptr=dummy.next;
//    while(ptr!=&dummy){
//        dummy.next=ptr->next;
//        free(ptr);
//        ptr=dummy.next;
//    }
//}
//
//template<typename T>
//bool MyList<T>::empty(){
//    return dummy.next==&dummy;
//}
//
//template<typename T>
//int MyList<T>::size(){
//    int count=0;
//    ListNode<T> *ptr=&dummy;
//    while(ptr->next!=&dummy){
//        ++count;
//        ptr=ptr->next;
//    }
//    return count;
//}
//
//template<typename T>
//void MyList<T>::push_back(const T &t){
//    cout<<"before push list now is "<<empty()<<" "<<size()<<endl;
//    ListNode<T> *ptr=(ListNode<T> *)malloc(sizeof(ListNode<T>));
//    ptr->data=t;
//    if(dummy.next==&dummy){
//        ptr->next=&dummy;
//        ptr->prev=&dummy;
//        dummy.next=ptr;
//        dummy.prev=ptr;
//    }
//    else{
//        ptr->prev=dummy.prev;
//        ptr->next=&dummy;
//        dummy.prev->next=ptr;
//        dummy.prev=ptr;
//    }
//    cout<<"after push list now is "<<empty()<<" "<<size()<<endl;
//}
//
//template<typename T>
//void MyList<T>::pop_back(){
//    cout<<"before pop list now is "<<empty()<<" "<<size()<<endl;
//    if(dummy.next!=&dummy){
//        ListNode<T> *ptr=dummy.next;
//        ptr->next->prev=&dummy;
//        dummy.next=ptr->next;
//        free(ptr);
//    }
//    cout<<"after push list now is"<<empty()<<" "<<size()<<endl;
//}
//
//template<typename T>
//void MyList<T>::erase(ListNode<T> *ptr){
//    ptr->prev->next=ptr->next;
//    ptr->next->prev=ptr->prev;
//    free(ptr);
//}
//
//template<typename T>
//void MyList<T>::clear(){
//    ListNode<T> *ptr=dummy.next;
//    while(ptr!=&dummy){
//        ListNode<T> *tmp=ptr;
//        ptr=ptr->next;
//        free(tmp);
//    }
//}
//
//template<typename T>
//const T &MyList<T>::front(){
//    return dummy.next->data;
//}
//
//template<typename T>
//const T &MyList<T>::back(){
//    return dummy.prev->data;
//}
//
//template<typename T>
//ListNode<T> * const MyList<T>::begin(){
//    return dummy.next;
//}
//
//template<typename T>
//ListNode<T> * const MyList<T>::end(){
//    return &dummy;
//}
//
////template<typename T>
////const ListIterator<T> &MyList<T>::begin(){
////    listiterator.ptr=dummy.next;
////    return listiterator;
////}
////
////template<typename T>
////const ListIterator<T> &MyList<T>::end(){
////    listiterator.ptr=&dummy;
////    return listiterator;
////}
//
//#endif
