#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

int loopstitcher( int totalFrames, int cameras) {
    string folderPath_exe, folderPath_output, fileName, folderPath;

    // Set the path to your C++ program
    cout << "Enter the directory path containing .exe for stitcher: ";
    getline(cin, folderPath);

    fileName = "directories.yml";

    //default if no directory
    if(folderPath.length() == 0){
        //reads from directories.yaml
        FileStorage fs(fileName, FileStorage::READ);
        fs[".exe"] >> folderPath_exe;
        fs.release(); 
        std::cout << "No directory input. Using default '" << folderPath_exe << "' from directories.yml" << endl;
    }

    else{
        FileStorage fs(fileName, FileStorage::WRITE);
        fs << ".exe" << folderPath;
        fs.release();
        std::cout << "Entered directory path '" << folderPath << "' used and written into directories.yml." << endl;
    }

    cout << "Enter output directory path: ";
    getline(cin, folderPath);

    //default if no directory
    if(folderPath.length() == 0){
        //reads from directories.yaml
        FileStorage fs(fileName, FileStorage::READ);
        fs[".output"] >> folderPath_output;
        fs.release(); 
        std::cout << "No output directory specified. Using default '" << folderPath_output << "' from directories.yml" << endl;
    }

    else{
        FileStorage fs(fileName, FileStorage::WRITE);
        fs << ".output" << folderPath;
        fs.release();
        std::cout << "Entered directory path '" << folderPath << "' used and written into directories.yml." << endl;
    }

    // Loop through each frame
    for (int i = 1; i < totalFrames; i++) {
        // Construct the arguments
        stringstream arguments;
        string cont_str;
        int cont;

        arguments << "--frameno " << i << " " << cameras << " --output " << folderPath_output << i << ".jpg --features akaze --conf_thresh 0.1 --match_conf 0.1 --warp mercator";

        // Convert the stringstream to a string
        string argumentString = arguments.str();

        // Execute the program with the arguments
        string command = folderPath_exe + " " + argumentString;
        cout << "Executing command: " << command << endl;
        system(command.c_str());

        if(i == 1){
            cout << "1st frame produced. Continue? (1/0)" << endl;
            getline(cin, cont_str);
                if(cont_str.length() == 0){
                    std::cout << "No input provided. Proceeding with stitching..." << endl;
                }

                else {
                    cont = stoi(cont_str);
                    if (cont)
                        std::cout << "Proceeding with stitching videos.." << endl;
                    else{
                        std::cout << "Stopping stiching..." << endl;
                        return 1;
                    }
                }
        }
    }

    return 0;
}