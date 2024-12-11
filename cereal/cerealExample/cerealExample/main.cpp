#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/map.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cereal/archives/json.hpp>

// Request and response structures
struct CreateAccountWeb {
    long requestId;
    std::string email;
    std::string plaintextPassword;

    template<class Archive>
    void serialize(Archive& ar) {
        // Ensure correct field names
        ar(CEREAL_NVP(requestId), CEREAL_NVP(email), CEREAL_NVP(plaintextPassword));
    }
};

struct CreateAccountWebSuccess {
    long requestId;
    long userId;

    template<class Archive>
    void serialize(Archive& ar) {
        ar(requestId, userId);
    }
};

struct CreateAccountWebFailure {
    long requestId;
    enum Reason {
        ACCOUNT_ALREADY_EXISTS,
        INVALID_PASSWORD,
        INTERNAL_SERVER_ERROR
    } reason;

    template<class Archive>
    void serialize(Archive& ar) {
        ar(requestId, reason);
    }
};

// Authentication Request and Response structures
struct AuthenticateWeb {
    long requestId;
    std::string email;
    std::string plaintextPassword;

    template<class Archive>
    void serialize(Archive& ar) {
        ar(requestId, email, plaintextPassword);
    }
};

struct AuthenticateWebSuccess {
    long requestId;
    long userId;
    std::string creationDate;

    template<class Archive>
    void serialize(Archive& ar) {
        // Ensure correct field names
        ar(CEREAL_NVP(requestId), CEREAL_NVP(userId), CEREAL_NVP(creationDate));
    }
};

struct AuthenticateWebFailure {
    long requestId;
    enum Reason {
        INVALID_CREDENTIALS,
        INTERNAL_SERVER_ERROR
    } reason;

    template<class Archive>
    void serialize(Archive& ar) {
        ar(requestId, reason);
    }
};

// Serialization and Deserialization Functions
// Serialization and Deserialization Functions
void serializeRequest(const CreateAccountWeb& request) {
    std::stringstream ss;
    cereal::JSONOutputArchive archive(ss);
    archive(request);

    std::cout << "Serialized Request: " << ss.str() << std::endl;
}

void deserializeRequest(const std::string& json) {
    std::stringstream ss(json);
    cereal::JSONInputArchive archive(ss);
    CreateAccountWeb request;
    archive(request);

    std::cout << "Deserialized Request: "
        << "Email: " << request.email
        << ", Password: " << request.plaintextPassword << std::endl;
}

void serializeAuthenticateResponse(const AuthenticateWebSuccess& response) {
    std::stringstream ss;
    cereal::JSONOutputArchive archive(ss);
    archive(response);

    std::cout << "Serialized Authentication Success Response: " << ss.str() << std::endl;
}

void deserializeAuthenticateResponse(const std::string& json) {
    std::stringstream ss(json);
    cereal::JSONInputArchive archive(ss);
    AuthenticateWebSuccess response;
    archive(response);

    std::cout << "Deserialized Authentication Success Response: "
        << "UserID: " << response.userId
        << ", Creation Date: " << response.creationDate << std::endl;
}

int main() {
    // CreateAccountWeb example request
    CreateAccountWeb request = { 1, "user@example.com", "password123" };
    serializeRequest(request);

    // Simulated JSON request (Valid JSON format)
    std::string json = R"({"requestId":1,"email":"user@example.com","plaintextPassword":"password123"})";
    deserializeRequest(json);

    // AuthenticateWebSuccess example response
    AuthenticateWebSuccess authSuccessResponse = { 1, 12345, "2024-12-10" };
    serializeAuthenticateResponse(authSuccessResponse);

    // Simulated JSON response (Valid JSON format)
    std::string authJson = R"({"requestId":1,"userId":12345,"creationDate":"2024-12-10"})";
    deserializeAuthenticateResponse(authJson);

    return 0;
}