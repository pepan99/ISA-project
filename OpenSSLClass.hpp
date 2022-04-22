#ifndef OPENSSLCLASS_HPP
#define OPENSSLCLASS_HPP

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <fstream>
#include <regex>
#include <sys/stat.h>

class OpenSSLClass {
    private:
        BIO *bio; // input output abstraction
        BIO *sslN; // input output abstraction
        BIO *ret; // input output abstraction
        SSL_CTX  *ctx = SSL_CTX_new(SSLv23_client_method());
        SSL  *ssl;

        std::string folder = "";
        std::string file = "";
        std::string answer;
        
        static const int len = 1024;
        char buffer[len];
        int mailCount;

        void clearBuffer();
        
        void connectionInit();
        void connectionInitSec();
        void connectionInitSLTS();
        int connectionOpen(const char *hostport);
        int connectionOpenSec(const char *hostport);
        int connectionRead();
        int connectionWrite(std::string buf);
        void connectionClose();
        void connectionCloseSec();
        
        bool isMessageOK();
        bool isMessageEnd();
        
        void getMailCount();


    public:
        void initUnsecConn(const char *hostport);
        void initSecConn(const char *hostport, std::string certFile, std::string certFolder);
        void initSLTS(std::string certFile, std::string certFolder);
        void closeConn(bool secure, bool tls);
        void retrieveMail(std::string directory, bool newMessagesOnly);
        void deleteMail();

        void writeCommand(std::string cmd);

};

#endif