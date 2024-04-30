#include "video_to_frames.cpp" 
#include "loopstitch.cpp"
#include "frames_to_video.cpp"

#include "run.hpp"

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//naming convention of video files: n.mp4 (e.g. 1.mp4, 2.mp4, 3.mp4)

string folderPath_mp4, folderPath_exe, folderPath_output_stitchedjpg;

int file_counter(string& directory)
{
   int count = 0;
   //cout << directory << endl;

   std::filesystem::path p1 (directory);

   for (auto& p : std::filesystem::directory_iterator(p1))
   {
      ++count;
   }    

   //p1 is string() since without it, display is "C:\\Users\\admin" due to "\" being an escape character
   std::cout << "# of files in " << p1.string() << ": " << count << '\n' << endl;;
   return count;
}

int main(){
    string fps_str, skip_str, fileName;
    int fps, skip;

    cout << "[IN] Enter the directory path containing .mp4 files: ";
    getline(cin, folderPath_mp4);
    cout << "[IN] Enter the directory path containing .exe for stitcher: ";
    getline(cin, folderPath_exe);
    cout << "[IN] Enter output directory path for the stitched frames: ";
    getline(cin, folderPath_output_stitchedjpg);    

    fileName = "../../directoryhandling.yml";

    //setting of blank entries/defaults
    //variable = (condition) ? value_if_true : value_if_false;
    //WRITE - fs << ".mp4" << folderPath_mp4;
    //READ - fs[".mp4"] >> folderPath_mp4;

    FileStorage fs1(fileName, FileStorage::READ);
    if (folderPath_mp4.length() == 0)  {
        cout << "[DIR] No camera source directory given. Reading from directoryhandling.yml..." << endl;
        fs1["mp4"] >> folderPath_mp4;} 
    else{ 
        cout << "[DIR] Writing camera sources directory to directoryhandling.yml.yml..." << endl;}
    if (folderPath_exe.length() == 0)  {
        cout << "[DIR] No stitching.exe directory given. Reading from directoryhandling.yml..." << endl;
        fs1["exe"] >> folderPath_exe;} 
    else{
        cout << "[DIR] Writing stitching.exe directory to directoryhandling.yml..." << endl;}
    if (folderPath_output_stitchedjpg.length() == 0) {
        cout << "[DIR] No stitching output directory given. Reading from directoryhandling.yml..." << endl;
        fs1["output"] >> folderPath_output_stitchedjpg; }
    else{
        cout << "[DIR] Writing stitching output directory to directoryhandling.yml.yml..." << endl;
    } 
    fs1.release(); 

    FileStorage fs(fileName, FileStorage::WRITE);
    if (folderPath_mp4.length() != 0) { 
        fs << "mp4" << folderPath_mp4;} 
    else {
        } 
    if (folderPath_exe.length() != 0) {
        fs << "exe" << folderPath_exe;} 
    else {
            } 
    if (folderPath_output_stitchedjpg.length() != 0) { 
        fs << "output" << folderPath_output_stitchedjpg;} 
    else {
        }
    fs << "yaml" << folderPa; 
    fs.release(); 



    //requests FPS
    cout << "[IN] Enter the frame rate (fps): ";
    getline(cin, fps_str);

    if(fps_str.length() == 0){
        fps = 1;
        std::cout << "[FPS] No specified fps. Set to 1 fps by default..." << endl;
    }

    else {
        fps = stoi(fps_str);
    }
    
    cout << "[IN] Existing video to frames? (1/0): " << endl;
    getline(cin, skip_str);

    if(skip_str.length() == 0){
        skip = 1;
        std::cout << "[SKP] No input provided. Skipping video to frames." << endl;
    }

    else {
        skip = stoi(skip_str);
        if (skip)
            std::cout << "[SKP] Skipping parsing videos..." << endl;
        else
            std::cout << "[SKP] Proceeding with parsing videos..." << endl;


    }

    //MP4 to frames
    //int to keep track of folders
    int no_of_folders = processMp4Files(folderPath_mp4, fps, skip);

//looping: stitch frames together
    
    //loop through each x_frames folder and get the minimum number of frames
    int framelimit = 0;
    int framelimit_holder = 0;
    for (int i = 1; i < no_of_folders + 1; ++i) {
        if (i == 1) 
        {
            string folder = to_string(i) + "_frames";
            framelimit = file_counter(folder);
        }

        else 
        {
            string folder = to_string(i) + "_frames";
            framelimit_holder = file_counter(folder);

            if(framelimit_holder < framelimit) 
            {
                framelimit = framelimit_holder;
            }
        }
    }

    cout << "[INFO] max frames:" << to_string(framelimit) << endl;
    int early_stop = loopstitcher(framelimit, no_of_folders);

    if(early_stop){
        cout << "[STOP] Cancelling stitching." << endl;
        return 0;
    }

    

    frames_to_video();
        
    return 0;
}