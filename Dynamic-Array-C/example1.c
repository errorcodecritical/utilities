#include "dynarray.h"

int main() {
    printf("%X\n", set.create);
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

    set.destroy(&nums);
    
    return 0;
}