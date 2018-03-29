#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "article.h"
#include "newsgroup.h"

/* Database Interface */
class Database {
public:
    virtual ~Database() {}
    // lists newsgourps;
    virtual const std::vector<Newsgroup>& list() = 0;
    // creates a new newsgroup
    virtual bool create(Newsgroup& ng) = 0;
    // removes a newsgroup and it's articles
    virtual bool remove(unsigned int ngId) = 0;
    // lists articles in a newsgroup
    virtual const std::vector<Article>& list(unsigned int ngId) = 0;
    // reads an article
    virtual const std::string* read(unsigned int ngId, unsigned int aId) = 0;
    // writes to an article
    virtual bool write(unsigned int ngId, unsigned int aId, Article& a) = 0;
    // removes an article
    virtual bool remove(unsigned int ngId, unsigned int aId) = 0;

private:
    unsigned int maxId = 0u;
};

#endif
