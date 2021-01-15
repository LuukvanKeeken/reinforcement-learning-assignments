/* Windows File Creation */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "parameterValuesStruct.hpp"
#include "general-functions.hpp"
#include <random>
#include <ctime>
#include <cmath>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <direct.h>


/* Windows will only make the last folder in the directory_name that is taken as input into this function
so need to make the folders above manually first */
void directoryCheck(const char* directory_name, std::string dir_name){
    struct stat info;
    if (stat(directory_name, &info) != 0){
        if (_mkdir(directory_name) != 0){
            std::cout << "Could not create directory \"" + dir_name + "\", please create it manually.\n";
        } else {
            std::cout << "Created directory \"" + dir_name + "\".\n";
        }
    } else if (!(info.st_mode & S_IFDIR)){
        std::cout << "\"" + dir_name + "\" is not an existing directory\n";
        if (_mkdir(directory_name) != 0){
            std::cout << "Could not create directory \"" + dir_name + "\", please create it manually with that exact name, and restart the experiment.\n";
        } else {
            std::cout << "Created directory \"" + dir_name + "\".\n";
        }
    }
}