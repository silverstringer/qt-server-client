#pragma once

#include <thread>
#include <iostream>
#include <chrono>
#include <mutex>
#include <vector>
#include <cstdarg>
#include <cassert>
#include <unordered_map>


#define CAPACITY 10000

template<typename T>
class container_map {
    using MuxGuard = std::lock_guard<std::recursive_mutex>;
    mutable std::recursive_mutex m_mutex;
    std::unordered_map<T, T> m_elements;
public:
    container_map() {}

    container_map(container_map const &other) {
        MuxGuard lock(other.m_mutex);
        m_elements = other.m_elements;
    }


    bool add(T &key, T value) {
        //not save thread
        MuxGuard locker(m_mutex);
        auto[iterator, success] = m_elements.insert({key, value});
        return success;
    }


    bool add(T value) {
        //not save thread
        MuxGuard locker(m_mutex);
        auto[iterator, success] = m_elements.insert({m_elements.size() + 1, value});
        return success;
    }

    const T &get(const uint64_t _index) const {
        assert(_index <= m_elements.size() && "Index out of range ");
        MuxGuard locker(m_mutex);
//        auto iter =  m_elements.begin();
//        std::advance(iter, _index);
//        return *iter;
        return m_elements.at(_index);
    }


    void print() const;

    bool empty() const {
        MuxGuard locker(m_mutex);
        return m_elements.empty();
    }

    size_t size() const {
        MuxGuard locker(m_mutex);
        return m_elements.size();
    }


};

template<typename T>
void container_map<T>::print() const {
    MuxGuard locker(m_mutex);
    for (const auto &e:m_elements)
        std::cout << "Elements:" << e.first << e.second << "\n";

}

template<typename T>
bool fillContainer(container_map<T> &cont, T &key, T data) {
    auto result = cont.add(key, data);
    return result;
}

//template <>
//bool fillContainer(container_map<std::string> &cont, std::string &key, std::string data)
//{
//
//    auto result = cont.add(key,data);
//    return result;
//}



template<typename T>
T getValue(container_map<T> &cont, const int _index) {
    return cont.get(_index);
}




