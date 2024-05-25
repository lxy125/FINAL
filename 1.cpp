#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <string.h>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <fstream>
#include <QTextEdit>
#include "functions.h"
#include "globals.h"
#include <sstream>

using namespace std;



// Global variables to store the grammar state
QString grammarInput;
std::map<std::string, int> VN2int;
std::map<std::string, int> VT2int;
int symbolNum = 0;
std::vector<std::string> grammar;

/********************************************************
 *                                                      *
 *                第一部分：词法分析                       *
 *                                                      *
 *******************************************************/
 // 关键字表置初始值
string Cppkeyword[100] = { "#", "标识符(变量名)", "整数", "实数", "字符常量", "+", "-", "*", "/", "<",
                          "<=", "==", "!=", ">=", ">", "&", "&&", "||", "=", "(",
                          ")", "[", "]", "{", "}", ":", ";", ",", "@", "!",
                          "void", "int", "float", "char", "if", "else", "while", "do", "for", "true",
                          "false", "using", "namespace", "std", "main", "return", "null" };
class word
{
public:
    int syn{};
    string token;
};
//存储词法分析结果
word lexicalTable[1000];
int lexicalTableLen = 0;
//定位二元式在哪源代码的哪一行
vector<int> whichLine;

// 处理关键词和变量的函数
word letterAnalysis(const string& subCode)
{
    word item;
    int isKeyword = 0;
    for (int i = 30; i <= 46; i++)
    {
        if (subCode.substr(0, Cppkeyword[i].length()) == Cppkeyword[i])
        {
            item.syn = i;
            isKeyword = 1;
        }
    }
    /* 不用上述for循环的话就得如下一一列举，如果关键词数目较多，就要写很多低级代码
    if (subCode.substr(0, 4) == "void")
    {
        item.syn = 28;
    }
    else if (subCode.substr(0, 3) == "int")
    {
        item.syn = 29;
    }
    else if (subCode.substr(0, 5) == "float")
*/
    if (isKeyword == 0)
    {
        // 如果不是上述关键词，一律视为变量名
        for (int i = 0; i <= subCode.length(); ++i)
        { //找到第一个不是数字、字母、下划线的位置
            if (!(subCode[i] >= 'a' && subCode[i] <= 'z' || subCode[i] >= 'A' && subCode[i] <= 'Z' || subCode[i] >= '0' && subCode[i] <= '9' || subCode[i] == '_'))
            {
                item.syn = 1; //1号位存储变量名
                Cppkeyword[item.syn] = subCode.substr(0, i);
                break;
            }
        }
    }
    item.token = Cppkeyword[item.syn];
    return item;
}

// 处理数字的函数
word numberAnalysis(string subCode)
{
    word item;
    for (int i = 0; i <= subCode.length(); ++i)
    {
        // 截取到第一个非数字和小数点字符
        if (!(subCode[i] >= '0' && subCode[i] <= '9' || subCode[i] == '.'))
        {
            string curNum = subCode.substr(0, i);
            if (curNum.find('.') == string::npos) //没读到'.'返回值为很大的数，若读到返回值是第一次出现的下标
                item.syn = 2;                     //2号位存整数
            else
                item.syn = 3; //3号位存实数

            Cppkeyword[item.syn] = curNum;
            break;
        }
    }
    item.token = Cppkeyword[item.syn];
    return item;
}

// 处理字符常量的函数
word strAnalysis(string subCode)
{ //"hello"<<endl;
    word item;
    int nextindex = subCode.find_first_of('"', 1); //找到第二个"出现的位置下标
    item.syn = 4;                                  //字符常量置为4
    Cppkeyword[item.syn] = subCode.substr(0, nextindex + 1);

    item.token = Cppkeyword[item.syn];
    return item;
}

// 处理字符的函数
word charAnalysis(string subCode)
{
    word item;
    switch (subCode[0])
    {
    case '#':
        item.syn = 0;
        break;
    case '+':
        item.syn = 5;
        break;
    case '-':
        item.syn = 6;
        break;
    case '*':
        item.syn = 7;
        break;
    case '/':
        item.syn = 8;
        break;
    case '<':
        if (subCode[1] == '=')
        {
            item.syn = 10;
        }
        else
        {
            item.syn = 9;
        }
        break;
    case '=':
        if (subCode[1] == '=')
        {
            item.syn = 11;
        }
        else
        {
            item.syn = 18;
        }
        break;
    case '!':
        if (subCode[1] == '=')
        {
            item.syn = 12;
        }
        else
            item.syn = 29;
        break;

    case '>':
        if (subCode[1] == '=')
        {
            item.syn = 13;
        }
        else
        {
            item.syn = 14;
        }
        break;
    case '&':
        if (subCode[1] == '&')
        {
            item.syn = 16;
        }
        else
        {
            item.syn = 15;
        }
        break;
    case '|':
        if (subCode[1] == '|')
        {
            item.syn = 17;
        }
        break;

    case '(':
        item.syn = 19;
        break;
    case ')':
        item.syn = 20;
        break;
    case '[':
        item.syn = 21;
        break;
    case ']':
        item.syn = 22;
        break;
    case '{':
        item.syn = 23;
        break;
    case '}':
        item.syn = 24;
        break;
    case ':':
        item.syn = 25;
        break;
    case ';':
        item.syn = 26;
        break;
    case ',':
        item.syn = 27;
        break;
    case '@':
        item.syn = 28;
        break;
    }
    item.token = Cppkeyword[item.syn];
    return item;
}

// 词法分析
void scanner(const string& code)
{ //if a=1;

    for (int i = 0; i < code.length(); ++i)
    {
        word item;
        if (code[i] >= 'a' && code[i] <= 'z' || code[i] >= 'A' && code[i] <= 'Z')
        { // 处理单词,假设句子是 if a=1;进行单词分析后返回“if”,i后移了两位，这点在该函数最后有做处理
            item = letterAnalysis(code.substr(i, code.length() - i + 1));
        }
        else if (code[i] >= '0' and code[i] <= '9')
        { // 处理数字
            item = numberAnalysis(code.substr(i, code.length() - i + 1));
        }
        else if (code[i] == ' ')
        { // 如果是空格，直接跳过
            continue;
        }
        else
        { // 处理特殊符号
            item = charAnalysis(code.substr(i, code.length() - i + 1));
        }
        i += int(item.token.length()) - 1;
        lexicalTable[lexicalTableLen++] = item; //词法处理完的结果存入lexicalTable中
        cout << "(" << item.syn << "," << item.token << ")" << endl;
    }
}
/****************************************************************
 *                                                              *
 *        第二部分、语法分析前的一些准备工作,主要包括:                 *
 *      (1) 扫描文法，获取文法中出现的所有的Vn和Vt                    *
 *      (2) 将文法中的产生式拆分为左部、右部,其中右部采用vector进行保存  *
 *      (3) 构造所有Vn和Vt的first集和follow集                      *
 *                                                              *
 ****************************************************************/
//vector<string> grammar;          //存储文法
//map<string, int> VN2int, VT2int; //VN、VT映射为下标索引
//int symbolNum = 0;
map<string, bool> nullable; //各终结符或非终结符是否可空
set<string> first[50];      //存储各Vn和Vt的first集，没错，Vt也构造first集，就是其自身
set<string> follow[50];     //存储各Vn和Vt的follow集，Vt的follow都是空，240~263行取消注释可查看Vt的first和follow集
extern QString grammarInput;
extern std::map<std::string, int> VN2int;
extern std::map<std::string, int> VT2int;
extern int symbolNum;
extern std::vector<std::string> grammar;
string getVn(string grammar) {
    if (grammar.substr(0, 2) == "<<") {
        int bracketsDelimiter = grammar.find(">>");
        string Vn = grammar.substr(0, bracketsDelimiter + 2);
        return Vn;
    }
    if (grammar[1] == '\'') {
        return grammar.substr(0, 2);
    }
    else {
        return grammar.substr(0, 1);
    }
}

string getVt(string grammar) {
    if (grammar[0] >= 'a' && grammar[0] <= 'z') {
        return grammar.substr(0, 1);
    }
    if (isdigit(grammar[0])) {
        size_t len = 0;
        while (len < grammar.length() && isdigit(grammar[len])) len++;
        return grammar.substr(0, len);
    }
    return grammar.substr(0, 1);
}



void readVnAndVt(QTextEdit* output) {
    VN2int.clear();
    VT2int.clear();
    symbolNum = 0;

    for (int i = 0; i < grammar.size(); i++) {
        for (int j = 0; j < grammar[i].length(); j++) {
            if (grammar[i][j] == ' ') continue;
            if (grammar[i].substr(j, 2) == "<<") {
                int bracketsDelimiter = grammar[i].substr(j, grammar[i].length() - j).find(">>");
                string Vn = grammar[i].substr(j, bracketsDelimiter + 2);
                if (VN2int[Vn] == 0)
                    VN2int[Vn] = ++symbolNum;
                j = j + Vn.length() - 1;
            }
            else if (grammar[i][j] >= 'A' && grammar[i][j] <= 'Z') {
                string Vn = getVn(grammar[i].substr(j, 2));
                if (VN2int[Vn] == 0)
                    VN2int[Vn] = ++symbolNum;
                j = j + Vn.length() - 1;
            }
            else if (grammar[i].substr(j, 2) == "->") {
                j = j + 2 - 1;
            }
            else {
                string Vt = getVt(grammar[i].substr(j, grammar[i].length() - j));
                if (VT2int[Vt] == 0)
                    VT2int[Vt] = ++symbolNum;
                j = j + Vt.length() - 1;
            }
        }
    }

    output->append("Non-terminals VN:");
    for (auto it = VN2int.begin(); it != VN2int.end(); it++) {
        output->append(QString("Index: %1\tName: %2").arg(it->second).arg(it->first.c_str()));
    }
    output->append("Terminals VT:");
    for (auto it = VT2int.begin(); it != VT2int.end(); it++) {
        output->append(QString("Index: %1\tName: %2").arg(it->second).arg(it->first.c_str()));
    }
}




vector<string> splitGrammarIntoYi(string rightGrama)
{ //将产生式的右部(左部->右部拆分)：X->Y1Y2...Yk
    vector<string> Y;

    for (int j = 0; j < rightGrama.length(); j++)
    {
        if (rightGrama[j] == ' ')
        {
            continue;
        }
        if (rightGrama[j] >= 'A' && rightGrama[j] <= 'Z' || rightGrama.substr(j, 2) == "<<")
        { //非终结符
            string Vn = getVn(rightGrama.substr(j, rightGrama.length() - j));
            Y.push_back(Vn);
            j = j + Vn.length() - 1;
        }
        else
        { //终结符
            string Vt = getVt(rightGrama.substr(j, rightGrama.length() - j));
            Y.push_back(Vt);
            j = j + Vt.length() - 1;
        }
    }
    return Y;
}

void split(string grama, string& X, vector<string>& Y)
{
    int delimiterIndex = grama.find("->");
    X = grama.substr(0, delimiterIndex);
    //trim()功能，C++不带，只能自己实现，剪除首尾的空格符号
    X.erase(0, X.find_first_not_of(" "));
    X.erase(X.find_last_not_of(" ") + 1);
    string rightGrama = grama.substr(delimiterIndex + 2, grama.length() - delimiterIndex - 2);
    Y = splitGrammarIntoYi(rightGrama);
}

bool allNullable(vector<string> Y, int left, int right)
{ //判断 Y[left]...Y[right]是否全可空
    if (left >= Y.size() || left > right || right < 0)
        return true;
    for (int i = left; i <= right; i++)
    {
        if (nullable[Y[i]] == false)
            return false;
    }
    return true;
}

void getFirstFollowSet() {
    // Initialize first sets for terminals
    for (const auto& it : VT2int) {
        std::string Vt = it.first;
        int Vt_index = it.second;
        first[Vt_index].insert(Vt);
    }

    bool changed;
    do {
        changed = false;
        for (const auto& production : grammar) {
            std::string X;
            std::vector<std::string> Y;
            split(production, X, Y);

            int X_index = VN2int[X];
            for (size_t i = 0; i < Y.size(); i++) {
                int Yi_index = VT2int.count(Y[i]) ? VT2int[Y[i]] : VN2int[Y[i]];
                size_t beforeSize = first[X_index].size();
                first[X_index].insert(first[Yi_index].begin(), first[Yi_index].end());
                if (first[X_index].size() != beforeSize) {
                    changed = true;
                }
                if (!nullable[Y[i]]) break;
            }
        }
    } while (changed);

    // Initialize follow set for start symbol S with end marker $
    follow[VN2int["S"]].insert("$");

    do {
        changed = false;
        for (const auto& production : grammar) {
            std::string X;
            std::vector<std::string> Y;
            split(production, X, Y);

            int X_index = VN2int[X];
            for (size_t i = 0; i < Y.size(); i++) {
                if (VN2int.count(Y[i])) {  // If Y[i] is a non-terminal
                    int Yi_index = VN2int[Y[i]];
                    bool allNullableAfter = true;
                    for (size_t j = i + 1; j < Y.size(); j++) {
                        int Yj_index = VT2int.count(Y[j]) ? VT2int[Y[j]] : VN2int[Y[j]];
                        size_t beforeSize = follow[Yi_index].size();
                        follow[Yi_index].insert(first[Yj_index].begin(), first[Yj_index].end());
                        follow[Yi_index].erase("ε");  // Remove epsilon if present
                        if (follow[Yi_index].size() != beforeSize) {
                            changed = true;
                        }
                        if (!nullable[Y[j]]) {
                            allNullableAfter = false;
                            break;
                        }
                    }
                    if (allNullableAfter) {
                        size_t beforeSize = follow[Yi_index].size();
                        follow[Yi_index].insert(follow[X_index].begin(), follow[X_index].end());
                        if (follow[Yi_index].size() != beforeSize) {
                            changed = true;
                        }
                    }
                }
            }
        }
    } while (changed);
}

void converge(QTextEdit* output) {
    // Clear existing first, follow sets, and nullable map
    for (int i = 0; i < 50; ++i) {
        first[i].clear();
        follow[i].clear();
    }
    nullable.clear();

    // Initialize follow set for start symbol S with end marker $
    follow[VN2int["S"]].insert("$");

    // Compute first and follow sets
    getFirstFollowSet();

    // Output first and follow sets
    QString result;
    for (const auto& it : VN2int) {
        int vnindex = it.second;
        result += QString("%1's first set:\t").arg(it.first.c_str());
        for (const auto& first_it : first[vnindex]) {
            result += QString("%1 ").arg(first_it.c_str());
        }
        result += "\n";
        result += QString("%1's follow set:\t").arg(it.first.c_str());
        for (const auto& follow_it : follow[vnindex]) {
            result += QString("%1 ").arg(follow_it.c_str());
        }
        result += "\n";
    }
    output->append(result);
}





/****************************************************************
 *                                                              *
 *                  第三部分：构造文法的LR1分析表                   *
 *          关键函数：(1) closure():计算项集的LR1闭包               *
 *                  (2) GOTO():由项集I读入Vn或Vt转向项集J          *
 *                                                              *
 ****************************************************************/
struct term
{                 //LR1项集内部的项
    int termType; //移进项目、待约项目、规约项目
    string leftPart;
    vector<string> rightPart;
    int dotPos{ -1 };             //活前缀在右部的位置,初始化为-1
    vector<string> subsequence; //LR1文法才用到的后继符,随便初始化一个字符串
    bool operator==(const term& b) const
    {
        if (leftPart == b.leftPart && rightPart == b.rightPart && dotPos == b.dotPos)
        {
            if (subsequence == b.subsequence)
                return true;
        }
        return false;
    }
};
const int maxN = 4000;        //预定最大有maxN个状态，不够再加，但最好不要超过10000，不然可能会发生未知错误
vector<term> statusSet[maxN]; //项集
int globalStatusNum = 1;      //项集编号
int actionTable[maxN][50];    //action表，行表示状态，列表示终结符。这里预定最多50个终结符，应该够用了
int gotoTable[maxN][50];      //goto表，行表示状态，列表示非终结符

void initGrammar(QTextEdit* output) {
    grammar.clear();
    std::istringstream stream(grammarInput.toStdString());
    std::string line;
    while (std::getline(stream, line)) {
        grammar.push_back(line);
    }
    QString result = "Grammar rules initialized:\n" + grammarInput;
    output->append(result);
}
//该函数作用:项集I读入Vn或Vt可能会生成新的项集J，但也有可能指向已有项集，该函数就是判断是否指向已有项集
int mergeSet()
{
    int flag = -1; //假定不可以进行合并
    for (int i = 0; i < globalStatusNum - 1; i++)
    {
        if (statusSet[globalStatusNum - 1].size() != statusSet[i].size())
            continue;
        flag = i; //可以和状态集i合并
        for (int j = 0; j < statusSet[globalStatusNum - 1].size(); j++)
        {
            if (!(statusSet[i][j] == statusSet[globalStatusNum - 1][j]))
            {
                flag = -1;
            }
        }
        if (flag != -1) //第一次遇到可以合并的集合就结束，不然等到后面会遇到项目数相等但状态集不同的情况
            return flag;
    }
    return -1;
}

void initI0()
{
    term firstTerm;
    string X;
    vector<string> Y;
    split(grammar[0], X, Y);
    if (firstTerm.dotPos == -1) //如果没有活前缀"·"
        firstTerm.dotPos = 0;   //就添加活前缀
    firstTerm.leftPart = X;
    firstTerm.rightPart = Y;
    firstTerm.subsequence.push_back("#");
    statusSet[0].push_back(firstTerm);
}

//相对于LR0和SLR，LR1在closure这个函数中有大改动，几乎是重写
void closure(int statusNum)
{ //计算LR1文法的项集闭包
    queue<term> termQueue;
    for (int i = 0; i < statusSet[statusNum].size(); i++)
        termQueue.push(statusSet[statusNum][i]);
    while (!termQueue.empty())
    { //对项集I中每个项[A->a·B beta,alpha]
        term curTerm = termQueue.front();
        if (curTerm.dotPos == curTerm.rightPart.size()) //如果是规约项，跳过并弹出该项
        {
            termQueue.pop();
            continue;
        }
        string B = curTerm.rightPart[curTerm.dotPos];
        for (int j = 0; j < grammar.size(); j++)
        { //对增广文法G'中的每个产生式B->gamma
            if (B != getVn(grammar[j].substr(0, grammar[j].length())))
                continue;
            //将[B->·gamma,b]加入集合I中，其中b是FIRST[beta alpha]中的终结符
            term newTerm;
            newTerm.dotPos = 0;
            split(grammar[j], newTerm.leftPart, newTerm.rightPart);
            //warning:好像没考虑B->null的情况，还是说已经考虑了(null看成终结符)
            if (VT2int.count(newTerm.rightPart[0]) != 0) //B->·bA，移进项目
                newTerm.termType = 2;
            else if (VN2int.count(newTerm.rightPart[0]) != 0) //B->b·A,待约项目
                newTerm.termType = 3;
            //找b
            string beta;
            vector<string> b;
            if (curTerm.dotPos == curTerm.rightPart.size() - 1)
            { //如果beta不存在，b即alpha
                b = curTerm.subsequence;
            }
            else if (VT2int.count(curTerm.rightPart[curTerm.dotPos + 1]) != 0)
            { //如果beta存在，b为first(beta)。特例：beta为终结符时，b就是beta
                b.push_back(curTerm.rightPart[curTerm.dotPos + 1]);
            }
            else
            { //遍历first(beta)中的终结符b
                beta = curTerm.rightPart[curTerm.dotPos + 1];
                for (auto it = first[VN2int[beta]].begin(); it != first[VN2int[beta]].end(); it++)
                {
                    b.push_back(*it);
                }
            }
            newTerm.subsequence = b;
            //只有闭包生成的新项B->gamma,不在集合I中才加入；在集合I中的只要相应的增加后继符
            int newTermFlag = -1; //先假设不在集合I中
            for (int k = 0; k < statusSet[statusNum].size(); k++)
            {
                if (newTerm.leftPart == statusSet[statusNum][k].leftPart && newTerm.rightPart == statusSet[statusNum][k].rightPart && newTerm.dotPos == statusSet[statusNum][k].dotPos)
                    newTermFlag = k;
            }
            if (newTermFlag == -1)
            { //不在集合I中就加入
                termQueue.push(newTerm);
                statusSet[statusNum].push_back(newTerm);
            }
            else
            { //如果新项B->gamma在集合I中，就新增其后继符(如果有新后继符的话)
                map<string, int> subsequenceMap;
                for (int m = 0; m < statusSet[statusNum][newTermFlag].subsequence.size(); m++)
                {
                    subsequenceMap[statusSet[statusNum][newTermFlag].subsequence[m]]++;
                }
                for (int m = 0; m < newTerm.subsequence.size(); m++)
                {
                    if (subsequenceMap[newTerm.subsequence[m]] == 0)
                        statusSet[statusNum][newTermFlag].subsequence.push_back(newTerm.subsequence[m]);
                }
            }
        }
        termQueue.pop();
    }
}

//GOTO函数无须变动，不管是LR0,SLR,LR1分析，都是从集合I读入一个符号经过闭包运算得到集合J。goto函数内部调用了closure()。
int GOTO(int statusNum, string symbol)
{
    int size = statusSet[statusNum].size();
    for (int i = 0; i < size; i++)
    {
        vector<string> right = statusSet[statusNum][i].rightPart;
        int dotPos = statusSet[statusNum][i].dotPos;
        if (dotPos < right.size() && symbol == right[dotPos])
        {
            term tmpTerm = statusSet[statusNum][i];
            tmpTerm.dotPos = tmpTerm.dotPos + 1;
            dotPos = tmpTerm.dotPos;
            if (tmpTerm.dotPos == tmpTerm.rightPart.size())
            {
                tmpTerm.termType = 4;
                statusSet[globalStatusNum].push_back(tmpTerm);
            }
            else if (VT2int.count(tmpTerm.rightPart[dotPos]) != 0)
            {
                tmpTerm.termType = 2;
                statusSet[globalStatusNum].push_back(tmpTerm);
            }
            else if (VN2int.count(tmpTerm.rightPart[dotPos]) != 0)
            {
                tmpTerm.termType = 3;
                statusSet[globalStatusNum].push_back(tmpTerm);
                closure(globalStatusNum);
            }
        }
    }
    globalStatusNum++;
    int flag = mergeSet();
    if (flag != -1)
    {
        statusSet[globalStatusNum - 1].clear();
        globalStatusNum--;
        return flag;
    }
    else
        return globalStatusNum - 1;
}


void printStatus(QTextEdit* output) {
    output->append("\nStatus Sets:\n");
    for (int i = 0; i < globalStatusNum; i++) {
        if (statusSet[i].size() == 0)
            continue;
        output->append("_______________________________");
        output->append(QString("       I%1").arg(i));
        output->append("_______________________________");
        for (auto it = statusSet[i].begin(); it != statusSet[i].end(); it++) {
            QString item = QString("        %1 -> ").arg(it->leftPart.c_str());
            for (int j = 0; j < it->rightPart.size(); j++) {
                if (j == it->dotPos)
                    item.append(".");
                item.append(it->rightPart[j].c_str());
            }
            if (it->rightPart.size() == it->dotPos)
                item.append(".");
            item.append(",");
            for (int j = 0; j < it->subsequence.size(); j++) {
                if (j != 0)
                    item.append("/");
                item.append(it->subsequence[j].c_str());
            }
            output->append(item);
        }
        output->append("_______________________________");
    }
}


void printTable(QTextEdit* output) {
    output->append("\nAnalysis Table:\n");
    QString header = "State\t";
    for (auto it = VT2int.begin(); it != VT2int.end(); it++)
        header.append(QString("%1\t").arg(it->first.c_str()));
    for (auto it = VN2int.begin(); it != VN2int.end(); it++) {
        if (it->first != getVn(grammar[0]))
            header.append(QString("%1\t").arg(it->first.c_str()));
    }
    output->append(header);

    for (int i = 0; i < globalStatusNum; i++) {
        if (statusSet[i].size() == 0)
            continue;
        QString row = QString("%1\t").arg(i);
        for (auto it = VT2int.begin(); it != VT2int.end(); it++) {
            if (actionTable[i][it->second] >= 20000 && actionTable[i][it->second] < 30000) {
                if (actionTable[i][it->second] == 29999)  // Check for s9999
                    row.append("0\t");  // Replace s9999 with 0
                else
                    row.append(QString("s%1\t").arg(actionTable[i][it->second] - 20000));
            }
            else if (actionTable[i][it->second] >= 30000 && actionTable[i][it->second] < 40000) {
                row.append(QString("r%1\t").arg(actionTable[i][it->second] - 30000));
            }
            else if (actionTable[i][it->second] == 10000) {
                row.append("acc\t");
            }
            else {
                row.append(" \t");
            }
        }
        for (auto it = VN2int.begin(); it != VN2int.end(); it++) {
            if (it->first != getVn(grammar[0])) {
                if (gotoTable[i][it->second] == 9999)  // Check for goto 9999
                    row.append("0\t");  // Replace goto 9999 with 0
                else
                    row.append(QString("%1\t").arg(gotoTable[i][it->second]));
            }
        }
        output->append(row);
    }
}




//相比LR0和SLR，LR1分析表只对规约项进行改动(项种类：移进项(不变)、接受项(?)、GOTO项(不变))
//删了十几行，加了1行就ok了
void constructStatusSet(QTextEdit* output) {
    initI0();
    closure(0);
    queue<string> symbolToRead;
    map<string, int> symbolMap;
    int curStatus = 0;

    for (int i = 0; i < statusSet[0].size(); i++) {
        string symbolStr = statusSet[0][i].rightPart[statusSet[0][i].dotPos];
        if (symbolMap[symbolStr] == 0) {
            symbolToRead.push(symbolStr);
            symbolMap[symbolStr]++;
        }
    }
    symbolToRead.push("sep");

    while (!symbolToRead.empty()) {
        if (symbolToRead.front() == "sep") {
            for (int ii = 0; ii < statusSet[curStatus].size(); ii++) {
                if (statusSet[curStatus][ii].dotPos == statusSet[curStatus][ii].rightPart.size()) {
                    term tmpTerm = statusSet[curStatus][ii];
                    string reduceTerm = tmpTerm.leftPart + "->";
                    for (int ii = 0; ii < tmpTerm.rightPart.size(); ii++)
                        reduceTerm = reduceTerm + tmpTerm.rightPart[ii];
                    int genNum = -1;
                    for (int ii = 0; ii < grammar.size(); ii++)
                        if (reduceTerm == grammar[ii])
                            genNum = ii;
                    if (genNum == 0)
                        actionTable[curStatus][VT2int["#"]] = 10000;
                    else {
                        for (int _i = 0; _i < tmpTerm.subsequence.size(); _i++) {
                            if (actionTable[curStatus][VT2int[tmpTerm.subsequence[_i]]] != 0)
                                output->append(QString("(状态%1存在规约-规约冲突)").arg(curStatus));
                            actionTable[curStatus][VT2int[tmpTerm.subsequence[_i]]] = 30000 + genNum;
                        }
                    }
                }
            }
            curStatus++;
            symbolToRead.pop();
            continue;
        }
        int nextStatus = GOTO(curStatus, symbolToRead.front());
        output->append(QString("I%1--%2-->I%3").arg(curStatus).arg(symbolToRead.front().c_str()).arg(nextStatus));
        if (VT2int.count(symbolToRead.front()) != 0) {
            if (actionTable[curStatus][VT2int[symbolToRead.front()]] != 0)
                output->append(QString("(状态%1移进%2存在冲突)").arg(curStatus).arg(symbolToRead.front().c_str()));
            actionTable[curStatus][VT2int[symbolToRead.front()]] = 20000 + nextStatus;
        }
        else if (nextStatus != -1) // 只在 nextStatus 有效时更新 gotoTable
            gotoTable[curStatus][VN2int[symbolToRead.front()]] = nextStatus;

        if (nextStatus == globalStatusNum - 1) {
            symbolMap.clear();
            for (int ii = 0; ii < statusSet[nextStatus].size(); ii++) {
                if (statusSet[nextStatus][ii].dotPos == statusSet[nextStatus][ii].rightPart.size())
                    continue;
                string symbolStr = statusSet[nextStatus][ii].rightPart[statusSet[nextStatus][ii].dotPos];
                if (symbolMap[symbolStr] == 0) {
                    symbolToRead.push(symbolStr);
                    symbolMap[symbolStr]++;
                }
            }
            symbolToRead.push("sep");
        }
        symbolToRead.pop();
    }
    printStatus(output); // 输出状态项集
    printTable(output);  // 输出分析表
}




//int main()
//{
 //   initGrammar();        //初始化文法
 //   readVnAndVt();        //读取文法中所有的VN和VT
 //   converge();           //构造first和follow集
 //   constructStatusSet(); //构造LR(1)分析表

 //   return 0;
//}


