#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include "bcrypt/BCrypt.hpp"
#include <string>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>
#include <sstream>

// Define the request and response structures for the protocol
struct CreateAccountWeb {
    long requestId;
    std::string email;
    std::string plaintextPassword;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(requestId, email, plaintextPassword);
    }
};

struct CreateAccountWebSuccess {
    long requestId;
    long userId;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(requestId, userId);
    }
};

struct CreateAccountWebFailure {
    long requestId;
    enum Reason { ACCOUNT_ALREADY_EXISTS, INVALID_PASSWORD, INTERNAL_SERVER_ERROR } reason;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(requestId, reason);
    }
};

struct AuthenticateWeb {
    long requestId;
    std::string email;
    std::string plaintextPassword;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(requestId, email, plaintextPassword);
    }
};

struct AuthenticateWebSuccess {
    long requestId;
    long userId;
    std::string creationDate;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(requestId, userId, creationDate);
    }
};

struct AuthenticateWebFailure {
    long requestId;
    enum Reason { INVALID_CREDENTIALS, INTERNAL_SERVER_ERROR } reason;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(requestId, reason);
    }
};

// Function to register user
void registerUser(sql::Connection* con, const std::string& username, const std::string& password) {
    try {
        std::string passwordHash = BCrypt::generateHash(password);
        sql::PreparedStatement* prepstmt = con->prepareStatement(
            "INSERT INTO users (username, hashed_password, last_login) VALUES (?, ?, NULL)");

        prepstmt->setString(1, username);
        prepstmt->setString(2, passwordHash);

        prepstmt->executeUpdate();

        std::cout << "New user registered successfully!" << std::endl;
        delete prepstmt;
    }
    catch (sql::SQLException& e) {
        if (e.getErrorCode() == 1062) {
            std::cout << "Error: Username already exists." << std::endl;
        }
        else {
            std::cout << "SQL ERROR during registration: " << e.what() << std::endl;
        }
    }
}

// Function to verify user login
bool verifyUser(sql::Connection* con, const std::string& username, const std::string& password) {
    try {
        sql::PreparedStatement* prepstmt = con->prepareStatement(
            "SELECT hashed_password FROM users WHERE username = ?");

        prepstmt->setString(1, username);

        sql::ResultSet* res = prepstmt->executeQuery();

        if (res->next()) {
            std::string storedHash = res->getString("hashed_password");
            if (BCrypt::validatePassword(password, storedHash)) {
                std::cout << "Authentication successful!" << std::endl;

                sql::PreparedStatement* updateStmt = con->prepareStatement(
                    "UPDATE users SET last_login = CURRENT_TIMESTAMP WHERE username = ?");
                updateStmt->setString(1, username);
                updateStmt->executeUpdate();

                delete updateStmt;
                delete res;
                delete prepstmt;

                return true;
            }
            else {
                std::cout << "Authentication Failed: Incorrect Password" << std::endl;
                delete res;
                delete prepstmt;
                return false;
            }
        }
        else {
            std::cout << "Authentication Failed: User Not Found" << std::endl;
            delete res;
            delete prepstmt;
            return false;
        }
    }
    catch (sql::SQLException& e) {
        std::cout << "SQL ERROR during authentication: " << e.what() << std::endl;
        return false;
    }
}

// Serialization utility function
template <class T>
std::string serializeToJson(const T& obj) {
    std::ostringstream os;
    {
        cereal::JSONOutputArchive archive(os);
        archive(obj);  // Serialize the object
    }
    return os.str();
}

int main() {
    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection* con = nullptr;

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3305", "root", "root");

        if (con->isValid()) {
            std::cout << "Connected to MySQL!" << std::endl;
        }

        con->setSchema("authenticationdb");

        while (true) {
            std::cout << "Enter command (1: Register, 2: Authenticate, 3: Exit): ";
            int choice;
            std::cin >> choice;

            if (choice == 1) {
                std::string username, password;
                std::cout << "Enter username: ";
                std::cin >> username;
                std::cout << "Enter password: ";
                std::cin >> password;

                registerUser(con, username, password);

                // Example of serializing the CreateAccountWeb response
                CreateAccountWeb response;
                response.requestId = 123;  // Example requestId
                response.email = username;
                response.plaintextPassword = password;

                std::string serializedResponse = serializeToJson(response);
                std::cout << "Serialized response: " << serializedResponse << std::endl;
            }
            else if (choice == 2) {
                std::string username, password;
                std::cout << "Enter username: ";
                std::cin >> username;
                std::cout << "Enter password: ";
                std::cin >> password;

                if (verifyUser(con, username, password)) {
                    std::cout << "User authenticated and last_login updated." << std::endl;
                }
                else {
                    std::cout << "Authentication failed." << std::endl;
                }
            }
            else if (choice == 3) {
                break;
            }
            else {
                std::cout << "Invalid command. Please try again." << std::endl;
            }
        }

    }
    catch (sql::SQLException& e) {
        std::cout << "SQL ERROR: " << e.what() << std::endl;
    }

    delete con;
    return 0;
}
