#include <dirent.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

#include <gflags/gflags.h>

#include "ft.h"

DEFINE_string(input, "", "Folder name to process all the img*.ppm in.");
DEFINE_double(inlier_threshold, 0.1f, "Distance Threshold for post-fundamental transformation check.");
DEFINE_double(nn_match_ratio, 0.8f, "Nearest Neighbor Matching Ratio to decide if a match is ambigious or not.");
DEFINE_double(best_percent, 0.05f, "The best percent of matches to keep track of.");
DEFINE_string(output, "", "Output file to put matches into.");
DEFINE_string(detector, "akaze", "Detector used on images for keypoint and descriptor extraction (AKAZE, BRISK, ORB, SIFT, SURF)");
DEFINE_bool(bayered, false, "Is the image bayered?");
DEFINE_bool(draw, true, "Should we be displaying all this stuff?");

using namespace cv;
using namespace std;

bool alphabetical(string one, string two) {
    auto cone = one.c_str();
    auto ctwo = two.c_str();
    int shortestLen = one.length() < two.length()? one.length(): two.length();
    for(int32_t i = 0; i < shortestLen; i++) {
        if(cone[i] != ctwo[i])
            return cone[i] < ctwo[i];
    }
    return one.length() < two.length();
}

int main(int argc, char** argv) {
    google::ParseCommandLineFlags(&argc, &argv, false);
    if (FLAGS_input == "") {
        std::cerr << "Invalid Folder Input" << std::endl;
        exit(1);
    }
    std::string o_filename = ((FLAGS_output == "")? "/dev/null" : FLAGS_output);
    FeatureTracker feature_tracker = FeatureTracker(FLAGS_inlier_threshold,
                                                    FLAGS_nn_match_ratio,
						    FLAGS_best_percent,
						    FLAGS_detector,
						    FLAGS_draw,
						    FLAGS_bayered,
						    o_filename);
    DIR* dir = opendir(FLAGS_input.c_str());
    struct dirent* dentry = nullptr;
    stringstream path;
    vector<string> images_to_process;
    while ((dentry = readdir(dir)) != NULL) {
        if (strstr(dentry->d_name, ".jpg") != NULL) {
            path << FLAGS_input << String(dentry->d_name);
            images_to_process.push_back(path.str());
            path.str(string());
        }
    }
    closedir(dir);
    if (FLAGS_draw) {
      cv::namedWindow("Display");
    }
    std::sort(images_to_process.begin(), images_to_process.end(), alphabetical);
    for (auto image_name: images_to_process) {
        feature_tracker.AddImage(image_name);
	if (FLAGS_draw && !cv::getWindowProperty("Display", cv::WND_PROP_VISIBLE)) {
	  break;
	}
    }
    cv::destroyAllWindows();
    return 0;
}
