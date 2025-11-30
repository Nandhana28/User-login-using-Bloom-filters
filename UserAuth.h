#ifndef USERAUTH_H
#define USERAUTH_H

#include <string>
#include <map>
#include "BloomFilter.h"

using namespace std;

// User Authentication class
class UserAuth
{
private:
    BloomFilter usernameFilter;            // bloom filter for usernames
    BloomFilter weakPasswordFilter;        // bloom filter for weak passwords
    map<string, string> userDatabase;      // map to store username and password
    map<string, string> securityQuestions; // map to store security questions
    map<string, string> securityAnswers;   // map to store security answers (hashed)
    string dbFilePath;                     // path to database file

    // initialize weak password filter
    void initializeWeakPasswords();

    // load users from file
    void loadUsersFromFile();

    // save user to file
    void saveUserToFile(const string &username, const string &password, const string &question, const string &answer);

    // update users file
    void updateUsersFile();

    // hash password
    string hashPassword(const string &password) const;

public:
    // constructor
    UserAuth(const string &filePath = "users.db");

    // sign up new user
    bool signUp(const string &username, const string &password, const string &securityQuestion, const string &securityAnswer);

    // login user
    bool login(const string &username, const string &password);

    // check if username exists
    bool usernameExists(const string &username);

    // get security question for username
    string getSecurityQuestion(const string &username);

    // verify security answer and reset password
    bool resetPassword(const string &username, const string &answer, const string &newPassword);

    // get total number of users
    int getTotalUsers() const;

    // check password strength
    string checkPasswordStrength(const string &password);
};

#endif
