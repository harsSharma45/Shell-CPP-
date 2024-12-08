// Include necessary header files
#include <bits/stdc++.h>   // Includes all standard libraries (not recommended for production)
#include <cstdlib>         // For getenv, exit
#include <unistd.h>        // For fork, execvp, access
#include <sys/wait.h>      // For waitpid
#include <sys/types.h>     // For pid_t

using namespace std;

int main() {
    // Set of built-in commands recognized by the shell
    unordered_set<string> builtins = {"echo", "exit", "type"};

    while (true) {
        // Ensure that output streams are flushed immediately
        cout << unitbuf;
        cerr << unitbuf;

        cout << "$ ";      // Display the shell prompt
        string input;
        getline(cin, input);  // Read user input from the console

        if (cin.eof()) break;        // Exit the loop if End-Of-File (Ctrl+D) is encountered
        if (input.empty()) continue; // Skip iteration if the input is empty

        stringstream ss(input); // Create a string stream from the input
        string command;
        ss >> command;          // Extract the command (first word)

        // Handling the "exit" command
        if (command == "exit") {
            int exitCode = 0;
            string arg;
            ss >> arg;  // Read an optional argument for the exit code
            if (!arg.empty()) {
                try {
                    exitCode = stoi(arg);  // Convert argument to integer
                } catch (const exception &e) {
                    cerr << "Invalid exit code: " << arg << endl;
                    continue;  // Continue to the next iteration if invalid
                }
            }
            return exitCode;  // Exit the shell with the given exit code

        // Handling the "echo" command
        } else if (command == "echo") {
            string message;
            getline(ss, message);  // Read the rest of the input as the message
            if (!message.empty() && message[0] == ' ') {
                message = message.substr(1);  // Remove leading space if present
            }
            cout << message << endl;  // Display the message

        // Handling the "type" command
        } else if (command == "type") {
            string str;
            ss >> str;  // Read the argument for the "type" command

            // Check if the command is a built-in
            if (builtins.find(str) != builtins.end()) {
                cout << str << " is a shell builtin" << endl;
            }
            // If not a built-in, check if it's an external command
            else if (!str.empty()) {
                char *path_env = getenv("PATH");  // Get the PATH environment variable
                if (path_env == nullptr) {
                    cout << str << ": not found" << endl;
                } else {
                    stringstream pathStream(path_env);
                    string directory;
                    bool found = false;

                    // Iterate through each directory in PATH
                    while (getline(pathStream, directory, ':')) {
                        string fullPath = directory + "/" + str;
                        if (access(fullPath.c_str(), X_OK) == 0) {  // Check if executable
                            cout << str << " is " << fullPath << endl;
                            found = true;
                            break;  // Stop searching if found
                        }
                    }

                    if (!found) {
                        cout << str << ": not found" << endl;
                    }
                }
            }

        // Handling external commands
        } else {
            // Split the input into program and its arguments
            stringstream argStream(input);
            string program;
            vector<string> args;
            argStream >> program;  // First word is the program name

            string arg;
            while (argStream >> arg) {
                args.push_back(arg);  // Collect all arguments
            }

            // Attempt to find and execute the external program
            char *path_env = getenv("PATH");  // Get PATH environment variable
            if (path_env == nullptr) {
                cout << program << ": not found" << endl;
            } else {
                stringstream pathStream(path_env);
                string directory;
                pid_t pid = fork();  // Create a new child process

                if (pid == 0) {  // Child process
                    // Search for the executable in each directory in PATH
                    while (getline(pathStream, directory, ':')) {
                        string fullPath = directory + "/" + program;
                        if (access(fullPath.c_str(), X_OK) == 0) {
                            // Prepare arguments for execvp
                            vector<const char*> execArgs;
                            execArgs.push_back(fullPath.c_str());  // Program path
                            for (const string& arg : args) {
                                execArgs.push_back(arg.c_str());   // Program arguments
                            }
                            execArgs.push_back(nullptr);  // Null-terminate the argument list

                            // Replace the current process with the new program
                            execvp(fullPath.c_str(), (char* const*)execArgs.data());
                            perror("execvp failed");  // Print error if execvp fails
                            exit(1);  // Exit child process
                        }
                    }

                    // If program not found
                    cout << program << ": not found" << endl;
                    exit(1);  // Exit child process

                } else if (pid > 0) {  // Parent process
                    waitpid(pid, nullptr, 0);  // Wait for the child process to finish
                } else {
                    cerr << "Fork failed" << endl;  // Error if fork fails
                }
            }
        }
    }

    return 0;  // Exit the shell
}





// Purpose of the Code:

// Shell Implementation: This program implements a basic shell (command-line interface) that can execute built-in commands and external programs.

// Built-in Commands:

// exit [code]: Exits the shell with an optional exit code.
// echo [message]: Prints the provided message to the console.
// type [command]: Checks if a command is a built-in or an external command, and shows its path.
// External Commands:

// For commands that are not built-in, the shell searches for the executable in the directories listed in the PATH environment variable.
// If found, it executes the command with any provided arguments in a child process.
// Process Management:

// Child Process: Created using fork() to execute external commands without blocking the shell.
// Process Replacement: The child process uses execvp() to replace its image with the external program.
// Parent Process: Waits for the child process to complete using waitpid().
