# User Authentication System with Bloom Filters

A complete user authentication system demonstrating the use of Bloom Filters for efficient username lookups.

## Features

- **Bloom Filter Implementation**: Custom Bloom filter for fast username existence checks
- **User Registration**: Sign up with username and password
- **User Login**: Secure login with password hashing
- **Username Availability Check**: Quick lookup using Bloom filters
- **File-based Database**: No external database required

## Project Structure

```
├── BloomFilter.h          # Bloom filter class header
├── BloomFilter.cpp        # Bloom filter implementation
├── UserAuth.h             # User authentication class header
├── UserAuth.cpp           # User authentication implementation
├── main.cpp               # Console application entry point
├── compile.bat            # Windows compilation script
├── index.html             # Web frontend
├── style.css              # Frontend styling
├── script.js              # Frontend logic with Bloom filter
└── README.md              # This file
```

## How to Run

### C++ Console Application

1. **Compile the program:**
   ```
   compile.bat
   ```
   Or manually:
   ```
   g++ -std=c++11 BloomFilter.cpp UserAuth.cpp main.cpp -o auth_system.exe
   ```

2. **Run the executable:**
   ```
   auth_system.exe
   ```

### Web Frontend

Simply open `index.html` in your web browser. The frontend uses localStorage as a database and implements Bloom filters in JavaScript.

## How Bloom Filters Work Here

1. **Fast Username Lookup**: Before checking the actual database, the Bloom filter quickly determines if a username might exist
2. **Space Efficient**: Uses a bit array instead of storing all usernames
3. **No False Negatives**: If Bloom filter says username doesn't exist, it definitely doesn't
4. **Possible False Positives**: If it says username exists, we verify with the actual database

## Usage

### Console Application Menu:
1. Sign Up - Create a new account
2. Login - Access existing account
3. Check Username Availability - See if username is taken
4. View Statistics - See total registered users
5. Exit - Close the application

### Web Interface:
- **Login Tab**: Login with existing credentials
- **Sign Up Tab**: Create a new account
- **Check Username Tab**: Verify username availability

## Technical Details

- **Hash Functions**: Uses 3 different hash functions for Bloom filter
- **Password Security**: Passwords are hashed before storage
- **Database**: Text file (users.db) for C++ version, localStorage for web version
- **Filter Size**: 10,000 bits for C++ version, 1,000 bits for web version

## Benefits of Using Bloom Filters

- Reduces unnecessary file I/O operations
- Faster username existence checks
- Memory efficient for large user bases
- Demonstrates practical data structure usage
