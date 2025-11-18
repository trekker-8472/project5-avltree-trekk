/*
Driver code for testing your AVL Tree
This is NOT the test code for grading,
instead for you to get an idea of how to test the tree
 */
#include "AVLTree.h"
#include <iostream>
#include <string>
#include <ranges>
#include <vector>
using namespace std;


int main() {
AVLTree tree;
bool insertResult;

// Initial Inserts
insertResult = tree.insert("Flank", 70);       // 'F'
//insertResult = tree.insert("Flank", 70);   // false, no duplicates allowed
insertResult = tree.insert("Loin", 76);         // 'L'
insertResult = tree.insert("Round", 82);       // 'R' // Single rotate left (RR case)
cout << endl << endl;
cout << tree << endl;

// Left-side Inserts
insertResult = tree.insert("Chuck", 67);       // 'C'
insertResult = tree.insert("Brisket", 66);   // 'B' // Single rotate right (LL case)
// cout << endl << endl;
// cout << tree << endl;

// Rotation Triggers
insertResult = tree.insert("Tenderloin", 84); // 'T' // Double rotate right (RL case)
// cout << endl << endl;
// cout << tree << endl;

insertResult = tree.insert("Sirloin", 83);   // 'S' // Double rotate left (LR case)
// cout << endl << endl;
// cout << tree << endl;

// Remaining Inserts
insertResult = tree.insert("Rib", 82);           // 'R'
// insertResult = tree.insert("Brisket", 66); // false, duplicate
insertResult = tree.insert("Short Loin", 83); // 'S'
insertResult = tree.insert("Plate", 80);       // 'P'
// insertResult = tree.insert("Tenderloin", 84); // false, duplicate
// cout << endl << endl;
// cout << tree << endl;

// size and getHeight
// cout << "tree size: " << tree.size() << endl; // 10
// cout << "tree height: " << tree.getHeight() << endl; // Likely 4
// cout << endl;

// contains
bool containsResult;
containsResult = tree.contains("Brisket"); // true
// containsResult = tree.contains("Wagyu"); // false

// get
optional<size_t> getResult;

getResult = tree.get("Brisket"); // 66 ('B')
cout << "Brisket: " << getResult.value() << endl;

getResult = tree.get("Chuck"); // 67 ('C')
cout << "Chuck: " << getResult.value() << endl;

getResult = tree.get("Shoulder"); // getResult has no value
cout << "Shoulder: " << getResult.has_value() << endl; // print 0
cout << endl;

// findRange
// vector<size_t> rangeTest = tree.findRange("Chuck", "Rib");
// Expected keys in range: Chuck, Flank, Loin, Plate, Rib, Round, Sirloin, Short Loin, Tenderloin
// for (auto val: rangeTest) {
//     cout << val << " ";
// }
// cout << endl << endl;

// operator[]
// tree["Brisket"] = 108; // Update Brisket's value
// cout << tree << endl;
// cout << endl;

// remove
// bool removeResult;
// removeResult= tree.remove("Brisket"); // Leaf
// cout << endl << endl;
// cout << tree << endl;

// removeResult = tree.remove("Chuck"); // One child
// cout << endl << endl;
// cout << tree << endl;

// removeResult = tree.remove("Flank"); // Two children
// cout << endl << endl;
// cout << tree << endl;

// removeResult = tree.remove("Rib");
// removeResult = tree.remove("Round");
// removeResult = tree.remove("Short Loin"); // double rotate right (RL case likely after removal)
// cout << endl << endl;
// cout << tree << endl;

    return 0;
}
