#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "article.h"
#include "newsgroup.h"

struct NewsgroupNotFoundException {};
struct ArticleNotFoundException {};

/* Database Interface */
class Database {
public:
    virtual ~Database() {}
    
    // Lists newsgroups
    virtual std::vector<Newsgroup> list() = 0;
    
    // Creates a new newsgroup
    virtual bool create(const Newsgroup&) = 0;
    
    // Removes a newsgroup and it's articles
    virtual bool remove(Newsgroup::Id) = 0;
    
    // Lists articles in a newsgroup
    virtual std::vector<Article> list(Newsgroup::Id) = 0;
    
    // Reads an article
    virtual Article read(Newsgroup::Id, Article::Id) = 0;
    
    // Creates an article
    virtual void createArticle(Newsgroup::Id, const Article&) = 0;
    
    // Removes an article
    virtual void removeArticle(Newsgroup::Id, Article::Id) = 0;
};

#endif
