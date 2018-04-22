#include "memdb.h"
#include <algorithm>

using std::remove_if;
using std::find_if;


std::vector<Newsgroup> MemDb::list() {
    std::vector<Newsgroup> result;
    result.reserve(newsgroups.size());
    for (auto& ng : newsgroups) {
        result.push_back(ng.second);
    }
    return result;
}

bool MemDb::create(const Newsgroup& ng)  {
    auto ngsIndex = find_if(newsgroups.begin(), newsgroups.end(),
        [&](std::pair<const unsigned int, Newsgroup>& n) 
            { return n.second.name == ng.name; });
    if (ngsIndex != newsgroups.end()) return false; // name already in use
    auto& n = newsgroups[++maxId] = ng;
    n.id = maxId;
    n.maxId = 0u;
    articles[maxId] = {};
    return true;
}

bool MemDb::remove(Newsgroup::Id ngId) {
    return newsgroups.erase(ngId) && articles.erase(ngId);
}

std::vector<Article> MemDb::list(Newsgroup::Id ngId) {
    auto ngs = articles.find(ngId);
    if (articles.end() == ngs) throw NewsgroupNotFoundException();
    auto& as = articles[ngId];
    std::vector<Article> result;
    for (auto& a : as) {
        result.push_back(a.second);
    }
    return result;
}

Article MemDb::read(Newsgroup::Id ngId, Article::Id aId) {
    auto ngs = articles.find(ngId);
    if (articles.end() == ngs) throw NewsgroupNotFoundException();
    auto as = ngs->second.find(aId);
    if (ngs->second.end() == as) throw ArticleNotFoundException();
    return as->second;
}

void MemDb::createArticle(Newsgroup::Id ngId, const Article& a)  {
    auto ngs = newsgroups.find(ngId);
    if (newsgroups.end() == ngs) throw NewsgroupNotFoundException();
    auto& ng = ngs->second;
    auto& as = articles[ngId];
    as[++ng.maxId] = a;
    as[ng.maxId].id = ng.maxId;
}

void MemDb::removeArticle(Newsgroup::Id ngId, Article::Id aId) {
    auto ngs = articles.find(ngId);
    if (articles.end() == ngs) throw NewsgroupNotFoundException();
    if (!ngs->second.erase(aId)) throw ArticleNotFoundException();
}
