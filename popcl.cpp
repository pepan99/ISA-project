/////////////////////////////////////////////////
// popcl - program utilizing POP3 to read mail
// author: Josef Krusina
// login:  xkrusi01
/////////////////////////////////////////////////

#include "ArgumentParser.hpp"
#include "ErrorHandling.hpp"
#include "OpenSSLClass.hpp"

int main(int argc, char* argv[]) {
    std::string cmd;
    ArgumentParser arguments;
    arguments.parseArguments(argc, argv);

    OpenSSLClass ossl;
    // SECURE CONNECTION
    if (arguments.getIsSecure()) {
        ossl.initSecConn((arguments.getServerAddress()+":"+arguments.getPort()).c_str(),
                        arguments.getCertFile(), arguments.getCertAddr());
    
    //////////////////// 
    
    } else {
    // UNSECURED CONNECTION 
        ossl.initUnsecConn((arguments.getServerAddress()+":"+arguments.getPort()).c_str());
        // connect with TLS if the user wants to
	if (arguments.getIsSLTS()) {
            cmd = "STLS";
            ossl.writeCommand(cmd);

            ossl.initSLTS(arguments.getCertFile(), arguments.getCertAddr());
        }
    ///////////////////////
    }
    cmd = "USER "+ arguments.getUsername();
    ossl.writeCommand(cmd);
    
    cmd = "PASS "+ arguments.getPassword();
    ossl.writeCommand(cmd);
    
    cmd = "STAT";
    ossl.writeCommand(cmd);
    ossl.retrieveMail(arguments.getOutDir(), arguments.getNewMessagesOnly());

    if (arguments.getDeleteMessages()) {
        ossl.deleteMail();
    }
    cmd = "QUIT";
    ossl.writeCommand(cmd);

    ossl.closeConn(arguments.getIsSecure(), arguments.getIsSLTS());
    return 0;
}

