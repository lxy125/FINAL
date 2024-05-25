#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <map>
#include <set>
#include <string>
#include <vector>

// Declare global variables as extern
extern QString grammarInput;
extern std::map<std::string, int> VN2int;
extern std::map<std::string, int> VT2int;
extern int symbolNum;
extern std::vector<std::string> grammar;
extern std::set<std::string> first[50];
extern std::set<std::string> follow[50];
extern std::map<std::string, bool> nullable;

#endif // GLOBALS_H
