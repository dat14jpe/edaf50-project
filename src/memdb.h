#ifndef MEMDB_H
#define MEMDB_H

#include <map>
#include "database.h"

class MemDb: public Database {
public:
    std::vector<Newsgroup> list() override;
    bool create(const Newsgroup&) override;
    bool remove(Newsgroup::Id) override;
    std::vector<Article> list(Newsgroup::Id) override;
    Article read(Newsgroup::Id, Article::Id) override;
    void createArticle(Newsgroup::Id, const Article&) override;
    void removeArticle(Newsgroup::Id, Article::Id) override;
    
private:
    Newsgroup::Id maxId = 0u; // highest newsgroup ID used before
    std::map<Newsgroup::Id, Newsgroup> newsgroups;
    std::map<Newsgroup::Id, std::map<Article::Id, Article>> articles;
};

#endif
