#ifndef ARTICLE_H
#define ARTICLE_H

#include <string>

class Article {
public:
    Article(unsigned int theId, std::string theAuthor, std::string theText): id(theId), author(theAuthor), text(theText) {} 
private:
    unsigned int id;
    std::string author;
    std::string text;
};
#endif
