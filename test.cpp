#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {

    FileStorage fs("directories.yml", FileStorage::WRITE);
    fs << ".mp4" << "raw";
    fs.release(); 

    return 0;
}
