#include "video_to_frames.cpp" 
#include "loopstitch.cpp"
#include "frames_to_video.cpp"

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//naming convention of video files: n.mp4 (e.g. 1.mp4, 2.mp4, 3.mp4)

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
   std::cout << "# of files(n " << p1.string() << ": " << count << '\n' << endl;;
   return count;
}

int main(){
    string folderPath_mp4, folderPath_exe, folderPath_output_stitchedjpg, fps_str, skip_str, fileName;
    int fps, skip;

    cout << "Enter the directory path containing .mp4 files: ";
    getline(cin, folderPath_mp4);
    cout << "Enter the directory path containing .exe for stitcher: ";
    getline(cin, folderPath_exe);
    cout << "Enter output directory path for the stitched frames: ";
    getline(cin, folderPath_output_stitchedjpg);

    fileName = "directories.yml";

    //setting of blank entries/defaults
    //variable = (condition) ? value_if_true : value_if_false;
    //WRITE - fs << ".mp4" << folderPath_mp4;
    //READ - fs[".mp4"] >> folderPath_mp4;
    FileStorage fs(fileName, FileStorage::WRITE);
    if (folderPath_mp4.length() != 0) { 
        fs << "mp4" << folderPath_mp4;} 
    else {
        cout << "No camera source directory given." << endl;} 
    if (folderPath_exe.length() != 0) {
        fs << "exe" << folderPath_exe;} 
    else {
            cout << "No stitching.exe directory given." << endl;} 
    if (folderPath_output_stitchedjpg.length() != 0) { 
        fs << "output" << folderPath_output_stitchedjpg;} 
    else {
        cout << "No stitching output directory given." << endl;} 
    fs.release(); 

    FileStorage fs1(fileName, FileStorage::READ);
    if (folderPath_mp4.length() == 0)  {
        fs1["mp4"] >> folderPath_mp4;} 
    else{ 
        cout << "Writing camera sources directory to directories.yml..." << endl;}
    if (folderPath_exe.length() == 0)  {
        fs1["exe"] >> folderPath_exe;} 
    else{
        cout << "Writing stitching.exe directory to directories.yml..." << endl;}
    if (folderPath_output_stitchedjpg.length() == 0) {
        fs1["output"] >> folderPath_output_stitchedjpg; }
    else{
        cout << "Writing stitching output directory to directories.yml..." << endl;
    } 
    fs1.release(); 

    //requests FPS
    cout << "Enter the frame rate (fps): ";
    getline(cin, fps_str);

    if(fps_str.length() == 0){
        fps = 1;
        std::cout << "No specified fps. Set to 1 fps by default..." << endl;
    }

    else {
        fps = stoi(fps_str);
    }
    
    cout << "Existing video to frames? (1/0): " << endl;
    getline(cin, skip_str);

    if(skip_str.length() == 0){
        skip = 1;
        std::cout << "No input provided. Skipping video to frames." << endl;
    }

    else {
        skip = stoi(skip_str);
        if (skip)
            std::cout << "Skipping parsing videos..." << endl;
        else
            std::cout << "Proceeding with parsing videos..." << endl;


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

    cout << "max frames:" << to_string(framelimit) << endl;
    int early_stop = loopstitcher(framelimit, no_of_folders);

    if(early_stop){
        cout << "Cancelling stitching." << endl;
        return 0;
    }

    

    frames_to_video();
        
    return 0;
}