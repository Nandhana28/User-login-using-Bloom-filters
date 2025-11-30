# User Authentication System with Bloom Filters

A complete user authentication system demonstrating the practical use of Bloom Filters for efficient username lookups and password strength checking.

## Features

- User Registration (Sign Up)
- User Login
- Forgot Password with Security Questions
- Username Availability Check
- Password Strength Checker
- Bloom Filter Implementation for fast lookups

## What are Bloom Filters?

Bloom filters are space-efficient probabilistic data structures used to test whether an element is a member of a set. In this project, Bloom filters are used for:

1. **Username Lookup**: Quickly check if a username exists before querying the database
2. **Weak Password Detection**: Check if a password is in the list of common weak passwords

### Advantages:
- Very fast lookups
- Memory efficient
- No false negatives (if it says username doesn't exist, it definitely doesn't)

### Trade-off:
- Possible false positives (if it says username exists, we verify with actual database)

## Project Structure

```
├── BloomFilter.h          # Bloom filter class header
├── BloomFilter.cpp        # Bloom filter implementation with 3 hash functions
├── UserAuth.h             # User authentication class header
├── UserAuth.cpp           # User authentication implementation
├── main.cpp               # Console application entry point
├── compile.bat            # Windows compilation script
├── index.html             # Web frontend
├── style.css              # Frontend styling
├── script.js              # Frontend logic with Bloom filter
├── .gitignore             # Git ignore file
└── README.md              # This file
```

## How to Run

### C++ Console Application

**Requirements:**
- C++ compiler (MinGW or Visual Studio)

**Compile:**
```bash
g++ -std=c++11 BloomFilter.cpp UserAuth.cpp main.cpp -o auth_system.exe
```

Or use the batch file:
```bash
compile.bat
```

**Run:**
```bash
auth_system.exe
```

### Web Frontend

Simply open `index.html` in your web browser. No installation required!

The web version uses:
- localStorage as database
- JavaScript implementation of Bloom filters
- Real-time password strength checking

## Usage

### Console Application Menu

1. **Sign Up** - Create a new account with username, password, and security question
2. **Login** - Access your account
3. **Forgot Password** - Reset password using security question
4. **Check Username** - See if username is available
5. **View Statistics** - See total registered users
6. **Exit** - Close the application

### Web Interface

- **Login Tab**: Login with existing credentials
- **Sign Up Tab**: Create new account with real-time password strength feedback
- **Forgot Password Tab**: Reset password using security question
- **Check Username Tab**: Verify username availability

## How Bloom Filters Work in This Project

### Username Lookup
```
User enters username → Check Bloom filter first
├─ If Bloom filter says "NO" → Username definitely doesn't exist (fast!)
└─ If Bloom filter says "MAYBE" → Check actual database to confirm
```

### Password Strength Check
```
User enters password → Check Bloom filter for weak passwords
├─ If found in Bloom filter → Reject as weak password
└─ If not found → Check other criteria (length, characters, etc.)
```

### Hash Functions
The Bloom filter uses 3 different hash functions:
- Hash 1: `hash = hash * 33 + char`
- Hash 2: `hash = hash * 31 + char`
- Hash 3: `hash = hash * 37 + char`

## Password Strength Criteria

The system checks passwords for:
- Minimum 6 characters
- Not in common weak password list (35+ passwords)
- Contains numbers
- Contains uppercase letters
- Contains lowercase letters
- Contains special characters (!@#$%&*)

**Strength Levels:**
- **Weak**: Too short or common password
- **Medium**: 2-3 criteria met
- **Strong**: 4+ criteria met

## Security Features

- Passwords are hashed before storage
- Security answers are hashed
- Bloom filter prevents timing attacks on username enumeration
- Common weak passwords are blocked
- File-based database (users.db)

## Technical Details

### Bloom Filter Configuration
- **Username Filter**: 10,000 bits, 3 hash functions
- **Weak Password Filter**: 5,000 bits, 3 hash functions
- **False Positive Rate**: Very low due to size and hash functions

### Database Format
```
username hashedPassword securityQuestion|hashedAnswer
```

### Common Weak Passwords Blocked
password, 123456, 12345678, qwerty, abc123, admin, welcome, and 28 more...

## Example Usage

### Sign Up
```
Enter username: john
Enter password: MyPass123!
Password Strength: Strong: Good password!
Enter security question: What is your pet's name?
Enter security answer: fluffy
Sign up successful!
```

### Login
```
Enter username: john
Enter password: MyPass123!
Login successful! Welcome, john!
```

### Forgot Password
```
Enter username: john
Security Question: What is your pet's name?
Enter your answer: fluffy
Enter new password: NewPass456!
Password reset successful!
```

## Learning Outcomes

This project demonstrates:
- Bloom filter implementation from scratch
- Hash function design
- Object-oriented programming in C++
- File I/O operations
- Password security best practices
- Web development with vanilla JavaScript
- localStorage usage
- Real-time form validation

## Future Enhancements

Possible improvements:
- Email verification
- Two-factor authentication
- Session management
- Password history
- Account lockout after failed attempts
- More sophisticated password rules

## License

Free to use for educational purposes.

## Author

Created as a demonstration of Bloom filters in practical applications.
