#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>

#include "run.hpp"


using namespace std;
namespace fs = std::filesystem;

void extractFramesFromVideo(const string& videoFile, const string& outputFolder, int fps) {
    // Create output folder if it doesn't exist
    if (!fs::exists(outputFolder))
        fs::create_directory(outputFolder);

    // Construct ffmpeg command
    string command = "ffmpeg -i \"" + videoFile + "\" -r " + to_string(fps) + " \"" + outputFolder + "/%d.jpg\"";

    // Execute command
    cout << "Executing command: " << command << endl;
    
    int result = system(command.c_str());
    if (result != 0) {
        cerr << "Failed to execute ffmpeg for file: " << videoFile << endl;
    }
}

int processMp4Files(const string& directory, int fps, int skip) {
    int count = 0;

    for (const auto& entry : fs::directory_iterator(directory)) {
        count++;
        if(!skip){
            if (entry.path().extension() == ".mp4") {
                string videoFile = entry.path().string();
                string outputFolder = folderPath_output_stitchedjpg + "/" + entry.path().stem().string() + "_frames";
                extractFramesFromVideo(videoFile, outputFolder, fps);
            }
        }
    }

    return count;
}

/*int start() {
    string folderPath;
    int fps;
    
    cout << "Enter the directory path containing .mp4 files: ";
    getline(cin, folderPath);
    
    cout << "Enter the frame rate (fps): ";
    cin >> fps;
    
    processMp4Files(folderPath, fps);

    return 0;
}*/

