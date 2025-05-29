#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<unordered_map>
#include<unordered_set>
#include<algorithm>

using namespace std;
vector<pair<string, vector<string>>> grammar;
unordered_set<string> setOfSymbols;
unordered_map<string, unordered_set<string>> First, follow;
unordered_map<string, unordered_map<string, string>> table;


// Function to split a production string into symbols
vector<string> splitProduction(const string& production) {
    vector<string> symbols;
    stringstream ss(production);
    string symbol;
    while (ss >> symbol)
        symbols.push_back(symbol);
    return symbols;
}

// Input grammar: vector of <head, {bodies}>
void inputGrammar() {
    int n;
    cout << "Enter number of production rules: ";
    cin >> n;
    cin.ignore();

    for (int i = 0; i < n; i++) {
        string head, body;
        cout << "Enter head of the production rule: ";
        getline(cin, head);
        cout << "Enter the body of the production rule (space-separated): ";
        getline(cin, body);

        bool found = false;
        for (auto& rule : grammar) {
            if (rule.first == head) {
                rule.second.push_back(body);
                found = true;
                break;
            }
        }
        if (!found)
            grammar.push_back({ head, { body } });
    }
}
// Remove Left Recursion (both indirect and immediate)
void removeLeftRecursion()
{
    vector<pair<string, vector<string>>> newGrammar;

    for(int i = 0; i < grammar.size(); i++)
    {
        string Ai = grammar[i].first;
        vector<string> Ai_prods = grammar[i].second;

        // Handle indirect recursion: replace Ai -> Aj ? with Aj's productions
        //j = 0 to i-1
        for(int j = 0; j < i; j++)
        {
            string Aj = grammar[j].first;
            vector<string> Aj_prods = grammar[j].second;

            vector<string> updatedProds;
            for (auto& prod : Ai_prods)//searching for a pr Ai->Aj gamma
            {
                vector<string> symbols = splitProduction(prod);
                if(!symbols.empty() && symbols[0] == Aj)
                {
                    for(auto& delta : Aj_prods)
                    {
                        string newProd = delta;
                        for (int k = 1; k < symbols.size(); k++)
                            newProd += " " + symbols[k];
                        updatedProds.push_back(newProd);
                    }
                }
                else
                {
                    updatedProds.push_back(prod);
                }
            }
            Ai_prods = updatedProds;
        }

        // Immediate Left Recursion Removal
        vector<string> alpha, beta;
        for(auto& prod : Ai_prods)
        {
            vector<string> symbols = splitProduction(prod);
            if(!symbols.empty() && symbols[0] == Ai)//there exists pr as Ai -> Ai alpha
            {
                string suffix;
                for(int k = 1; k < symbols.size(); k++)
                    suffix += symbols[k] + " ";
                alpha.push_back(suffix);
            }
            else
            {
                string full;
                for(auto& sym : symbols)
                    full += sym + " ";

                beta.push_back(full);
            }
        }

        if (alpha.empty())//no immediate left recursion was found
        {
            newGrammar.push_back({ Ai, Ai_prods }); // no left recursion
        }
        else
        {
            string Ai_dash = Ai + "'";
            vector<string> newAiProds;
            for(auto& b : beta)
                newAiProds.push_back(b + " " + Ai_dash);

            vector<string> AiDashProds;
            for(auto& a : alpha)
                AiDashProds.push_back(a + " " + Ai_dash);
            AiDashProds.push_back("e");

            newGrammar.push_back({ Ai, newAiProds });
            newGrammar.push_back({ Ai_dash, AiDashProds });
        }
    }

    grammar = newGrammar;

    // Final Output
    cout << "\nGrammar after removing left recursion:\n";
    for (auto& rule : grammar) {
        cout << rule.first << " -> ";
        for (int i = 0; i < rule.second.size(); ++i) {
            cout << rule.second[i];
            if (i != rule.second.size() - 1)
                cout << " | ";
        }
        cout << endl;
    }
}
bool isTerminal(const string& X) {
    return !(X[0] >= 'A' && X[0] <= 'Z'); // lowercase or e = terminal
}

unordered_set<string> findFirst(const string& X)
{
    if(First.count(X)) //already computed
        return First[X];

    if(isTerminal(X))
    {
        First[X] = {X};
        return {X};
    }

    if(X == "e")
        return {X};

    unordered_set<string> result;

    // go through productions for X
    for(auto& rule : grammar)
    {
        if(rule.first != X)
                continue;

        for(auto& prod : rule.second)
        {
            vector<string> symbols = splitProduction(prod);
            bool allNullable = true;

            for(const string& sym : symbols)
            {
                unordered_set<string> tempFirst = findFirst(sym);

                for(const string& val : tempFirst)
                    if (val != "e")
                        result.insert(val);

                if (!tempFirst.count("e"))
                {
                    allNullable = false;
                    break;
                }
            }

            if (allNullable)
                result.insert("e");
        }
    }

    First[X] = result;
    return result;
}

void computeFirst()
{

    for(auto& rule : grammar)
    {
        setOfSymbols.insert(rule.first);
        for(auto& prod : rule.second) {
            vector<string> symbols = splitProduction(prod);
            for(auto& sym : symbols) {
                if(sym != "e")
                    setOfSymbols.insert(sym);
            }
        }
    }

    for(const string& X : setOfSymbols) {
        findFirst(X);
    }

    //first.erase("e");

    // Display FIRST sets
    cout << "\nFIRST sets:\n";
    for (auto& entry :First) {
        cout << "FIRST(" << entry.first << ") = { ";
        for (auto it = entry.second.begin(); it != entry.second.end(); ++it) {
            cout << *it;
            if (next(it) != entry.second.end())
                cout << ", ";
        }
        cout << " }\n";
    }
}

void findFollow(const string& A, const string& startSymbol)
{
    if (follow.find(A) != follow.end()) return;
    if(A == startSymbol)
        follow[A].insert("$");
    for(const auto& rule : grammar)
    {
        const string& head = rule.first;
        for(const string& prod : rule.second)
        {
            vector<string> symbols = splitProduction(prod);
            for(size_t i = 0; i < symbols.size(); ++i)
            {
                if (symbols[i] == A)
                {
                    if (i + 1 < symbols.size())
                    {
                        string nextSym = symbols[i + 1];

                        for(const string& f : First[nextSym])
                            if (f != "e")
                                follow[A].insert(f);

                        if(First[nextSym].count("e"))
                        {
                            findFollow(head, startSymbol);
                            for(const auto& f : follow[head])
                                follow[A].insert(f);
                        }
                    }
                    else if (head != A)
                    {
                        findFollow(head, startSymbol);
                        for (const auto& f : follow[head])
                            follow[A].insert(f);
                    }
                }
            }
        }
    }
}

void computeFollow(const string& startSymbol){
    for (const string& A : setOfSymbols)
        if (!isTerminal(A) && A != "e" )
            findFollow(A, startSymbol);

    cout << "\nFOLLOW sets:\n";
    for (const auto& entry : follow)
    {
        cout << "FOLLOW(" << entry.first << ") = { ";
        for (auto it = entry.second.begin(); it != entry.second.end(); ++it)
        {
            cout << *it;
            if (next(it) != entry.second.end())
                cout << ", ";
        }
        cout << " }\n";
    }
}
void constructParsingTable()
{
    for (auto& rule : grammar)
    {
        string head = rule.first;
        for (auto& prod : rule.second)
        {
            vector<string> symbols = splitProduction(prod);
            unordered_set<string> firstSet;

            // Step 1: Collect FIRST set of production
            bool allNullable = true;
            for (auto& sym : symbols)
            {
                unordered_set<string> first = First[sym];
                for (auto& f : first)
                    if (f != "e") firstSet.insert(f);
                if (!first.count("e"))
                {
                    allNullable = false;
                    break;
                }
            }

            // Step 2: Insert production into parsing table using FIRST set
            for (auto& terminal : firstSet)
                table[head][terminal] = prod;

            // Step 3: If production can derive e, use fOLLOW(head)
            if (allNullable)
            {
                for (auto& f : follow[head])
                    table[head][f] = "e";
            }
        }
    }

    // Output parsing table
    cout << "\nPredictive Parsing Table:\n";
    for (auto& row : table)
    {
        for (auto& col : row.second)
        {
            cout << "M[" << row.first << ", " << col.first << "] = " << row.first << " -> " << col.second << endl;
        }
    }
}
void predictiveParse(const string& inputStr, const string& startSymbol) {
    vector<string> stack;
    stack.push_back("$");
    stack.push_back(startSymbol);

    vector<string> input;
    stringstream ss(inputStr);
    string token;
    while (ss >> token)
        input.push_back(token);
    input.push_back("$");

    int ip = 0;
    cout << "\nParsing steps:\n";

    while (!stack.empty()) {
        string X = stack.back();
        string a = input[ip];

        if (X == "$" && a == "$") {
            cout << "Accepted\n";
            return;
        }

        if (X == a) {
            stack.pop_back();
            ip++;
        }
        else if (isTerminal(X) || X == "$") {
            cout << "Error: Unexpected terminal '" << X << "' on stack with input '" << a << "'\n";
            return;
        }
        else if (table[X].find(a) == table[X].end()) {
            cout << "Error: No rule for M[" << X << ", " << a << "]\n";
            return;
        }
        else {
            string production = table[X][a];
            cout << X << " -> " << production << endl;
            stack.pop_back();
            if (production != "e") {
                vector<string> symbols = splitProduction(production);
                for (int i = symbols.size() - 1; i >= 0; --i)
                    stack.push_back(symbols[i]);
            }
        }
    }

    cout << "Error: Stack empty but input not fully parsed\n";
}
// Main Function
int main()
{
    //clearing all the global variables
    grammar.clear();
    setOfSymbols.clear();
    First.clear();
    follow.clear();
    table.clear();

    inputGrammar();
    removeLeftRecursion();
    computeFirst();

    string startSymbol = grammar[0].first;
    computeFollow(startSymbol);

    constructParsingTable();

    string input;
    cout<<"Enter space-separated input string: ";
    getline(cin, input);
    predictiveParse(input, startSymbol);

    return 0;
}
