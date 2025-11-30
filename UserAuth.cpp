#include "UserAuth.h"
#include <fstream>
#include <iostream>
#include <sstream>

// constructor
UserAuth::UserAuth(const string &filePath)
{
    dbFilePath = filePath;
    usernameFilter = BloomFilter(10000, 3);     // create bloom filter for usernames
    weakPasswordFilter = BloomFilter(5000, 3);  // create bloom filter for weak passwords
    initializeWeakPasswords();                  // load common weak passwords
    loadUsersFromFile();                        // load existing users
}

// initialize weak password filter with common passwords
void UserAuth::initializeWeakPasswords()
{
    // list of common weak passwords
    string weakPasswords[] = {
        "password", "123456", "12345678", "qwerty", "abc123",
        "monkey", "1234567", "letmein", "trustno1", "dragon",
        "baseball", "111111", "iloveyou", "master", "sunshine",
        "ashley", "bailey", "passw0rd", "shadow", "123123",
        "654321", "superman", "qazwsx", "michael", "football",
        "password1", "admin", "welcome", "login", "test",
        "pass", "root", "user", "guest", "demo"
    };

    // add all weak passwords to bloom filter
    int size = sizeof(weakPasswords) / sizeof(weakPasswords[0]);
    for (int i = 0; i < size; i++)
    {
        weakPasswordFilter.insert(weakPasswords[i]);
    }
}

// hash password function
string UserAuth::hashPassword(const string &password) const
{
    unsigned long hash = 5381;
    for (int i = 0; i < password.length(); i++)
    {
        char c = password[i];
        hash = ((hash << 5) + hash) + c;
    }
    // convert hash to string
    stringstream ss;
    ss << hash;
    return ss.str();
}

// load users from file
void UserAuth::loadUsersFromFile()
{
    ifstream file(dbFilePath);
    if (!file.is_open())
    {
        return; // file doesn't exist yet
    }

    string username, hashedPassword, question, hashedAnswer;
    // read each line
    while (file >> username >> hashedPassword)
    {
        userDatabase[username] = hashedPassword; // store in map
        usernameFilter.insert(username);         // add to bloom filter

        // read security question and answer (if exists)
        getline(file, question); // read rest of line
        if (!question.empty() && question.find("|") != string::npos)
        {
            int pos = question.find("|");
            string q = question.substr(1, pos - 1);           // extract question
            string a = question.substr(pos + 1);              // extract answer
            securityQuestions[username] = q;
            securityAnswers[username] = a;
        }
    }
    file.close();
}

// save user to file
void UserAuth::saveUserToFile(const string &username, const string &password, const string &question, const string &answer)
{
    ofstream file(dbFilePath, ios::app); // open in append mode
    if (file.is_open())
    {
        file << username << " " << password << " " << question << "|" << answer << endl;
        file.close();
    }
}

// update all users to file
void UserAuth::updateUsersFile()
{
    ofstream file(dbFilePath); // open in write mode (overwrites)
    if (file.is_open())
    {
        // write all users
        for (map<string, string>::iterator it = userDatabase.begin(); it != userDatabase.end(); it++)
        {
            string username = it->first;
            string password = it->second;
            string question = securityQuestions[username];
            string answer = securityAnswers[username];
            file << username << " " << password << " " << question << "|" << answer << endl;
        }
        file.close();
    }
}

// check if username exists
bool UserAuth::usernameExists(const string &username)
{
    // first check bloom filter
    if (usernameFilter.mightContain(username) == false)
    {
        return false; // definitely doesn't exist
    }

    // check actual database
    if (userDatabase.find(username) != userDatabase.end())
    {
        return true;
    }
    return false;
}

// sign up new user
bool UserAuth::signUp(const string &username, const string &password, const string &securityQuestion, const string &securityAnswer)
{
    // check if username or password is empty
    if (username.empty() || password.empty() || securityQuestion.empty() || securityAnswer.empty())
    {
        return false;
    }

    // check if username already exists
    if (usernameExists(username) == true)
    {
        return false;
    }

    // hash the password
    string hashedPassword = hashPassword(password);

    // hash the security answer
    string hashedAnswer = hashPassword(securityAnswer);

    // add to database
    userDatabase[username] = hashedPassword;
    securityQuestions[username] = securityQuestion;
    securityAnswers[username] = hashedAnswer;

    // add to bloom filter
    usernameFilter.insert(username);

    // save to file
    saveUserToFile(username, hashedPassword, securityQuestion, hashedAnswer);

    return true;
}

// login user
bool UserAuth::login(const string &username, const string &password)
{
    // check bloom filter first
    if (usernameFilter.mightContain(username) == false)
    {
        return false; // username doesn't exist
    }

    // find username in database
    map<string, string>::iterator it = userDatabase.find(username);
    if (it == userDatabase.end())
    {
        return false; // username not found
    }

    // hash the entered password
    string hashedPassword = hashPassword(password);

    // check if password matches
    if (it->second == hashedPassword)
    {
        return true;
    }
    return false;
}

// get security question for username
string UserAuth::getSecurityQuestion(const string &username)
{
    // check bloom filter first
    if (usernameFilter.mightContain(username) == false)
    {
        return ""; // username doesn't exist
    }

    // find security question
    map<string, string>::iterator it = securityQuestions.find(username);
    if (it != securityQuestions.end())
    {
        return it->second;
    }
    return "";
}

// reset password
bool UserAuth::resetPassword(const string &username, const string &answer, const string &newPassword)
{
    // check if username exists
    if (usernameExists(username) == false)
    {
        return false;
    }

    // hash the provided answer
    string hashedAnswer = hashPassword(answer);

    // check if answer matches
    if (securityAnswers[username] != hashedAnswer)
    {
        return false; // wrong answer
    }

    // hash new password
    string hashedPassword = hashPassword(newPassword);

    // update password
    userDatabase[username] = hashedPassword;

    // update file
    updateUsersFile();

    return true;
}

// check password strength
string UserAuth::checkPasswordStrength(const string &password)
{
    // check if password is too short
    if (password.length() < 6)
    {
        return "Weak: Password too short (minimum 6 characters)";
    }

    // check if password is common/weak using bloom filter
    if (weakPasswordFilter.mightContain(password))
    {
        return "Weak: Common password detected";
    }

    // check for numbers
    bool hasNumber = false;
    bool hasUpper = false;
    bool hasLower = false;
    bool hasSpecial = false;

    for (int i = 0; i < password.length(); i++)
    {
        char c = password[i];
        if (c >= '0' && c <= '9')
            hasNumber = true;
        if (c >= 'A' && c <= 'Z')
            hasUpper = true;
        if (c >= 'a' && c <= 'z')
            hasLower = true;
        if (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' || c == '&' || c == '*')
            hasSpecial = true;
    }

    // calculate strength
    int strength = 0;
    if (hasNumber)
        strength++;
    if (hasUpper)
        strength++;
    if (hasLower)
        strength++;
    if (hasSpecial)
        strength++;
    if (password.length() >= 10)
        strength++;

    // return strength message
    if (strength >= 4)
    {
        return "Strong: Good password!";
    }
    else if (strength >= 2)
    {
        return "Medium: Add numbers, uppercase, or special characters";
    }
    else
    {
        return "Weak: Use mix of letters, numbers, and special characters";
    }
}

// get total number of users
int UserAuth::getTotalUsers() const
{
    return userDatabase.size();
}
