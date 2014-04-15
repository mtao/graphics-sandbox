#include "linereader.hpp"
#include <iostream>

mtao::FileParser::FileParser(const std::string& filename): m_filename(filename) {}

mtao::FileParser::FileIterator::operator bool() const {
    return false;
}
auto mtao::FileParser::FileIterator::operator++() -> FileIterator&{
    m_str = find_line();
    return *this;
}
bool mtao::FileParser::FileIterator::operator==(const FileIterator& other) const {
    return (this->m_str == other.m_str);
}

mtao::FileParser::FileIterator::FileIterator(const std::string& filename): m_ifs(std::make_shared<std::ifstream>(filename.c_str())) {
    m_str = find_line();

}
std::string mtao::FileParser::FileIterator::find_line() {
    std::array<char,1024> buf;
    while(m_ifs->good()) {
        m_ifs->getline(buf.begin(),1024);
    std::string line(buf.begin());
    if(!line.empty()) {
        return line;
    }
    }
    return std::string();
}




mtao::LineParser::LineParser(const std::string& str): m_ss(std::make_shared<std::istringstream>(str)) {}

mtao::LineParser::LineIterator::operator bool() const {
    return false;
}
auto mtao::LineParser::LineIterator::operator++() -> LineIterator&{
    if(!std::getline(*m_ss,m_tok,' ')) {
        m_tok = std::string();
    }
    return *this;
}
auto mtao::LineParser::LineIterator::operator+(int x) -> LineIterator{
    for(;x>0; --x) {
        if(!std::getline(*m_ss,m_tok,' ')) {
            m_tok = std::string();
            return *this;
        }
    }
    return *this;
}
bool mtao::LineParser::LineIterator::operator==(const LineIterator& other) const {
    return (this->m_tok == other.m_tok);
}

mtao::LineParser::LineIterator::LineIterator(const std::shared_ptr<std::istringstream>& ss): m_ss(ss) {
    if(!std::getline(*m_ss,m_tok,' ')) {
        m_tok = std::string();
    }

}

mtao::LineParser::LineIterator::operator float() const {
    float val;
    std::istringstream(m_tok) >> val;
    return val;
}
mtao::LineParser::LineIterator::operator double() const {
    double val;
    std::istringstream(m_tok) >> val;
    return val;
}
mtao::LineParser::LineIterator::operator int() const {
    int val;
    std::istringstream(m_tok) >> val;
    return val;
}
std::string mtao::LineParser::LineIterator::string() const {
    return m_tok;
}


int main(int argc, char * argv[]) {
    mtao::FileParser fp(argv[1]);
    for(auto&& l: fp) {
        char v;
        std::istringstream ss(l);
        ss >> v;
        if(v == 'v') {
            std::array<float,3> arr;
            ss >> arr[0] >> arr[1] >> arr[2];
            for(auto&& v: arr) {
                std::cout << v << " ";
            }
            std::cout << std::endl;
        }
            /*
        if(l[0] == 'v') {
            std::cout << "Vertex: ";
            mtao::LineParser lp(l);
            for(auto tok = (lp.begin()+1); tok != lp.end(); ++tok) {
                std::cout << float(tok) << " ";
            }
        }
        if(l[0] == 'f') {
            std::cout << "Face: ";
            mtao::LineParser lp(l);
            for(auto tok = (lp.begin()+1); tok != lp.end(); ++tok) {
                std::cout << int(tok) << " ";
            }
        }
        std::cout << std::endl;
        */
    }
}
