#ifndef ARGUMENTPARSER_HPP
#define ARGUMENTPARSER_HPP

#include <stdio.h>
#include <iostream>
#include <fstream>

class ArgumentParser {
    private:
        std::string argument = "";
        bool isSecure = false;
        bool isSLTS = false;
        bool deleteMessages = false;
        bool newMessagesOnly = false;
        std::string port = "110"; // default port for unsecured connection
        std::string certFile = "";
        std::string certAddr = "";
        std::string authFile = "";
        std::string outDir = "";
        std::string serverAddress = "";
        std::string username = "";
        std::string password = "";

        void readAuth();

    public:
        int parseArguments(int argNum, char* argVar[]);

        bool getIsSecure();
        bool getIsSLTS();
        bool getDeleteMessages();
        bool getNewMessagesOnly();

        std::string getPort();
        std::string getCertFile();
        std::string getCertAddr();
        std::string getAuthFile();
        std::string getOutDir();
        std::string getServerAddress();
        std::string getUsername();
        std::string getPassword();
};

#endif