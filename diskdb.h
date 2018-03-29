#ifndef DISKDB_H
#define DISKDB_H

#include "database.h"

class DiskDb : public Database {
    const std::vector<Newsgroup>& list() override;
    bool create(Newsgroup& ng) override;
    bool remove(unsigned int ngId) override;
    const std::vector<Article>& list(unsigned int ngId) override;
    const std::string* read(unsigned int ngId, unsigned int aId) override;
    bool write(const unsigned int ngId, Article& a) override;
    bool remove(const unsigned int ngId, const unsigned int aId) override;
};

#endif
