//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Harkamal Sandhu
// Version     : 1.0
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// define a structure to hold course information
struct Course {
    string title;
    vector<string> prerequisites;
};

unordered_map<string, Course> courseData;

// Function to load data from a file into the data structure
void loadDataStructure(const string& fileName) {
    ifstream file(fileName);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) { //read line by line
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

            // Storing the course in the courseData map
            courseData[courseNumber] = course;
        }
        file.close();
        cout << "Data structure loaded successfully." << endl;
    } else {
        cout << "Unable to open file." << endl;
    }
}

// Function to print the course list in alphanumeric order
void printCourseList() {
    vector<string> courseNumbers;
    
	// Collecting all the course numbers in the courseData map
    for (const auto& pair : courseData) {
        courseNumbers.push_back(pair.first);
    }
    
	// Sorting the course numbers in alphanumeric order
    sort(courseNumbers.begin(), courseNumbers.end());

    cout << "\nCourse List:" << endl;
	// Printing the course number and title for each course in the sorted order
    for (const auto& courseNumber : courseNumbers) {
        cout << courseNumber << ", " << courseData[courseNumber].title << endl;
    }
}

// Function to print course information, including prerequisites
void printCourse(string& courseNumber) {
    
	// Convert the courseNumber to uppercase for case-insensitive matching
    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

    auto it = courseData.find(courseNumber);

    if (it != courseData.end()) {
        cout << it->first << ", " << it->second.title << endl;

        if (!it->second.prerequisites.empty()) {
            
			cout << "Prerequisites: ";
            // Printing the prerequisites separated by commas
            for (size_t i = 0; i < it->second.prerequisites.size(); ++i) {
                cout << it->second.prerequisites[i];
                if (i != it->second.prerequisites.size() - 1) {
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

/**
 * The one and only main() method
 */
int main() {
    int option = 0;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;
	/*
	 * MENU
	 */
    while (option != 9) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl << endl;
        cout << "What would you like to do? ";
        cin >> option;

        switch (option) {
            case 1:
            	/* Here, fileName is "ABCUinput.txt */
                cout << "Enter the file name: ";
                cin >> fileName;
                loadDataStructure(fileName);
                break;
            case 2:
                printCourseList();
                break;
            case 3:
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                printCourse(courseNumber);
                break;
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << option << " is not a valid option." << endl;
                break;
        }
    }

    return 0;
}

