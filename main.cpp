// ===================================================================
// PROFESSIONAL C++ PASSWORD MANAGEMENT SYSTEM / DIGITAL SECURITY VAULT
// ===================================================================
// Academic-Level Final Project with OOP, Inheritance, Polymorphism,
// Templates, STL, Exception Handling, File I/O, and Professional UI
// ===================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

// ==================== GLOBAL CONSTANTS ====================
const int SEPARATOR_WIDTH = 80;
const string HEADER_LINE = string(SEPARATOR_WIDTH, '=');
const string DASH_LINE = string(SEPARATOR_WIDTH, '-');

// ==================== UTILITY FUNCTIONS ====================
void displayHeader(string title) {
    cout << "\n" << HEADER_LINE << endl;
    cout << setw((SEPARATOR_WIDTH + title.length()) / 2) << title << endl;
    cout << HEADER_LINE << endl;
}

void displaySubHeader(string title) {
    cout << "\n" << DASH_LINE << endl;
    cout << "  ► " << title << endl;
    cout << DASH_LINE << endl;
}

void clearScreen() {
    cout << "\n\nPress ENTER to continue...";
    cin.ignore();
    cin.get();
}

void displaySection(string text) {
    cout << "\n[System] " << text << endl;
}

void displaySuccess(string text) {
    cout << "[✓ SUCCESS] " << text << endl;
}

void displayError(string text) {
    cout << "[✗ ERROR] " << text << endl;
}

void displayWarning(string text) {
    cout << "[⚠ WARNING] " << text << endl;
}

void displayAdmin(string text) {
    cout << "[ADMIN] " << text << endl;
}

void displaySecurity(string text) {
    cout << "[Security] " << text << endl;
}

void displayBackup(string text) {
    cout << "[Backup] " << text << endl;
}

// ==================== SECURITY NAMESPACE ====================
namespace Security {
    
    // Caesar Cipher Encryption (Shift by 3)
    string encrypt(string password) {
        displaySecurity("Encrypting password...");
        string encrypted = "";
        for (char c : password) {
            encrypted += (char)(c + 3);
        }
        displaySecurity("Encryption complete.");
        return encrypted;
    }
    
    // Caesar Cipher Decryption
    string decrypt(string encryptedPassword) {
        displaySecurity("Decrypting password...");
        string decrypted = "";
        for (char c : encryptedPassword) {
            decrypted += (char)(c - 3);
        }
        displaySecurity("Decryption complete.");
        return decrypted;
    }
    
    // Password Policy Validation
    bool validatePasswordPolicy(string password) {
        displaySection("Validating password policy...");
        
        if (password.length() < 6) {
            displayError("Password must be at least 6 characters long!");
            return false;
        }
        
        bool hasUpper = false, hasLower = false, hasDigit = false;
        for (char c : password) {
            if (isupper(c)) hasUpper = true;
            if (islower(c)) hasLower = true;
            if (isdigit(c)) hasDigit = true;
        }
        
        if (!hasUpper) {
            displayWarning("Password should contain at least one uppercase letter.");
        }
        if (!hasLower) {
            displayWarning("Password should contain at least one lowercase letter.");
        }
        if (!hasDigit) {
            displayWarning("Password should contain at least one digit.");
        }
        
        displaySuccess("Password validated successfully!");
        return true;
    }
    
    // OTP Generation (One-Time Password)
    int generateOTP() {
        int otp = rand() % 9000 + 1000;
        cout << "\n[2FA] OTP Generated: " << otp << endl;
        return otp;
    }
    
    // Email validation
    bool validateEmail(string email) {
        size_t atPos = email.find('@');
        size_t dotPos = email.find('.');
        return (atPos != string::npos && dotPos != string::npos && dotPos > atPos);
    }
}

// ==================== RECORD CLASSES (INHERITANCE) ====================

// Base class using pure virtual functions
class Record {
protected:
    string service;
    string username;
    string password;
    string timestamp;
    
public:
    Record(string s, string u, string p) 
        : service(s), username(u), password(Security::encrypt(p)) {
        displaySection("Creating base record...");
        time_t now = time(0);
        timestamp = ctime(&now);
    }
    
    virtual ~Record() {}
    
    virtual void display() const = 0;
    virtual string save() const = 0;
    virtual string getType() const = 0;
    
    string getService() const { return service; }
    string getUsername() const { return username; }
    string getDecryptedPassword() const { return Security::decrypt(password); }
    string getTimestamp() const { return timestamp; }
};

// Web Account Record
class WebRecord : public Record {
private:
    string url;
    string securityQuestion;
    string recoveryEmail;
    
public:
    WebRecord(string s, string u, string p, string url, string sq, string re)
        : Record(s, u, p), url(url), securityQuestion(sq), recoveryEmail(re) {
        displaySection("Web record created successfully.");
    }
    
    virtual ~WebRecord() {}
    
    void display() const override {
        cout << left << setw(4) << "WEB" 
             << setw(20) << service 
             << setw(20) << username 
             << setw(30) << url 
             << setw(20) << recoveryEmail << endl;
    }
    
    string save() const override {
        stringstream ss;
        ss << "WEB|" << service << "|" << username << "|" << password 
           << "|" << url << "|" << securityQuestion << "|" << recoveryEmail << "|" << timestamp;
        return ss.str();
    }
    
    string getType() const override { return "WEB"; }
};

// Mobile Account Record
class MobileRecord : public Record {
private:
    string appName;
    string phoneNumber;
    string backupCode;
    
public:
    MobileRecord(string s, string u, string p, string app, string phone, string backup)
        : Record(s, u, p), appName(app), phoneNumber(phone), backupCode(backup) {
        displaySection("Mobile record created successfully.");
    }
    
    virtual ~MobileRecord() {}
    
    void display() const override {
        cout << left << setw(4) << "MOB" 
             << setw(20) << service 
             << setw(20) << username 
             << setw(30) << appName 
             << setw(20) << phoneNumber << endl;
    }
    
    string save() const override {
        stringstream ss;
        ss << "MOB|" << service << "|" << username << "|" << password 
           << "|" << appName << "|" << phoneNumber << "|" << backupCode << "|" << timestamp;
        return ss.str();
    }
    
    string getType() const override { return "MOBILE"; }
};

// Desktop/Software Account Record
class DesktopRecord : public Record {
private:
    string softwareName;
    string licenseKey;
    string installationPath;
    
public:
    DesktopRecord(string s, string u, string p, string sw, string key, string path)
        : Record(s, u, p), softwareName(sw), licenseKey(key), installationPath(path) {
        displaySection("Desktop record created successfully.");
    }
    
    virtual ~DesktopRecord() {}
    
    void display() const override {
        cout << left << setw(4) << "DES" 
             << setw(20) << service 
             << setw(20) << username 
             << setw(30) << softwareName 
             << setw(20) << licenseKey << endl;
    }
    
    string save() const override {
        stringstream ss;
        ss << "DES|" << service << "|" << username << "|" << password 
           << "|" << softwareName << "|" << licenseKey << "|" << installationPath << "|" << timestamp;
        return ss.str();
    }
    
    string getType() const override { return "DESKTOP"; }
};

// ==================== RECORD MANAGER CLASS ====================
class RecordManager {
private:
    vector<Record*> records;
    string filename;
    string username;
    
public:
    RecordManager(string u) : username(u) {
        filename = u + "_records.txt";
        displaySection("Initializing Record Manager for user: " + u);
        loadRecords();
    }
    
    ~RecordManager() {
        for (auto rec : records) {
            delete rec;
        }
    }
    
    void displayRecordHeader() const {
        cout << "\n" << DASH_LINE << endl;
        cout << left << setw(4) << "TYPE" 
             << setw(20) << "SERVICE" 
             << setw(20) << "USERNAME" 
             << setw(30) << "DETAILS" 
             << setw(20) << "EXTRA" << endl;
        cout << DASH_LINE << endl;
    }
    
    void createRecord() {
        displaySubHeader("CREATE NEW PASSWORD RECORD");
        
        cout << "\nSelect Record Type:\n";
        cout << "1. Web Account (Website/Email)\n";
        cout << "2. Mobile Account (App/Phone)\n";
        cout << "3. Desktop Account (Software/License)\n";
        cout << "\nEnter Choice (1-3): ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        string service, username, password;
        
        cout << "\nEnter Service Name: ";
        getline(cin, service);
        
        cout << "Enter Username/Email: ";
        getline(cin, username);
        
        cout << "Enter Password: ";
        getline(cin, password);
        
        if (!Security::validatePasswordPolicy(password)) {
            displayError("Password does not meet security requirements!");
            return;
        }
        
        if (choice == 1) {
            string url, sq, re;
            cout << "Enter Website URL: ";
            getline(cin, url);
            cout << "Enter Security Question: ";
            getline(cin, sq);
            cout << "Enter Recovery Email: ";
            getline(cin, re);
            
            if (!Security::validateEmail(re)) {
                displayError("Invalid email format!");
                return;
            }
            
            records.push_back(new WebRecord(service, username, password, url, sq, re));
        } 
        else if (choice == 2) {
            string app, phone, backup;
            cout << "Enter App Name: ";
            getline(cin, app);
            cout << "Enter Associated Phone Number: ";
            getline(cin, phone);
            cout << "Enter Backup Code: ";
            getline(cin, backup);
            
            records.push_back(new MobileRecord(service, username, password, app, phone, backup));
        }
        else if (choice == 3) {
            string sw, key, path;
            cout << "Enter Software Name: ";
            getline(cin, sw);
            cout << "Enter License Key: ";
            getline(cin, key);
            cout << "Enter Installation Path: ";
            getline(cin, path);
            
            records.push_back(new DesktopRecord(service, username, password, sw, key, path));
        }
        else {
            displayError("Invalid choice!");
            return;
        }
        
        saveRecords();
        displaySuccess("Record created and saved successfully!");
    }
    
    void viewAllRecords() const {
        displaySubHeader("VIEW ALL PASSWORD RECORDS");
        
        if (records.empty()) {
            displayWarning("No password records found!");
            return;
        }
        
        displayRecordHeader();
        for (const auto& rec : records) {
            rec->display();
        }
    }
    
    void searchRecord() {
        displaySubHeader("SEARCH PASSWORD RECORD");
        
        string searchTerm;
        cout << "\nEnter service name to search: ";
        cin.ignore();
        getline(cin, searchTerm);
        
        bool found = false;
        for (const auto& rec : records) {
            if (rec->getService() == searchTerm) {
                displayRecordHeader();
                rec->display();
                cout << "\nDecrypted Password: " << rec->getDecryptedPassword() << endl;
                found = true;
            }
        }
        
        if (!found) {
            displayError("No records found for service: " + searchTerm);
        }
    }
    
    void updateRecord() {
        displaySubHeader("UPDATE PASSWORD RECORD");
        
        string searchTerm;
        cout << "\nEnter service name to update: ";
        cin.ignore();
        getline(cin, searchTerm);
        
        for (int i = 0; i < records.size(); i++) {
            if (records[i]->getService() == searchTerm) {
                string newUsername, newPassword;
                
                cout << "Enter new username: ";
                getline(cin, newUsername);
                cout << "Enter new password: ";
                getline(cin, newPassword);
                
                if (!Security::validatePasswordPolicy(newPassword)) {
                    displayError("Password does not meet security requirements!");
                    return;
                }
                
                delete records[i];
                records[i] = new WebRecord(searchTerm, newUsername, newPassword, "", "", "");
                
                saveRecords();
                displaySuccess("Record updated successfully!");
                return;
            }
        }
        
        displayError("Record not found!");
    }
    
    void deleteRecord() {
        displaySubHeader("DELETE PASSWORD RECORD");
        
        string searchTerm;
        cout << "\nEnter service name to delete: ";
        cin.ignore();
        getline(cin, searchTerm);
        
        for (int i = 0; i < records.size(); i++) {
            if (records[i]->getService() == searchTerm) {
                delete records[i];
                records.erase(records.begin() + i);
                saveRecords();
                displaySuccess("Record deleted successfully!");
                return;
            }
        }
        
        displayError("Record not found!");
    }
    
    void backupRecords() {
        displayBackup("Starting backup process...");
        
        string backupFilename = username + "_backup_" + to_string(time(0)) + ".txt";
        ofstream backupFile(backupFilename);
        
        for (const auto& rec : records) {
            backupFile << rec->save() << endl;
        }
        
        backupFile.close();
        displayBackup("Backup file created: " + backupFilename);
        displaySuccess("Backup completed successfully!");
    }
    
    void restoreRecords() {
        displayBackup("Starting restore process...");
        
        string backupFilename;
        cout << "\nEnter backup filename to restore: ";
        cin.ignore();
        getline(cin, backupFilename);
        
        ifstream backupFile(backupFilename);
        if (!backupFile.is_open()) {
            displayError("Backup file not found!");
            return;
        }
        
        // Clear existing records
        for (auto rec : records) {
            delete rec;
        }
        records.clear();
        
        // Load from backup
        loadRecordsFromFile(backupFile);
        backupFile.close();
        
        saveRecords();
        displaySuccess("Records restored successfully from backup!");
    }
    
private:
    void saveRecords() {
        displaySection("Saving records to file...");
        
        ofstream file(filename);
        for (const auto& rec : records) {
            file << rec->save() << endl;
        }
        file.close();
        
        displaySuccess("Records saved to: " + filename);
    }
    
    void loadRecords() {
        displaySection("Loading records from file...");
        
        ifstream file(filename);
        if (!file.is_open()) {
            displayWarning("Record file does not exist. Starting with empty vault.");
            return;
        }
        
        loadRecordsFromFile(file);
        file.close();
        
        cout << "Loaded " << records.size() << " records." << endl;
    }
    
    void loadRecordsFromFile(ifstream& file) {
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            
            stringstream ss(line);
            string type;
            getline(ss, type, '|');
            
            string service, username, password;
            getline(ss, service, '|');
            getline(ss, username, '|');
            getline(ss, password, '|');
            
            if (type == "WEB") {
                string url, sq, re, timestamp;
                getline(ss, url, '|');
                getline(ss, sq, '|');
                getline(ss, re, '|');
                getline(ss, timestamp, '|');
                records.push_back(new WebRecord(service, username, Security::decrypt(password), url, sq, re));
            }
            else if (type == "MOB") {
                string app, phone, backup, timestamp;
                getline(ss, app, '|');
                getline(ss, phone, '|');
                getline(ss, backup, '|');
                getline(ss, timestamp, '|');
                records.push_back(new MobileRecord(service, username, Security::decrypt(password), app, phone, backup));
            }
            else if (type == "DES") {
                string sw, key, path, timestamp;
                getline(ss, sw, '|');
                getline(ss, key, '|');
                getline(ss, path, '|');
                getline(ss, timestamp, '|');
                records.push_back(new DesktopRecord(service, username, Security::decrypt(password), sw, key, path));
            }
        }
    }
};

// ==================== USER MANAGER CLASS ====================
class UserManager {
private:
    map<string, string> users; // username -> encrypted password
    string usersFile = "users.txt";
    string userlistFile = "userlist.txt";
    
public:
    UserManager() {
        displaySection("Initializing User Manager...");
        loadUsers();
    }
    
    void registerUser() {
        displaySubHeader("USER REGISTRATION");
        
        string username, password, confirmPassword;
        
        cout << "\nEnter Username (alphanumeric, no spaces): ";
        cin >> username;
        
        // Check if user already exists
        if (users.find(username) != users.end()) {
            displayError("Username already exists! Please choose another.");
            return;
        }
        
        cout << "Enter Password: ";
        cin >> password;
        
        cout << "Confirm Password: ";
        cin >> confirmPassword;
        
        if (password != confirmPassword) {
            displayError("Passwords do not match!");
            return;
        }
        
        if (!Security::validatePasswordPolicy(password)) {
            displayError("Password does not meet security requirements!");
            return;
        }
        
        // Save user
        users[username] = Security::encrypt(password);
        saveUsers();
        
        // Add to userlist
        ofstream userlistFileStream(userlistFile, ios::app);
        userlistFileStream << username << endl;
        userlistFileStream.close();
        
        displaySuccess("User registration successful! Username: " + username);
    }
    
    string loginUser() {
        displaySubHeader("USER LOGIN");
        
        string username, password;
        
        cout << "\nEnter Username: ";
        cin >> username;
        
        if (users.find(username) == users.end()) {
            displayError("Username not found!");
            return "";
        }
        
        cout << "Enter Password: ";
        cin >> password;
        
        if (Security::encrypt(password) != users[username]) {
            displayError("Incorrect password!");
            return "";
        }
        
        // 2FA - OTP Verification
        int otp = Secu
