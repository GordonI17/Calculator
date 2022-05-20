//simple calculator, reads in from the screen and returns the value of the operation

#include <iostream>
#include <sstream>
using namespace std;

struct operation {
    int index = 0;
    char operand = ' ';
};

struct operation* getOpernads(string copy) {
    int counter = 0;
    for (int i = 0; i < copy.length(); i++) {
        int spaceLoc = copy.find(" ", i); //start at i to prevent rechecking values
        if (spaceLoc != -1) {
            copy.replace(spaceLoc, 1, "");
        }
        else if (copy[i] == '*') {
            counter++;
        }
        else if (copy[i] == '/') {
            counter++;
        }
        else if (copy[i] == '+') {
            counter++;
        }
        else if (copy[i] == '-') {
            counter++;
        }
    }
    struct operation* operands = (struct operation*)calloc(counter, sizeof(struct operation));
    counter = 0;
    for (int i = 0; i < copy.length(); i++) {
        if (copy[i] == '*') {
            operands[counter].index = i;
            operands[counter].operand = '*';
            counter++;
        }
        else if (copy[i] == '/') {
            operands[counter].index = i;
            operands[counter].operand = '/';
            counter++;
        }
        else if (copy[i] == '+') {
            operands[counter].index = i;
            operands[counter].operand = '+';
            counter++;
        }
        else if (copy[i] == '-') {
            operands[counter].index = i;
            operands[counter].operand = '-';
            counter++;
        }
    }
    return operands;
}

float getNum1(int start, int end, int i, string copy, struct operation* &operands) {
    float num1 = 0;
    if (i == 0) { //first case
        cout << "before edit: " << copy << endl;
        num1 = stoi(copy.substr(start, operands[i].index - start));
        cout << "got num1: " << num1 << endl;
    }
    else {
        num1 = stoi(copy.substr(operands[i - 1].index + 1, start - operands[i].index - 1));
    }
    return num1;
}

float getNum2(int end, int i, string copy, struct operation* operands) {
    float num2 = 0;
    if (i == sizeof(operands) / sizeof(struct operation)) { //end case
        num2 = stoi(copy.substr(operands[i].index + 1, end));
    }
    else {
        num2 = stoi(copy.substr(operands[i].index + 1, operands[i + 1].index - operands[i].index - 1));
    }
    cout << "got num2: " << num2 << endl;
    return num2;
}

void PMDAS(int start, int end, string input, string &copy, struct operation* &operands, int level) {
    //initalize variable to keep track of sub operations for the calulator
    int subStart = -1;
    int subEnd = 0;
    float num1 = 0;
    float num2 = 0;
    float fresult = 0;
    int iresult = 0;
    int offset = 0;
    //cout << "level: " << level << endl;
    //cout << copy << endl;
    //cout << "start: " << start << endl;
    //cout << "end: " << end << endl;
    //check for left parentheses
    subStart = copy.find("(", start);
    if (subStart >= start) {
        for (int i = start; i <= end; i++) {
            if (copy.find(")", i) > subEnd) {
                subEnd = copy.find(")", i);
                i = subEnd;
            }
        }
        //cout << "sub-start: " << subStart << endl;
        //cout << "sub-end: " << subEnd << endl << endl;
        if (subEnd > start) {
            //recursively enter parenthases to get to first order of operation
            copy.replace(subEnd, 1, "");
            copy.replace(subStart, 1, "");
            for (int i = 0; i < sizeof(operands) / sizeof(struct operation); i++) {
                if (operands[i].index > subStart) {
                    operands[i].index--;
                }
                if (operands[i].index > subEnd) {
                    operands[i].index--;
                }
            }
            start--;
            end -= 2;
            subEnd = end;
            PMDAS(subStart, subEnd, input, copy, operands, level + 1);
            cout << endl << "level: " << level << endl << endl;
        }
        else {
            cout << "Missing )" << endl;
            return;
        }
    }
    else if (copy.find(")", start) < end) {
        cout << "Missing (" << endl;
        return;
    }
    //multiplication or division
    for (int i = 0; i < sizeof(operands) / sizeof(struct operation); i++) {
        if (operands[i].operand == '*') {
            //get numbers and multiply
            num1 = getNum1(start, end, i, copy, operands);
            num2 = getNum2(end, i, copy, operands);
            //cout << "returned num1: " << num1 << endl;
            //cout << "returned num2: " << num2 << endl;
            fresult = num1 * num2;
            iresult = fresult;
            if (iresult == fresult) {
                copy.replace(start, end - start + 1, to_string(iresult));
            }
            else {
                copy.replace(start, end - start + 1, to_string(fresult));
            }
            
        }
        else if (operands[i].operand == '/') {
            //get numbers and divide
            num1 = getNum1(start, end, i, copy, operands);
            num2 = getNum2(end, i, copy, operands);
            fresult = num1 / num2;
            iresult = fresult;
            if (iresult == fresult) {
                copy.replace(start, end - start + 1, to_string(iresult));
            }
            else {
                copy.replace(start, end - start + 1, to_string(fresult));
            }
        }
        cout << "after edit: " << copy << endl;
    }
    //addition and subtraction
    for (int i = 0; i < sizeof(*operands) / sizeof(struct operation); i++) {
        if (operands[i].operand == '+') {
            //get numbers and add
            cout << "number of operands: " << sizeof(operands) / sizeof(struct operation) << endl;
            num1 = getNum1(start, end, i, copy, operands);
            num2 = getNum2(end, i, copy, operands);
            fresult = num1 + num2;
            iresult = fresult;
            if (i == 0) { //first case
                offset = operands[i+1].index;
            }
            else if (i == sizeof(operands) / sizeof(struct operation) - 1) { //last case
                start = operands[i-1].index + 1;
                offset = end - operands[i-1].index;
            }
            else {//all other cases
                start = operands[i-1].index + 1;
                offset = operands[i + 1].index - operands[i - 1].index - 1;
            }
            if (iresult == fresult) {
                cout << "replacing: " << offset << " digits" << endl;
                copy.replace(start, offset, to_string(iresult));
            }
            else {
                cout << "replacing: " << offset << " digits" << endl;
                copy.replace(start, offset, to_string(fresult));
            }
        }
        else if (operands[i].operand == '-') {
            //get numbers and subtract
            num1 = getNum1(start, end, i, copy, operands);
            num2 = getNum2(end, i, copy, operands);
            fresult = num1 - num2;
            iresult = fresult;
            if (iresult == fresult) {
                copy.replace(start, end - start + 1, to_string(iresult));
            }
            else {
                copy.replace(start, end - start + 1, to_string(fresult));
            }
        }
    }
    operands = getOpernads(copy);
    if (level == 0 && copy != "") {
        cout << input << " = " << copy << endl;
        return;
    }
}

int main() {
    //tell the user the exit condition
    cout << "Enter 'exit' to exit" << endl << endl;
    string input = " ";
    while (input != "exit") {
        cout << "Enter an equation: ";
        getline(cin, input);
        string copy = input;
        struct operation* operands = getOpernads(copy);
        PMDAS(0, input.length()-1,input, copy, operands, 0);
        free(operands);
    }
    return 0;
}

