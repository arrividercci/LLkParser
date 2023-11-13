#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <iomanip>

using namespace std;

bool isNonTerminal(char ch)
{
    return ch >= 'A' && ch <= 'Z';
}

vector<string> combineAllWords(vector<char> first, vector<char> second)
{
    vector<string> result;
    for (auto const& ch1 : first)
    {
        string firstS(1, ch1);

        for (auto const& ch2 : second)
        {
            string secondS(1, ch2);
            string newWord = firstS + secondS;
            if (find(result.begin(), result.end(), newWord) == result.end())
            {
                result.push_back(newWord);
            }
        }
    }
    return result;
}

vector<string> combineAllWords(vector<string> first, vector<string> second)
{
    vector<string> result;
    for (auto const& s1 : first)
    {
        for (auto const& s2 : second)
        {
            string newWord = s1 + s2;
            if (find(result.begin(), result.end(), newWord) == result.end())
            {
                result.push_back(newWord);
            }
        }
    }
    return result;
}


vector<string> combineAllWords(vector<string> first, char second)
{
    vector<string> result;
    for (auto const& st1 : first)
    {
        string secondS(1, second);
        string newWord = st1 + secondS;
        if (find(result.begin(), result.end(), newWord) == result.end())
        {
            result.push_back(newWord);
        }
    }
    return result;
}


vector<string> combineAllWords(vector<string> first, vector<char> second)
{
    vector<string> result;
    for (auto const& st1 : first)
    {
        for (auto const& ch2 : second)
        {
            string secondS(1, ch2);
            string newWord = st1 + secondS;
            if (find(result.begin(), result.end(), newWord) == result.end())
            {
                result.push_back(newWord);
            }
        }
    }
    return result;
}

char getFirstNotEmptyChar(string word)
{
    for (int i = 0; i < word.size(); i++)
    {
        if (word[i] != 'e') return word[i];
    }
    return 'e';
}

vector<string> GetAllFirstCharactersOfWords(vector<string> words)
{
    vector<string> result;
    for (auto const& w : words)
    {
        char firstChar = getFirstNotEmptyChar(w);
        string firstChatStr = string(1, firstChar);
        if (find(result.begin(), result.end(), firstChatStr) != result.end()) continue;
        result.push_back(firstChatStr);
    }
    return result;
}


class Transition
{
public:
    Transition(string start, string end)
    {
        this->start = start;
        this->end = end;
    }
    string start;
    string end;
};
//parse transition
Transition ParseToTransition(const string& str)
{
    size_t pos = str.find("->");//gets position of separator("->")
    string start = str.substr(0, pos);//gets string before separator
    start.erase(remove(start.begin(), start.end(), ' '), start.end());//removes all whitespaces
    string end = str.substr(pos + 2);//gets string after separator
    end.erase(remove(end.begin(), end.end(), ' '), end.end());
    Transition transition(start, end);
    return transition;
}

//reading all instructions
vector<Transition> ReadAllTransitions(const string& filePath)
{
    vector<Transition> transitions;
    ifstream fin;
    fin.open(filePath);
    if (fin.is_open())
    {
        string str;
        while (getline(fin, str))
        {
            transitions.push_back(ParseToTransition(str));
        }
    }
    fin.close();
    return transitions;
}

map<string, vector<string>> FirstK(vector<Transition> transitions)
{
    map<string, vector<string>> result;
    for (auto const& currentTransition : transitions) //adding first step terminals to the result
    {
        vector<string> terminals;
        for (auto const& transition : transitions)
        {
            if (transition.start != currentTransition.start) continue;
            if (!isNonTerminal(transition.end[0]))
            {
                string terminal(1, transition.end[0]);
                if (find(terminals.begin(), terminals.end(), terminal) == terminals.end())
                {
                    terminals.push_back(terminal);
                }
            }
        }
        result[currentTransition.start] = terminals;
    }

    bool isNotChanged = false;//flag that detects if previous step is equal to the current
    while (!isNotChanged)
    {
        for (auto & currentNonTerminalFirstK : result)
        {
            string currentNonTerminal = currentNonTerminalFirstK.first;

            vector<string> previousStepTerminals = currentNonTerminalFirstK.second;

            for (auto const& transition : transitions)
            {
                if (transition.start == currentNonTerminal)
                {
                    vector<string> previousNonTerminalFirstK = result[string(1, transition.end[0])];
                    vector<string> combinedWords = previousNonTerminalFirstK;

                    if (!isNonTerminal(transition.end[0]))
                    {
                        if (previousStepTerminals == currentNonTerminalFirstK.second)
                        {
                            isNotChanged = true;
                        }
                        continue;
                    }

                    if (combinedWords.empty())
                    {
                        if (previousStepTerminals == currentNonTerminalFirstK.second)
                        {
                            isNotChanged = true;
                        }
                        continue;
                    }
                    for (int i = 1; i < transition.end.size(); i++)
                    {
                        if (combinedWords.empty()) continue;
                        if (isNonTerminal(transition.end[i]))
                        {
                            vector<string> temp = result[string(1, transition.end[i])];
                            if (temp.empty())
                            {
                                continue;
                                combinedWords.clear();
                            }
                            combinedWords = combineAllWords(combinedWords, temp);
                        }
                        else
                        {
                            combinedWords = combineAllWords(combinedWords, transition.end[i]);
                        }
                    }
                    vector<string> firstChars = GetAllFirstCharactersOfWords(combinedWords);
                    for (auto const& fc : firstChars)
                    {
                        if (find(currentNonTerminalFirstK.second.begin(), currentNonTerminalFirstK.second.end(), fc) == currentNonTerminalFirstK.second.end())
                        {
                            currentNonTerminalFirstK.second.push_back(fc);
                        }
                    }

                    if (previousStepTerminals != currentNonTerminalFirstK.second)
                    {
                        isNotChanged = true;
                    }
                }
            }
        }
    }
    map<string, vector<string>> result2;
    for (auto& element : result)
    {
        if (isNonTerminal(element.first[0]))
        {
            result2[element.first] = element.second;
        }
    }

    return result2;
}

set<string>findEpsNonTerminals(vector<Transition>transitions) {
    set<string>result;
    for (int i = 0; i < transitions.size(); i++) {
        if (transitions[i].end == "e") {
            result.insert(transitions[i].start);
        }
    }
    for (int i = 0; i < transitions.size(); i++) {
        int count = 0;
        string end = transitions[i].end;
        for (int j = 0; j < end.length(); j++) {
            string ch = "";
            ch += end[j];
            if (result.count(ch) > 0) {
                count++;
            }
        }
        if (count == transitions[i].end.length()) {
            result.insert(transitions[i].start);
        }
    }
    return result;
}

map<string, vector<string>> FollowK1(vector<Transition> transitions, map<string, vector<string>>firstk, set<string>epsilon) {
    map<string, vector<string>>result;
    for (int i = 0; i < transitions.size(); i++) {
        string left_nonterminal = transitions[i].start;
        vector<string>follow;
        if (left_nonterminal == "S") {
            follow.push_back("e");
        }
        for (int j = 0; j < transitions.size(); j++) {
            if (transitions[j].end.find(left_nonterminal) != string::npos) {
                int at = transitions[j].end.find(left_nonterminal);
                if (at == transitions[j].end.length() - 1) {
                    string symbol = "";
                    symbol += transitions[j].end[at];
                    if (transitions[j].start == symbol) {
                        continue;
                    }
                    else {
                        vector<string> followleft = result[transitions[j].start];
                        for (int k = 0; k < followleft.size(); k++) {
                            //cout << "Follow: " << transitions[j].start << " " << followleft[k] << endl;
                            follow.push_back(followleft[k]);
                        }
                    }
                }
                else {
                    int eps = 0;
                    for (int p = at + 1; p < transitions[j].end.length(); p++) {

                        if (!isNonTerminal(transitions[j].end[p])) {
                            string symbol = "";
                            symbol += transitions[j].end[p];
                            follow.push_back(symbol);
                            //cout << symbol;
                            break;
                        }
                        else {
                            string symbol = "";
                            symbol += transitions[j].end[p];
                            if (epsilon.count(symbol) > 0) {
                                eps++;
                            }
                            vector<string>first = firstk[symbol];
                            for (int k = 0; k < first.size(); k++) {
                                follow.push_back(first[k]);
                            }
                        }
                    }
                    if (eps = transitions[j].end.length() - at + 1) {
                        vector<string> followleft = result[transitions[j].start];
                        for (int k = 0; k < followleft.size(); k++) {
                            //cout << "Follow: " << transitions[j].start << " " << followleft[k] << endl;
                            follow.push_back(followleft[k]);
                        }
                    }
                }
            }
            //result[left_nonterminal] = follow;
        }
        set<string>followset;
        for (int l = 0; l < follow.size(); l++) {
            followset.insert(follow[l]);
        }
        vector<string> helper(followset.begin(), followset.end());
        result[left_nonterminal] = helper;
    }
    return result;
}

map<pair<string, string>, string> buildParsingTable(vector<Transition> transitions, map<string, vector<string>> firstk, map<string, vector<string>> followk, set<string> epsilon)
{
    map<pair<string, string>, string> parsingTable;

    for (auto const &transition : transitions)
    {
        string start = transition.start;
        string end = transition.end;

        // Визначаємо FIRST для end
        vector<string> firstSet;

        for (int i = 0; i < end.size(); ++i)
        {
            char currentChar = end[i];
            if (isNonTerminal(currentChar))
            {
                vector<string> firstOfNonTerminal = firstk[string(1, currentChar)];
                firstSet.insert(firstSet.end(), firstOfNonTerminal.begin(), firstOfNonTerminal.end());

                // Якщо epsilon не є в FIRST[nonTerminal], перериваємо цикл
                if (find(firstOfNonTerminal.begin(), firstOfNonTerminal.end(), "e") == firstOfNonTerminal.end())
                    break;
            }
            else
            {
                string currentTerminal(1, currentChar);
                firstSet.push_back(currentTerminal);
                break;
            }
        }

        // Додаємо FIRST до таблиці
        for (const string &terminal : firstSet)
        {
            if (terminal != "e") // Якщо epsilon не є в FIRST, додаємо запис в таблицю
                parsingTable[{start, terminal}] = transition.end;
            else
            {
                // Якщо epsilon є в FIRST, додаємо FOLLOW[start] до таблиці
                vector<string> followSet = followk[start];
                for (const string &followTerminal : followSet)
                {
                    parsingTable[{start, followTerminal}] = transition.end;
                }
            }
        }
    }

    return parsingTable;
}

void printParsingTable(const map<pair<string, string>, string> &parsingTable)
{
    // Вивід заголовків
    cout << setw(10) << "Non-Term" << setw(10) << "Terminal" << setw(10) << "Production" << endl;

    // Вивід рядків таблиці
    for (const auto &entry : parsingTable)
    {
        string nonTerminal = entry.first.first;
        string terminal = entry.first.second;
        string production = entry.second;

        cout << setw(10) << nonTerminal << setw(10) << terminal << setw(10) << production << endl;
    }
}

// Функція для побудови LL(1)-синтаксичного аналізатора за таблицею управління
void LL1Parser(const string& input, const map<pair<string, string>, string>& parsingTable)
{
    stack<string> parseStack;
    parseStack.push("$");  // Bottom of the stack
    parseStack.push("S");  // Starting symbol

    cout << setw(20) << "Stack" << setw(20) << "Input" << setw(20) << "Action" << endl;

    size_t inputIndex = 0;
    while (!parseStack.empty())
    {
        // Print the current state
        cout << setw(20);
        for (stack<string> tmp = parseStack; !tmp.empty(); tmp.pop())
            cout << tmp.top();
        cout << setw(20) << input.substr(inputIndex) << setw(20);

        // Get the top of the stack and the current input symbol
        string stackTop = parseStack.top();
        string currentInput = (inputIndex < input.size()) ? string(1, input[inputIndex]) : "";

        // Check if the stack top is a non-terminal
        if (isNonTerminal(stackTop[0]))
        {
            // Look up the production in the parsing table
            auto it = parsingTable.find({ stackTop, currentInput });
            if (it != parsingTable.end())
            {
                string production = it->second;

                // Pop the non-terminal from the stack
                parseStack.pop();

                // Push the production onto the stack in reverse order
                for (int i = production.size() - 1; i >= 0; --i)
                {
                    if (production[i] != 'e')  // Skip epsilon
                        parseStack.push(string(1, production[i]));
                }

                // Print the production used
                cout << "Apply production: " << stackTop << " -> " << production;
            }
            else
            {
                // Error handling: no production in the parsing table
                cout << "Error: No production for " << stackTop << " and input " << currentInput;
                break;
            }
        }
        else if (stackTop == currentInput)  // Check if the stack top matches the current input symbol
        {
            // Matched terminal, pop from stack and move to the next input symbol
            parseStack.pop();
            ++inputIndex;
            cout << "Matched: " << currentInput;
        }
        else if (stackTop == "$" && currentInput.length() == 0){
            cout << "Parsing ended successfully";
            break;
        }
        else
        {
            // Error handling: stack top and input do not match
            cout << "Error: Mismatch between stack top " << stackTop << " and input " << currentInput;
            break;
        }

        cout << endl;
    }

    cout << endl;

    if (inputIndex == input.size() && !parseStack.empty() && parseStack.top() == "$" && parseStack.top().length() == 1)
        cout << "Parsing successful!" << endl;
    else
        cout << "Parsing failed!" << endl;
}

int main()
{
    string filePath = "commands.txt";
    vector<Transition> transitions = ReadAllTransitions(filePath);
    map<string, vector<string>> firstK = FirstK(transitions);
    cout << "Epsilon non-terminals:" << endl;
    set<string>epsilon = findEpsNonTerminals(transitions);
    for (auto it = epsilon.begin(); it != epsilon.end(); ++it) {
        cout << *it << endl;
    }
    //vector<char>epsilonnonterminals;
    cout << "First():" << endl;
    for (auto const& element : firstK)
    {
        cout << element.first << " ";
        for (auto const& value : element.second)
        {
            cout << value << " ";
        }
        cout << endl;
    }
    cout << "Follow():" << endl;
    map<string, vector<string>> followk = FollowK1(transitions, firstK, epsilon);
    for (auto const& element : followk)
    {
        cout << element.first << " ";
        for (auto const& value : element.second)
        {
            cout << value << " ";
        }
        cout << endl;
    }

    map<pair<string, string>, string> parsingTable = buildParsingTable(transitions, firstK, followk, epsilon);

    // Вивід таблиці управління
    cout << "Parsing Table:" << endl;
    printParsingTable(parsingTable);


    // побудова LL(1)-синтаксичного аналізатора за таблицею управління
    string input;
    //a
    //a+a
    //a*a
    //(a+a)*a
    //a+(a*a)
    cout << "Enter input string: " ;
    cin  >> input;

    cout << "Parsing Input: " << input << endl;
    cout << "----------------------------------------" << endl;


    LL1Parser(input, parsingTable);

    return 0;
}