#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

class SortApp
{
    inline static const auto get_int = [](const std::string & s) {
        std::string res;
        std::size_t pos = 0;
        while (pos < s.size() && std::isspace(s[pos])) {
            pos++;
        }
        if (pos < s.size() && s[pos] == '-') {
            res += '-';
            pos++;
        }
        res += '0';
        while (pos < s.size() && std::isdigit(s[pos])) {
            res += s[pos++];
        }
        return res == "-0" ? "" : res;
    };

    inline static const std::map<std::string, std::function<bool(const std::string &, const std::string &)>> process_functions{
            {"", [](const std::string & lhs, const std::string & rhs) { return lhs < rhs; }},
            {"-n", [](const std::string & lhs, const std::string & rhs) {
                 const auto x = get_int(lhs);
                 const auto y = get_int(rhs);
                 if (!x.empty() && !y.empty()) {
                     const auto xi = std::stoi(x);
                     const auto yi = std::stoi(y);
                     if (xi == yi) {
                         return lhs < rhs;
                     }
                     return xi < yi;
                 }
                 if (x.empty() && y.empty()) {
                     return lhs < rhs;
                 }
                 return x.size() < y.size();
             }},
            {"-f", [](std::string lhs, std::string rhs) {
                 std::transform(lhs.begin(), lhs.end(), lhs.begin(), ::toupper);
                 std::transform(rhs.begin(), rhs.end(), rhs.begin(), ::toupper);
                 return lhs < rhs;
             }},
    };

public:
    static void solve(std::istream & in, const std::string & f_option)
    {
        std::vector<std::string> data;
        std::string a;
        while (std::getline(in, a)) {
            data.push_back(a);
        }
        std::sort(data.begin(), data.end(), process_functions.at(f_option));
        for (const auto & i : data) {
            std::cout << i << std::endl;
        }
    }
};

int main(int argc, char ** argv)
{
    std::string f_option = "";
    bool from_file = false;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-n") == 0 || std::strcmp(argv[i], "-nf") == 0 || std::strcmp(argv[i], "--numeric-sort") == 0) {
            f_option = "-n";
        }
        else if (f_option.empty() && (std::strcmp(argv[i], "-f") == 0 || std::strcmp(argv[i], "--ignore-case") == 0)) {
            f_option = "-f";
        }
        else {
            from_file = true;
        }
    }
    if (!from_file) {
        SortApp::solve(std::cin, f_option);
    }
    else {
        std::ifstream input_file(argv[argc - 1]);
        if (!input_file) {
            std::cerr << "File not found" << std::endl;
            return 1;
        }
        SortApp::solve(input_file, f_option);
    }
    return 0;
}
