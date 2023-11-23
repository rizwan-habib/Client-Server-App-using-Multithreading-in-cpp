#include <gtest/gtest.h>
#include "header.h"

// Database db1;

TEST(CreateTest, TableCreation) {
    int result = db.create();
    ASSERT_EQ(result, 0);
}
TEST(DatabaseTest, Insertion) {
    std::string message = "Test message";
    std::string timestamp = "2023-07-13 10:00:00";
    db.insert(message, timestamp);

    // Fetch messages from the database
    std::vector<std::string> messages = db.fetchMessages();

    // Check if the inserted message is present in the fetched messages
    bool messageFound = false;
    for (const auto& fetchedMessage : messages) {
        if (fetchedMessage == message) {
            messageFound = true;
            break;
        }
    }
    ASSERT_TRUE(messageFound);
}
TEST(TimestampTest, Exists) {
    string str=getTimestamp();
    bool messageFound=false;
    if(str.length()>0){
        messageFound=true;
    }
    ASSERT_TRUE(messageFound);
}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
