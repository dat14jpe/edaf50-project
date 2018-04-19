#ifndef DISKDB_H
#define DISKDB_H

#include "database.h"
#include <string>

class DiskDb : public Database {
public:
    DiskDb(const std::string& path = "db/");
    
    std::vector<Newsgroup> list() override;
    bool create(const Newsgroup&) override;
    bool remove(Newsgroup::Id) override;
    std::vector<Article> list(Newsgroup::Id) override;
    Article read(Newsgroup::Id, Article::Id) override;
    void createArticle(Newsgroup::Id, const Article&) override;
    void removeArticle(Newsgroup::Id, Article::Id) override;
    
private:
    const std::string path;
    
    void writeNewsgroup(Newsgroup::Id, const Newsgroup&);
    void writeArticle(Newsgroup::Id, Article::Id, const Article&);
    Newsgroup readNewsgroup(Newsgroup::Id);
    Article readArticle(Newsgroup::Id, Article::Id);
    void testNewsgroupExists(Newsgroup::Id);
    std::string newsgroupDirName(Newsgroup::Id) const;
    std::string newsgroupFileName(Newsgroup::Id) const;
    std::string articleFileName(Newsgroup::Id, Article::Id) const;
};

#endif
