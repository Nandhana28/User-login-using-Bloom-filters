#include <iostream>
#include <string>
#include "UserAuth.h"

using namespace std;

// function to show menu
void displayMenu()
{
    cout << "\n================================" << endl;
    cout << "   USER AUTHENTICATION SYSTEM" << endl;
    cout << "================================" << endl;
    cout << "  1. Sign Up" << endl;
    cout << "  2. Login" << endl;
    cout << "  3. Forgot Password" << endl;
    cout << "  4. Check Username" << endl;
    cout << "  5. View Statistics" << endl;
    cout << "  6. Exit" << endl;
    cout << "================================" << endl;
    cout << "Enter your choice: ";
}

int main()
{
    // create auth system object
    UserAuth authSystem("users.db");
    int choice;
    string username, password, securityQuestion, securityAnswer;

    cout << "\nWelcome to the Authentication System\n"
         << endl;

    // main loop
    while (true)
    {
        displayMenu();
        cin >> choice;

        // handle user choice
        if (choice == 1)
        {
            // sign up
            cout << "\n--- SIGN UP ---" << endl;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            // check password strength
            string strength = authSystem.checkPasswordStrength(password);
            cout << "Password Strength: " << strength << endl;

            cin.ignore(); // clear input buffer
            cout << "Enter security question: ";
            getline(cin, securityQuestion);
            cout << "Enter security answer: ";
            getline(cin, securityAnswer);

            bool result = authSystem.signUp(username, password, securityQuestion, securityAnswer);
            if (result == true)
            {
                cout << "\nSign up successful! You can now login." << endl;
            }
            else
            {
                cout << "\nSign up failed! Username already exists." << endl;
            }
        }
        else if (choice == 2)
        {
            // login
            cout << "\n--- LOGIN ---" << endl;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            bool result = authSystem.login(username, password);
            if (result == true)
            {
                cout << "\nLogin successful! Welcome, " << username << "!" << endl;
            }
            else
            {
                cout << "\nLogin failed! Invalid username or password." << endl;
            }
        }
        else if (choice == 3)
        {
            // forgot password
            cout << "\n--- FORGOT PASSWORD ---" << endl;
            cout << "Enter username: ";
            cin >> username;

            string question = authSystem.getSecurityQuestion(username);
            if (question.empty())
            {
                cout << "\nUsername not found!" << endl;
            }
            else
            {
                cout << "\nSecurity Question: " << question << endl;
                cin.ignore(); // clear input buffer
                cout << "Enter your answer: ";
                getline(cin, securityAnswer);
                cout << "Enter new password: ";
                cin >> password;

                bool result = authSystem.resetPassword(username, securityAnswer, password);
                if (result == true)
                {
                    cout << "\nPassword reset successful! You can now login." << endl;
                }
                else
                {
                    cout << "\nPassword reset failed! Incorrect answer." << endl;
                }
            }
        }
        else if (choice == 4)
        {
            // check username
            cout << "\n--- CHECK USERNAME ---" << endl;
            cout << "Enter username to check: ";
            cin >> username;

            bool exists = authSystem.usernameExists(username);
            if (exists == true)
            {
                cout << "\nUsername is already taken." << endl;
            }
            else
            {
                cout << "\nUsername is available!" << endl;
            }
        }
        else if (choice == 5)
        {
            // show stats
            cout << "\n--- STATISTICS ---" << endl;
            int total = authSystem.getTotalUsers();
            cout << "Total registered users: " << total << endl;
        }
        else if (choice == 6)
        {
            // exit
            cout << "\nThank you for using the system!" << endl;
            cout << "Goodbye!" << endl;
            break;
        }
        else
        {
            cout << "\nInvalid choice! Please try again." << endl;
        }
    }

    return 0;
}
