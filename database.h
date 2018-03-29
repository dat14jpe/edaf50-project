#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include "article.h"
#include "newsgroup.h"

/* Database Interface */
class Database {
public:
    // lists newsgourps;
    virtual std::vector<std::string> list();
    // creates a new newsgroup
    virtual bool create(newsgroup ng);
    // removes a newsgroup and it's articles
    virtual bool remove(unsigned int ngId);
    // lists articles in a newsgroup
    virtual std::vector<Article> list(unsigned int ngId);
    // reads an article
    virtual std::string read(unsigned int ngId,unsigned int aId);
    // writes to an article
    virtual bool write(unsigned int ngId,unsigned int aId, std::string text);
    // removes an article
    virtual bool remove(unsigned int ngId, unsigned int aId);
private:

};
#endif
