//eval.cpp

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool validOperator(char ch);
bool validOperand(char ch);
int operatorPrecedence(char ch);
bool validInfix(string infix);
bool inMap(string infix, const Map& values);
string infixToPostfix(string infix);
bool divisionByZero(string infix, const Map& values);
int calculatePostfixValue(string postfix, const Map& values);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    if (!validInfix(infix))
        return 1;
    postfix = infixToPostfix(infix);
    if (!inMap(infix, values))
        return 2;
    if (divisionByZero(postfix, values))
        return 3;
    result = calculatePostfixValue(postfix, values);
    return 0;
}

bool validOperator(char ch)
{
    switch (ch)
    {
        case '(':
        case ')':
        case '*':
        case '/':
        case '+':
        case '-':
        case ' ':
            return true;
    }
    return false;
}

bool validOperand(char ch)
{
    if (isalpha(ch) && islower(ch))
        return true;
    return false;
}

int operatorPrecedence(char ch)
{
    int precedence = 0;
    switch (ch)
    {
        case '(':
        case ')':
            precedence = 0;
            break;
        case '+':
        case '-':
            precedence = 1;
            break;
        case '*':
        case '/':
            precedence = 2;
    }
    return precedence;
}

bool validInfix(string infix)
{
    if (infix == "")
        return false;
    for (int i = 0; i < infix.size(); i++)
    {
        if (isalpha(infix[i]) && !islower(infix[i]))
            return false;
        else if (!isalpha(infix[i]))
        {
            if (!validOperator(infix[i]))
                return false;
        }
    }
    
    int letterCount = 0;
    int operatorCount = 0;
    int openParenCount = 0;
    int closedParenCount = 0;
    for (int i = 0; i < infix.size(); i++)
    {
        if (isalpha(infix[i]) && islower(infix[i]))
            letterCount++;
        else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/')
            operatorCount++;
        else if (infix[i] == '(')
            openParenCount++;
        else if (infix[i] == ')')
            closedParenCount++;
    }
    
    if (openParenCount != closedParenCount)
        return false;
    if (letterCount - operatorCount != 1)
        return false;
    
    for (int i = 0; i < infix.size() - 1; i++)
    {
        if ((infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/') &&
            (infix[i + 1] == '+' || infix[i + 1] == '-' || infix[i + 1] == '*' || infix[i + 1] == '/'))
            return false;
    }
    
    for (int i = 0; i < infix.size() - 1; i++)
    {
        if ((isalpha(infix[i]) && islower(infix[i]) && (isalpha(infix[i + 1]) && islower(infix[i + 1]))))
            return false;
    }
    
    return true;
}

bool inMap(string infix, const Map& values)
{
    for (int i = 0; i < infix.size(); i++)
    {
        if (isalpha(infix[i]) && islower(infix[i]))
        {
            if (!values.contains(infix[i]))
                return false;
        }
    }
    return true;
}

string infixToPostfix(string infix)
{
    string postfix = "";
    stack<char> operatorStack;
    
    for (int i = 0; i < infix.size(); i++)
    {
        switch (infix[i])
        {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
                postfix += infix[i];
                break;
            case '(':
                operatorStack.push(infix[i]);
                break;
            case '+':
            case '-':
            case '*':
            case '/':
                while (!operatorStack.empty() && operatorStack.top() != '(' &&
                       operatorPrecedence(infix[i]) <= operatorPrecedence(operatorStack.top()))
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(infix[i]);
                break;
            case ')':
                while (operatorStack.top() != '(')
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.pop();
                break;
        }
    }
    
    while (!operatorStack.empty())
    {
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    return postfix;
}

bool divisionByZero(string postfix, const Map& values)
{
    stack<int> operandStack;
    for (int i = 0; i < postfix.size(); i++)
    {
        if (isalpha(postfix[i]))
        {
            int getValue = 0;
            values.get(postfix[i], getValue);
            operandStack.push(getValue);
        }
        else
        {
            int operand2 = operandStack.top();
            operandStack.pop();
            int operand1 = operandStack.top();
            operandStack.pop();
            int result = 0;
            switch (postfix[i])
            {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    if (operand2 == 0)
                        return true;
                    result = operand1 / operand2;
                    break;
            }
            operandStack.push(result);
            
        }
    }
    return false;
}

int calculatePostfixValue(string postfix, const Map& values)
{
    if (postfix.size() == 1)
    {
        int getValue = 0;
        values.get(postfix[0], getValue);
        return getValue;
    }
    
    stack<int> operandStack;
    for (int i = 0; i < postfix.size(); i++)
    {
        if (isalpha(postfix[i]))
        {
            int getValue = 0;
            values.get(postfix[i], getValue);
            operandStack.push(getValue);
        }
        else
        {
            int operand2 = operandStack.top();
            operandStack.pop();
            int operand1 = operandStack.top();
            operandStack.pop();
            int result = 0;
            switch (postfix[i])
            {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    result = operand1 / operand2;
                    break;
            }
            operandStack.push(result);
        }
    }
    return operandStack.top();
}

int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
           pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary minus not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
           pf == "ab*"  &&  answer == 999);
    assert(evaluate("y+o*(a-u) ", m, pf, answer) == 0  &&
           pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
           pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
}