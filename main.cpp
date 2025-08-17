#include <iostream> // for output
#include <fstream> // for opening file
#include <string> // for string work
#include <cstring> // for strcat
#include <cstdio> // for popen
#include <cctype> // for iscntrl
#include <filesystem> // for exists

std::string remove_symbols(std::string str) {
    std::string result {};
    for (char c : str) {
        if (isalnum(c)) {
            result += c;
        }
    }
    return result;
}

std::string remove_space(std::string str) {
    std::string result {};
    for (unsigned char c : str) {
        if (!isspace(c) && !iscntrl(c)) { // checks if it is not space and not symbol like \a
            result += c;
        }
    }
    return result;
}

std::string execute(const char *cmd) {
    char buffer[1024]; // can change to less if outout not so big 
    std::string result {};

    FILE *pipe = popen(cmd, "r"); // r - means read, w - means write
    if (!pipe) {
        throw std::runtime_error("Error in command execution");
    }
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe); 
    return result;
}

int main(int argc, char *argv[]) {
    if (argc == 1 || argc > 2 || remove_symbols(std::string(argv[0])) == remove_symbols(std::string(argv[1]))) {
        std::cout << "Usage: " << argv[0] << " [executable] \n";
        return -1;
    }
    else {
        if (!std::filesystem::exists(argv[1])) {
            std::cout << "Specify executable file! \n";
            return -1;
        }
        std::ifstream file(argv[1], std::ios::binary);
        char combiner[strlen(argv[1])] = ".";
        char header[4];
        std::string expected_output {}; 
        file.seekg(0);
        file.read(header, sizeof(header));
        file.close();
        if (header[0] == 0x7F || header[1] == 'E' && header[2] == 'L' && header[3] == 'F') {
            strcat(combiner, "/");
        }
        else if (header[0] == 'M' && header[1] == 'Z') {
            strcat(combiner, "\\");
        }
        else {
            std::cout << "The file format is not recognizable \n";
            return -1;
        }
        strcat(combiner, argv[1]);
        std::cout << "Enter output that you expect from executable " << argv[1] << ": ";
        std::getline(std::cin, expected_output);
        std::string output = execute(combiner);
        output = remove_space(output);
        expected_output = remove_space(expected_output);
        
        std::cout << "\nInfo: \n\n";
        std::cout << "    Output from file: " << output << std::endl;
        std::cout << "    Expectation: " << expected_output << std::endl;
        
        if (output == expected_output) {
            std::cout << "\033[0;32m" << "\n    Test passed! \n\n" << "\033[0m";
        }
        else {
            std::cout << "\033[0;31m" << "\n    Test failed! \n\n" << "\033[0m";
        }
        
    }
    return 0;
}
