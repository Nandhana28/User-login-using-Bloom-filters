// Bloom Filter class
class BloomFilter {
    constructor(size) {
        this.size = size;
        this.bitArray = new Array(size);
        // initialize all bits to false
        for (let i = 0; i < size; i++) {
            this.bitArray[i] = false;
        }
    }

    // hash function 1
    hash1(str) {
        let hash = 5381;
        for (let i = 0; i < str.length; i++) {
            hash = ((hash << 5) + hash) + str.charCodeAt(i);
        }
        if (hash < 0) {
            hash = hash * -1;
        }
        return hash % this.size;
    }

    // hash function 2
    hash2(str) {
        let hash = 0;
        for (let i = 0; i < str.length; i++) {
            hash = hash * 31 + str.charCodeAt(i);
        }
        if (hash < 0) {
            hash = hash * -1;
        }
        return hash % this.size;
    }

    // hash function 3
    hash3(str) {
        let hash = 0;
        for (let i = 0; i < str.length; i++) {
            hash = hash * 37 + str.charCodeAt(i);
        }
        if (hash < 0) {
            hash = hash * -1;
        }
        return hash % this.size;
    }

    // add item to bloom filter
    add(item) {
        this.bitArray[this.hash1(item)] = true;
        this.bitArray[this.hash2(item)] = true;
        this.bitArray[this.hash3(item)] = true;
    }

    // check if item might be in filter
    mightContain(item) {
        let pos1 = this.hash1(item);
        let pos2 = this.hash2(item);
        let pos3 = this.hash3(item);

        if (this.bitArray[pos1] == true && 
            this.bitArray[pos2] == true && 
            this.bitArray[pos3] == true) {
            return true;
        }
        return false;
    }
}

// Authentication System class
class AuthSystem {
    constructor() {
        this.users = this.loadUsers();
        this.securityQuestions = this.loadSecurityQuestions();
        this.securityAnswers = this.loadSecurityAnswers();
        this.bloomFilter = new BloomFilter(1000);
        this.weakPasswordFilter = new BloomFilter(500);
        this.initializeBloomFilter();
        this.initializeWeakPasswords();
    }

    // initialize weak password filter
    initializeWeakPasswords() {
        // common weak passwords
        var weakPasswords = [
            'password', '123456', '12345678', 'qwerty', 'abc123',
            'monkey', '1234567', 'letmein', 'trustno1', 'dragon',
            'baseball', '111111', 'iloveyou', 'master', 'sunshine',
            'ashley', 'bailey', 'passw0rd', 'shadow', '123123',
            '654321', 'superman', 'qazwsx', 'michael', 'football',
            'password1', 'admin', 'welcome', 'login', 'test',
            'pass', 'root', 'user', 'guest', 'demo'
        ];

        // add all to bloom filter
        for (var i = 0; i < weakPasswords.length; i++) {
            this.weakPasswordFilter.add(weakPasswords[i]);
        }
    }

    // load users from localStorage
    loadUsers() {
        let stored = localStorage.getItem('users');
        if (stored) {
            return JSON.parse(stored);
        }
        return {};
    }

    // load security questions
    loadSecurityQuestions() {
        let stored = localStorage.getItem('securityQuestions');
        if (stored) {
            return JSON.parse(stored);
        }
        return {};
    }

    // load security answers
    loadSecurityAnswers() {
        let stored = localStorage.getItem('securityAnswers');
        if (stored) {
            return JSON.parse(stored);
        }
        return {};
    }

    // save users to localStorage
    saveUsers() {
        localStorage.setItem('users', JSON.stringify(this.users));
    }

    // save security questions
    saveSecurityQuestions() {
        localStorage.setItem('securityQuestions', JSON.stringify(this.securityQuestions));
    }

    // save security answers
    saveSecurityAnswers() {
        localStorage.setItem('securityAnswers', JSON.stringify(this.securityAnswers));
    }

    // initialize bloom filter with existing usernames
    initializeBloomFilter() {
        let usernames = Object.keys(this.users);
        for (let i = 0; i < usernames.length; i++) {
            this.bloomFilter.add(usernames[i]);
        }
    }

    // hash password
    hashPassword(password) {
        let hash = 0;
        for (let i = 0; i < password.length; i++) {
            let char = password.charCodeAt(i);
            hash = ((hash << 5) - hash) + char;
            hash = hash & hash;
        }
        return hash.toString();
    }

    // sign up new user
    signUp(username, password, securityQuestion, securityAnswer) {
        if (!username || !password || !securityQuestion || !securityAnswer) {
            return { success: false, message: 'All fields are required' };
        }

        if (this.usernameExists(username)) {
            return { success: false, message: 'Username already exists' };
        }

        let hashedPassword = this.hashPassword(password);
        let hashedAnswer = this.hashPassword(securityAnswer);
        
        this.users[username] = hashedPassword;
        this.securityQuestions[username] = securityQuestion;
        this.securityAnswers[username] = hashedAnswer;
        
        this.bloomFilter.add(username);
        this.saveUsers();
        this.saveSecurityQuestions();
        this.saveSecurityAnswers();

        return { success: true, message: 'Sign up successful!' };
    }

    // login user
    login(username, password) {
        // check bloom filter first
        if (!this.bloomFilter.mightContain(username)) {
            return { success: false, message: 'Invalid username or password' };
        }

        let hashedPassword = this.hashPassword(password);
        if (this.users[username] == hashedPassword) {
            return { success: true, message: 'Welcome back, ' + username + '!' };
        }

        return { success: false, message: 'Invalid username or password' };
    }

    // check if username exists
    usernameExists(username) {
        if (!this.bloomFilter.mightContain(username)) {
            return false;
        }
        if (username in this.users) {
            return true;
        }
        return false;
    }

    // get security question
    getSecurityQuestion(username) {
        if (!this.bloomFilter.mightContain(username)) {
            return null;
        }
        if (username in this.securityQuestions) {
            return this.securityQuestions[username];
        }
        return null;
    }

    // reset password
    resetPassword(username, answer, newPassword) {
        if (!this.usernameExists(username)) {
            return { success: false, message: 'Username not found' };
        }

        let hashedAnswer = this.hashPassword(answer);
        if (this.securityAnswers[username] != hashedAnswer) {
            return { success: false, message: 'Incorrect answer' };
        }

        let hashedPassword = this.hashPassword(newPassword);
        this.users[username] = hashedPassword;
        this.saveUsers();

        return { success: true, message: 'Password reset successful!' };
    }

    // check password strength
    checkPasswordStrength(password) {
        // check length
        if (password.length < 6) {
            return { strength: 'weak', message: 'Too short (minimum 6 characters)' };
        }

        // check if common password using bloom filter
        if (this.weakPasswordFilter.mightContain(password.toLowerCase())) {
            return { strength: 'weak', message: 'Common password detected' };
        }

        // check for different character types
        var hasNumber = /[0-9]/.test(password);
        var hasUpper = /[A-Z]/.test(password);
        var hasLower = /[a-z]/.test(password);
        var hasSpecial = /[!@#$%&*]/.test(password);

        // calculate strength
        var score = 0;
        if (hasNumber) score++;
        if (hasUpper) score++;
        if (hasLower) score++;
        if (hasSpecial) score++;
        if (password.length >= 10) score++;

        // return strength
        if (score >= 4) {
            return { strength: 'strong', message: 'Strong password!' };
        } else if (score >= 2) {
            return { strength: 'medium', message: 'Add numbers, uppercase, or special characters' };
        } else {
            return { strength: 'weak', message: 'Use mix of letters, numbers, and special characters' };
        }
    }

    // get total users
    getTotalUsers() {
        return Object.keys(this.users).length;
    }
}

// create auth system
var authSystem = new AuthSystem();

// function to switch tabs
function showTab(tabName) {
    // hide all tabs
    let tabs = document.querySelectorAll('.tab-content');
    for (let i = 0; i < tabs.length; i++) {
        tabs[i].classList.remove('active');
    }

    // remove active from all buttons
    let buttons = document.querySelectorAll('.tab-button');
    for (let i = 0; i < buttons.length; i++) {
        buttons[i].classList.remove('active');
    }

    // show selected tab
    document.getElementById(tabName + '-tab').classList.add('active');
    event.target.classList.add('active');

    clearMessages();
}

// clear all messages
function clearMessages() {
    let messages = document.querySelectorAll('.message');
    for (let i = 0; i < messages.length; i++) {
        messages[i].style.display = 'none';
        messages[i].className = 'message';
    }
}

// show message
function showMessage(elementId, message, isSuccess) {
    let element = document.getElementById(elementId);
    element.textContent = message;
    if (isSuccess) {
        element.className = 'message success';
    } else {
        element.className = 'message error';
    }
    element.style.display = 'block';
}

// update statistics
function updateStats() {
    let total = authSystem.getTotalUsers();
    document.getElementById('total-users').textContent = total;
}

// login form submit
document.getElementById('login-form').addEventListener('submit', function(e) {
    e.preventDefault();
    let username = document.getElementById('login-username').value;
    let password = document.getElementById('login-password').value;

    let result = authSystem.login(username, password);
    showMessage('login-message', result.message, result.success);

    if (result.success) {
        e.target.reset();
    }
    updateStats();
});

// check password strength on input
document.getElementById('signup-password').addEventListener('input', function() {
    var password = this.value;
    if (password.length > 0) {
        var result = authSystem.checkPasswordStrength(password);
        var indicator = document.getElementById('password-strength');
        indicator.textContent = result.message;
        indicator.className = 'password-strength ' + result.strength;
        indicator.style.display = 'block';
    } else {
        document.getElementById('password-strength').style.display = 'none';
    }
});

// signup form submit
document.getElementById('signup-form').addEventListener('submit', function(e) {
    e.preventDefault();
    let username = document.getElementById('signup-username').value;
    let password = document.getElementById('signup-password').value;
    let confirm = document.getElementById('signup-confirm').value;
    let question = document.getElementById('signup-question').value;
    let answer = document.getElementById('signup-answer').value;

    if (password != confirm) {
        showMessage('signup-message', 'Passwords do not match', false);
        return;
    }

    // check password strength
    var strengthCheck = authSystem.checkPasswordStrength(password);
    if (strengthCheck.strength == 'weak') {
        showMessage('signup-message', 'Password too weak: ' + strengthCheck.message, false);
        return;
    }

    let result = authSystem.signUp(username, password, question, answer);
    showMessage('signup-message', result.message, result.success);

    if (result.success) {
        e.target.reset();
        document.getElementById('password-strength').style.display = 'none';
    }
    updateStats();
});

// get security question
function getSecurityQuestion() {
    let username = document.getElementById('forgot-username').value;
    if (!username) {
        showMessage('forgot-message', 'Please enter username', false);
        return;
    }

    let question = authSystem.getSecurityQuestion(username);
    if (question == null) {
        showMessage('forgot-message', 'Username not found', false);
        document.getElementById('security-question-section').style.display = 'none';
    } else {
        document.getElementById('security-question-label').textContent = question;
        document.getElementById('security-question-section').style.display = 'block';
        clearMessages();
    }
}

// reset password
function resetPassword() {
    let username = document.getElementById('forgot-username').value;
    let answer = document.getElementById('forgot-answer').value;
    let newPassword = document.getElementById('forgot-newpassword').value;

    if (!answer || !newPassword) {
        showMessage('forgot-message', 'Please fill all fields', false);
        return;
    }

    let result = authSystem.resetPassword(username, answer, newPassword);
    showMessage('forgot-message', result.message, result.success);

    if (result.success) {
        document.getElementById('forgot-username').value = '';
        document.getElementById('forgot-answer').value = '';
        document.getElementById('forgot-newpassword').value = '';
        document.getElementById('security-question-section').style.display = 'none';
    }
}

// check username form submit
document.getElementById('check-form').addEventListener('submit', function(e) {
    e.preventDefault();
    let username = document.getElementById('check-username').value;

    let exists = authSystem.usernameExists(username);
    let message = '';
    if (exists) {
        message = 'Username is already taken';
    } else {
        message = 'Username is available!';
    }

    showMessage('check-message', message, !exists);
});

// update stats on page load
updateStats();
