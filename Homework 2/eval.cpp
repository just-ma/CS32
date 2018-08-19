#include <string>
#include <stack>
using namespace std;

int evaluate(string infix, string& postfix, bool& result)
{
    postfix = "";
    stack<char> s;
    for (int n = 0 ; n < infix.size() ; n++)
    {
        switch (infix[n])
        {
            case ' ':
                break;
            case 'T':
            case 'F':
                postfix += infix[n];
                while ((!s.empty()) && s.top() == '!')
                {
                    postfix += s.top();
                    s.pop();
                }
                break;
            case '(':
                if (n < infix.size() && (infix[n+1] == '&' || infix[n+1] == '|'))
                    return 1;
                s.push('(');
                break;
            case')':
                if (n > 0 && (infix[n-1] == '&' || infix[n-1] == '|' || infix[n-1] == '!'))
                    return 1;
                while((!s.empty()) && s.top() != '(') //s.top() create an error if empty
                {
                    postfix += s.top();
                    s.pop();
                }
                s.pop();
                break;
            case '&':
            case '|':
                while(((!s.empty()) && (s.top() != '(')) && (((infix[n] == '&') && (s.top() != '|')) || ((infix[n] == '|'))))
                {
                    postfix += s.top();
                    s.pop();
                }
                s.push(infix[n]);
                break;
            case '!':
                s.push(infix[n]);
                break;
            default:
                return 1;
        }
    }
    while (!s.empty())
    {
        if (s.top() == '(' || s.top() == ')')
            return 1;
        postfix += s.top();
        s.pop();
    }
    
    stack<char> o;
    char operand1, operand2;
    for ( int n = 0; n < postfix.size(); n++)
    {
        if (postfix[n] == 'T' || postfix[n] == 'F')
        {
            o.push(postfix[n]);
        }
        else if (postfix[n] == '!')
        {
            if (o.empty())
                return 1;
            else if (o.top() == 'T')
            {
                o.pop();
                o.push('F');
            }
            else if (o.top() == 'F')
            {
                o.pop();
                o.push('T');
            }
        }
        else
        {
            if (o.empty())
                return 1;
            operand2 = o.top();
            o.pop();
            if (o.empty())
                return 1;
            operand1 = o.top();
            o.pop();
            
            if (postfix[n] == '&')
            {
                if (operand1 == 'T' && operand2 == 'T')
                    o.push('T');
                else
                    o.push('F');
            }
            else if (postfix[n] == '|')
            {
                if (operand1 == 'T' || operand2 == 'T')
                    o.push('T');
                else
                    o.push('F');
            }
        }
    }
    if (o.size() == 1)
    {
        if (o.top() == 'T')
            result = true;
        else
            result = false;
        return 0;
    }
    else
        return 1;
}
