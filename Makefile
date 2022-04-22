popcl : OpenSSLClass.o popcl.o ErrorHandling.o ArgumentParser.o
	g++ -Wall -Wextra -o popcl OpenSSLClass.o popcl.o ErrorHandling.o ArgumentParser.o -lcrypto -lssl

ErrorHandling.o : ErrorHandling.cpp ErrorHandling.hpp
	g++ -Wall -Wextra -c -o ErrorHandling.o ErrorHandling.cpp

popcl.o : popcl.cpp
	g++ -Wall -Wextra -c -o popcl.o popcl.cpp

ArgumentParser.o : ArgumentParser.cpp ArgumentParser.hpp
	g++ -Wall -Wextra -c -o ArgumentParser.o ArgumentParser.cpp

OpenSSLClass.o : OpenSSLClass.cpp OpenSSLClass.hpp
	g++ -Wall -Wextra -c -o OpenSSLClass.o OpenSSLClass.cpp
