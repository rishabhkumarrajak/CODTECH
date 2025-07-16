#include <iostream>
#include <stack>
#include <sstream>
#include <cctype>
#include <map>
using namespace std;

// Check if character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Set precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Convert infix expression to postfix
string infixToPostfix(const string& infix) {
    stack<char> opStack;
    string postfix;
    
    for (size_t i = 0; i < infix.length(); ++i) {
        char token = infix[i];

        if (isspace(token)) continue;

        if (isdigit(token)) {
            while (i < infix.length() && (isdigit(infix[i]) || infix[i] == '.'))
                postfix += infix[i++];
            postfix += ' ';
            i--;
        }
        else if (token == '(') {
            opStack.push(token);
        }
        else if (token == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top(); postfix += ' ';
                opStack.pop();
            }
            opStack.pop(); // Remove '('
        }
        else if (isOperator(token)) {
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(token)) {
                postfix += opStack.top(); postfix += ' ';
                opStack.pop();
            }
            opStack.push(token);
        }
    }

    while (!opStack.empty()) {
        postfix += opStack.top(); postfix += ' ';
        opStack.pop();
    }

    return postfix;
}

// Evaluate postfix expression
double evaluatePostfix(const string& postfix) {
    stack<double> valStack;
    istringstream tokens(postfix);
    string token;

    while (tokens >> token) {
        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            valStack.push(stod(token));
        }
        else if (token.size() == 1 && isOperator(token[0])) {
            double b = valStack.top(); valStack.pop();
            double a = valStack.top(); valStack.pop();
            switch (token[0]) {
                case '+': valStack.push(a + b); break;
                case '-': valStack.push(a - b); break;
                case '*': valStack.push(a * b); break;
                case '/': valStack.push(a / b); break;
            }
        }
    }

    return valStack.top();
}

int main() {
    string expression;
    cout << "🔢 Enter arithmetic expression (e.g., 2 + 3 * (4 - 1)): ";
    getline(cin, expression);

    string postfix = infixToPostfix(expression);
    double result = evaluatePostfix(postfix);

    cout << "✅ Result: " << result << endl;
    return 0;
}
