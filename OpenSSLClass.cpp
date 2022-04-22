#include "OpenSSLClass.hpp"
#include "ErrorHandling.hpp"

void OpenSSLClass::connectionInit() {
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();
}

void OpenSSLClass::connectionInitSec() {

    // load the trust store
    if (file != "") {
        if(!SSL_CTX_load_verify_locations(ctx, file.c_str(), NULL)) {
            // Handle failed load here /
        }    
    } else if (folder != "") {
        if(!SSL_CTX_load_verify_locations(ctx, NULL, folder.c_str())) {
            // Handle failed load here /
        } 
    } else {
        SSL_CTX_set_default_verify_paths(ctx);
    }   
}

void OpenSSLClass::connectionInitSLTS() {
    BIO *sslN = BIO_new_ssl(ctx, 1);
    BIO *ret = BIO_push(sslN, bio);
    BIO_get_ssl(ret, &ssl);
    bio = ret;

    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        connectionClose();
        writeError(1, "Error verifying certificate.");
    }
}

// int connectionOpen(std::string addHostname, std::string addPort) {
int OpenSSLClass::connectionOpen(const char *hostport) {
    
    bio = BIO_new_connect(hostport);
    if(bio == NULL) {
        connectionClose();
        writeError(1, "Error creating BIO.");
    }
    // verify if connection was successful
    if(BIO_do_connect(bio) <= 0) {
        connectionClose();
        writeError(1, "Connection failed.");
        /* Handle failed connection */
    }
    return 0;
}

int OpenSSLClass::connectionOpenSec(const char *hostport) {
    // setup BIO object
    bio = BIO_new_ssl_connect(ctx);
    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    // initiate secure connection
    BIO_set_conn_hostname(bio, hostport);
    // verify connection happened, perform handshake 
    if (BIO_do_connect(bio) <= 0) {
        // handle error
	connectionCloseSec();
        writeError(1, "Connection failed.");
    }
    // check if certificate valid
    if(SSL_get_verify_result(ssl) != X509_V_OK) {
        // Handle the failed verification /
        connectionCloseSec();
        writeError(2, "Verification failed.");
    }
    return 0;
}

int OpenSSLClass::connectionRead() {
    clearBuffer();

    int x = BIO_read(bio, buffer, len);
    
    // Handle closed connection 
    if(x == 0) {
        connectionClose();
        writeError(2, "Closed connection.");
    }
    else if(x < 0) {
        // Handle failed read here 
        if(!BIO_should_retry(bio)) {
            connectionClose();
            writeError(2, "Read failed.");
        }
        // else try again
        else {
            connectionRead();
        }
        
    }
    else {
        answer.clear();
        answer.append(buffer);
    }
    return 0;
}


int OpenSSLClass::connectionWrite(std::string buf) {

    if(BIO_write(bio, buf.c_str(), buf.length()) <= 0)
    {
        if(!BIO_should_retry(bio))
        {
            connectionClose();
            writeError(3, "Write failed.");
            // / Handle failed write here /
        }
        else {
            connectionWrite(buf);
        }
    }
    return 0;
}

// function for generic POP3 commands
void OpenSSLClass::writeCommand(std::string cmd) {
    cmd += "\r\n";
    connectionWrite(cmd);
    connectionRead();
    answer.clear();
    answer.append(buffer);
    
    if (!isMessageOK()) {
        writeError(3, cmd + " failed.");
    }
    return;
}

void OpenSSLClass::connectionClose() {
    // free connection from memory 
    BIO_free_all(bio);
}

void OpenSSLClass::connectionCloseSec() {
    // free connection from memory 
    BIO_free_all(bio);
    SSL_CTX_free(ctx);
}

bool OpenSSLClass::isMessageOK() {
    return answer.find("+OK") != std::string::npos ? true : false;
}

bool OpenSSLClass::isMessageEnd() {
    return answer.find("\r\n.\r\n") != std::string::npos ? true : false;
}

void OpenSSLClass::clearBuffer() {
    memset(buffer, 0, len);
}

void OpenSSLClass::getMailCount() {
    answer.erase(0,4);
    size_t position = answer.find(" ");
    answer.erase(position, answer.length());
    
    mailCount = (stoi(answer));
}

void OpenSSLClass::initSecConn(const char *hostport, std::string certFile, std::string certFolder) {
    file = certFile;
    folder = certFolder;
    connectionInitSec();
    connectionOpenSec(hostport);
    connectionRead();

    if (!isMessageOK()) {
            writeError(2, "Secure connection failed");
        }
}

void OpenSSLClass::initUnsecConn(const char *hostport) {
    connectionInit();
    connectionOpen(hostport);
    // initial greeting
    connectionRead();

    if (!isMessageOK()) {
        writeError(2, "Unsecured connection failed");
    }
}

void OpenSSLClass::initSLTS(std::string certFile, std::string certFolder) {
    file = certFile;
    folder = certFolder;

    connectionInitSec(); 
    connectionInitSLTS(); 
}

void OpenSSLClass::closeConn(bool secure, bool tls) {
    if (secure || tls) {
        connectionCloseSec();
    } else {
        connectionClose();
    }
}

void OpenSSLClass::retrieveMail(std::string directory, bool newMessagesOnly) {
    getMailCount();
    int mailsDownloaded = 0;

    DIR *dir;
    dir = opendir(directory.c_str());
    // check if able to open out_dir
    if (dir == NULL) {
        writeError(1, "Could not open out_dir.\n");
    }
    
    // main for loop for downloading emails, starting from 1 up to mailCount
    for (int i = 1; i<=mailCount; i++) {
        bool isFirstIteration = true;
        bool fileExists = false;
        
        std::string message = "";
        std::string messageID = "";
        std::string filename = directory + "file_" + std::to_string(i) + ".out";;

        // sys/stat.h to check if file is accessible
        struct stat buff;
        static const std::regex msgIdRgx ("message-id: <(.*)>", std::regex_constants::icase);

        std::string retrMail = "RETR " + std::to_string(i) + "\r\n";
        connectionWrite(retrMail);
        
        // download messages from the server, until \r\n.\r\n is read
        do {
            connectionRead();
            if (isFirstIteration) {
                answer.erase(0, answer.find_first_of("\r\n") + 2); // delete +OK server message line
                isFirstIteration = false;
            }
            message.append(answer);
        } while(!isMessageEnd());
        
        // tag fileExists as true if the message was already downloaded 
        if (newMessagesOnly) {
            std::smatch msgId;
            std::regex_search(message, msgId, msgIdRgx);
            messageID = msgId.str();

            messageID.erase(0, messageID.find("<"));
            // servers like seznam have some messages without message id, they get saved as non unique files
            // therefore overwrite other non unique files on each download
            if (messageID != "") {
                filename = directory + "file_'" + messageID + "'.out";
                
                // check if file is accessible 
                // if it is accessible then dont write to filename  
                if (stat (filename.c_str(), &buff) == 0) {
                    fileExists = true;
                }
            }
        } 

        // fileExists is false if the file doesnt exist or we care only about new messages (-n)
        if (!fileExists) {
                message.replace(message.find("\r\n.\r\n"), 5, "\r\n"); // replace message ending dot 
                std::ofstream File(filename);
                File << message;
                File.close();
                mailsDownloaded++;
        }
    }
    
    if (newMessagesOnly) {
        std::cout << "Downloaded " << mailsDownloaded << " new mail(s)." << std::endl;
    } else {
        std::cout << "Downloaded " << mailsDownloaded << " mail(s)." << std::endl;
    }
}

void OpenSSLClass::deleteMail() {
    for (int i = 1; i<=mailCount; i++) {
        std::string cmd = "DELE " + std::to_string(i);
        writeCommand(cmd);
    }
}
