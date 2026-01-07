#pragma once
#include <Arduino.h>

template <typename T, size_t MaxObservers>
class Detector {
public:
    explicit Detector(T initial) : value(initial) {
        for (size_t i = 0; i < MaxObservers; ++i) seen[i] = false;
    }

    void set(const T& v) {
        if (v == value) return;

        value = v;
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

    const T& get() const {
        return value;
    }

private:
    T value;
    bool seen[MaxObservers];
};
