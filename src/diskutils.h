#ifndef DISK_UTILS_H
#define DISK_UTILS_H

#include <sys/stat.h>
#include <dirent.h>
#include <fstream>

// Helper class for managing directories:
class Directory {
public:
    Directory(const std::string& path);
    operator bool() const;
    static bool make(const std::string& path);
    static bool remove(const std::string& path);
    
    struct Entry {
        ~Entry();
        
        bool operator!=(const Entry& o) const;
        Entry& operator++();
        std::string operator*() const;
        
    private:
        friend class Directory;
        Entry(DIR* dir, struct dirent* ent) ;
            
        DIR* dir;
        struct dirent* ent;
    };
    Entry begin();
    Entry end();
    
private:
    const std::string path;
};

// Helper class to read a file:
class FileReader {
public:
    FileReader(const std::string& path);
    operator bool() const;
    unsigned char readByte();
    int readInt();
    std::string readString();

private:
    std::ifstream f;
};

// Helper class to write a file:
class FileWriter {
public:
    FileWriter(const std::string& path);
    void writeByte(unsigned char c);
    void writeInt(int value);
    void writeString(const std::string& value);
    
private:
    std::ofstream f;
};

#endif
