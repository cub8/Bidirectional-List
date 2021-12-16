#pragma once
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <type_traits>
#include <initializer_list>
#include <span>
#include <functional>

template <typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
    };
    int size = 0;
    Node* start = nullptr;
    Node* end = nullptr;
    void initialize(T data);
    auto getNode(int position) -> Node*;
    int C__getPosition(Node* node); // getting position of node
    void C__swap(Node* node1, Node* node2); // in-class swapping
    auto C__partition(Node* _start, Node* _end, 
        std::function<bool(T const&, T const&)> compareFunc) -> Node*; // in-class sorting to make things easier for user
    void C__sort(Node* _start, Node* _end, std::function<bool(T const&, T const&)> compareFunc); // in-class sorting to make things easier for user
public:
    // constructor and destructor
    List();
    List(std::initializer_list<T> const);
    ~List();
    // operators
    void operator= (std::initializer_list<T>);
    List<T>& operator= (List<T> const&);
    T& operator[] (int const position);
    T& operator[] (int const position) const;
    // editing data
    void push_back(T _data); // adds node at the end
    void insert(T _data, int position); // adds node at given position
    void push_front(T _data); // adds node at the start
    void replace(T _data, int position); // changes data at given position
    void importArrayToList(std::span<T>);
    void pop_back(); // deletes node at the end
    void erase(int position); // deletes node at given position
    void pop_front(); // deletes node at the start
    void clear(); // clears whole list and sets size to 0
    void sort(std::function<bool(T const&, T const&)> compareFun = [](T x, T y) -> bool { return x > y; }); // sorts data
    void swap(int position1, int position2); // swaps values of elements at positions 1 and 2
    void reverse(); // reverses list's values

    // getting access data
    int getSize() const; // returns size of list
    int getPosition(T const& _data) const; // returns position of searched data; if didn't find - return -1
    int count(T const& _data) const; // counts how many times
    void printList() const; // prints list
};

/* ############################### private stuff ################# */
template<typename T>
void List<T>::initialize(T data) {
    start = new Node { data, nullptr, nullptr };
    size++;
    end = start;
}
template<typename T>
auto List<T>::getNode(int position) -> Node* {
    if (position < 0 || position >= size) {
        throw std::invalid_argument("Incorrect position! Exceeded list range!");
    }
    Node* temp = start;
    int i = 0;
    while (i < position) {
        temp = temp->next;
        i++;
    }
    return temp;
}
template<typename T>
int List<T>::C__getPosition(Node* node) {
    Node* temp = start;
    int i = 0;
    while (temp != nullptr) {
        if (temp == node)
            return i;
        temp = temp->next;
        i++;
    }
    return -1;
}
template<typename T>
void List<T>::C__swap(Node* node1, Node* node2) {
    T holder = node1->data;
    node1->data = node2->data;
    node2->data = holder;
}
template<typename T>
auto List<T>::C__partition(Node* _start, Node* _end, 
    std::function<bool(T const&, T const&)> compareFunc) -> Node* {
    Node* pivot = _end;
    if (_start->prev == nullptr) {
        _start->prev = new Node;
        _start->prev->next = _start;
    }
    Node* iterate = _start;
    Node* toSwap = _start->prev;
    while (iterate != _end) {
        if (compareFunc(pivot->data, iterate->data)) {
            toSwap = toSwap->next;
            C__swap(toSwap, iterate);
        }
        iterate = iterate->next;
    }
    toSwap = toSwap->next;
    C__swap(toSwap, _end);
    return toSwap;
}
template<typename T>
void List<T>::C__sort(Node* _start, Node* _end, std::function<bool(T const&, T const&)> compareFunc) {
    if (C__getPosition(_start) < C__getPosition(_end) && _start != nullptr && _end != nullptr) {
        Node* pi = C__partition(_start, _end, compareFunc);
        C__sort(_start, pi->prev, compareFunc);
        C__sort(pi->next, _end, compareFunc);
    }
}
/* ############################### public stuff ################# */
/***************************** constructor and destructor **************************************/
template<typename T>
List<T>::List() {
    start = nullptr;
    end = nullptr;
    size = 0;
}
template<typename T>
List<T>::List(std::initializer_list<T> arr) {
    start = nullptr;
    end = nullptr;
    size = 0;
    for (auto& val : arr) {
        push_back(val);
    }
}

template<typename T>
List<T>::~List() {
    clear();
}
/***************************** operators **************************************/
template<typename T>
void List<T>::operator=(std::initializer_list<T> arr) {
    clear();
    for (auto& val : arr) {
        push_back(val);
    }
}
template<typename T>
List<T>& List<T>::operator= (List<T> const& list) {
    if (this == &list) {
        return *this;
    }
    clear();
    int size = list.getSize();
    for (int i = 0; i < size; i++) {
        push_back(list[i]);
    }
    return *this;
}

template<typename T>
T& List<T>::operator[] (int const position) const {
    if (position < 0 || position >= size) {
        throw std::invalid_argument("Incorrect position! Exceeded list range!");
    }
    int backposition = size - 1 - position;
    Node* temp;
    if (position < backposition) {
        temp = start;
        for (int i = 1; i <= position; i++) {
            temp = temp->next;
        }
    }
    else {
        temp = end;
        for (int i = 1; i <= backposition; i++) {
            temp = temp->prev;
        }
    }
    return temp->data;
}

template<typename T>
T& List<T>::operator[] (int const position) {
    if (position < 0 || position >= size) {
        throw std::invalid_argument("Incorrect position! Exceeded list range!");
    }
    int backposition = size - 1 - position;
    Node* temp;
    if (position < backposition) {
        temp = start;
        for (int i = 1; i <= position; i++) {
            temp = temp->next;
        }
    }
    else {
        temp = end;
        for (int i = 1; i <= backposition; i++) {
            temp = temp->prev;
        }
    }
    return temp->data;
}

/***************************** editing data **************************************/
template<typename T>
void List<T>::push_back(T _data) {
    if (size == 0) {
        initialize(_data);
    }
    else {
        Node* temp = end;
        temp->next = new Node { _data, nullptr, temp };
        end = temp->next;
        size++;
    }
}
template<typename T>
void List<T>::insert(T _data, int position) {
    if (position < 0 || position > size) {
        throw std::invalid_argument("Incorrect position! Exceeded list range!");
    }
    if (position == 0) {
        push_front(_data);
    }
    else if (position == size) {
        push_back(_data);
    }
    else {
        int backposition = size - 1 - position;
        if (position < backposition) {
            Node* temp = start;
            for(int i = 1; i < position; i++) {
                temp = temp->next;
            }
            Node* q = new Node { _data, temp->next, temp };
            temp->next = q;
            q->next->prev = q;
        }
        else {
            Node* temp = end;
            for (int i = 1; i < backposition; i++) {
                temp = temp->prev;
            }
            Node* q = new Node { _data, temp, temp->prev };
            temp->prev = q;
            q->prev->next = q;
        }
        size++;        
    }
}
template<typename T>
void List<T>::push_front(T _data) {
    if (size == 0) {
        initialize(_data);
    }
    else {
        start->prev = new Node{_data, start, nullptr};
        start = start->prev;
        size++;
    }
}
template<typename T>
void List<T>::replace(T _data, int position) {
    if (position < 0 || position >= size) {
        throw std::invalid_argument("Incorrect position! Exceeded list range!");
    }
    int backposition = size - 1 - position;
    Node* temp;
    if (position < backposition) {
        temp = start;
        for (int i = 1; i <= position; i++) {
            temp = temp->next;
        }
        temp->data = _data;
    }
    else {
        temp = end;
        for (int i = 1; i <= backposition; i++) {
            temp = temp->prev;
        }
        temp->data = _data;
    }
}
template<typename T>
void List<T>::importArrayToList(std::span<T> arr) {
    for (int i = 0; i < arr.size(); i++) {
        push_back(arr[i]);
    }
}
template<typename T>
void List<T>::pop_back() {
    Node* temp = end->prev;
    delete (end);
    temp->next = nullptr;
    end = temp;
    size--;
}
template<typename T>
void List<T>::erase(int position) {
    if (position < 0 || position >= size) {
        throw std::invalid_argument("Incorrect position! Exceeded list range!");
    }
    if (position == 0) {
        pop_front();
    }
    else if (position == size - 1) {
        pop_back();
    }
    else {
        Node* temp;
        Node* q;
        int backposition = size - 1 - position;
        if (position < backposition) {
            temp = start;
            for (int i = 1; i < position; i++) {
                temp=temp->next;
            }
            q = temp->next;
            temp->next = q->next;
            q->next->prev = temp;
        }
        else {
            temp = end;
            for (int i = 1; i < backposition; i++) {
                temp = temp->prev;
            }
            q = temp->prev;
            temp->prev = q->prev;
            q->prev->next = temp;
        }
        delete (q);
        size--;
    }
}
template<typename T>
void List<T>::pop_front() {
    Node* temp = start->next;
    delete (start);
    temp->prev = nullptr;
    start = temp;
    size--;
}
template<typename T>
void List<T>::clear() {
    if (size == 0) {
        return;
    }
    while (start->next != nullptr) {
        start = start->next;
        delete (start->prev);
        start->prev = nullptr;
    }
    delete (start);
    start = nullptr;
    end = nullptr;
    size = 0;
}
template<typename T>
void List<T>::sort(std::function<bool(T const&, T const&)> compareFunc) {
    C__sort(start, end, compareFunc);
    delete (start->prev);
    start->prev = nullptr;
}
template<typename T>
void List<T>::swap(int position1, int position2) {
    Node* temp1 = getNode(position1);
    Node* temp2 = getNode(position2);
    C__swap(temp1, temp2);
}
template<typename T>
void List<T>::reverse() {
    Node* temp1 = start;
    Node* temp2 = end;
    T holder;
    while (temp2->next != temp1 && temp1 != temp2) {
        T holder = temp1->data;
        temp1->data = temp2->data;
        temp2->data = holder;
        temp1 = temp1->next;
        temp2 = temp2->prev;
    }
}
/***************************** getting access to data **************************************/
template<typename T>
int List<T>::getSize() const { return size; }

template<typename T>
int List<T>::getPosition(T const& _data) const {
    Node* temp = start;
    int i = 0;
    while (temp != nullptr) {
        if (temp->data == _data)
            return i;
        temp = temp->next;
        i++;
    }
    return -1;
}
template<typename T>
int List<T>::count(T const& _data) const {
    int count = 0;
    Node* temp = start;
    while (temp != nullptr) {
        if (temp->data == _data) {
            count++;
        }
        temp = temp->next;
    }
    return count;
}
template<typename T>
void List<T>::printList() const {
    if (size == 0) {
        throw std::invalid_argument("This list is empty!");
    }
    Node* temp = start;
    std::cout << "[";
    while (temp->next != nullptr) {
        std::cout << temp->data << ", ";
        temp = temp->next;
    }
    std::cout << temp->data << "]\n";
}
/* ######################################################## */
/* ###################3 Made by Cub8 E##################### */
/* ######################################################## */
