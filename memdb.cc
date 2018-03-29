#include "memdb.h"

const std::vector<Newsgroup>& MemDb::list() {

}

bool MemDb::create(Newsgroup& ng)  {

}

bool MemDb::remove(unsigned int ngId) {

}

const std::vector<Article>& MemDb::list(unsigned int ngId) {

}

const std::string* MemDb::read(unsigned int ngId, unsigned int aId) {

}

bool MemDb::write(const unsigned int ngId, Article& a)  {

}

bool MemDb::remove(const unsigned int ngId, const unsigned int aId) {

}
