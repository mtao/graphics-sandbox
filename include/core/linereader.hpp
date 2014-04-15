#ifndef LINEREADER_HPP
#define LINEREADER_HPP
#include <fstream>
#include <sstream>
#include <memory>
namespace mtao {

class FileParser {
    public:
        FileParser(const std::string& filename);
        class FileIterator {
            public:
                FileIterator() {}
                FileIterator(const FileIterator& other): m_str(other.m_str), m_ifs(other.m_ifs){}
                FileIterator(const std::string& filename);
                FileIterator& operator++();
                ~FileIterator() {}
                operator bool() const;
                bool operator==(const FileIterator& other) const;
                bool operator!=(const FileIterator& other) const {return !(*this == other);}
                std::string str() const {return m_str;}
                std::string& operator*() {return m_str;}
            private:
                std::string find_line();
                std::string m_str;
                std::shared_ptr<std::ifstream> m_ifs;//meh should figure out a better way to use this, but 
        };
        FileIterator begin() const {return FileIterator(m_filename.c_str());}
        FileIterator end() {return FileIterator();}
        FileIterator end() const {return FileIterator();}
        FileIterator cend() const {return FileIterator();}
    private:
        std::string m_filename;

};

class LineParser {
    public:
        LineParser(const std::string& filename);
        class LineIterator {
            public:
                LineIterator() {}
                LineIterator(const LineIterator& other): m_tok(other.m_tok), m_ss(other.m_ss) {}
                LineIterator(const std::shared_ptr<std::istringstream>& ss);
                LineIterator& operator++();
                LineIterator operator+(int x);
                ~LineIterator() {}
                operator bool() const;
                bool operator==(const LineIterator& other) const;
                bool operator!=(const LineIterator& other) const {return !(*this == other);}
                operator float() const;
                operator double() const;
                operator int() const;
                std::string string() const;
                std::string& operator*() {return m_tok;}
            private:
                std::string m_tok;
                std::shared_ptr<std::istringstream> m_ss;
        };
        LineIterator begin() const {return LineIterator(m_ss);}
        LineIterator end() {return LineIterator();}
        LineIterator end() const {return LineIterator();}
        LineIterator cend() const {return LineIterator();}
    private:
        std::shared_ptr<std::istringstream> m_ss;
};

}
#endif
