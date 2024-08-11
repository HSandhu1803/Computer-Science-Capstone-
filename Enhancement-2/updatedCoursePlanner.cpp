/*============================================================================
// Name         : ProjectTwo.cpp
// Author       : Harkamal Sandhu
// Version      : 2.1 (Enhanced Version)
// Enhancements : 1. Linked list added to handle courses.
//                2. Options added in menu to insert & remove course.
//                3. Error handling improved.
//                4. Memory management enhanced.
//                5. Encapsulation of data structure.
//Enhancements as Instructor feedback:
//				1. Error Handling: Added error handling for file operations.
//				2. Memory Management: Added a destructor to the LinkedList class to
//				   ensure all nodes are properly deleted when the list is destroyed. 
//				3. Encapsulation: Encapsulated the unordered_map and LinkedList into 
//				   a single CoursePlanner class to improve data consistency.
//				4. File Persistence: Implemented functionality to store all added
//                   and removed courses back into the original file used for input
//                   when the user chooses to exit the program.
// Input File   : ABCUinput.txt (under load data structure option)
============================================================================*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Define a structure to hold course information
struct Course {
    string title;
    vector<string> prerequisites;
};

// Node structure for the linked list
struct Node {
    string courseNumber;
    Course course;
    Node* next;

    Node(string courseNum, Course crs) : courseNumber(courseNum), course(crs), next(nullptr) {}
};

// Linked list class to handle courses
class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        clear(); // Ensure all nodes are properly deleted when the list is destroyed
    }

    void append(string courseNumber, Course course) {
        Node* newNode = new Node(courseNumber, course); // Create a new node
        if (!head) { // If the list is empty, set the new node as the head
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) { // Traverse to the end of the list
                temp = temp->next;
            }
            temp->next = newNode; // Append new node
        }
    }

    // Method to prepend a new node at the beginning of the list
    void prepend(string courseNumber, Course course) {
        Node* newNode = new Node(courseNumber, course);
        newNode->next = head;
        head = newNode;
    }

    // Method to remove a node with a specific course number
    void remove(string courseNumber) {
        if (!head) return;

        if (head->courseNumber == courseNumber) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node* temp = head;

        // Traverse to find the node before the one to be removed
        while (temp->next && temp->next->courseNumber != courseNumber) {
            temp = temp->next;
        }

        if (temp->next) {
            Node* toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
        }
    }

    // Method to find a node by its course number
    Node* find(string courseNumber) {
        Node* temp = head;
        while (temp && temp->courseNumber != courseNumber) {
            temp = temp->next;
        }
        return temp; // Return the node if found, otherwise nullptr
    }

    // Method to print the list of course numbers and their titles in sorted order
    void printCourseList() {
        Node* temp = head;
        vector<string> courseNumbers;
        while (temp) {
            courseNumbers.push_back(temp->courseNumber);
            temp = temp->next;
        }
        sort(courseNumbers.begin(), courseNumbers.end()); // Sort the course numbers
        cout << "\nCourse List:" << endl;
        for (const auto& courseNumber : courseNumbers) {
            Node* node = find(courseNumber);
            if (node) {
                cout << courseNumber << ", " << node->course.title << endl;
            }
        }
    }

    // Method to print details of a specific course by its course number
    void printCourse(string& courseNumber) {
        transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
        Node* node = find(courseNumber); // Find the node
        if (node) {
            cout << node->courseNumber << ", " << node->course.title << endl; // Print course number and title
            if (!node->course.prerequisites.empty()) {
                cout << "Prerequisites: ";
                for (size_t i = 0; i < node->course.prerequisites.size(); ++i) {
                    cout << node->course.prerequisites[i]; // Print each prerequisite
                    if (i != node->course.prerequisites.size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
            } else {
                cout << "No prerequisites." << endl;
            }
        } else {
            cout << "Course not found." << endl;
        }
    }

    // Clear the entire linked list to free up memory
    void clear() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Get all courses in a vector for writing to file
    vector<pair<string, Course>> getAllCourses() {
        vector<pair<string, Course>> courses;
        Node* temp = head;
        while (temp) {
            courses.push_back({temp->courseNumber, temp->course});
            temp = temp->next;
        }
        return courses;
    }
};

class CoursePlanner {
private:
    unordered_map<string, Course> courseData;
    LinkedList courseList;

public:
    void loadDataStructure(const string& fileName) {
        ifstream file(fileName);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) { // read line by line
                stringstream ss(line);
                string courseNumber, title, prerequisites;
                // Parsing the CSV line to extract courseNumber, title, and prerequisites
                getline(ss, courseNumber, ',');
                getline(ss, title, ',');
                getline(ss, prerequisites);

                Course course;
                course.title = title;

                stringstream prereqStream(prerequisites);
                string prereq;
                // Parsing the prerequisites separated by commas
                while (getline(prereqStream, prereq, ',')) {
                    course.prerequisites.push_back(prereq);
                }

                // Storing the course in the courseData unordered_map
                courseData[courseNumber] = course;

                // Adding the course to the linked list
                courseList.append(courseNumber, course);
            }
            file.close();
            cout << "Data structure loaded successfully." << endl;
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    void printCourseList() {
        courseList.printCourseList();
    }

    void printCourse(const string& courseNumber) {
        string num = courseNumber;
        courseList.printCourse(num);
    }

    void addCourse(const string& courseNumber, const string& courseTitle, const string& prerequisites) {
        Course course;
        course.title = courseTitle;
        stringstream prereqStream(prerequisites);
        string prereq;
        while (getline(prereqStream, prereq, ',')) {
            course.prerequisites.push_back(prereq);
        }
        courseList.append(courseNumber, course);
        cout << "Course added successfully." << endl;
    }

    void removeCourse(const string& courseNumber) {
        courseList.remove(courseNumber);
        cout << "Course removed successfully." << endl;
    }

    void saveDataStructure(const string& fileName) {
        ofstream file(fileName); // Opening file for writing
        if (file.is_open()) {
            vector<pair<string, Course>> courses = courseList.getAllCourses();
            for (const auto& coursePair : courses) {
                file << coursePair.first << "," << coursePair.second.title << ",";
                for (size_t i = 0; i < coursePair.second.prerequisites.size(); ++i) {
                    file << coursePair.second.prerequisites[i];
                    if (i != coursePair.second.prerequisites.size() - 1) {
                        file << ",";
                    }
                }
                file << endl;
            }
            file.close();
            cout << "Data saved successfully to " << fileName << endl;
        } else {
            cout << "Unable to open file for writing." << endl;
        }
    }
};

/**
 * The one and only main() method
 */
int main() {
    int option = 0;
    string fileName;
    string courseNumber;
    string courseTitle;
    string prerequisites;

    CoursePlanner planner;

    cout << "Welcome to the course planner." << endl;
    /*
     * MENU
     */
    while (option != 6) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Search a Course." << endl;
        cout << "4. Add a Course." << endl;
        cout << "5. Remove a Course." << endl;
        cout << "6. Exit" << endl << endl;
        cout << "Enter option between 1 to 6, What would you like to do? ";
        cin >> option;
        
	    switch (option) {
            case 1:
                cout << "Enter the file name: ";
                cin >> fileName;
                planner.loadDataStructure(fileName);
                break;

            case 2:
                planner.printCourseList();
                break;

            case 3:
                cout << "What course do you want to know about?\n";
                cout << "Input course number: ";
                cin >> courseNumber;
                planner.printCourse(courseNumber);
                break;

            case 4:
                cout << "Enter the course number: ";
                cin >> courseNumber;
                cout << "Enter the course title: ";
                cin.ignore();
                getline(cin, courseTitle);
                cout << "Enter the prerequisites (comma separated): ";
                getline(cin, prerequisites);
                planner.addCourse(courseNumber, courseTitle, prerequisites);
                break;

            case 5:
                cout << "Enter the course number to remove: ";
                cin >> courseNumber;
                planner.removeCourse(courseNumber);
                break;

            case 6:
                planner.saveDataStructure(fileName);
                cout << "Thank you for using the course planner!" << endl;
                break;

            default:
                cout << option << " is not a valid option." << endl;
        }
    }

    return 0;
}
