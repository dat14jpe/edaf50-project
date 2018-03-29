#ifndef NEWS_H
#define NEWS_H

#include <string>

class NewsGroup {
public:
    NewsGroup(unsigned int theId, std::string thename) : id(theId), name(theName)  {}
private:
    unsigned int id;
    std::string name;
};
#endif
