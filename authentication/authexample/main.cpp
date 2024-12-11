#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include "bcrypt/BCrypt.hpp"
#include <string>
#include <winsock2.h>
#include <cppconn/resultset.h>

void registerUser(sql::Connection* con, const std::string& username, const std::string& password) {
    try {
        // Generate a hashed password using BCrypt
        std::string passwordHash = BCrypt::generateHash(password);

        // Prepare the SQL statement
        sql::PreparedStatement* prepstmt = con->prepareStatement(
            "INSERT INTO users (username, hashed_password, last_login) VALUES (?, ?, NULL)");

        // Set the prepared statement parameters
        prepstmt->setString(1, username);             // Insert the provided username
        prepstmt->setString(2, passwordHash);         // Insert the hashed password

        // Execute the statement
        prepstmt->executeUpdate();

        std::cout << "New user registered successfully!" << std::endl;

        // Clean up
        delete prepstmt;
    }
    catch (sql::SQLException& e) {
        std::cout << "SQL ERROR during registration: " << e.what() << std::endl;
    }
}

bool verifyUser(sql::Connection* con, const std::string& username, const std::string& password) {
    try {
        // Prepare the SQL statement to fetch the hashed password for the given username
        sql::PreparedStatement* prepstmt = con->prepareStatement(
            "SELECT hashed_password FROM users WHERE username = ?");

        // Set the username parameter
        prepstmt->setString(1, username);

        // Execute the query
        sql::ResultSet* res = prepstmt->executeQuery();

        // Check if a result is available
        if (res->next()) {
            // Retrieve the hashed password from the result set
            std::string storedHash = res->getString("hashed_password");

            // Validate the provided password against the stored hash
            if (BCrypt::validatePassword(password, storedHash)) {
                std::cout << "Authentication successful!" << std::endl;

                // Update the last_login timestamp
                sql::PreparedStatement* updateStmt = con->prepareStatement(
                    "UPDATE users SET last_login = CURRENT_TIMESTAMP WHERE username = ?");
                updateStmt->setString(1, username);
                updateStmt->executeUpdate();

                // Clean up
                delete updateStmt;
                delete res;
                delete prepstmt;

                return true;
            }
            else {
                std::cout << "Authentication Failed: Incorrect Password" << std::endl;

                // Clean up
                delete res;
                delete prepstmt;

                return false;
            }
        }
        else {
            std::cout << "Authentication Failed: User Not Found" << std::endl;

            // Clean up
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

int main() {
    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection* con = nullptr;

    try {
        // Connect to the database
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3305", "root", "root");

        if (con->isValid()) {
            std::cout << "Connected to MySQL!" << std::endl;
        }

        // Select the schema
        con->setSchema("authenticationdb");

        // Test: Register a new user
        // registerUser(con, "Roger", "12345");

        // Test: Verify the registered user
        if (verifyUser(con, "Roger", "12345")) {
            std::cout << "User authenticated and last_login updated." << std::endl;
        }
        else {
            std::cout << "Authentication failed." << std::endl;
        }

    }
    catch (sql::SQLException& e) {
        std::cout << "SQL ERROR: " << e.what() << std::endl;
    }

    // Clean up
    delete con;
    return 0;
}


























//void registerUser(sql::Connection* con, const std::string& username, const std::string& password)
//{
//    try
//    {
//        // Generate a hashed password using BCrypt
//        std::string passwordHash = BCrypt::generateHash(password);
//
//        // Prepare the SQL statement
//        sql::PreparedStatement* prepstmt = con->prepareStatement(
//            "INSERT INTO users (username, hashed_password, last_login) VALUES (?, ?, ?)");
//
//        // Set the prepared statement parameters
//        prepstmt->setString(1, username);             // Insert the provided username
//        prepstmt->setString(2, passwordHash);         // Insert the hashed password
//        prepstmt->setString(3, "2024-12-11 03:33:00"); // Insert the last login timestamp
//
//        // Execute the statement
//        prepstmt->executeUpdate();
//
//        std::cout << "New record inserted successfully." << std::endl;
//
//        // Clean up
//        delete prepstmt;
//    }
//    catch (sql::SQLException& e)
//    {
//        std::cout << "SQL ERROR: " << e.what() << std::endl;
//    }
//}
//
//
//
//bool verifyUser(sql::Connection* con, const std::string& username, const std::string& password)
//{
//    try
//    {
//        // Prepare the SQL statement to fetch the hashed password for the given username
//        sql::PreparedStatement* prepstmt = con->prepareStatement(
//            "SELECT hashed_password FROM users WHERE username = ?");
//
//        // Set the username parameter
//        prepstmt->setString(1, username);
//
//        // Execute the query
//        sql::ResultSet* res = prepstmt->executeQuery();
//
//        // Check if a result is available
//        if (res->next())
//        {
//            // Retrieve the hashed password from the result set
//            std::string storedHash = res->getString("hashed_password");
//
//            // Validate the provided password against the stored hash
//            if (BCrypt::validatePassword(password, storedHash))
//            {
//                std::cout << "Authentication successful!" << std::endl;
//
//                // Clean up
//                delete res;
//                delete prepstmt;
//
//                return true;
//            }
//            else
//            {
//                std::cout << "Authentication Failed: Incorrect Password" << std::endl;
//
//                // Clean up
//                delete res;
//                delete prepstmt;
//
//                return false;
//            }
//        }
//        else
//        {
//            std::cout << "Authentication Failed: User Not Found" << std::endl;
//
//            // Clean up
//            delete res;
//            delete prepstmt;
//
//            return false;
//        }
//    }
//    catch (sql::SQLException& e)
//    {
//        std::cout << "SQL ERROR: " << e.what() << std::endl;
//        return false;
//    }
//}
//
//
//int main()
//{
//    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
//    sql::Connection* con = nullptr;
//
//    try
//    {
//        // Connect to the database
//        driver = sql::mysql::get_mysql_driver_instance();
//        con = driver->connect("tcp://127.0.0.1:3305", "root", "root");
//
//        if (con->isValid())
//        {
//            std::cout << "Connect to MySQL!!!" << std::endl;
//        }
//
//        // Select the schema
//        con->setSchema("authenticationdb");
//
//        // Test: Register a new user
//        //registerUser(con, "Roger", "12345");
//
//        // Test: Verify the registered user
//        verifyUser(con, "Roger", "12345");
//
//    }
//    catch (sql::SQLException& e)
//    {
//        std::cout << "SQL ERROR: " << e.what() << std::endl;
//    }
//
//    // Clean up
//    delete con;
//    return 0;
//}


