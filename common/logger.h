#pragma once

#include <vector>
#include <sstream>
#include <iterator>
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <QDir>


namespace Log {

using string = std::string;

class CSVLogger
{
    string path;
    string fileName;
    string delimeter;
    bool write_enabled;

public:
    CSVLogger(string directory = string(getenv("HOME")) + "/.incoma", string delm = ";", bool write = false ) :
        path(directory), delimeter(delm), write_enabled(write)
    {}
    template<typename T>
    void addDatainRow(T first, T last);

    void inline set_write_enabled(bool write){this->write_enabled = write;}

    void inline set_path(string directory){
        this->path = directory + "/";
    }
    string inline getPath (){return path;}
    string inline getFilename() { return fileName;}

    void inline set_filename(string filename){
        if(!write_enabled)
            return;
        QDir home(QDir::homePath() + "/.incoma");
        if(!home.exists())
            home.mkdir(home.path());
        this->fileName = path + filename;
    }
    void inline settings(bool is_write = false, string path = " ", string filename = " ")
    {
        write_enabled = is_write;
        if(!path.empty())
            set_path(path);
        if(!filename.empty())
            set_filename(filename);
    }
};


template<typename T>
void CSVLogger::addDatainRow(T first, T last)
{
    if(!write_enabled)
        return;

    std::fstream file;

    try {
        file.open(fileName, std::ios::out | std::ios::app);
    }
    catch (std::exception &ex)
    {
        std::cerr<<ex.what() + fileName;
    }

    for (; first != last; )
    {
        file << *first;
        if (++first != last)
            file << delimeter;
    }
    file << "\n";

    file.close();
}

}

