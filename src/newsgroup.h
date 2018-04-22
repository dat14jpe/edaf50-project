#ifndef NEWS_H
#define NEWS_H

#include <string>
#include "article.h"

struct Newsgroup {
    typedef unsigned int Id;
    Id id;
    std::string name;
    Article::Id maxId; // highest article ID used before
};

#endif
