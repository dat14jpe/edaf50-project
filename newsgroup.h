#ifndef NEWS_H
#define NEWS_H

#include <string>
#include <vector>
#include "article.h"

struct Newsgroup {
    unsigned int id;
    std::string name;
    std::vector<Article> articles;
    unsigned int maxId = 0u;
};

#endif
