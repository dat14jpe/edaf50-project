#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

struct Article {
    typedef unsigned int Id;
    Id id;
    std::string title, author, text;
};

#endif
