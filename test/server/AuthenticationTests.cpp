#include "server/Authentication.hpp"
#include <gtest/gtest.h>

class AuthenticationTest : public ::testing::Test {
protected:
    Authentication auth;

    void SetUp() override {
        auth.addCredentials(1, "password123");
    }
};

TEST_F(AuthenticationTest, SuccessfulAuthentication) {
    EXPECT_TRUE(auth.authenticate(1, "password123"));
    EXPECT_TRUE(auth.isAuthorized(1));
}

TEST_F(AuthenticationTest, FailedAuthenticationWrongPassword) {
    EXPECT_FALSE(auth.authenticate(1, "wrong"));
    EXPECT_FALSE(auth.isAuthorized(1));
}

TEST_F(AuthenticationTest, MultipleFailedAttemptsLocksAccount) {
    for (int i = 0; i < 3; ++i) auth.authenticate(1, "wrong");
    EXPECT_FALSE(auth.authenticate(1, "password123"));
    EXPECT_FALSE(auth.isAuthorized(1));
}

TEST_F(AuthenticationTest, UnregisteredClientCannotAuthenticate) {
    EXPECT_FALSE(auth.authenticate(99, "anything"));
    EXPECT_FALSE(auth.isAuthorized(99));
}

TEST_F(AuthenticationTest, AddCredentialsOnlyOnce) {
    auth.addCredentials(1, "newpass");
    EXPECT_FALSE(auth.authenticate(1, "newpass"));
    EXPECT_TRUE(auth.authenticate(1, "password123"));
}

TEST_F(AuthenticationTest, RemoveCredentials) {
    auth.addCredentials(2, "otherpass");
    auth.removeCredentials(1);

    EXPECT_FALSE(auth.authenticate(1, "password123"));
    EXPECT_FALSE(auth.isAuthorized(1));
    EXPECT_TRUE(auth.authenticate(2, "otherpass"));
    EXPECT_TRUE(auth.isAuthorized(2));
}

TEST_F(AuthenticationTest, ManualStatusChange) {
    auth.authenticate(1, "password123");
    EXPECT_TRUE(auth.isAuthorized(1));
    auth.setLoggedOut(1);
    EXPECT_FALSE(auth.isAuthorized(1));
}

TEST_F(AuthenticationTest, ResetAfterFingerprintUnlock) {
    for (int i = 0; i < 3; ++i) auth.authenticate(1, "wrong");
    EXPECT_FALSE(auth.authenticate(1, "password123"));

    auth.unblockWithFingerprint(1);
    EXPECT_TRUE(auth.isAuthorized(1));
}

TEST_F(AuthenticationTest, ResetDoesNotAffectOtherClients) {
    auth.addCredentials(2, "otherpass");
    for (int i = 0; i < 3; ++i) auth.authenticate(2, "fail");
    auth.unblockWithFingerprint(1);

    EXPECT_FALSE(auth.authenticate(2, "otherpass"));
}

TEST_F(AuthenticationTest, BlockDueToEmergency) {
    auth.blockDueToEmergency(1);
    EXPECT_FALSE(auth.isAuthorized(1));
    EXPECT_FALSE(auth.authenticate(1, "password123"));
}

TEST_F(AuthenticationTest, UnlockWithCorrectSecretPhrase) {
    auth.blockDueToEmergency(1);
    EXPECT_FALSE(auth.isAuthorized(1));

    EXPECT_TRUE(auth.unlockWithSecretPhrase(1, "emergencyUnlock"));
    EXPECT_TRUE(auth.authenticate(1, "password123"));
    EXPECT_TRUE(auth.isAuthorized(1));
}

TEST_F(AuthenticationTest, UnlockWithIncorrectSecretPhrase) {
    auth.blockDueToEmergency(1);
    EXPECT_FALSE(auth.isAuthorized(1));

    EXPECT_FALSE(auth.unlockWithSecretPhrase(1, "wrongPhrase"));
    EXPECT_FALSE(auth.isAuthorized(1));
}

TEST_F(AuthenticationTest, BlockDueToEmergencyForUnregisteredClient) {
    auth.blockDueToEmergency(99);
    EXPECT_FALSE(auth.isAuthorized(99));
}

TEST_F(AuthenticationTest, UnlockWithSecretPhraseForUnregisteredClient) {
    EXPECT_FALSE(auth.unlockWithSecretPhrase(99, "emergencyUnlock"));
}