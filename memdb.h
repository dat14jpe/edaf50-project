#ifndef MEMDB_H
#define MEMDB_H

#include "database.h"

class MemDb: public Database {
public:
    const std::vector<Newsgroup>& list() override;
    bool create(Newsgroup ng) override;
    bool remove(unsigned int ngId) override;
    const std::vector<Article>& list(unsigned int ngId) override;
    const std::string& read(unsigned int ngId, unsigned int aId) override;
    bool write(const unsigned int ngId, const unsigned int aId, const std::string& text) override;
    bool remove(const unsigned int ngId, const unsigned int aId) override;
private:
    std::vector<Newsgroup> ng;
};

#endif
