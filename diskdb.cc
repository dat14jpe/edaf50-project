#include "diskdb.h"
#include <algorithm>

const std::vector<Newsgroup>& DiskDb::list() {
    return ngs;
}

bool DiskDb::create(Newsgroup& ng)  {
    ng.id = maxId++;
    ngs.push_back(ng)
}

bool DiskDb::remove(unsigned int ngId) {
    auto ngsIndex = std::remove_if(ngs.begin(), ngs.end(), [&](const unsigned int ngIdO) {return ngId == ngIdO;});
    if (ngsIndex == ngs.end()) return false;
    return true;
}

const std::vector<Article>& DiskDb::list(unsigned int ngId) {
    auto ngsIndex = find_if(ngs.begin(), ngs.end(), [&](const unsigned int ngIdO) {return ngId == ngIdO;});
    if (ngsIndex == ngs.end()) return nullptr;
    return nsgIndex->articles;
}

const std::string* DiskDb::read(unsigned int ngId, unsigned int aId) {
    auto ngsIndex = find_if(ngs.begin(), ngs.end(), [&](const unsigned int ngIdO) {return ngId == ngIdO;});
    if (ngsIndex == ngs.end()) return nullptr;

    auto articleIndex = find_if(ngsIndex->begin(), ngsIndex->end(), [&](const unsigned int artIdO) {return aId == artIdO;});

    if (articleIndex == ngs.end()) return nullptr;

    return &articleIndex->text;
}

bool DiskDb::write(const unsigned int ngId, Article& a)  {
    auto ngsIndex = find_if(ngs.begin(), ngs.end(), [&](const unsigned int ngIdO) {return ngId == ngIdO;});
    if (ngsIndex == ngs.end()) return false;

    unsigned int maxId = ngsIndex->maxId++;
    a.id = maxId;
    ngs.push_back(a);
    return true;
}

bool DiskDb::remove(const unsigned int ngId, const unsigned int aId) {

}
