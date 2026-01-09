#pragma once
#include <Arduino.h>

template <typename T, size_t MaxObservers>
class Detector {
public:
    explicit Detector(T initial) : value(initial), prev(initial) {
        for (size_t i = 0; i < MaxObservers; ++i) seen[i] = false;
    }

    void set(const T& v) {
        if (v == value) return;
        prev = value;
        value = v;
        for (size_t i = 0; i < MaxObservers; ++i) seen[i] = false;
    }

    void amend(const T& v) {
        value = v;
    }

    void call() {
        for (size_t i = 0; i < MaxObservers; ++i) seen[i] = false;
    }

    bool changed(size_t id) {
        if (id >= MaxObservers) return false;
        if (!seen[id]) {
            seen[id] = true;
            return true;
        }
        return false;
    }

    const T& get() const { return value; }
    const T& getPrev() const { return prev; }

    template <typename U>
    Detector& operator+=(const U& d) { set(value + d); return *this; }

    template <typename U>
    Detector& operator-=(const U& d) { set(value - d); return *this; }

    template <typename U>
    Detector& operator*=(const U& d) { set(value * d); return *this; }

    template <typename U>
    Detector& operator/=(const U& d) { set(value / d); return *this; }

    template <typename U>
    Detector& operator%=(const U& d) { set(value % d); return *this; }

    template <typename U>
    T operator+(const U& d) const { return value + d; }

    template <typename U>
    T operator-(const U& d) const { return value - d; }

    template <typename U>
    T operator*(const U& d) const { return value * d; }

    template <typename U>
    T operator/(const U& d) const { return value / d; }

    template <typename U>
    T operator%(const U& d) const { return value % d; }

    template <typename U>
    bool operator==(const U& v) const { return value == v; }

    template <typename U>
    bool operator!=(const U& v) const { return value != v; }

    template <typename U>
    bool operator<(const U& v) const { return value < v; }

    template <typename U>
    bool operator>(const U& v) const { return value > v; }

    template <typename U>
    bool operator<=(const U& v) const { return value <= v; }

    template <typename U>
    bool operator>=(const U& v) const { return value >= v; }

    Detector& operator=(const T& v) {
        set(v);
        return *this;
    }

    operator const T&() const {
        return value;
    }

private:
    T value;
    T prev;
    bool seen[MaxObservers];
};
