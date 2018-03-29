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
    virtual std::vector<std::string> list() = 0;
    // creates a new newsgroup
    virtual bool create(newsgroup ng) = 0;
    // removes a newsgroup and it's articles
    virtual bool remove(unsigned int ngId) = 0;
    // lists articles in a newsgroup
    virtual std::vector<Article> list(unsigned int ngId) = 0;
    // reads an article
    virtual std::string read(unsigned int ngId,unsigned int aId) = 0;
    // writes to an article
    virtual bool write(unsigned int ngId,unsigned int aId, std::string text) = 0;
    // removes an article
    virtual bool remove(unsigned int ngId, unsigned int aId) = 0;
private:

};

#endif
