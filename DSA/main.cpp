#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include <set>

using namespace std;

const string LOGGED_IN_MSG = "\t\t\t Welcome Master! "; // log in account
const string INVALID_CHOICE_MSG = "\t\t\tInvalid choice. Please enter 'yes' or 'no'."; // invalid choice from the user
const string LOGGED_OUT_MSG = "\t\t\tLogged out successfully!";
const string GOODBYE_MSG = "\t\t\t Goodbye!"; //Exiting the program.

// Structure to store user information
struct User {
    string username;
    string password;
    vector<string> friendList;
    vector<string> friendRequests;
    vector<string> sentFriendRequests;
};

// Function to check if a user is registered
bool isUserRegistered(const vector<User>& users, const string& username) {
    return find_if(users.begin(), users.end(), [&](const User& user) {
        return user.username == username;
    }) != users.end();
}

// Function to find a user by username
User* findUser(vector<User>& users, const string& username) {
    auto it = find_if(users.begin(), users.end(), [&](User& user) {
        return user.username == username;
    });
    return it != users.end() ? &(*it) : nullptr;
}

// Function to show user friend list with index
void showFriendListWithIndex(const User& user) {
    cout << "\t\t\tFriend List for " << user.username << ":" << endl;
    if (user.friendList.empty()) {
        cout << "\t\t\tNo available Friend List" << endl;
    } else {
        for (size_t i = 0; i < user.friendList.size(); i++) {
            cout << "\t\t\t" << left << setw(5) << to_string(i + 1) + "." << right << setw(5) << user.friendList[i] << endl;
        }
    }
}

// Function to show user profile
void showUserProfile(const User& user) {
    cout << "\t\t\tUsername: " << user.username << endl;
    cout << "\t\t\tFriend Count: " << user.friendList.size() << endl;
}

// Function to add a friend to a user's friend list
void addFriend(User& user, const string& friendUsername, vector<User>& users) {
    if (find(user.friendList.begin(), user.friendList.end(), friendUsername) == user.friendList.end()) {
        user.friendList.push_back(friendUsername);
        User* friendUser = findUser(users, friendUsername);
        if (friendUser != nullptr) {
            friendUser->friendList.push_back(user.username);
            user.friendRequests.erase(remove(user.friendRequests.begin(), user.friendRequests.end(), friendUsername), user.friendRequests.end());
            user.sentFriendRequests.erase(remove(user.sentFriendRequests.begin(), user.sentFriendRequests.end(), friendUsername), user.sentFriendRequests.end());
        }
    } else {
        cout << friendUsername << "\t\t\t is already a friend." << endl;
    }
}

// Function to remove a friend from a user's friend list
void removeFriend(User& user, const string& friendUsername, vector<User>& users) {
    auto it = find(user.friendList.begin(), user.friendList.end(), friendUsername);
    if (it != user.friendList.end()) {
        user.friendList.erase(it);
        User* friendUser = findUser(users, friendUsername);
        if (friendUser != nullptr) {
            auto friendIt = find(friendUser->friendList.begin(), friendUser->friendList.end(), user.username);
            if (friendIt != friendUser->friendList.end()) {
                friendUser->friendList.erase(friendIt);
            }
        }
    } else {
        cout << friendUsername << "\t\t\t is not in your friend list." << endl;
    }
}

// Function to send a friend request
void sendFriendRequest(User& sender, const string& friendUsername, vector<User>& users) {
    if (sender.username == friendUsername) {
        cout << "\t\t\tYou cannot send a friend request to yourself." << endl;
        return;
    }

    User* friendUser = findUser(users, friendUsername);
    if (friendUser != nullptr) {
        if (find(sender.sentFriendRequests.begin(), sender.sentFriendRequests.end(), friendUsername) != sender.sentFriendRequests.end()) {
            cout << "\t\t\tYou have already sent a friend request to " << friendUsername << "." << endl;
            return;
        }
        sender.sentFriendRequests.push_back(friendUsername);
        friendUser->friendRequests.push_back(sender.username);
        cout << "\t\t\tFriend request sent successfully!" << endl;
        sender.sentFriendRequests.erase(remove(sender.sentFriendRequests.begin(), sender.sentFriendRequests.end(), friendUsername), sender.sentFriendRequests.end());
    } else {
        cout << "\t\t\tUser not found." << endl;
    }
}

// Function to handle friend request with yes or no choice
void handleFriendRequest(User& receiver, const string& senderUsername, vector<User>& users, const string& choice) {
    User* sender = findUser(users, senderUsername);
    if (sender != nullptr) {
        if (choice == "yes") {
            addFriend(receiver, sender->username, users);
            cout << "\t\t\tFriend request accepted!" << endl;
        } else if (choice == "no") {
            receiver.friendRequests.erase(remove(receiver.friendRequests.begin(), receiver.friendRequests.end(), sender->username), receiver.friendRequests.end());
            sender->sentFriendRequests.erase(remove(sender->sentFriendRequests.begin(), sender->sentFriendRequests.end(), receiver.username), sender->sentFriendRequests.end());
            cout << "\t\t\tFriend request declined!" << endl;
        } else {
            cout << INVALID_CHOICE_MSG << endl;
        }
    } else {
        cout << "\t\t\tUser not found." << endl;
    }
}

// Function to find mutual friends between two users
vector<string> findMutualFriends(const User& user1, const User& user2) {
    set<string> user1Friends(user1.friendList.begin(), user1.friendList.end());
    vector<string> mutualFriends;

    for (const string& friendName : user2.friendList) {
        if (user1Friends.find(friendName) != user1Friends.end()) {
            mutualFriends.push_back(friendName);
        }
    }

    return mutualFriends;
}

// Function to show mutual friends between current user and another user
void showMutualFriends(const User& currentUser, const User& otherUser) {
    vector<string> mutualFriends = findMutualFriends(currentUser, otherUser);

    cout << "\t\t\tMutual Friends with " << otherUser.username << ":\n";
    if (mutualFriends.empty()) {
        cout << "\t\t\tNo mutual friends found.\n";
    } else {
        for (const string& mutualFriend : mutualFriends) {
            cout << "\t\t\t" << mutualFriend << "\n";
        }
    }
}

// Function to store user login data
void storeUserLoginData(const string& username, const string& password, vector<pair<string, string>>& loginData) {
    loginData.push_back(make_pair(username, password));
}

// Function to handle the login process
bool login(const string& username, const string& password, const vector<pair<string, string>>& loginData) {
    return find_if(loginData.begin(), loginData.end(), [&](const pair<string, string>& userData) {
        return userData.first == username && userData.second == password;
    }) != loginData.end();
}

// Function to validate user choice
bool isValidChoice(const string& choice) {
    return choice == "yes" || choice == "Yes" || choice == "YES" ||
           choice == "no" || choice == "No" || choice == "NO";
}

// Function for case-insensitive string comparison
/*bool isEqualIgnoreCase(const string& str1, const string& str2) {
    return str1.size() == str2.size() &&
           equal(str1.begin(), str1.end(), str2.begin(), [](char a, char b) {
               return tolower(a) == tolower(b);
           });
}*/

int main() {
    vector<User> users;
    User* currentUser = nullptr;
    vector<pair<string, string>> loginData;

    cout << "\t\t\t-----------------------------------" << endl;
    cout << "\t\t\t|   Welcome to Social Networking  |" << endl;
    cout << "\t\t\t-----------------------------------" << endl;

    while (true) {
        cout << "\t\t\t-----------------------------------" << endl;
        cout << "\t\t\t|            Main Menu            |" << endl;
        cout << "\t\t\t-----------------------------------" << endl;
        cout << "\t\t\t| 1. Sign in                      |" << endl;
        cout << "\t\t\t| 2. Exit                         |" << endl;
        cout << "\t\t\t-----------------------------------" << endl;
        cout << "\t\t\tEnter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {            // Sign in
            string username, password;
            cout << "\t\t\tEnter your username: ";
            cin >> username;
            cout << "\t\t\tEnter your password: ";
            cin >> password;
            cout << "\t\t\t--------------------------------" << endl;

            if (currentUser == nullptr) {
                if (login(username, password, loginData)) {
                    currentUser = findUser(users, username);
                    cout << LOGGED_IN_MSG << endl;
                } else {
                    cout << "\t\t\tUser not found. Do you want to register? (Y/N): ";
                    char registerChoice;
                    cin >> registerChoice;
                    cout << endl;

                    if (registerChoice == 'Y' || registerChoice == 'y') {
                        users.push_back({username, password});
                        currentUser = &users.back();
                        storeUserLoginData(username, password, loginData);
                        cout << "\t\t\tRegistered and logged in successfully!" << endl;
                    } else {
                        cout << "\t\t\tRegistration aborted. Please sign in with an existing account." << endl;
                    }
                }
            } else {
                cout << "\t\t\tAlready logged in as " << currentUser->username << "." << endl;
            }
        } else if (choice == 2) {
            break;
        } else {
            cout << INVALID_CHOICE_MSG << endl;
        }

        while (currentUser != nullptr) {

            cout << "\t\t\t-----------------------------------" << endl;
            cout << "\t\t\t|    Currently Log in as:       " << currentUser-> username <<"|"<<endl;
            cout << "\t\t\t-----------------------------------" << endl;
            cout << "\t\t\t|          User Menu              |" << endl;
            cout << "\t\t\t-----------------------------------" << endl;
            cout << "\t\t\t| 1. Show Profile                 |" << endl;
            cout << "\t\t\t| 2. Show Friend List             |" << endl;
            cout << "\t\t\t| 3. Remove Friend                |" << endl;
            cout << "\t\t\t| 4. Handle Friend Requests       |" << endl;
            cout << "\t\t\t| 5. Send Friend Request          |" << endl;
            cout << "\t\t\t| 6. Show Mutual Friends          |" << endl;
            cout << "\t\t\t| 7. Add Friend                   |" << endl;
            cout << "\t\t\t| 8. Logout                       |" << endl;
            cout << "\t\t\t-----------------------------------" << endl;
            cout << "\t\t\tEnter your choice: ";
            int userChoice;
            cin >> userChoice;
            cout << endl;

            switch (userChoice) {
                case 1: {
                    showUserProfile(*currentUser);
                    break;
                }
                case 2: {
                    showFriendListWithIndex(*currentUser);
                    break;
                }
                case 3: {
                    cout << "\t\t\tEnter the username of the friend you want to remove: ";
                    string friendUsername;
                    cin >> friendUsername;
                    removeFriend(*currentUser, friendUsername, users);
                    break;
                }
                case 4: {
                    if (currentUser->friendRequests.empty()) {
                        cout << "\t\t\tNo friend requests." << endl;
                    } else {
                        cout << "\t\t\tFriend Requests:" << endl;
                        for (const auto& request : currentUser->friendRequests) {
                            cout << "\t\t\t" << request << endl;
                            cout << "\t\t\tDo you want to accept? (yes/no): ";
                            string choice;
                            cin >> choice;
                            if (isValidChoice(choice)) {
                                handleFriendRequest(*currentUser, request, users, choice);
                            } else {
                                cout << INVALID_CHOICE_MSG << endl;
                            }
                        }
                    }
                    break;
                }
                case 5: {
                    // Sending Friend Request
                    cout << "\t\t\tExisting Accounts:" << endl;
                    for (const auto& user : users) {
                        if (user.username != currentUser->username) {
                            cout << "\t\t\t" << user.username << endl;
                        }
                    }
                    cout << "\t\t\tEnter the username you want to send a request to: ";
                    string friendUsername;
                    cin >> friendUsername;
                    sendFriendRequest(*currentUser, friendUsername, users);
                    break;
                }
                case 6: {
                    cout << "\t\t\tMutual Friends:" << endl;
                    for (const auto& user : users) {
                        if (user.username != currentUser->username) {
                            vector<string> mutualFriends = findMutualFriends(*currentUser, user);
                            if (!mutualFriends.empty()) {
                                cout << "\t\t\tMutual friends with " << user.username << ":" << endl;
                                for (const auto& mutualFriend : mutualFriends) {
                                    cout << "\t\t\t" << mutualFriend << endl;
                                }
                            }
                        }
                    }
                    break;
                }
                case 7:{
            string friendUsername;
            cout << "Enter the username of the friend you want to add: ";
            cin >> friendUsername;
            addFriend(*currentUser, friendUsername);
            cout << "Friend added successfully!" << endl;
                }
                case 8: {
                    currentUser = nullptr;
                    cout << LOGGED_OUT_MSG << endl;
                    break;
                }
                default: {
                    cout << "\t\t\tInvalid choice. Please try again." << endl;
                    break;
                }
            }
        }
    }
    cout << GOODBYE_MSG << endl;
    return 0;
}

