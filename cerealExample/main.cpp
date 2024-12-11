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
        ar(requestId, email, plaintextPassword);
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

// Repeat similar structures for AuthenticateWeb, AuthenticateWebSuccess, and AuthenticateWebFailure

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
        ar(requestId, userId, creationDate);
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


int main() {
    CreateAccountWeb request = { 1, "user@example.com", "password123" };
    serializeRequest(request);

    std::string json = R"({"value0":1,"value1":"user@example.com","value2":"password123"})";
    deserializeRequest(json);

    return 0;
}













//
//#include <cereal/archives/binary.hpp>
//#include <cereal/archives/json.hpp>
//#include <cereal/types/string.hpp>
//#include <sstream>
//#include <iostream>
//
//class Player 
//{
//public:
//	int id;
//	std::string name;
//	int score;
//
//	Player() = default;
//
//	Player(int id, const std::string name, int score) : id(id), name(name), score(score) {}
//
//	// serialization function
//	template <class Archive>
//	void serialize(Archive& archive)
//	{
//		archive(id, name, score);
//	}
//};
//
//std::string serializePlayerToBinary(const Player& player)
//{
//	std::ostringstream os;
//	cereal::BinaryOutputArchive archive(os);
//	archive(player);
//	return os.str();
//}
//
//Player deserializePlayerFromBinary(const std::string& data)
//{
//	std::istringstream is(data);
//	cereal::BinaryInputArchive archive(is);
//	Player player;
//	archive(player);
//	return player;
//}
//
//
//int main()
//{
//	// Create a player object
//	Player player(1, "Rob", 10020);
//
//	// Serialize the player to binary string;
//	std::string serializedData = serializePlayerToBinary(player);
//	std::cout << "Serialized data (binary format): " << serializedData << std::endl;
//
//	// Simulate "sending" the data 
//	std::string receivedData = serializedData;
//
//	// Deserialize the received data back into a player object
//
//	Player receivedPlayer = deserializePlayerFromBinary(receivedData);
//
//	// Output the received player
//	std::cout << "Deserialized Player: ID = " << receivedPlayer.id << ", Name = " << receivedPlayer.name << ", Score = " << receivedPlayer.score << std::endl;
//
//	return 0;
//}