#include <iostream>
#include <string>
#include <vector>
#include <fstream>

/*void read_header(std::string file) {}
void read_logical_screen_descriptor(std::string file) {}
void read_global_color_table(std::string file) {}
void read_image_descriptor(std::string file) {}
void read_table_based_image_data(std::string file) {}
void read_trailer(std::strinf file) {}
*/
int main() {

    std::string image = "";
    std::string result = "";

    std::cout << "GIF IMAGE VIEWER" << std::endl;
    std::cout << std::endl;
    
    while (result != ".gif") {
        result = "";
        std::cout << "Enter file name: " << std::endl;
        std::cin >> image;

        char len1 = image[image.size() - 1];
        char len2 = image[image.size() - 2];
        char len3 = image[image.size() - 3];
        char len4 = image[image.size() - 4];

        result.push_back(len4);
        result.push_back(len3);
        result.push_back(len2);
        result.push_back(len1);
    }

    std::fstream file_handler;
    file_handler.open("test.txt", std::fstream::in | std::fstream::out);

    if (file_handler.is_open()) {

        std::cout << "The file has been opened properly!" << std::endl;
        /*file_handler << "this is sample text 1";
        file_handler << "this is sample text 2";
        file_handler << "this is sample text 3";*/

        std::string line = "";
        unsigned int counter = 0;

        while (!file_handler.eof()) {
            //file_handler >> 
        }

        file_handler.close();
    }
    else {
        std::cout << "Error with opening the file!" << std::endl;
        exit(1);
    }

    return 0;
}