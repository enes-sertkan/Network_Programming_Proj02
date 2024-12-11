#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include "bcrypt/BCrypt.hpp"

// INITIAL EXAMPLE
//int main() 
//{
//	sql::mysql::MySQL_Driver* driver = nullptr;
//	sql::Connection* con = nullptr;
//	sql::Statement* stmt = nullptr;
//	sql::ResultSet* res = nullptr;
//	sql::PreparedStatement* prepstmt = nullptr;
//
//	try 
//	{
//		driver = sql::mysql::get_mysql_driver_instance();
//
//		con = driver->connect("tcp://127.0.0.1:3305", "user", "user");
//
//		if (con->isValid())
//		{
//			std::cout << "Connect to MySQL!!" << std::endl;
//		}
//
//		con->setSchema("authexampledb_matthewdouglas");
//
//		stmt = con->createStatement();
//
//		res = stmt->executeQuery("SELECT id, value FROM test_table");
//
//		while (res->next())
//		{
//			std::cout << "ID: " << res->getInt("id") << ", Value: " << res->getString("value") << std::endl;
//		}
//
//		prepstmt = con->prepareStatement("INSERT INTO test_table (value) VALUES(?)");
//		prepstmt->setString(1, "NEW VALUE");
//		prepstmt->executeUpdate();
//
//		std::cout << "New record inserted succesfully" << std::endl;
//
//		res = stmt->executeQuery("SELECT id, value FROM test_table");
//
//		while (res->next())
//		{
//			std::cout << "ID: " << res->getInt("id") << ", Value: " << res->getString("value") << std::endl;
//		}
//
//	}
//	catch (sql::SQLException& e)
//	{
//		std::cerr << "MySQL Error: " << e.what() << std::endl;
//	}
//
//	delete con;
//	delete stmt;
//	delete res;
//
//	return 0;
//}

void registerUser(sql::Connection* con, const std::string& username, const std::string& password)
{
	try 
	{
		std::string passwordHash = BCrypt::generateHash(password);

		sql::PreparedStatement* prepstmt = con->prepareStatement(
			"INSERT INTO users (username, password_hash) VALUES (?, ?)"
		);
		prepstmt->setString(1, username);
		prepstmt->setString(2, passwordHash);
		prepstmt->executeUpdate();

		std::cout << "User registerd successfully" << std::endl;
	}
	catch (sql::SQLException e)
	{
		std::cerr << "MySQL Error: " << e.what() << std::endl;
	}
}

bool verifyUser(sql::Connection* con, const std::string& username, const std::string& password)
{
	try
	{
		sql::PreparedStatement* prepstmt = con->prepareStatement(
			"SELECT password_hash FROM users WHERE username = ?"
		);

		prepstmt->setString(1, username);

		sql::ResultSet* res = prepstmt->executeQuery();

		if (res->next())
		{
			std::string storedHash = res->getString("password_hash");

			if (BCrypt::validatePassword(password, storedHash))
			{
				std::cout << "Authentication successful!" << std::endl;
				return true;
			}
			else
			{
				std::cout << "Authentication failed." << std::endl;
				return false;
			}
		}
		else
		{
			std::cout << "user not found." << std::endl;
			return false;
		}
	}
	catch (sql::SQLException e)
	{
		std::cerr << "MySQL Error: " << e.what() << std::endl;
	}
}

int main()
{
	sql::mysql::MySQL_Driver* driver = nullptr;
	sql::Connection* con = nullptr;

	try
	{
		driver = sql::mysql::get_mysql_driver_instance();

		con = driver->connect("tcp://127.0.0.1:3305", "user", "user");

		if (con->isValid())
		{
			std::cout << "Connect to MySQL!!" << std::endl;
		}

		con->setSchema("authexampledb_matthewdouglas");
	}
	catch (sql::SQLException& e)
	{
		std::cerr << "MySQL Error: " << e.what() << std::endl;
	}


	// Register user
	//registerUser(con, "rob3", "password");

	// Verify user
	verifyUser(con, "matt123", "password");
}