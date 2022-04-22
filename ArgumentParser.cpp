#include "ArgumentParser.hpp"
#include "ErrorHandling.hpp"

// argument parsing

int ArgumentParser::parseArguments(int argNum, char* argVar[]) {
    // first argument is always the requested IP address or domain name 
    serverAddress = argVar[1]; 
    
    for (int i = 2; i < argNum; i++) {
        argument = argVar[i];
        if (argument.compare("-T") == 0) {
            isSecure = true;

        } else if (argument.compare("-S") == 0) {
            isSLTS = true;

        } else if (argument.compare("-d") == 0) {
            deleteMessages = true;

        } else if (argument.compare("-n") == 0) {
            newMessagesOnly = true;
        // else segfault can occur 
        } else if (!((i + 1) >= argNum)) {
            if(argument.compare("-p") == 0) {
                port = argVar[++i];

            } else if (argument.compare("-a") == 0) {
                authFile = argVar[++i];

            } else if (argument.compare("-o") == 0) {
                outDir = argVar[++i];

            } else if (argument.compare("-c") == 0 && (isSLTS || isSecure)) {
                certFile = argVar[++i];
                
            } else if (argument.compare("-C") == 0 && (isSLTS || isSecure)) {
                certAddr = argVar[++i];

            }
        } else {
            writeError(1, "Error at argument "+std::to_string(i)+ " (" + argument + ").");
        }
    }
    readAuth();
    if (authFile == "" || outDir == "") {
        writeError(1, "Missing auth_file (-a) and/or out_dir (-o).");
    }
    return 0;
}

void ArgumentParser::readAuth() {
    std::string line;
    std::ifstream readFile(authFile);
    std::string usr = "username = ";
    std::string pwd = "password = ";

    if (readFile.is_open()) {
        getline(readFile, line);
        if (line.find(usr) != std::string::npos) {
            username = line.substr(usr.length());
        }
        getline(readFile, line);
        if (line.find(pwd) != std::string::npos) {
            password = line.substr(pwd.length());
        }
    } else {
        writeError(7, "Error opening auth file.");
    }
}


bool ArgumentParser::getIsSecure() {
    return isSecure;
}

bool ArgumentParser::getIsSLTS() {
    return isSLTS;
}

bool ArgumentParser::getDeleteMessages() {
    return deleteMessages;
}

bool ArgumentParser::getNewMessagesOnly() {
    return newMessagesOnly;
}

std::string ArgumentParser::getPort() {
    return port;
}
std::string ArgumentParser::getCertFile() {
    return certFile;
}
std::string ArgumentParser::getCertAddr() {
    return certAddr;
}
std::string ArgumentParser::getAuthFile() {
    return authFile;
}
std::string ArgumentParser::getOutDir() {
    return outDir;
}
std::string ArgumentParser::getServerAddress() {
    return serverAddress;
}
std::string ArgumentParser::getUsername() {
    return username;
}
std::string ArgumentParser::getPassword() {
    return password;
}