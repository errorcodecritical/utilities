#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>
#include <unordered_map>

class container {
private:
    struct {
        std::string identifier;
        container* base = NULL;
        std::unordered_map <std::string, container*> contents;
    } data;

    friend std::string getNextAvailableID(std::string identifier, container* base);
public:
    container(std::string identifier = "Container", container* base = NULL);

    ~container();

    std::string name(std::string identifier = std::string());

    container& parent(container* base = NULL);

    std::unordered_map <std::string, container*>& children();
};

void lockenv(bool state);

#endif