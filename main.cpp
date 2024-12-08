#include <bits/stdc++.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

int main() {
    unordered_set<string> builtins = {"echo", "exit", "type"};

    while (true) {
        cout << unitbuf;  // Make sure that output is immediately flushed
        cerr << unitbuf;

        cout << "$ ";  // Prompt
        string input;
        getline(cin, input);

        if (cin.eof()) break;  // Exit on Ctrl+D
        if (input.empty()) continue;  // Skip empty lines

        stringstream ss(input);
        string command;
        ss >> command;

        // Handling exit command
        if (command == "exit") {
            int exitCode = 0;
            string arg;
            ss >> arg;
            if (!arg.empty()) {
                try {
                    exitCode = stoi(arg);  // Convert argument to an exit code
                } catch (const exception &e) {
                    cerr << "Invalid exit code: " << arg << endl;
                    continue;
                }
            }
            return exitCode;  // Exit with the provided exit code

        } else if (command == "echo") {  // Handling echo command
            string message;
            getline(ss, message);  // Read the message after "echo"
            if (!message.empty() && message[0] == ' ') {
                message = message.substr(1);  // Trim leading spaces
            }
            cout << message << endl;  // Output the message

        } else if (command == "type") {  // Handling type command
            string str;
            ss >> str;

            // Check if the command is a built-in
            if (builtins.find(str) != builtins.end()) {
                cout << str << " is a shell builtin" << endl;
            }
            else if (!str.empty()) {  // Check if it's a path command
                char *path_env = getenv("PATH");
                if (path_env == nullptr) {
                    cout << str << ": not found" << endl;
                }
                else {
                    stringstream pathStream(path_env);
                    string directory;
                    bool found = false;

                    // Check each directory in PATH
                    while (getline(pathStream, directory, ':')) {
                        string fullPath = directory + "/" + str;
                        if (access(fullPath.c_str(), X_OK) == 0) {  // Check if executable
                            cout << str << " is " << fullPath << endl;
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        cout << str << ": not found" << endl;
                    }
                }
            }

        } else {  // For unknown commands, we check if it's an external program
            // Split the command and its arguments
            stringstream argStream(input);
            string program;
            vector<string> args;
            argStream >> program;  // First word is the program name

            string arg;
            while (argStream >> arg) {
                args.push_back(arg);  // Collect any arguments
            }

            // Find the program in PATH
            char *path_env = getenv("PATH");
            if (path_env == nullptr) {
                cout << program << ": not found" << endl;
            } else {
                stringstream pathStream(path_env);
                string directory;
                bool found = false;
                pid_t pid = fork();

                if (pid == 0) {  // Child process
                    while (getline(pathStream, directory, ':')) {
                        string fullPath = directory + "/" + program;
                        if (access(fullPath.c_str(), X_OK) == 0) {
                            // Prepare arguments for execvp
                            vector<const char*> execArgs;
                            execArgs.push_back(fullPath.c_str());
                            for (const string& arg : args) {
                                execArgs.push_back(arg.c_str());
                            }
                            execArgs.push_back(nullptr);  // Null-terminate the arguments list

                            execvp(fullPath.c_str(), (char* const*)execArgs.data());
                            perror("execvp failed");  // If execvp fails
                            exit(1);
                        }
                    }

                    // If no executable found
                    cout << program << ": not found" << endl;
                    exit(1);
                } else if (pid > 0) {  // Parent process
                    waitpid(pid, nullptr, 0);  // Wait for the child process to finish
                } else {
                    cerr << "Fork failed" << endl;
                }
            }
        }
    }

    return 0;
}
