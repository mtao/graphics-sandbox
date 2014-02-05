#ifndef LOADER_H
#define LOADER_H
#include <string>

template
class Loader {
    public:
        virtual void open(const std::string& str) = 0;
        virtual void save(const std::string& str) = 0;
    private:


};

#endif