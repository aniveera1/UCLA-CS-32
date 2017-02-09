//
//  eval.cpp
//  hw2
//
//  Created by Anirudh Veeraragavan on 2/2/17.
//  Copyright © 2017 Anirudh Veeraragavan. All rights reserved.
//

#include <stack>
#include <string>
#include <cctype>
using namespace std;

// Evaluates a boolean expression
// Postcondition: If infix is a syntactically valid infix boolean
//   expression, then postfix is set to the postfix form of that
//   expression, result is set to the value of the expression (where
//   in that expression, each digit k represents element k of the
//   values array), and the function returns zero.  If infix is not a
//   syntactically valid expression, the function returns 1.  (In that
//   case, postfix may or may not be changed, but result must be
//   unchanged.)

bool finalEval (const bool values[], string& postfix);

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
    postfix = "";
    stack<char> operatorStack;
    
    // Keeps track of whether for each open parantheses
    // there is a closed parantheses
    int paranthesesTracker = 0;
    
    // Keeps track of the number of digits
    int digitTracker = 0;
    
    for (int i = 0; i < infix.size(); i++)
    {
        switch (infix[i])
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                // This code checks whether two digits
                // are next to each other in the infix string
                int j = i;
                while (j > 0 && infix[j - 1] == ' ')
                    j--;
                if (j > 0 && isdigit(infix[j - 1]))
                    return 1;
                
                digitTracker++;
                postfix += infix[i];
                break;
            }
            case '(':
            {
                // This code checks whether a digit is immediately
                // to the left of the open parantheses
                int j = i;
                while (j > 0 && infix[j - 1] == ' ')
                    j--;
                if (j > 0 && isdigit(infix[j - 1]))
                    return 1;
                
                paranthesesTracker++;
                operatorStack.push(infix[i]);
                break;
            }
            case '!':
            {
                operatorStack.push(infix[i]);
                break;
            }
            case ')':
            {
                // This code checks whether a digit is immediately
                // to the right of the closed parantheses
                int j = i;
                while (j < infix.size() && infix[j + 1] == ' ')
                    j++;
                if (j < infix.size() && isdigit(infix[j + 1]))
                    return 1;
                
                paranthesesTracker--;
                while (! operatorStack.empty() && operatorStack.top() != '(')
                {
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                
                if (!operatorStack.empty())
                    operatorStack.pop();
                break;
            }
            case '&':
            case '|':
            {
                int j = i;
                // This code checks that a digit exists on
                // the right of the binary operator
                while (j > 0 && (infix[j - 1] == ' ' || infix[j - 1] == ')'))
                    j--;
                if (j == 0 || ! isdigit(infix[j - 1]))
                {
                    return 1;
                }
                
                j =i;
                // This code checks that a digit exists on
                // the left of the binary operator
                while (j < infix.size() && (infix[j + 1] == ' ' || infix[j + 1] == '(' || infix[j + 1] == '!'))
                    j++;
                if (j == infix.size() || ! isdigit(infix[j + 1]))
                {
                    return 1;
                }
                while (! operatorStack.empty() && ! (operatorStack.top() == '('))
                {
                    if (infix[i] == '&')
                    {
                        if (operatorStack.top() == '|')
                            break;
                    }
                    else if (infix[i] == '!')
                    {
                        if (operatorStack.top() == '|' || operatorStack.top() == '&')
                            break;
                    }
                    postfix += operatorStack.top();
                    operatorStack.pop();
                }
                operatorStack.push(infix[i]);
                break;
            }
        }
    }
    
    if (paranthesesTracker != 0 || digitTracker == 0)
        return 1;
    
    while (! operatorStack.empty())
    {
        postfix += operatorStack.top();
        operatorStack.pop();
    }
    
    result = finalEval(values, postfix);
    return 0;
}

bool finalEval (const bool values[], string& postfix)
{
    stack<bool> operandStack;
    for (int j = 0; j < postfix.size(); j++)
    {
        if (isdigit(postfix[j]))
        {
            int count = postfix[j] - '0';
            operandStack.push(values[count]);
        }
        else if (postfix[j] == '!')
        {
            bool operand = operandStack.top();
            operandStack.pop();
            operand = ! operand;
            operandStack.push(operand);
        }
        else
        {
            bool operand2 = operandStack.top();
            operandStack.pop();
            bool operand = operandStack.top();
            operandStack.pop();
            bool result;
            if (postfix[j] == '&')
            {
                if (operand == true && operand2 == true)
                    result = true;
                else
                    result = false;
            }
            else if (postfix[j] == '|')
            {
                if (operand == false && operand2 == false)
                    result = false;
                else
                    result = true;
            }
            operandStack.push(result);
        }
    }
    return operandStack.top();
}
