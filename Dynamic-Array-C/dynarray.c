#include <stdio.h>
#include <stdlib.h>

#define assert(condition, error) if (!condition) {printf(error); return;} // static assertion

void __Memcpy(void* dest, void* src, int n) {
    for (int i = 0; i < n; i++) {
        ((char*)dest)[i] = ((char*)src)[i];
    }
}

void __Memmove(void* dest, void* src, int n) {
    char* temp = (char*)malloc(n * sizeof(char));

    for (int i = 0; i < n; i++) {
        ((char*)temp)[i] = ((char*)src)[i];
    }

    for (int i = 0; i < n; i++) {
        ((char*)dest)[i] = ((char*)temp)[i];
    }
    free(temp);
}

void __Memset(void* dest, char value, int n) {
    for (int i = 0; i < n; i++) {
        ((char*)dest)[i] = value;
    }
}

typedef struct __SetObject {
    int width;
    int length;
    int next;
    void* contents;
} set_t;

struct __SetMethods {
    set_t* (*create)(int width, int length);
    void (*destroy)(set_t** set_ptr);
    void (*insert)(set_t** set_ptr, int index, void* value);
    void (*remove)(set_t** set_ptr, int index);
} set;

void __Shift(set_t* this, int index, int shift) {
    int n_byte = ((this->next) - index) * this->width;

    __Memmove(
        &((char*)this->contents)[(index + shift) * this->width],
        &((char*)this->contents)[index * this->width], 
        n_byte
    );
    this->next += shift;
}

void __Grow(set_t** set_ptr, int amount) {
    set_t* this = realloc(*set_ptr, sizeof(set_t) + ((*set_ptr)->length + amount) * (*set_ptr)->width);
    assert(this, "Error: failed to grow set.\n");

    this->length += amount;
    this->contents = this + 1;
    *set_ptr = this;
}

set_t* __Create(int width, int length) {
    set_t* new_set = malloc(sizeof(set_t) + width * length);

    new_set->width = width;
    new_set->length = length;
    new_set->next = 0;
    new_set->contents = new_set + 1; // naughty >:)
    
    return new_set;
}

void __Insert(set_t** set_ptr, int index, void* value) {
    set_t* this = *set_ptr;
    assert(value, "Pointer to value must not be NULL.\n");

    if (this->next >= this->length) {
        __Grow(set_ptr, this->length / 2);
        this = *set_ptr;
    }

    if (index == this->next) {    
        __Memcpy((char*)this->contents + index * this->width, value, this->width);
        this->next += 1;
    } else if (index < this->next) {
        __Shift(this, index, 1);
        __Memcpy((char*)this->contents + index * this->width, value, this->width);
    } else {
        // idfk
    }
}

void __Remove(set_t** set_ptr, int index) {
    set_t* this = *set_ptr;
    
    if (index == this->next) {
        __Memset((char*)this->contents + index * this->width, 0, this->width);
        this->next -= 1;
    } else if (index < this->next) {
        __Shift(this, index, -1);
        __Memset((char*)this->contents + this->next * this->width, 0, this->width);
    } else {
        // still idfk
    }
}

void __Destroy(set_t** set_ptr) {
    free(*set_ptr);
    *set_ptr = 0;
}

struct __SetMethods set = {
    .create = __Create,
    .destroy = __Destroy,
    .insert = __Insert,
    .remove = __Remove
};

int main() {
    set_t* nums = set.create(sizeof(int), 10);

    for (int i = 0; i < 100; i++) {
        set.insert(&nums, nums->next, &i);
    }

    for (int i = 0; i < 40; i++) {
        set.remove(&nums, 25);
    }

    for (int i = 0; i < nums->length; i++) {
        printf("%d\n", ((int*)nums->contents)[i]);
    }   
    
    return 0;
}

int notmain() {
    set_t* names = set.create(sizeof(char*), 5);

    char* name1 = "Bob Marley";
    char* name2 = "Bob Ross";
    char* name3 = "Bob the Builder";
    char* name4 = "Bob 'Just Bob' Bob";

    set.insert(&names, names->next, &name1);
    set.insert(&names, names->next, &name2);
    set.insert(&names, names->next, &name3);
    set.insert(&names, names->next, &name4);

    for (int n = 0; n < names->next; n++) {
        printf("Name: %s\n", ((char**)names->contents)[n]);
    }

    set.remove(&names, 2);
    printf("\n\n");

    for (int n = 0; n < names->next; n++) {
        printf("Name: %s\n", ((char**)names->contents)[n]);
    }
    
    set.destroy(&names);

    return 0;
}