#include "dynarray.h"

int main() {
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