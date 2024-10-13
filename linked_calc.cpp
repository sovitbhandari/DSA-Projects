/*
Name: Sovit Bhandari
UID: U83561265
Project Description: This project implements a basic calculator using a singly linked list to store and evaluate mathematical expressions. It validates the expression by ensuring correct digit and operator placement, supports floating-point numbers, and handles operations such as addition, subtraction, multiplication, and division. The program processes expressions efficiently and includes error handling for invalid cases like division by zero.
*/

#include "linked_calc.hpp"

// Default constructor definition
template <typename T>
LinkedCalc<T>::LinkedCalc() : head(nullptr) {}

// Destructor to deallocate memory for all nodes in the linked list
template <typename T>
LinkedCalc<T>::~LinkedCalc() {
    if (head == nullptr) {
        return; // No nodes to delete if the list is empty
    }
    Node<T>* currentNode = head;
    Node<T>* previousNode = nullptr;
    while (currentNode != nullptr) {
        previousNode = currentNode;
        currentNode = currentNode->next;
        delete previousNode; // Free memory for each node while traversing the list
    }
}

// Function to insert a new node at the end of the linked list
template <typename T>
void LinkedCalc<T>::insert(const T& value) {
    Node<T>* newNode = new Node<T>(value); // Allocate a new node with the given value
    if (head == nullptr) {
        head = newNode; // If the list is empty, the new node becomes the head
    } else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next; // Traverse to the end of the list
        }
        current->next = newNode; // Link the new node at the end
    }
}

// Helper function to determine if a character is a digit
template <typename T>
bool LinkedCalc<T>::isDigit(const T& c) {
    return (c >= '0' && c <= '9'); // Checks if the character is a digit
}

// Convert to float value
template<typename T>
float LinkedCalc<T>::convertToFloat(Node<T>*& current) {
   if (current == nullptr) {
       return 0.0f; // Return 0 if the node is null
   }
   return static_cast<float>(current->data); // Convert node data to float
}


// Function to validate the mathematical expression stored in the linked list
template <typename T>
bool LinkedCalc<T>::validateExpression() {
    if (head == nullptr) {
        return false; // Return false if the list is empty (invalid expression)
    }

    Node<T>* tempNode = head;
    bool foundDigit = false;  // Flag to track if a digit has been encountered
    bool foundDecimal = false; // Flag to track if a decimal point has been encountered

    while (tempNode != nullptr) {
        // Check if the current node contains an operator
        if (tempNode->data == '+' || tempNode->data == '-' || tempNode->data == '/' || tempNode->data == '*') {
            if (!foundDigit) {
                return false; // An operator without a preceding number is invalid
            }
            foundDigit = false;  // Reset digit flag after an operator
            foundDecimal = false; // Reset decimal flag after an operator
        } else if (isDigit(tempNode->data)) {
            foundDigit = true; // Mark that a number has been encountered
        } else if (tempNode->data == '.') {
            if (foundDecimal) {
                return false; // Invalid if multiple decimal points are found in one number
            }
            foundDecimal = true; // Mark that a decimal has been encountered
        } else {
            return false; // Invalid character encountered in the expression
        }
        tempNode = tempNode->next;
    }

    return foundDigit; // The expression must end with a number to be valid
}

// Function to evaluate the expression stored in the linked list
template <typename T>
float LinkedCalc<T>::evaluateExpression() {
    if (head == nullptr) {
        return 0.0f; // Return 0 if the list is empty
    }

    Node<T>* tempNode = head;
    float totalResult = 0.0f;       // Holds the cumulative result of the expression
    float currentNumber = 0.0f;     // Stores the current number being processed
    char lastOperation = '+';       // Initially assume addition for the first number
    bool foundDecimal = false;      // Flag to track if a decimal point is encountered in a number
    int decimalCount = 0;           // Track the number of decimal places for floating-point numbers

    // Traverse the linked list and process the expression
    while (tempNode != nullptr) {
        if (isDigit(tempNode->data)) {
            // Build the current number digit by digit
            if (foundDecimal) {
                decimalCount++;
                currentNumber += (tempNode->data - '0') / pow(10, decimalCount); // Handle fractional part for decimal numbers
            } else {
                currentNumber = currentNumber * 10 + (tempNode->data - '0'); // Build the whole number
            }
        } else if (tempNode->data == '.') {
            // Mark that a decimal point has been encountered
            foundDecimal = true;
        } else if (tempNode->data == '*' || tempNode->data == '/') {
            // Handle multiplication and division immediately
            char operation = tempNode->data;
            tempNode = tempNode->next; // Move to the next node, which should be a number
            float nextNumber = 0.0f;
            foundDecimal = false;      // Reset decimal tracking for the new number
            decimalCount = 0;

            // Build the next number (to be used in the multiplication/division)
            while (tempNode != nullptr && (isDigit(tempNode->data) || tempNode->data == '.')) {
                if (isDigit(tempNode->data)) {
                    if (foundDecimal) {
                        decimalCount++;
                        nextNumber += (tempNode->data - '0') / pow(10, decimalCount); // Build the fractional part
                    } else {
                        nextNumber = nextNumber * 10 + (tempNode->data - '0'); // Build the whole number
                    }
                } else if (tempNode->data == '.') {
                    foundDecimal = true; // Handle decimal point in the next number
                }
                tempNode = tempNode->next;
            }

            // Perform the multiplication or division operation
            if (operation == '*') {
                currentNumber *= nextNumber;
            } else if (operation == '/') {
                if (nextNumber == 0) {
                    throw std::runtime_error("Division by zero."); // Throw an error if division by zero is attempted
                }
                currentNumber /= nextNumber;
            }

            continue; // Skip to the next iteration after performing the operation
        } else {
            // Handle addition and subtraction after processing the number
            if (lastOperation == '+') {
                totalResult += currentNumber; // Add the current number to the total
            } else if (lastOperation == '-') {
                totalResult -= currentNumber; // Subtract the current number from the total
            }

            // Store the current operator for the next number
            lastOperation = tempNode->data;
            currentNumber = 0.0f;  // Reset current number for the next number
            foundDecimal = false;  // Reset decimal tracking for the next number
            decimalCount = 0;      // Reset decimal count for the next number
        }

        tempNode = tempNode->next;
    }

    // Final operation to handle the last number in the expression
    if (lastOperation == '+') {
        totalResult += currentNumber;
    } else if (lastOperation == '-') {
        totalResult -= currentNumber;
    }

    return totalResult; // Return the final evaluated result of the expression
}
