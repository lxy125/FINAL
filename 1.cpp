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
 *                ��һ���֣��ʷ�����                       *
 *                                                      *
 *******************************************************/
 // �ؼ��ֱ��ó�ʼֵ
string Cppkeyword[100] = { "#", "��ʶ��(������)", "����", "ʵ��", "�ַ�����", "+", "-", "*", "/", "<",
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
//�洢�ʷ��������
word lexicalTable[1000];
int lexicalTableLen = 0;
//��λ��Ԫʽ����Դ�������һ��
vector<int> whichLine;

// ����ؼ��ʺͱ����ĺ���
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
    /* ��������forѭ���Ļ��͵�����һһ�о٣�����ؼ�����Ŀ�϶࣬��Ҫд�ܶ�ͼ�����
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
        // ������������ؼ��ʣ�һ����Ϊ������
        for (int i = 0; i <= subCode.length(); ++i)
        { //�ҵ���һ���������֡���ĸ���»��ߵ�λ��
            if (!(subCode[i] >= 'a' && subCode[i] <= 'z' || subCode[i] >= 'A' && subCode[i] <= 'Z' || subCode[i] >= '0' && subCode[i] <= '9' || subCode[i] == '_'))
            {
                item.syn = 1; //1��λ�洢������
                Cppkeyword[item.syn] = subCode.substr(0, i);
                break;
            }
        }
    }
    item.token = Cppkeyword[item.syn];
    return item;
}

// �������ֵĺ���
word numberAnalysis(string subCode)
{
    word item;
    for (int i = 0; i <= subCode.length(); ++i)
    {
        // ��ȡ����һ�������ֺ�С�����ַ�
        if (!(subCode[i] >= '0' && subCode[i] <= '9' || subCode[i] == '.'))
        {
            string curNum = subCode.substr(0, i);
            if (curNum.find('.') == string::npos) //û����'.'����ֵΪ�ܴ����������������ֵ�ǵ�һ�γ��ֵ��±�
                item.syn = 2;                     //2��λ������
            else
                item.syn = 3; //3��λ��ʵ��

            Cppkeyword[item.syn] = curNum;
            break;
        }
    }
    item.token = Cppkeyword[item.syn];
    return item;
}

// �����ַ������ĺ���
word strAnalysis(string subCode)
{ //"hello"<<endl;
    word item;
    int nextindex = subCode.find_first_of('"', 1); //�ҵ��ڶ���"���ֵ�λ���±�
    item.syn = 4;                                  //�ַ�������Ϊ4
    Cppkeyword[item.syn] = subCode.substr(0, nextindex + 1);

    item.token = Cppkeyword[item.syn];
    return item;
}

// �����ַ��ĺ���
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

// �ʷ�����
void scanner(const string& code)
{ //if a=1;

    for (int i = 0; i < code.length(); ++i)
    {
        word item;
        if (code[i] >= 'a' && code[i] <= 'z' || code[i] >= 'A' && code[i] <= 'Z')
        { // ������,��������� if a=1;���е��ʷ����󷵻ء�if��,i��������λ������ڸú��������������
            item = letterAnalysis(code.substr(i, code.length() - i + 1));
        }
        else if (code[i] >= '0' and code[i] <= '9')
        { // ��������
            item = numberAnalysis(code.substr(i, code.length() - i + 1));
        }
        else if (code[i] == ' ')
        { // ����ǿո�ֱ������
            continue;
        }
        else
        { // �����������
            item = charAnalysis(code.substr(i, code.length() - i + 1));
        }
        i += int(item.token.length()) - 1;
        lexicalTable[lexicalTableLen++] = item; //�ʷ�������Ľ������lexicalTable��
        cout << "(" << item.syn << "," << item.token << ")" << endl;
    }
}
/****************************************************************
 *                                                              *
 *        �ڶ����֡��﷨����ǰ��һЩ׼������,��Ҫ����:                 *
 *      (1) ɨ���ķ�����ȡ�ķ��г��ֵ����е�Vn��Vt                    *
 *      (2) ���ķ��еĲ���ʽ���Ϊ�󲿡��Ҳ�,�����Ҳ�����vector���б���  *
 *      (3) ��������Vn��Vt��first����follow��                      *
 *                                                              *
 ****************************************************************/
//vector<string> grammar;          //�洢�ķ�
//map<string, int> VN2int, VT2int; //VN��VTӳ��Ϊ�±�����
//int symbolNum = 0;
map<string, bool> nullable; //���ս������ս���Ƿ�ɿ�
set<string> first[50];      //�洢��Vn��Vt��first����û��VtҲ����first��������������
set<string> follow[50];     //�洢��Vn��Vt��follow����Vt��follow���ǿգ�240~263��ȡ��ע�Ϳɲ鿴Vt��first��follow��
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
{ //������ʽ���Ҳ�(��->�Ҳ����)��X->Y1Y2...Yk
    vector<string> Y;

    for (int j = 0; j < rightGrama.length(); j++)
    {
        if (rightGrama[j] == ' ')
        {
            continue;
        }
        if (rightGrama[j] >= 'A' && rightGrama[j] <= 'Z' || rightGrama.substr(j, 2) == "<<")
        { //���ս��
            string Vn = getVn(rightGrama.substr(j, rightGrama.length() - j));
            Y.push_back(Vn);
            j = j + Vn.length() - 1;
        }
        else
        { //�ս��
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
    //trim()���ܣ�C++������ֻ���Լ�ʵ�֣�������β�Ŀո����
    X.erase(0, X.find_first_not_of(" "));
    X.erase(X.find_last_not_of(" ") + 1);
    string rightGrama = grama.substr(delimiterIndex + 2, grama.length() - delimiterIndex - 2);
    Y = splitGrammarIntoYi(rightGrama);
}

bool allNullable(vector<string> Y, int left, int right)
{ //�ж� Y[left]...Y[right]�Ƿ�ȫ�ɿ�
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
                        follow[Yi_index].erase("��");  // Remove epsilon if present
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
 *                  �������֣������ķ���LR1������                   *
 *          �ؼ�������(1) closure():�������LR1�հ�               *
 *                  (2) GOTO():���I����Vn��Vtת���J          *
 *                                                              *
 ****************************************************************/
struct term
{                 //LR1��ڲ�����
    int termType; //�ƽ���Ŀ����Լ��Ŀ����Լ��Ŀ
    string leftPart;
    vector<string> rightPart;
    int dotPos{ -1 };             //��ǰ׺���Ҳ���λ��,��ʼ��Ϊ-1
    vector<string> subsequence; //LR1�ķ����õ��ĺ�̷�,����ʼ��һ���ַ���
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
const int maxN = 4000;        //Ԥ�������maxN��״̬�������ټӣ�����ò�Ҫ����10000����Ȼ���ܻᷢ��δ֪����
vector<term> statusSet[maxN]; //�
int globalStatusNum = 1;      //����
int actionTable[maxN][50];    //action���б�ʾ״̬���б�ʾ�ս��������Ԥ�����50���ս����Ӧ�ù�����
int gotoTable[maxN][50];      //goto���б�ʾ״̬���б�ʾ���ս��

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
//�ú�������:�I����Vn��Vt���ܻ������µ��J����Ҳ�п���ָ����������ú��������ж��Ƿ�ָ�������
int mergeSet()
{
    int flag = -1; //�ٶ������Խ��кϲ�
    for (int i = 0; i < globalStatusNum - 1; i++)
    {
        if (statusSet[globalStatusNum - 1].size() != statusSet[i].size())
            continue;
        flag = i; //���Ժ�״̬��i�ϲ�
        for (int j = 0; j < statusSet[globalStatusNum - 1].size(); j++)
        {
            if (!(statusSet[i][j] == statusSet[globalStatusNum - 1][j]))
            {
                flag = -1;
            }
        }
        if (flag != -1) //��һ���������Ժϲ��ļ��Ͼͽ�������Ȼ�ȵ������������Ŀ����ȵ�״̬����ͬ�����
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
    if (firstTerm.dotPos == -1) //���û�л�ǰ׺"��"
        firstTerm.dotPos = 0;   //����ӻ�ǰ׺
    firstTerm.leftPart = X;
    firstTerm.rightPart = Y;
    firstTerm.subsequence.push_back("#");
    statusSet[0].push_back(firstTerm);
}

//�����LR0��SLR��LR1��closure����������д�Ķ�����������д
void closure(int statusNum)
{ //����LR1�ķ�����հ�
    queue<term> termQueue;
    for (int i = 0; i < statusSet[statusNum].size(); i++)
        termQueue.push(statusSet[statusNum][i]);
    while (!termQueue.empty())
    { //���I��ÿ����[A->a��B beta,alpha]
        term curTerm = termQueue.front();
        if (curTerm.dotPos == curTerm.rightPart.size()) //����ǹ�Լ���������������
        {
            termQueue.pop();
            continue;
        }
        string B = curTerm.rightPart[curTerm.dotPos];
        for (int j = 0; j < grammar.size(); j++)
        { //�������ķ�G'�е�ÿ������ʽB->gamma
            if (B != getVn(grammar[j].substr(0, grammar[j].length())))
                continue;
            //��[B->��gamma,b]���뼯��I�У�����b��FIRST[beta alpha]�е��ս��
            term newTerm;
            newTerm.dotPos = 0;
            split(grammar[j], newTerm.leftPart, newTerm.rightPart);
            //warning:����û����B->null�����������˵�Ѿ�������(null�����ս��)
            if (VT2int.count(newTerm.rightPart[0]) != 0) //B->��bA���ƽ���Ŀ
                newTerm.termType = 2;
            else if (VN2int.count(newTerm.rightPart[0]) != 0) //B->b��A,��Լ��Ŀ
                newTerm.termType = 3;
            //��b
            string beta;
            vector<string> b;
            if (curTerm.dotPos == curTerm.rightPart.size() - 1)
            { //���beta�����ڣ�b��alpha
                b = curTerm.subsequence;
            }
            else if (VT2int.count(curTerm.rightPart[curTerm.dotPos + 1]) != 0)
            { //���beta���ڣ�bΪfirst(beta)��������betaΪ�ս��ʱ��b����beta
                b.push_back(curTerm.rightPart[curTerm.dotPos + 1]);
            }
            else
            { //����first(beta)�е��ս��b
                beta = curTerm.rightPart[curTerm.dotPos + 1];
                for (auto it = first[VN2int[beta]].begin(); it != first[VN2int[beta]].end(); it++)
                {
                    b.push_back(*it);
                }
            }
            newTerm.subsequence = b;
            //ֻ�бհ����ɵ�����B->gamma,���ڼ���I�вż��룻�ڼ���I�е�ֻҪ��Ӧ�����Ӻ�̷�
            int newTermFlag = -1; //�ȼ��費�ڼ���I��
            for (int k = 0; k < statusSet[statusNum].size(); k++)
            {
                if (newTerm.leftPart == statusSet[statusNum][k].leftPart && newTerm.rightPart == statusSet[statusNum][k].rightPart && newTerm.dotPos == statusSet[statusNum][k].dotPos)
                    newTermFlag = k;
            }
            if (newTermFlag == -1)
            { //���ڼ���I�оͼ���
                termQueue.push(newTerm);
                statusSet[statusNum].push_back(newTerm);
            }
            else
            { //�������B->gamma�ڼ���I�У����������̷�(������º�̷��Ļ�)
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

//GOTO��������䶯��������LR0,SLR,LR1���������ǴӼ���I����һ�����ž����հ�����õ�����J��goto�����ڲ�������closure()��
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




//���LR0��SLR��LR1������ֻ�Թ�Լ����иĶ�(�����ࣺ�ƽ���(����)��������(?)��GOTO��(����))
//ɾ��ʮ���У�����1�о�ok��
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
                                output->append(QString("(״̬%1���ڹ�Լ-��Լ��ͻ)").arg(curStatus));
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
                output->append(QString("(״̬%1�ƽ�%2���ڳ�ͻ)").arg(curStatus).arg(symbolToRead.front().c_str()));
            actionTable[curStatus][VT2int[symbolToRead.front()]] = 20000 + nextStatus;
        }
        else if (nextStatus != -1) // ֻ�� nextStatus ��Чʱ���� gotoTable
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
    printStatus(output); // ���״̬�
    printTable(output);  // ���������
}




//int main()
//{
 //   initGrammar();        //��ʼ���ķ�
 //   readVnAndVt();        //��ȡ�ķ������е�VN��VT
 //   converge();           //����first��follow��
 //   constructStatusSet(); //����LR(1)������

 //   return 0;
//}


