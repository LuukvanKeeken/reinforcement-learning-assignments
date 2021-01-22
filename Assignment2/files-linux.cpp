/* Linux File Creation */

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
#include <sys/types.h>
#include <sys/stat.h>

/* Function that checks if a directory exists. If it doesn't, a new
 * directory with the desired name is created. */
void directoryCheck(const char* directory_name, std::string dir_name){
    struct stat info;
    if (stat(directory_name, &info) != 0){
        if (mkdir(directory_name, 0777) != 0){
            std::cout << "Could not create directory \"" + dir_name + "\", please create it manually.\n";
        } else {
            std::cout << "Created directory \"" + dir_name + "\".\n";
        }
    } else if (!(info.st_mode & S_IFDIR)){
        std::cout << "\"" + dir_name + "\" is not an existing directory\n";
        if (mkdir(directory_name, 0777) != 0){
            std::cout << "Could not create directory \"" + dir_name + "\", please create it manually with that exact name, and restart the experiment.\n";
        } else {
            std::cout << "Created directory \"" + dir_name + "\".\n";
        }
    }
}