#include "diskdb.h"
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>
#include <fstream>
#include "diskutils.h"
#include <stdexcept>

using std::find_if;
using std::string;

DiskDb::DiskDb(const string& path)
    : path(path)
{
    Directory::make(path);    
    if (!FileReader{path + "db"}) { // no file storing max ID - create it
        FileWriter{path + "db"}.writeInt(0);
    }
}

std::vector<Newsgroup> DiskDb::list() {
    std::vector<Newsgroup> result;
    for (auto sid : Directory(path)) {
        Newsgroup ng{};
        try {
            ng.id = std::stoi(sid);
            if (ng.id <= 0) throw std::invalid_argument("less-than-zero ID");
        } catch (const std::invalid_argument&) {
            continue; // not a newsgroup
        }
        result.push_back(readNewsgroup(ng.id));
    }
    
    // - sort seemingly required for second test client
    std::sort(result.begin(), result.end(), 
        [](const Newsgroup& a, const Newsgroup& b) { return a.id < b.id; });
    return result;
}

bool DiskDb::create(const Newsgroup& ng) {
    auto ngs = list();
    auto ngsIndex = find_if(ngs.begin(), ngs.end(),
        [&](const Newsgroup& n) { return n.name == ng.name; });
    if (ngsIndex != ngs.end()) return false; // name already in use
    
    auto id = FileReader{path + "db"}.readInt();
    FileWriter{path + "db"}.writeInt(++id);
    Directory::make(path + std::to_string(id));
    auto n = ng;
    n.maxId = 0u;
    writeNewsgroup(id, n);
    
    return true;
}

bool DiskDb::remove(Newsgroup::Id ngId) {
    Directory dir{newsgroupDirName(ngId)};
    if (!dir) return false; // doesn't exist
    // - note that this explicitly removes files created by this program:
    // if the directory contains other files, it won't be removed
    for (auto sid : dir) {
        Article::Id aId;
        try {
            aId = std::stoi(sid);
            if (aId <= 0) throw std::invalid_argument("less-than-zero ID");
        } catch (const std::invalid_argument&) {
            continue; // not an article
        }
        Directory::remove(articleFileName(ngId, aId));
    }
    Directory::remove(newsgroupFileName(ngId));
    return Directory::remove(newsgroupDirName(ngId));
}

std::vector<Article> DiskDb::list(Newsgroup::Id ngId) {
    Directory dir{newsgroupDirName(ngId)};
    if (!dir) throw NewsgroupNotFoundException();
    std::vector<Article> result;
    for (auto aid : dir) {
        Article a{};
        try {
            a.id = std::stoi(aid);
            if (a.id <= 0) throw std::invalid_argument("less-than-zero ID");
        } catch (const std::invalid_argument&) {
            continue; // not an article
        }
        result.push_back(readArticle(ngId, a.id));
    }
    
    std::sort(result.begin(), result.end(), 
        [](const Article& a, const Article& b) { return a.id < b.id; });
    return result;
}

Article DiskDb::read(Newsgroup::Id ngId, Article::Id aId) {
    testNewsgroupExists(ngId);
    auto a = readArticle(ngId, aId);
    if (!a.id) throw ArticleNotFoundException();
    return a;
}

void DiskDb::createArticle(Newsgroup::Id ngId, const Article& a) {
    testNewsgroupExists(ngId);
    auto ng = readNewsgroup(ngId);
    auto aId = ++ng.maxId;
    writeNewsgroup(ngId, ng);
    writeArticle(ngId, aId, a);
}

void DiskDb::removeArticle(Newsgroup::Id ngId, Article::Id aId) {
    testNewsgroupExists(ngId);
    auto name = articleFileName(ngId, aId);
    if (!Directory::remove(name)) throw ArticleNotFoundException();
}


void DiskDb::writeNewsgroup(Newsgroup::Id id, const Newsgroup& ng) {
    FileWriter f{newsgroupFileName(id)};
    f.writeInt(ng.maxId);
    f.writeString(ng.name);
}

void DiskDb::writeArticle(Newsgroup::Id ngId, Article::Id aId, const Article& a) {
    FileWriter f{articleFileName(ngId, aId)};
    f.writeString(a.title);
    f.writeString(a.author);
    f.writeString(a.text);
}

Newsgroup DiskDb::readNewsgroup(Newsgroup::Id id) {
    FileReader f{newsgroupFileName(id)};
    Newsgroup ng{};
    ng.id = id;
    ng.maxId = f.readInt();
    ng.name = f.readString();
    return ng;
}

Article DiskDb::readArticle(Newsgroup::Id ngId, Article::Id aId) {
    FileReader f{articleFileName(ngId, aId)};
    Article a{};
    if (!f) return a;
    a.id = aId;
    a.title = f.readString();
    a.author = f.readString();
    a.text = f.readString();
    return a;
}

void DiskDb::testNewsgroupExists(Newsgroup::Id ngId) {
    if (!Directory{newsgroupDirName(ngId)}) throw NewsgroupNotFoundException();
}

string DiskDb::newsgroupDirName(Newsgroup::Id ngId) const {
    return path + std::to_string(ngId);
}
string DiskDb::newsgroupFileName(Newsgroup::Id ngId) const {
    return newsgroupDirName(ngId) + "/newsgroup";
}
string DiskDb::articleFileName(Newsgroup::Id ngId, Article::Id aId) const {
    return newsgroupDirName(ngId) + '/' + std::to_string(aId);
}
