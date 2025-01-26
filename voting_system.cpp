#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <ctime>
using namespace std;
// Block structure to simulate a simple block chain
struct Block {
    int index;
    string previousHash;
    string hash;
    string timestamp;
    string vote;
    string cnic;
};

// Function to calculate the hash of a block
 string calculateHash(int index, const string& previousHash, const string& timestamp, const string& vote,const string& cnic) {
    return to_string(index) + previousHash + timestamp + vote + cnic;
}

// Function to display the blockchain (Admin view)
void displayBlockchain(const vector<Block>& blockchain) {
    for (const auto& block : blockchain) {
        cout << "Block " << block.index << ":\n";
        cout << "Timestamp: " << block.timestamp;
        cout << "Vote: " << block.vote << "\n";
        cout << "CNIC: " << block.cnic << "\n";
        cout << "Hash: " << block.hash << "\n";
        cout << "Previous Hash: " << block.previousHash << "\n\n";
    }
}

// Function to add a new block to the blockchain (when a vote is cast)
void addBlock(vector<Block>& blockchain, const string& vote, const string& cnic, unordered_set<string>& votedCNICs) {
    // Check if CNIC has already voted
    if (votedCNICs.find(cnic) != votedCNICs.end()) {
        cout << "Error: CNIC " << cnic << " has already voted.\n";
        return;
    }

    // Record the vote and add the CNIC to the voted list
    time_t currentTime;
    time(&currentTime);
    string timestamp = ctime(&currentTime);

    int index = blockchain.size();
    string previousHash = index == 0 ? "0" : blockchain.back().hash;

    Block newBlock;
    newBlock.index = index;
    newBlock.previousHash = previousHash;
    newBlock.timestamp = timestamp;
    newBlock.vote = vote;
    newBlock.cnic = cnic;
    newBlock.hash = calculateHash(index, previousHash, timestamp, vote, cnic);

    blockchain.push_back(newBlock);
    votedCNICs.insert(cnic);  // Mark this CNIC as voted
    cout << "Vote recorded successfully for CNIC " << cnic << "\n";
}

// Function to display the voting results
void displayResults(const vector<Block>& blockchain) {
    unordered_map<string, int> voteCounts;
    for (const auto& block : blockchain) {
        voteCounts[block.vote]++;
    }

    cout << "Vote counts:\n";
    for (const auto& voteCount : voteCounts) {
        cout << voteCount.first << ": " << voteCount.second << " votes\n";
    }
}

// Function to load CNICs from a file
void loadEligibleVoters(unordered_set<string>& eligibleVoters) {
    ifstream file("eligible_voters.txt");
    string cnic;
    if (file.is_open()) {
        while (getline(file, cnic)) {
            eligibleVoters.insert(cnic);
        }
        file.close();
    } else {
        cout << "Error: Could not open eligible_voters.txt\n";
    }
}

int main() {
    // Load CNICs from the file into the unordered_set
    unordered_set<string> eligibleVoters;
    loadEligibleVoters(eligibleVoters);

    if (eligibleVoters.empty()) {
        cout << "Error: No eligible voters found. Please check the eligible_voters.txt file.\n";
        return 1;
    }

    // For tracking votes
    vector<Block> blockchain;
    unordered_set<string> votedCNICs;  // Set to store CNICs that have voted
    bool running = true;

    while (running) {
        cout << "\nVoting System Menu:\n";
        cout << "1. Cast your vote\n";
        cout << "2. View blockchain (Admin)\n";
        cout << "3. View results (Admin)\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.ignore();  // Ignore any leftover newline character

        switch (choice) {
            case 1: {
                // Voting functionality
                string cnic;
                cout << "Enter your CNIC: ";
                getline(cin, cnic);

                // Check if the CNIC is in the eligible voters list
                if (eligibleVoters.find(cnic) == eligibleVoters.end()) {
                    cout << "Error: CNIC " << cnic << " is not registered to vote.\n";
                } else if (votedCNICs.find(cnic) != votedCNICs.end()) {
                    cout << "Error: CNIC " << cnic << " has already voted.\n";
                } else {
                    string vote;
                    cout << "Enter your vote (Candidate 1, Candidate 2, etc.): ";
                    getline(cin, vote);

                    // Cast vote and record it in the blockchain
                    addBlock(blockchain, vote, cnic, votedCNICs);
                }
                break;
            }
            case 2: {
                // Admin access to view blockchain
                string password;
                cout << "Enter admin password to view blockchain: ";
                getline(cin, password);

                if (password == "admin123") {
                    displayBlockchain(blockchain);
                } else {
                    cout << "Incorrect password! Access denied.\n";
                }
                break;
            }
            case 3: {
                // Admin access to view results
                string password;
                cout << "Enter admin password to view results: ";
                getline(cin, password);

                if (password == "admin123") {
                    displayResults(blockchain);
                } else {
                    cout << "Incorrect password! Access denied.\n";
                }
                break;
            }
            case 4: {
                // Exit the program
                cout << "Exiting the voting system.\n";
                running = false;
                break;
            }
            default:
                cout << "Invalid choice! Please select a valid option.\n";
        }
    }

    return 0;
}
