//simple calculator, readds in from the screen and returns the value of the operation

#include <iostream>
using namespace std;

int main()
{
    cout << "To exit enter 'exit'\n";
    string input = " ";
    while (input != "exit") {
        cout << "Enter an equation: ";

        cin >> input;
    }
         
}

