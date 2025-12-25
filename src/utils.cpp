#include "utils.h"

using namespace std;
using namespace glm;

void error(std::string msg, int code){
    cout << "Error: " << msg << endl;
    exit(code);
}

void warning(std::string msg){
    cout << "Warning: " << msg << endl;
}

void todo(std::string msg){
    cout << "TODO: " << msg << endl;
    exit(0);
}
