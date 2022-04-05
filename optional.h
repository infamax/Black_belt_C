#pragma once

#include <utility>

// Исключение при обращении к пустому optional
struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) unsigned char data[sizeof(T)]{};
    bool defined = false;

public:
    Optional() = default;
    Optional(const T& elem) {
        new (data) T(elem);
        defined = true;
    }

    Optional(T&& elem) {
        new (data) T(std::move(elem));
        defined = true;
    }

    Optional(const Optional& other) {
        if (other.defined) {
            new (data) T(*other);
            defined = true;
        }
    }

    Optional(Optional&& other) {
        if (other.defined) {
            new (data) T(std::move(*other));
            other.defined = false;
        }
    }

    Optional& operator=(const T& elem) {
        if (defined) {
            *this = elem;
        }
        new (data) T(elem);
        defined = true;
        return *this;
    }

    Optional& operator=(T&& elem) {
        if (defined) {
            *this = std::move(elem);
        }
        new (data) T(std::move(elem));
        defined = true;
        return *this;
    }

    Optional& operator=(const Optional& other) {
        if (!other.defined) {
            // Если мы пытаемся присвоить к неопределенному объекту сбрасываем
            Reset();
        } else if (defined) {
            **this = *other;
        } else {
            new (data) T(*other);
            defined = true;
        }
        return *this;
    }

    Optional& operator=(Optional&& other) {
        if (!other.defined) {
            // Если мы пытаемся присвоить к неопределенному объекту сбрасываем
            Reset();
        } else if (defined) {
            **this = *other;
        } else {
            new (data) T(*other);
            defined = true;
        }
        return *this;
    }

    bool HasValue() const {
        return defined;
    }

    // Эти операторы не должны делать никаких проверок на пустоту.
    // Проверки остаются на совести программиста.
    T& operator*() {
        return *reinterpret_cast<T*>(data);
    }

    const T& operator*() const {
        return *reinterpret_cast<const T*>(data);
    }

    T* operator->() {
        return reinterpret_cast<T*>(data);
    }

    const T* operator->() const {
        return reinterpret_cast<const T*>(data);
    }

    // Генерирует исключение BadOptionalAccess, если объекта нет
    T& Value() {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return **this; // *this -> object Optional, // **this *Optional
    }

    const T& Value() const {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return **this;
    }

    void Reset() {
        if (defined) {
            // Необходимо понять как вызвать деструктор у объекта T явно
            this->Value().~T();
            defined = false;
        }
    }

    ~Optional() {
        Reset();
    }
};

