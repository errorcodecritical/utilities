#include "container.hpp"

container __nullbase = container("null");
bool __envlocked = true;

container::container(std::string identifier, container* base) {
    if (__envlocked) { return; }
    if (base) {
        data.identifier = getNextAvailableID(identifier, base);
        data.base = base;
        data.base->data.contents.insert_or_assign(data.identifier, this);
    } else {
        data.identifier = getNextAvailableID(identifier, &__nullbase);
        data.base = &__nullbase;
        data.base->data.contents.insert_or_assign(data.identifier, this);
    }
}

container::~container() {
    if (__envlocked) { return; }

    for (auto kv : data.contents) {
        kv.second->parent(&__nullbase);
    }
}

std::string container::name(std::string identifier) {
    if (!identifier.empty() && !__envlocked) {
        data.base->data.contents.erase(data.identifier);
        data.identifier = getNextAvailableID(identifier, data.base);
    }
    return data.identifier;
}

container& container::parent(container* base) {
    if (base && !__envlocked) {
        data.base->data.contents.erase(data.identifier);
        data.identifier = getNextAvailableID(data.identifier, data.base);
        data.base = base;
        data.base->data.contents.insert_or_assign(data.identifier, this);
    }
    return *data.base;
}

std::unordered_map <std::string, container*>& container::children() {
    return data.contents;
}

std::string getNextAvailableID(std::string identifier, container* base) {
    if (base->data.contents.find(identifier) == base->data.contents.end()) {
        return identifier;
    }

    std::string result = identifier;
    for (int attempt = 1; base->data.contents.find(result) != base->data.contents.end(); attempt++) {
        result = identifier + "(" + std::to_string(attempt) + ")";
    }
    return result;
}

void lockenv(bool state) {
    __envlocked = state;
}