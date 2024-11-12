/*
Name= Sovit Bhandari
UID= U83561265
Description: This C++ program implements an in-memory file system using a tree structure, allowing users to navigate and manage directories and files. Key functionalities include creating directories (mkdir), creating files (touch), changing directories (cd), listing contents (ls), printing the current path (pwd), and removing files or directories (rm).
*/


#include "FileSystem.hpp"
#include <stdexcept>
#include <sstream>

FileSystemNode::FileSystemNode(std::string name, bool isDir) 
    : name(name), isDirectory(isDir), parent(nullptr) {}

FileSystemNode::~FileSystemNode() {
    for (auto child : children) {
        delete child;
    }
}

FileSystem::FileSystem() {
    root = new FileSystemNode("/", true);                                                           // Create root directory
    currentDirectory = root;                                                                        // Set current directory to root
}

FileSystem::~FileSystem() {
    delete root;
}

// Creates a new directory in the current directory
void FileSystem::mkdir(const std::string& name) {
    for (auto child : currentDirectory->children) {                                                 // Check if directory already exists
        if (child->name == name && child->isDirectory) {
            throw std::runtime_error("File already exists");
        }
    }
    FileSystemNode* newDir = new FileSystemNode(name, true);                                        // Create new directory node
    newDir->parent = currentDirectory;                                                              // Set current directory as parent
    currentDirectory->children.push_back(newDir);                                                   // Add new directory to children
}

// Creates a new file in the current directory
void FileSystem::touch(const std::string& name) {
    for (auto child : currentDirectory->children) {                                                 // Check if file already exists
        if (child->name == name && !child->isDirectory) {
            throw std::runtime_error("File already exists");
        }
    }
    FileSystemNode* newFile = new FileSystemNode(name, false);                                      // Create new file node
    newFile->parent = currentDirectory;                                                             // Set current directory as parent
    currentDirectory->children.push_back(newFile);                                                  // Add new file to children
}

// Lists the contents of the current directory
std::string FileSystem::ls() {
    std::stringstream ss;
    for (auto child : currentDirectory->children) {
        ss << child->name << (child->isDirectory ? "/" : "") << "\n";                               // Append directory/file names with formatting
    }
    return ss.str();
}

// Changes the current directory based on the path provided
void FileSystem::cd(const std::string& path) {
    if (path == "/") {                                                                              // Go to root
        currentDirectory = root;
    } else if (path == "..") {                                                                      // Go to parent directory
        if (currentDirectory->parent != nullptr) {
            currentDirectory = currentDirectory->parent;
        }
    } else {                                                                                        // Go to a specified child directory
        bool found = false;
        for (auto child : currentDirectory->children) {
            if (child->name == path && child->isDirectory) {
                currentDirectory = child;
                found = true;
                break;
            }
        }
        if (!found) {
            throw std::runtime_error("Directory not found");
        }
    }
}

// Removes a file or directory in the current directory
void FileSystem::rm(const std::string& name) {
    for (auto it = currentDirectory->children.begin(); it != currentDirectory->children.end(); ++it) {
        if ((*it)->name == name) {                                                                  // Find the file or directory by name
            delete *it;                                                                             // Delete the node
            currentDirectory->children.erase(it);                                                   // Remove from children vector
            return;
        }
    }
    throw std::runtime_error("File or directory not found");
}

// Prints the full path of the current directory
std::string FileSystem::pwd() {
    if (currentDirectory == root) return "/";                                                       // Root directory path
    
    std::string path;
    FileSystemNode* node = currentDirectory;
    while (node != root) {                                                                          // Build path by traversing up to the root
        path = "/" + node->name + path;
        node = node->parent;
    }
    return path + "/";
}

// Recursive helper function to find a node by name starting from a given node
FileSystemNode* FileSystem::findNode(FileSystemNode* startNode, const std::string& name) {
    if (startNode->name == name) {
        return startNode;
    }
    for (auto child : startNode->children) {
        FileSystemNode* found = findNode(child, name);                                              // Recursive search in children
        if (found) {
            return found;
        }
    }
    return nullptr;
}

// Public find function to start the search from the root
FileSystemNode* FileSystem::find(const std::string& name) {
    return findNode(root, name);
}

// Recursive helper function to display the tree structure from a given node
std::string FileSystem::displayTree(FileSystemNode* node, std::string indent) {
    std::stringstream ss;
    ss << indent << node->name << (node->isDirectory ? "/" : "") << "\n";                          // Display node with indentation
    if (node->isDirectory) {
        for (auto child : node->children) {
            ss << displayTree(child, indent + "  ");                                               // Recursive call with increased indent
        }
    }
    return ss.str();
}

// Displays the entire tree structure starting from the root
std::string FileSystem::tree() {
    return displayTree(root, "");                                                                  // Call helper function with root and initial indentation
}