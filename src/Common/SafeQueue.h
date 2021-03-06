#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <string>
#include <queue>
#include <mutex>
#include <cstdint>
#include <condition_variable>

#include <iostream>

template <class T>
class SafeQueue {
private:
	std::mutex mtx;
	std::condition_variable cv_empty;
	std::queue<T> elems;
	bool blocking;

public:
	explicit SafeQueue(bool blocking = true);

	void push(const T& elem);
	void pop(T& elem);
	bool get(T& elem);
	bool empty();
	uint32_t size();
	void clear();

	SafeQueue(SafeQueue&& otra) = delete;
    SafeQueue(const SafeQueue& otra) = delete;
    SafeQueue& operator=(const SafeQueue& otra) = delete;
    SafeQueue& operator=(SafeQueue&& otra) = delete;
};

template<class T>
SafeQueue<T>::SafeQueue(bool blocking): blocking(blocking) {}

template <class T>
void SafeQueue<T>::push(const T& elem) {
    {
        std::unique_lock<std::mutex> lck(mtx);
        elems.push(elem);
    }
    cv_empty.notify_one();
}

template <class T>
void SafeQueue<T>::pop(T& elem) {
    std::unique_lock<std::mutex> lck(mtx);
    //Mientras este vacia y la cola sea bloqueante, espero.
    while(elems.empty() && blocking) {
        cv_empty.wait(lck);
    }
    
    if (elems.empty() && !blocking) {
        elem = NULL;
        return;
    }
    elem = elems.front();
    elems.pop();
}

template <class T>
bool SafeQueue<T>::empty() {
    std::unique_lock<std::mutex> lck(mtx);
    return elems.empty();
}

template <class T>
uint32_t SafeQueue<T>::size() {
    std::unique_lock<std::mutex> lck(mtx);
    return elems.size();
}

template<class T>
bool SafeQueue<T>::get(T &elem) {
    this->pop(elem);
    return !(elem == NULL);
}

template<class T>
void SafeQueue<T>::clear() {
    while (!elems.empty()) {
        elems.pop();
    }
}

#endif // BLOCKING_QUEUE_H
