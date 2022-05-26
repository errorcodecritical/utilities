#include <iostream>
#include <memory>

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

    inline void* top() {
        return reinterpret_cast<void*>(static_cast<char*>(stack_base) + stack_size);
    };

public:
    binarystack(size_t reserve = 1);

    ~binarystack();

    template <typename T>
    binarystack& operator<<(T data);

    template <typename T>
    binarystack& operator>>(T& data);

    template <typename T, size_t array_length>
    binarystack& operator<<(T (&data)[array_length]);

    template <typename T, size_t array_length>
    binarystack& operator>>(T (&data)[array_length]);

    char* data() { return (char*)stack_base; }
    size_t size() { return stack_size; }
};

binarystack::binarystack(size_t reserve) {
    stack_capacity = reserve;
    stack_size = 0;
    stack_base = malloc(stack_capacity);
}

binarystack::~binarystack() {
    // warn if there is still data on the stack, potential memory leak!
    free(stack_base);
}

template <typename T>
binarystack& binarystack::operator<<(T data) {
    size_t new_stack_size = stack_size + sizeof(data);
    size_t new_stack_capacity = stack_capacity;
    void* new_stack_base = stack_base;
    
    while (new_stack_size > new_stack_capacity) {
        new_stack_capacity = new_stack_capacity * 2;
    }
    
    if (new_stack_capacity != stack_capacity) {
        std::cout << "growing: " << new_stack_capacity << std::endl;

        do {
            new_stack_base = realloc(stack_base, new_stack_capacity);
        } while (new_stack_base == nullptr);
    }

    T* stack_top = new(top()) T();
    *stack_top = data;

    int64_t dist = ((int64_t)stack_top - (int64_t)stack_base);
    std::cout << "push: " << *stack_top << " to: stack_base + " << dist << std::endl;
    
    stack_size = new_stack_size;
    stack_capacity = new_stack_capacity;
    stack_base = new_stack_base;

    return *this;
}

template <typename T>
binarystack& binarystack::operator>>(T& data) {
    size_t new_stack_size = stack_size - sizeof(data);
    size_t new_stack_capacity = stack_capacity;
    void* new_stack_base = stack_base;

    T* stack_top = reinterpret_cast<T*>(static_cast<char*>(stack_base) + new_stack_size);
    data = *stack_top;

    int64_t dist = ((int64_t)stack_top - (int64_t)stack_base);
    std::cout << "pop: " << *stack_top << " from: stack_base + " << dist << std::endl;
        
    while (new_stack_size < new_stack_capacity / 2) {
        new_stack_capacity = new_stack_capacity / 2;
    }
    
    if (new_stack_capacity != stack_capacity) {
        std::cout << "shrinking: " << new_stack_capacity << std::endl;

        do {
            new_stack_base = realloc(stack_base, new_stack_capacity);
        } while (new_stack_base == nullptr);
    }
    
    stack_size = new_stack_size;
    stack_capacity = new_stack_capacity;
    stack_base = new_stack_base;
    
    return *this;
}

template <typename T, size_t array_length> 
binarystack& binarystack::operator<<(T (&data)[array_length]) {
    for (int i = 0; i < array_length; i++) {
        *this << data[array_length - i - 1];
    }
    // size_t new_stack_size = stack_size + sizeof(data);
    // size_t new_stack_capacity = stack_capacity;
    // void* new_stack_base = stack_base;
    
    // while (new_stack_size > new_stack_capacity) {
    //     new_stack_capacity = new_stack_capacity * 2;
    // }
    
    // if (new_stack_capacity != stack_capacity) {
    //     std::cout << "growing: " << new_stack_capacity << std::endl;

    //     do {
    //         new_stack_base = realloc(stack_base, new_stack_capacity);
    //     } while (new_stack_base == nullptr);
    // }
    
    // std::cout << new_stack_size << std::endl;
    // std::cout << new_stack_capacity << std::endl;

    // T* temp = (T*)top();

    // for (int i = 0; i < array_length; i++) {
    //     T* stack_top = new(temp++) T();
    //     *stack_top = data[i];

    //     int64_t dist = ((int64_t)stack_top - (int64_t)stack_base);
    //     std::cout << "push: " << *stack_top << " to: stack_base + " << dist << std::endl;
    // }

    // stack_size = new_stack_size;
    // stack_capacity = new_stack_capacity;
    // stack_base = new_stack_base;

    return *this;
}

template <typename T, size_t array_length>
binarystack& binarystack::operator>>(T (&data)[array_length]) {
size_t new_stack_size = stack_size - sizeof(data);
    for (int i = 0; i < array_length; i++) {
        *this >> data[i];
    }
    
    return *this;
}

int main() {
    binarystack stack;

    int set_original[] = {0, 1, 2, 3, 4, 5};
    int set_cloned[6];

    char str_original[] = "abcdefghijklmnopqrstuvwxyz";
    char str_cloned[9];

    int num;
    char chr;
    double dbl;

    stack << str_original << 1337 << '@' << set_original << 69.0;

    stack >> dbl >> set_cloned >> chr >> num >> str_cloned;

    return 0;
}
