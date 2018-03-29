#ifndef MEMDB_H
#define MEMDB_H

#include "database.h"

class MemDb: public Database {
    const std::vector<std::string>& list() override;
    bool create(NewsGroup ng) override;
    bool remove(unsigned int ngId) override;
    const std::vector<Article>& list(unsigned int ngId) override;
    const std::string& read(unsigned int ngId, unsigned int aId) override;
    bool write(const unsigned int ngId, const unsigned int aId, const std::string& text) override;
    bool remove(const unsigned int ngId, const unsigned int aId) override;
};

#endif
