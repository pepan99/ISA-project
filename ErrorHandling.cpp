#include "ErrorHandling.hpp"

void writeError(int errCode, std::string errReason) {
    std::cerr << errReason << std::endl;
    exit(errCode);
}