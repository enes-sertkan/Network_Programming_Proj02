#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/exception.h>
#include "bcrypt/BCrypt.hpp"
#include <string>
#include <winsock2.h>

//#pragma comment(lib, "ws2_32.lib")  // Link Winsock library
//
//// Function to connect to MySQL database
//sql::Connection* connectToDatabase() {
//    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
//    sql::Connection* con = driver->connect("tcp://127.0.0.1:3305", "root", "root");  // Adjust your DB credentials
//    con->setSchema("authenticationdb");
//    return con;
//}
//
//// Function to register a user in the database
//void registerUser(sql::Connection* con, const std::string& username, const std::string& password) {
//    try {
//        // Hash the password using bcrypt
//        std::string passwordHash = BCrypt::generateHash(password);
//
//        // Prepare SQL statement to insert user into the database
//        sql::PreparedStatement* prepstmt = con->prepareStatement(
//            "INSERT INTO users (username, password_hash) VALUES (?, ?)"
//        );
//        prepstmt->setString(1, username);
//        prepstmt->setString(2, passwordHash);
//        prepstmt->executeUpdate();
//
//        std::cout << "User registered successfully" << std::endl;
//    }
//    catch (sql::SQLException& e) {
//        std::cerr << "MySQL Error: " << e.what() << std::endl;
//    }
//}
//
//// Function to verify the user's credentials during login
//bool verifyUser(sql::Connection* con, const std::string& username, const std::string& password) {
//    try {
//        // Prepare SQL statement to retrieve the user's password hash from the database
//        sql::PreparedStatement* prepstmt = con->prepareStatement(
//            "SELECT password_hash FROM users WHERE username = ?"
//        );
//        prepstmt->setString(1, username);
//
//        sql::ResultSet* res = prepstmt->executeQuery();
//
//        if (res->next()) {
//            std::string storedHash = res->getString("password_hash");
//
//            // Validate password hash with bcrypt
//            if (BCrypt::validatePassword(password, storedHash)) {
//                std::cout << "Authentication successful!" << std::endl;
//                return true;
//            }
//            else {
//                std::cout << "Authentication failed." << std::endl;
//                return false;
//            }
//        }
//        else {
//            std::cout << "User not found." << std::endl;
//            return false;
//        }
//    }
//    catch (sql::SQLException& e) {
//        std::cerr << "MySQL Error: " << e.what() << std::endl;
//        return false;
//    }
//}
//
//// Winsock setup to initialize the server
//void initWinsock() {
//    WSADATA wsaData;
//    WSAStartup(MAKEWORD(2, 2), &wsaData);
//}
//
//// Function to create and bind the server socket
//SOCKET createServerSocket(int port) {
//    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
//    if (serverSocket == INVALID_SOCKET) {
//        std::cerr << "Error creating socket!" << std::endl;
//        exit(1);
//    }
//
//    sockaddr_in serverAddress;
//    serverAddress.sin_family = AF_INET;
//    serverAddress.sin_port = htons(port);
//    serverAddress.sin_addr.s_addr = INADDR_ANY;
//
//    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
//        std::cerr << "Error binding socket!" << std::endl;
//        exit(1);
//    }
//
//    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
//        std::cerr << "Error listening on socket!" << std::endl;
//        exit(1);
//    }
//
//    return serverSocket;
//}
//
//// Function to accept a new client connection
//SOCKET acceptConnection(SOCKET serverSocket) {
//    sockaddr_in clientAddress;
//    int clientSize = sizeof(clientAddress);
//    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientSize);
//    if (clientSocket == INVALID_SOCKET) {
//        std::cerr << "Error accepting connection!" << std::endl;
//        exit(1);
//    }
//    return clientSocket;
//}
//
//// Main function where the server logic runs
//int main() {
//    initWinsock();
//
//    // Set up the server to listen on port 12345
//    SOCKET serverSocket = createServerSocket(12345);
//    SOCKET clientSocket;
//
//    sql::Connection* con = connectToDatabase();
//
//    while (true) {
//        // Accept incoming client connections
//        clientSocket = acceptConnection(serverSocket);
//
//        // Dummy logic to simulate client communication for testing
//        // In practice, you'd read from the socket, process the request, and then respond accordingly
//
//        // Simulate user registration (In real code, handle client request and input)
//        std::string username = "testuser";
//        std::string password = "hashedpassword";
//        registerUser(con, username, password);
//
//        // Simulate user authentication (In real code, handle client request and input)
//        bool isAuthenticated = verifyUser(con, username, password);
//        if (isAuthenticated) {
//            std::cout << "User authenticated!" << std::endl;
//        }
//        else {
//            std::cout << "User authentication failed!" << std::endl;
//        }
//
//        // Close the client connection (for simplicity, not handling socket communication in this example)
//        closesocket(clientSocket);
//    }
//
//    // Clean up and close the server socket
//    closesocket(serverSocket);
//    WSACleanup();
//
//    return 0;
//}


int main() {

    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection* con = nullptr;
    sql::Statement* stmt = nullptr;
    sql::ResultSet* res = nullptr;
    sql::PreparedStatement* prepstmt = nullptr;

    try
    {
        driver = sql::mysql::get_mysql_driver_instance();

        con = driver->connect("tcp://127.0.0.1:3305", "root", "root");

        if (con->isValid())
        {
            std::cout << "Connect to MySQL!!!" << std::endl;
        }

        con->setSchema("authenticationdb");

        stmt = con->createStatement();

        // Use the correct table 'users'
        res = stmt->executeQuery("SELECT id, username FROM users");

        while (res->next())
        {
            std::cout << "ID: " << res->getInt("id") << ", Username: " << res->getString("username") << std::endl;
        }

        prepstmt = con->prepareStatement("INSERT INTO users (username, salt, hashed_password, last_login) VALUES (?, ?, ?, ?)");
        prepstmt->setString(1, "newuser");                  // username
        prepstmt->setString(2, "new salt");              // salt
        prepstmt->setString(3, "new password");          // hashed_password
        prepstmt->setString(4, "2024-12-11 03:33:00");     // last_login (use correct timestamp format)
        prepstmt->executeUpdate();

        std::cout << "New record inserted successfully " << std::endl;

        res = stmt->executeQuery("SELECT id, username FROM users");

        while (res->next())
        {
            std::cout << "ID: " << res->getInt("id") << ", Username: " << res->getString("username") << std::endl;
        }

    }
    catch (sql::SQLException e)
    {
        std::cout << "SQL ERROR: " << e.what() << std::endl;
    }

    delete con, stmt, res;

    return 0;
}
