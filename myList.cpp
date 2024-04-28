#include <iostream>
#include <utility>
#include <string>
#include "myList.hpp"
#include "myInteger.hpp"

// default constructor
template <typename T>
MyList<T>::MyList() {
    // Set head and tail to null ptr and list size to 0
    // This is initialising an empty list
    head_ = nullptr;
    tail_ = nullptr;
    sentinel = new Node(T(0), nullptr, nullptr);
    size_ = 0;
}

// copy constructor
template <typename T>
MyList<T>::MyList(const MyList& other) {
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;

    if (!other.empty()) {
        // Set the head to the same as the other list
        // Then set the tail to be the same as the head, as thr list only has one node
        head_ = new Node(other.head_->data);
        tail_ = head_;
        ++size_;

        // Gets the next node in the other list
        Node* nextNode = other.head_->next;
        while (nextNode != nullptr) {
            // Set the tail next value to the next node
            tail_->next = new Node(nextNode->data, tail_, nullptr);
            
            // Then make the tail node that same node
            tail_ = tail_->next;

            // Increase the size of the list and repeat with remaining nodes
            ++size_;
            nextNode = nextNode->next;
        }
    }
}

// assignment operator
template <typename T>
MyList<T>& MyList<T>::operator=(MyList other) {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
    return *this;
}

// destructor
template <typename T>
MyList<T>::~MyList() {
    while (!empty()) {
        pop_back();
    }
    delete sentinel;
}

// constructor from an initializer list
template <typename T>
MyList<T>::MyList(std::initializer_list<T> vals) : MyList {} {
    for(const T& val : vals){
        push_back(val);
    }
} 

// push back
template <typename T>
void MyList<T>::push_back(T val) {
    Node* newNode = new Node(val);
    if(empty()){
        //If list is empty then set the first and last node to the new value
        head_ = newNode;
        tail_ = newNode;
    }else{
        //If list is not empty then:
        //1. Set current last nodes next pointer to pointer of newNode
        //2. Set newNode's previous pointer to the current last node
        //3. Set the last node to be newNode

        tail_->next = newNode;
        newNode->prev = tail_;
        newNode->next = sentinel;
        sentinel->prev = newNode;
        tail_=newNode;
    }
    ++size_;
}

// pop back
template <typename T>
void MyList<T>::pop_back() {
    if(empty()){
        return;
    }
        
    // Sets the last node to be the node to pop
    // Then moves the tail to be the second last node
    // Then if a previous node exists its sets the next to be a null ptr
    // Then deletes the nodeToPop and decrements the size
    Node *nodeToPop = tail_;
    tail_ = tail_->prev;
    if(tail_){
        tail_->next = nullptr;
    } else {
        // Set head to null if the list becomes empty after popping
        head_ = nullptr; 
    }
    
    delete nodeToPop;
    --size_;
}

template <typename T>
void MyList<T>::push_front(T val) {
    Node* newNode = new Node(val);
    if(empty()){
        //If list is empty then set the first and last node to the new value
        head_ = newNode;
        tail_ = newNode;
    }else{
        //If list is not empty then:
        //1. Set current first nodes prev pointer to newNode
        //2. Set newNode's next pointer to the current first node
        //3. Set the first node to be newNode

        head_->prev = newNode;
        newNode->next = head_;
        head_ = newNode;
    }
    ++size_;
}

template <typename T>
void MyList<T>::pop_front() {
    if(empty()){
        return;
    }
    //Set the node to pop as the head node
    //Set the new head as the next node
    // If the list isnt empty after removing node then 
    // set the previous node to a nullptr
    Node *nodeToPop = head_; 
    head_ = head_->next; 
    if (head_) {
        head_->prev = nullptr;
    } 
    delete nodeToPop;
    --size_;
}

// return the first element by reference
template <typename T>
T& MyList<T>::front() {
    return head_->data;
}

// return the first element by const reference
template <typename T>
const T& MyList<T>::front() const {
    return head_->data;
}

// return the last element by reference
template <typename T>
T& MyList<T>::back() {
    return tail_->data;
}

// return the last element by const reference
template <typename T>
const T& MyList<T>::back() const {
    return tail_->data;
}

// is the list empty?
template <typename T>
bool MyList<T>::empty() const {
    if(size_ == 0){
        return true;
    }
    return false;
}

// return the number of elements in the list
template <typename T>
int MyList<T>::size() const {
    return size_;
}

// return an iterator pointing to the first element
template <typename T>
typename MyList<T>::Iterator MyList<T>::begin() const {
    return Iterator(head_);
}

// return an iterator pointing to the last element
template <typename T>
typename MyList<T>::Iterator MyList<T>::end() const {
    return Iterator(sentinel);
}

// insert an element into the linked list before *it with the value val
template <typename T>
void MyList<T>::insert(const Iterator& it, const T& val) {

    MyList::Node* current = it.getCurrent();

    if (empty()) {
        Node* newNode = new Node(val, nullptr, sentinel); 

        // Update head_ and tail_ to point to the new node
        head_ = newNode;
        tail_ = newNode;
        sentinel->prev = newNode;
        size_++; 
        return;
    }
    
    // If trying to insert at back of the list
    if (current == sentinel) {
        Node* newNode = new Node(val, tail_, sentinel);

        // Update the tail pointer to point to the new node
        tail_->next = newNode;
        tail_ = newNode;

        // Update sentinel to point to the new node
        sentinel->prev = newNode;

    }else{
        Node* newNode = new Node(val, current->prev, current);
    
        if (current->prev != nullptr) {
            current->prev->next = newNode;
        } else {
            head_ = newNode;
        }
        current->prev = newNode;
    }

    ++size_;
}

// remove *it from the list
template <typename T>
void MyList<T>::erase(const Iterator& it) {
    
    Node* nodeToRemove = it.getCurrent();

    if (nodeToRemove == nullptr || empty()) {
        return;
    }

    // Update head_ if removing the first node
    if (nodeToRemove == head_) {
        head_ = nodeToRemove->next;
    }

    // Update tail_ if removing the last node
    if (nodeToRemove == tail_) {
        tail_ = nodeToRemove->prev;
    }

    // Update the neighbour nodes pointers
    if (nodeToRemove->prev != nullptr) {
        nodeToRemove->prev->next = nodeToRemove->next;
    }
    if (nodeToRemove->next != nullptr) {
        nodeToRemove->next->prev = nodeToRemove->prev;
    }

    delete nodeToRemove;

    --size_;
}

// Iterator constructor
template <typename T>
MyList<T>::Iterator::Iterator(Node* pointer) {
    current_ = pointer;
}

// (pre-)increment an iterator
template <typename T>
typename MyList<T>::Iterator& MyList<T>::Iterator::operator++() {
    current_ = current_->next;
    return *this;
}

// (pre-)decrement an iterator
template <typename T>
typename MyList<T>::Iterator& MyList<T>::Iterator::operator--() {
    current_ = current_->prev;
    return *this;
}

// return the value pointed at by an iterator
template <typename T>
T& MyList<T>::Iterator::operator*() const {
    return current_->data;
}

// returns the node as current_
template <typename T>
MyList<T>::Node* MyList<T>::Iterator::getCurrent() const {
    return current_;
}

template class MyList<int>;
template class MyList<std::string>;
template class MyList<MyInteger>;
