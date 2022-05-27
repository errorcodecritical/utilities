#include "binarystack.h"

template <typename T>
void binarystack::push(T& data) {
    std::cout << "push: " << data << " to: stack_base + " << stack_size << std::endl; 
    
    // only because templates preserve const qualifier;
    unconst(*reinterpret_cast<T*>(static_cast<char*>(stack_base) + stack_size)) = data;
    stack_size = stack_size + sizeof(data);
};

template <typename T>
void binarystack::pop(T& data) {
    stack_size = stack_size - sizeof(data);
    data = *reinterpret_cast<T*>(static_cast<char*>(stack_base) + stack_size);

    std::cout << "pop: " << data << " from: stack_base + " << stack_size << std::endl; 
};

void binarystack::resize(size_t new_stack_capacity) {
    if (new_stack_capacity != stack_capacity) {
        void* new_stack_base = nullptr;

        do {
            new_stack_base = realloc(stack_base, new_stack_capacity);
        } while (new_stack_base == nullptr);

        stack_base = new_stack_base;
        stack_capacity = new_stack_capacity; 
    }
}

binarystack::binarystack(size_t reserve) {
    stack_capacity = reserve;
    stack_size = 0;
    stack_base = malloc(stack_capacity);
}

binarystack::~binarystack() {
    // warn if there is still data on the stack;
    // potential memory leak if additional space was allocated for it;
   
    if (stack_size > 0) {
        std::cout << "Warning: stack destructor called before all contents were removed." << std::endl;
    }

    free(stack_base);
}

template <typename T>
binarystack& binarystack::operator<<(T data) {
    size_t new_stack_size = stack_size + sizeof(data);
    size_t new_stack_capacity = stack_capacity;
    
    if (new_stack_size > SIZE_MAX / 2) {
        return *this;
    }

    while (new_stack_size > new_stack_capacity) {
        new_stack_capacity = new_stack_capacity * 2;
    }

    resize(new_stack_capacity);
    push(data);
    
    return *this;
}

template <typename T>
binarystack& binarystack::operator>>(T& data) {
    size_t new_stack_size = stack_size - sizeof(data);
    size_t new_stack_capacity = stack_capacity;

    if (new_stack_size > SIZE_MAX / 2) {
        return *this;
    }

    while (new_stack_size < new_stack_capacity / 2) {
        new_stack_capacity = new_stack_capacity / 2;
    }
    
    pop(data);
    resize(new_stack_capacity);
    
    return *this;
}

template <typename T, size_t array_length> 
binarystack& binarystack::operator<<(T (&data)[array_length]) {
    size_t new_stack_size = stack_size + sizeof(data);
    size_t new_stack_capacity = stack_capacity;
    
    if (new_stack_size > SIZE_MAX / 2) {
        return *this;
    }

    while (new_stack_size > new_stack_capacity) {
        new_stack_capacity = new_stack_capacity * 2;
    }

    resize(new_stack_capacity);
    
    for (int i = 0; i < array_length; i++) {
        push(data[array_length - i - 1]);
    }

    return *this;
}

template <typename T, size_t array_length>
binarystack& binarystack::operator>>(T (&data)[array_length]) {
    size_t new_stack_size = stack_size - sizeof(data);
    size_t new_stack_capacity = stack_capacity;

    if (new_stack_size > SIZE_MAX / 2) {
        return *this;
    }
    
    while (new_stack_size < new_stack_capacity / 2) {
        new_stack_capacity = new_stack_capacity / 2;
    }

    for (int i = 0; i < array_length; i++) {
        pop(data[array_length - i - 1]);
    }

    resize(new_stack_capacity);

    return *this;
}