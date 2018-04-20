#include "diskutils.h"

using std::string;

Directory::Directory(const string& path) : path(path) {}

Directory::operator bool() const { // checks if the directory can be opened
    auto dir = opendir(path.c_str());
    if (dir) closedir(dir);
    return dir;
}
bool Directory::make(const string& path) {
    return 0 == mkdir(path.c_str(), 0775);
}
bool Directory::remove(const string& path) {
    // - is this correct if the file didn't exist? Check
    return 0 == ::remove(path.c_str());
}

Directory::Entry::~Entry() {
    if (dir) closedir(dir);
}

bool Directory::Entry::operator!=(const Entry& o) const {
    return ent != o.ent;
}
Directory::Entry& Directory::Entry::operator++() {
    ent = readdir(dir);
    return *this;
}
string Directory::Entry::operator*() const {
    return ent->d_name;
}
    
Directory::Entry::Entry(DIR* dir, struct dirent* ent) 
    : dir(dir), ent(ent) {}
        
Directory::Entry Directory::begin() {
    auto dir = opendir(path.c_str());
    return {dir, readdir(dir)};
}

Directory::Entry Directory::end() {
    return {nullptr, nullptr};
}


FileReader::FileReader(const string& path) : f(path, std::ios::binary) {}

FileReader::operator bool() const { return f.is_open(); }

unsigned char FileReader::readByte() {
    char b;
    f.read(&b, 1);
    return b;
}

int FileReader::readInt() {
    int value = 0;
    for (auto i = 3; i >= 0; --i) {
        value |= readByte() << (8 * i);
    }
    return value;
}

string FileReader::readString() {
    string s;
    s.resize(readInt());
    for (auto& c : s) {
        c = readByte();
    }
    return s;
}


FileWriter::FileWriter(const string& path) : f(path, std::ios::binary) {}

void FileWriter::writeByte(unsigned char c) {
    char b = c;
    f.write(&b, 1);
}

void FileWriter::writeInt(int value) {
    for (auto i = 3; i >= 0; --i) {
        writeByte((value >> (8 * i)) & 0xFF);
    }
}

void FileWriter::writeString(const string& value) {
    writeInt(value.size());
    for (auto c : value) {
        writeByte(c);
    }
}

