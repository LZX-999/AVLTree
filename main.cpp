
#include <iostream>
#include <string>
#include <vector>

#include "tree.h"
using namespace std;

/**int main() {
    Stack<char> s;
    string valid = "()()()";
    string invalid = "((((((";
    for (int i = 0; i < invalid.size(); i++) {
        if (invalid[i] == '(') {
            s.push(invalid[i]);
        }
        else if (s.top() == '(' && invalid[i] == ')') {
            s.pop();
        }
        else {
            cout << "Invalid" << endl;
            return 0;
        }
    }
    if (s.empty()) {
        cout << "Valid" << endl;
    }
    else {
        cout << "Invalid" << endl;
    } 
}**/

int main() {
    Tree<int> t;
    for (int i = 0; i < 10; i++) {
        t.insert(i);
    }
    t.deleteNode(3);
    cout << t.in_order();
    

        

    

}