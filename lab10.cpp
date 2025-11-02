#include <iostream> // For input/output
#include <string> // For input/output and string handling
#include <fstream> // For file handling
#include <algorithm> // For string manipulation
using namespace std; // Exclude std:: prefix

bool validate(const string& s) { // Function to validate if a string is a valid double
    if (s.empty()) return false; // Check for empty string
    int i = 0, n = s.size(); // Initialize index and size
    if (s[i] == '+' || s[i] == '-') i++; // Handle optional sign
    if (i == n) return false; // String cannot be just a sign

    bool period = false; // Flag for decimal point
    bool numberb4 = false;  // Flags for digits before and after period
    bool numbera8 = false; 

    for (; i < n; i++) { // Iterate through characters
        char c = s[i]; // Current character
        if (c >= '0' && c <= '9') { // Handle digits
            if (period) numberb4 = true; // Digits after period
            else numbera8 = true; // Digits before period
        }
        else if (c == '.') { // Handle decimal point
            if (period) return false;
            period = true; // Mark that we have seen a period
        }
        else return false; // Invalid character
    }

    if (period) return (numbera8 && numberb4); // Need digits both before and after period
    return numbera8; // If no period, just need digits
}

int comparePositive(const string &int1, const string &frac1, const string &int2, const string &frac2) { // Function to compare two positive numbers 
    string i1 = int1, i2 = int2; // Integer parts
    while (i1.size() < i2.size()) i1 = '0' + i1; // Pad integer parts
    while (i2.size() < i1.size()) i2 = '0' + i2; // Pad integer parts
    if (i1 > i2) return 1; // Compare integer parts
    if (i1 < i2) return -1; // Compare integer parts

    string f1 = frac1, f2 = frac2; // Fractional parts
    while (f1.size() < f2.size()) f1 += '0'; // Pad fractional parts
    while (f2.size() < f1.size()) f2 += '0'; // Pad fractional parts
    if (f1 > f2) return 1; // Compare fractional parts
    if (f1 < f2) return -1; // Compare fractional parts
    return 0; // They are equal
}

void splitNumber(const string &s, string &intPart, string &fracPart, char &sign) { // Function to split number into parts
    size_t i = 0; // Start index
    sign = '+'; // Default sign
    if (s[i] == '+' || s[i] == '-') { sign = s[i]; i++; } // Handle sign
    size_t dotPos = s.find('.', i); // Find decimal point
    if (dotPos == string::npos) {// No decimal point
        intPart = s.substr(i); //  Exclude the sign
        fracPart = ""; // No fractional part
    } else { 
        intPart = s.substr(i, dotPos - i); //   Exclude the sign
        fracPart = s.substr(dotPos + 1); // Exclude the dot
    }
}

string addStrings(const string &num1, const string &num2) { // Function to add two string numbers
    string int1, frac1, int2, frac2; //     Parts of the numbers
    char sign1, sign2; // Signs of the numbers
    splitNumber(num1, int1, frac1, sign1); // Split first number
    splitNumber(num2, int2, frac2, sign2); // Split second number

    while (frac1.size() < frac2.size()) frac1 += '0'; // Pad fractional parts
    while (frac2.size() < frac1.size()) frac2 += '0'; // Pad fractional parts

    string intSum = "", fracSum = ""; // Result parts
    int carry = 0; // Carry for addition

    if (sign1 == sign2) { // Same sign, perform addition
        for (int i = (int)frac1.size() - 1; i >= 0; i--) { // Add fractional part
            int digitSum = (frac1[i] - '0') + (frac2[i] - '0') + carry; // Add digits
            carry = digitSum / 10; //  Calculate carry
            fracSum = char(digitSum % 10 + '0') + fracSum; // Build fractional part
        }

        while (int1.size() < int2.size()) int1 = '0' + int1; // Pad integer parts
        while (int2.size() < int1.size()) int2 = '0' + int2; // Pad integer parts

        for (int i = (int)int1.size() - 1; i >= 0; i--) { // Add integer part
            int digitSum = (int1[i] - '0') + (int2[i] - '0') + carry; // Add digits
            carry = digitSum / 10; //   Calculate carry
            intSum = char(digitSum % 10 + '0') + intSum; // Build integer part
        }
        if (carry) intSum = '1' + intSum; // Handle final carry

        while (!fracSum.empty() && fracSum.back() == '0') fracSum.pop_back(); // Remove trailing zeros

        string result = (sign1 == '-' ? "-" : "") + intSum; // Start with sign and integer part
        if (!fracSum.empty()) result += "." + fracSum; // Append fractional part if exists
        return result; // Return the final result
    } else {
        int cmp = comparePositive(int1, frac1, int2, frac2); // Compare absolute values
        string *bigInt, *bigFrac, *smallInt, *smallFrac; // Pointers to bigger and smaller numbers
        char resultSign; // Result sign

        if (cmp == 0) return "0"; // They are equal

        if (cmp > 0) { // num1 is bigger
            bigInt = &int1; bigFrac = &frac1; // Note the swap
            smallInt = &int2; smallFrac = &frac2; // Note the swap
            resultSign = sign1; // Note the sign
        } else {
            bigInt = &int2; bigFrac = &frac2; // Note the swap 
            smallInt = &int1; smallFrac = &frac1; // Note the swap 
            resultSign = sign2; // Note the sign change here
        }

        carry = 0; // Reset carry for subtraction
        fracSum = ""; // Initialize fractional sum
        for (int i = (int)bigFrac->size() - 1; i >= 0; i--) { // Subtract fractional part
            int diff = ((*bigFrac)[i] - '0') - ((*smallFrac)[i] - '0') - carry; // Subtract digits
            if (diff < 0) { diff += 10; carry = 1; } else carry = 0; // Borrow if needed
            fracSum = char(diff + '0') + fracSum; // Build fractional part
        }

        while (bigInt->size() < smallInt->size()) *bigInt = '0' + *bigInt; // Pad integer parts
        while (smallInt->size() < bigInt->size()) *smallInt = '0' + *smallInt; // Pad integer parts

        intSum = "";
        for (int i = (int)bigInt->size() - 1; i >= 0; i--) { // Subtract integer part
            int diff = ((*bigInt)[i] - '0') - ((*smallInt)[i] - '0') - carry; // Subtract digits
            if (diff < 0) { diff += 10; carry = 1; } else carry = 0; // Borrow if needed
            intSum = char(diff + '0') + intSum; // Build integer part
        }

        while (intSum.size() > 1 && intSum[0] == '0') intSum.erase(0,1); // Remove leading zeros
        while (!fracSum.empty() && fracSum.back() == '0') fracSum.pop_back(); // Remove trailing zeros

        string result = (resultSign == '-' ? "-" : "") + intSum; // Start with sign and integer part
        if (!fracSum.empty()) result += "." + fracSum; // Append fractional part if exists
        return result; // Return the final result
    }
}

int main() { // Main function
    string filename; // Variable to hold filename
    cout << "Enter the filename: "; // Prompt for filename
    cin >> filename; // Get filename from user

    ifstream file(filename); // Open the file
    if (!file) { // Check if file opened successfully
        cerr << "Error opening file." << endl; // Error message
        return 1; // Exit if file cannot be opened
    }

    string a, b; // Variables to hold input numbers
    while (file >> a >> b) { // Read pairs of numbers
        if (!validate(a)) { cout << a << " is not a valid double.\n"; continue; } //    Validate inputs
        if (!validate(b)) { cout << b << " is not a valid double.\n"; continue; } // Validate inputs

        string result = addStrings(a, b); // Perform addition
        cout << a << " + " << b << " = " << result << endl; //  Output the result
    }

    return 0; // End of main function
}

