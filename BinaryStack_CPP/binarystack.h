#include <iostream>
#include <cmath>
#include <memory>

// pure evil
template<class T> 
inline T& unconst(T const& t){
    return const_cast<T&>(t);
}

class binarystack {
private:
    size_t stack_size;
    size_t stack_reserve;
    size_t stack_capacity;
    void* stack_base;

    template <typename T>
    void push(T& data);

    template <typename T>
    void pop(T& data);

    void resize(size_t new_stack_capacity);

public:
    binarystack(size_t reserve = 1);

    ~binarystack();

    // Push single element on to stack;
    template <typename T>
    binarystack& operator<<(T data);

    // Pop single element from stack;
    template <typename T>
    binarystack& operator>>(T& data);

    // Push array of elements on to stack;
    template <typename T, size_t array_length>
    binarystack& operator<<(T (&data)[array_length]);

    // Pop array of elements from stack;
    template <typename T, size_t array_length>
    binarystack& operator>>(T (&data)[array_length]);

    char* data() {
        return (char*)stack_base; 
    }

    size_t size() { 
        return stack_size; 
    }
};