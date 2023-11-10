// LLkParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <set>

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
                    if (!isNonTerminal(transition.end[0])) continue;
                    
                    vector<string> previousNonTerminalFirstK = result[string(1, transition.end[0])];
                    vector<string> combinedWords = previousNonTerminalFirstK;
                    if (combinedWords.empty()) continue;
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
    return result;
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
    cout << ">>>>>>>>>>" << endl;
    vector<char>epsilonnonterminals;
    for (auto const& element : firstK)
    {
        cout << element.first << " ";
        for (auto const& value : element.second)
        {
            cout << value << " ";
        }
        cout << endl;
    }
    
        
    return 0;
}

