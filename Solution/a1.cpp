#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

int size_of_global_color_table = 0;
int size_of_local_color_table = 0;
int current_position = 0;
bool set_global_color_table_flag = 0;
bool gif89_version = 0;

void read_header(std::string file) {

    std::fstream file_handler;
    file_handler.open(file, std::fstream::in | std::fstream::out);

    if (!file_handler.is_open()) {
        std::cout << "Error with opening of the file!" << std::endl;
        file_handler.close();
        exit(-1);
    }

    std::string signature = "";
    std::string version = "";

    char buffer[129];
    file_handler.read(buffer, 128);
    buffer[128] = '\0';

    signature.push_back(buffer[0]);
    signature.push_back(buffer[1]);
    signature.push_back(buffer[2]);

    version.push_back(buffer[3]);
    version.push_back(buffer[4]);
    version.push_back(buffer[5]);

    if (version == "89a") {
        gif89_version = true;
    }

    std::cout << std::endl;
    std::cout << ">> read header" << std::endl;
    std::cout << std::endl;
    std::cout << "Header: " << std::endl;
    std::cout << "signature:\t\t\t" << signature << std::endl;
    std::cout << "version:\t\t\t" << version << std::endl;

    file_handler.close();
}

void read_logical_screen_descriptor(std::string file) {
    std::fstream file_handler;
    file_handler.open(file, std::fstream::in | std::fstream::out);

    if (!file_handler.is_open()) {
        std::cout << "Error with opening of the file!" << std::endl;
        file_handler.close();
        exit(-1);
    }

    char buffer[257];
    file_handler.read(buffer, 256);
    buffer[256] = '\0';

    std::string width = "";
    width.push_back(buffer[6]);
    width.push_back(buffer[7]);

    std::string height = "";
    height.push_back(buffer[8]);
    height.push_back(buffer[9]);

    unsigned size1 = 0;
    unsigned size2 = 0;
    char packed_fields = buffer[10];
    int background_color_index = buffer[11];
    int aspect_ratio = buffer[12];

    *((char*)&size1) = width[0];
    *((char*)&size1 + 1) = width[1];

    *((char*)&size2) = height[0];
    *((char*)&size2 + 1) = height[1];

    int global_ct_flag = (packed_fields >> 7) & 1;
    int sort_flag = (packed_fields >> 3) & 1;

    int f1 = (packed_fields >> 6) & 1;
    int s1 = (packed_fields >> 5) & 1;
    int t1 = (packed_fields >> 4) & 1;

    int f2 = (packed_fields >> 2) & 1;
    int s2 = (packed_fields >> 1) & 1;
    int t2 = (packed_fields >> 0) & 1;

    int color_res = f1 * 4 + s1 * 2 + t1 * 1;
    int global_ct_size = f2 * 4 + s2 * 2 + t2 * 1;

    if (global_ct_flag == 1) {
        set_global_color_table_flag = true;
        size_of_global_color_table = global_ct_size;
    }

    std::cout << std::endl;
    std::cout << ">> read logical screen descriptor" << std::endl;
    std::cout << std::endl;
    std::cout << "Logical Screen Descriptor: " << std::endl;
    std::cout << "width:\t\t\t" << size1 << std::endl;
    std::cout << "height:\t\t\t" << size2 << std::endl;
    std::cout << "global ct flag:\t\t" << global_ct_flag << std::endl;
    std::cout << "color res:\t\t" << color_res << std::endl;
    std::cout << "sort flag:\t\t" << sort_flag << std::endl;
    std::cout << "global ct size:\t\t" << global_ct_size << std::endl;
    std::cout << "background color i:\t" << background_color_index << std::endl;
    std::cout << "aspect ratio:\t\t" << aspect_ratio << std::endl;


    file_handler.close();
}

void read_global_color_table(std::string file) {
    std::fstream file_handler;
    file_handler.open(file, std::fstream::in | std::fstream::out);

    if (!file_handler.is_open() || !(set_global_color_table_flag)) {
        std::cout << "Error with opening the file!" << std::endl;
        file_handler.close();
        exit(-1);
    }

    char buffer[8193];
    file_handler.read(buffer, 8192);
    buffer[8192] = '\0';

    size_of_global_color_table = pow(2, size_of_global_color_table + 1);

    std::cout << std::endl;
    std::cout << ">> read global color table" << std::endl;
    std::cout << std::endl;
    std::cout << "Global Color Table: " << std::endl;

    int counter = 0;

    for (int i = 0; i < size_of_global_color_table; i++) {

        int number1 = buffer[12 + 3 * i + 1];
        int number2 = buffer[12 + 3 * i + 2];
        int number3 = buffer[12 + 3 * i + 3];

        counter = counter + 3;

        std::cout << "#" << i << '\t';
        std::cout << " " << number1 << " " << number2 << " " << number3 << std::endl;
    }

    file_handler.close();
}

void read_image_descriptor(std::string file) {

    std::fstream file_handler;
    file_handler.open(file, std::fstream::in | std::fstream::out);

    if (!file_handler.is_open()) {
        std::cout << "Error with opening the file!" << std::endl;
        file_handler.close();
        exit(-1);
    }

    char buffer[8193];
    file_handler.read(buffer, 8192);
    buffer[8193] = '\0';

    int new_size = size_of_global_color_table - 1;
    int index = 12 + 3 * new_size + 3;
    bool check_extension = false;
    std::cout << index << std::endl;

    if (gif89_version == true) {
        for (int i = index; i < 8193; i++) {
            if (buffer[i] == 0x21) {
                check_extension = true;
            }

            if (buffer[i] == 0x2C && check_extension) {
                index = i;
                break;
            }
        }
    }

    std::cout << index << std::endl;

    std::string image_left_position = "";
    image_left_position.push_back(buffer[index + 1]);
    image_left_position.push_back(buffer[index + 2]);

    std::string image_top_position = "";
    image_top_position.push_back(buffer[index + 3]);
    image_top_position.push_back(buffer[index + 4]);

    std::string image_width = "";
    image_width.push_back(buffer[index + 5]);
    image_width.push_back(buffer[index + 6]);

    std::string image_height = "";
    image_height.push_back(buffer[index + 7]);
    image_height.push_back(buffer[index + 8]);

    unsigned left_pos = 0;
    unsigned top_pos = 0;
    unsigned size1 = 0;
    unsigned size2 = 0;

    *((char*)&left_pos) = image_left_position[0];
    *((char*)&left_pos + 1) = image_left_position[1];

    *((char*)&top_pos) = image_top_position[0];
    *((char*)&top_pos + 1) = image_top_position[1];

    *((char*)&size1) = image_width[0];
    *((char*)&size1 + 1) = image_width[1];

    *((char*)&size2) = image_height[0];
    *((char*)&size2 + 1) = image_height[1];

    char packed_fields = buffer[index + 9];

    int local_ct_flag = (packed_fields >> 7) & 1;
    int interlace_flag = (packed_fields >> 6) & 1;
    int sort_flag = (packed_fields >> 5) & 1;

    int f1 = (packed_fields >> 4) & 1;
    int s1 = (packed_fields >> 3) & 1;

    int f2 = (packed_fields >> 2) & 1;
    int s2 = (packed_fields >> 1) & 1;
    int t2 = (packed_fields >> 0) & 1;

    current_position = index + 9;

    int reserved = f1 * 2 + s1 * 1;
    int local_ct_size = f2 * 4 + s2 * 2 + t2 * 1;

    std::cout << std::endl;
    std::cout << ">> read image descriptor" << std::endl;
    std::cout << std::endl;
    std::cout << "Image Descriptor: " << std::endl;
    std::cout << "left position:\t" << left_pos << std::endl;
    std::cout << "top position:\t" << top_pos << std::endl;
    std::cout << "image width:\t" << size1 << std::endl;
    std::cout << "image height:\t" << size2 << std::endl;
    std::cout << "local ct flag:\t" << local_ct_flag << std::endl;
    std::cout << "interlace flag: " << interlace_flag << std::endl;
    std::cout << "sort flag:\t" << sort_flag << std::endl;
    std::cout << "reserved:\t" << reserved << std::endl;
    std::cout << "local ct size:\t" << local_ct_size << std::endl; 

    size_of_local_color_table = local_ct_size;
    file_handler.close();
}

void read_table_based_image_data(std::string file) {
    
    std::fstream file_handler;
    file_handler.open(file, std::fstream::in | std::fstream::out);

    if (!file_handler.is_open()) {
        std::cout << "Error with opening the file!" << std::endl;
        file_handler.close();
        exit(-1);
    }

    char buffer[65537];
    file_handler.read(buffer, 65536);
    buffer[65536] = '\0';


    unsigned LZW_minimum_code_size = buffer[current_position + 1];

    std::cout << std::endl;
    std::cout << ">> read table based image data" << std::endl;
    std::cout << std::endl;
    std::cout << "LZW Minimum Code Size: " << LZW_minimum_code_size << std::endl;
    std::cout << std::endl;

    file_handler.close();
}


void read_trailer(std::string file) {
    
    std::fstream file_handler;
    file_handler.open(file, std::fstream::in | std::fstream::out);

    if (!file_handler.is_open()) {
        std::cout << "Error with opening the file!" << std::endl;
        file_handler.close();
        exit(-1);
    }

    std::cout << std::endl;
    std::cout << ">> read trailer" << std::endl;
    std::cout << std::endl;


    file_handler.close();
}

// void get_colot_table(std::string file){}
// void get_initial_cade_table(std::string file){}
// void get_block_bytes(std::string file){}
// void get_code_stream(std::striong file){}
// void get_index_stream(std::string file){}
// void get_image_data_red_channel(std::string file){}
// void get_image_data_green_channel(std::string file){}
// void get_image_data_blue_channel(std::string file){}
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
    file_handler.open(image, std::fstream::in | std::fstream::out);

    if (file_handler.is_open()) {
        read_header(image);
        read_logical_screen_descriptor(image);
        read_global_color_table(image);
        read_image_descriptor(image);
        read_table_based_image_data(image);
        read_trailer(image);
        std::cout << std::endl;
        std::cout << ">> program ended" << std::endl;
        file_handler.close();
    }
    else {
        std::cout << "Error with opening the file!" << std::endl;
        exit(1);
    }

    return 0;
}