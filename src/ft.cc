#include "ft.h"

#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/xfeatures2d.hpp"

using cv::Mat;
using cv::Point;
using cv::String;
using cv::DMatch;
using cv::KeyPoint;
using std::unordered_map;
using std::vector;
using std::string;
using std::pair;

FeatureTracker::FeatureTracker(double inlier_threshold,
                               double nn_match_ratio,
                               double best_percent,
                               int min_track_length,
                               const std::string &detector,
                               bool draw,
                               bool bayered,
                               const string& o_filename,
                               const string& tracks_file) {
  min_track_length_ = min_track_length;
  inlier_threshold_ = inlier_threshold;
  nn_match_ratio_ = nn_match_ratio;
  best_percent_ = best_percent;
  frame_num_ = 0;
  bayered_ = bayered;
  draw_ = draw;
  o_file_.open(o_filename);
  if (!o_file_.is_open()) {
    std::cerr << "Could not open: " << o_filename << std::endl;
    exit(1);
  }
  tracks_file_.open(tracks_file);
  if (!tracks_file_.is_open()) {
    std::cerr << "Could not open track file: " << o_filename << std::endl;
  }
  detector_ = detector;
  // Convert detector to lower case and find matching feature tracker.
  for (uint i = 0; i < detector.length(); i++) {
    detector_[i] = std::tolower(detector_[i]);
  }
  if (detector_.compare("akaze") == 0) {
    feature_finder = cv::AKAZE::create(
        cv::AKAZE::DESCRIPTOR_MLDB,
        0,
        3,
        0.0001f,
        10,
        5,
        cv::KAZE::DIFF_PM_G2);
    matcher_params_ = cv::NORM_HAMMING;
  } else if (detector_.compare("orb") == 0) {
    feature_finder = cv::ORB::create(
        10000, 1.04f, 50, 31, 0, 2, cv::ORB::HARRIS_SCORE, 31, 20);
    matcher_params_ = cv::NORM_HAMMING;
  } else if (detector_.compare("brisk") == 0) {
    feature_finder = cv::BRISK::create(20, 7, 1.1f);
    matcher_params_ = cv::NORM_HAMMING;
  } else if (detector_.compare("surf") == 0) {
    feature_finder = cv::xfeatures2d::SURF::create();
    matcher_params_ = cv::NORM_L2;
  } else if (detector_.compare("sift") == 0) {
    feature_finder = cv::xfeatures2d::SIFT::create();
    matcher_params_ = cv::NORM_L2;
  } else if (detector_.compare("freak") == 0) {
    feature_finder = cv::xfeatures2d::FREAK::create(false, true, 40.0f, 20);
    keypoint_finder = cv::FastFeatureDetector::create(10, true);
    matcher_params_ = cv::NORM_HAMMING;
  } else {
    std::cerr << "Unrecognized Detector Option: " << detector_ << std::endl;
    exit(1);
  }
}

void FeatureTracker::AddImage(const String &filename) {
  // Create new Image.
  cv::Mat new_image = cv::imread(filename, cv::IMREAD_GRAYSCALE);
  if (new_image.empty()) {
    std::cerr << "Problem loading image: " << filename << std::endl;
    exit(1);
  }
  if (bayered_) {
    cv::cvtColor(new_image, new_image, cv::COLOR_BayerBG2BGR);
    cv::cvtColor(new_image, new_image, cv::COLOR_BGR2GRAY);
  }
  // Get the feature keypoints and descriptors.
  cv::Mat new_descriptors;
  std::vector<cv::KeyPoint> new_keypoints;
  if (detector_.compare("freak") == 0) {
    keypoint_finder->detect(new_image, new_keypoints);
    feature_finder->compute(new_image, new_keypoints, new_descriptors);
  } else {
    feature_finder->detectAndCompute(
        new_image, cv::noArray(), new_keypoints, new_descriptors);
  }
  // If not the first image...
  if (!old_descriptors_.empty()) {
    // Find non-ambigious matches between the last image and the new one.
    // "query" == last image
    // "train" == current image
    std::vector<DMatch> matches =
        MatchFeatureDescs(old_descriptors_, new_descriptors);
     // Add the best new matches to our map.
    unordered_map<int, Match> new_matches;
    for (auto match : matches) {
      const Match new_match(2,
                            old_keypoints_[match.queryIdx].pt,
                            new_keypoints[match.trainIdx].pt ,
                            frame_num_);
      new_matches.insert({ match.trainIdx, new_match});
    }
    // Do any of these matches appear in the last image matches?
    if (old_matches_.size() > 0) {
      int max_age = (matches.size() > 0) ? 1 : 0;
      for (auto match : matches) {
        auto last_image_match = old_matches_.find(match.queryIdx);
        if (last_image_match != old_matches_.end()) {
          // Match exists in the last file, copy its age value.
          Match& new_match = new_matches.at(match.trainIdx);
          new_match.frame_age_ = last_image_match->second.frame_age_ + 1;
          last_image_match->second.last_frame_ = false;
          new_match.track_history_.insert(
              new_match.track_history_.begin(),
              last_image_match->second.track_history_.begin(),
              last_image_match->second.track_history_.end());
          if (new_match.frame_age_ > max_age) {
              max_age = new_match.frame_age_;
          }
        }
      }
      int mean_age = 0;
      for (const auto& match : new_matches) {
        mean_age += match.second.frame_age_;
      }
      if (mean_age > 0) {
        mean_age = mean_age / new_matches.size();
      }
      if (draw_) {
        std::cout << "Max Age: " << max_age
                  << " Mean: " << mean_age << std::endl;
      }
    }
    // For all the matches that are ending write them to the output file.
    WriteEndingMatches();
    WriteFeatureTracks();
    old_matches_ = new_matches;
    if (draw_) {
      if (false) {
        for (std::pair<int, Match> match_pair : new_matches) {
          if (match_pair.second.frame_age_ > 2) {
            cv::circle(new_image,
                       match_pair.second.last_image_point_,
                       2,
                       CV_RGB(0, 0, 255));
            cv::addText(new_image,
                        std::to_string(match_pair.second.frame_age_),
                        match_pair.second.image_point_,
                        "Times");
            cv::line(new_image,
                     match_pair.second.last_image_point_,
                     match_pair.second.image_point_,
                     CV_RGB(255, 0, 0),
                     2);
          }
        }
      }
      Mat temp;
      cv::drawMatches(last_image_,
                            old_keypoints_,
                      new_image,
                      new_keypoints,
                      matches,
                      temp,
                      CV_RGB(0, 0, 255),
                      cv::Scalar(-1),
                      {},
                      cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
      cv::imshow("Display", temp);
      if (cv::waitKey(5) == 27) {
        cv::destroyAllWindows();
        exit(2);
      }
    }
  }
  last_image_ = new_image;
  old_descriptors_ = new_descriptors;
  old_keypoints_ = new_keypoints;
  frame_num_++;
}


std::vector<DMatch> FeatureTracker::MatchFeatureDescs(
    const cv::Mat &descs_1, const cv::Mat &descs_2) {
  // Brute force matching between the descriptor sets (find the best 2 matches).
  cv::BFMatcher matcher(matcher_params_);
  vector<vector<DMatch>> nn_matches;
  // "query" == from descs_1 (old image)
  // "train" == from descs_2 (new image)
  matcher.knnMatch(descs_1, descs_2, nn_matches, 2);
  // Only take the matches that are non-ambigious (i.e. one distance is match
  // ratio larger than the other).
  vector<DMatch> best_matches;
  for (size_t i = 0; i < nn_matches.size(); i++) {
    DMatch first = nn_matches[i][0];
    float dist1 = nn_matches[i][0].distance;
    float dist2 = nn_matches[i][1].distance;
    if (dist1 < nn_match_ratio_ * dist2) {
      // Add non-ambiguous matches.
      best_matches.push_back(first);
    } else {
      // Also add matches if either of the top two matches was there in
      // the previous frame.
      auto result_0 = old_matches_.find(nn_matches[i][0].queryIdx);
      auto result_1 = old_matches_.find(nn_matches[i][1].queryIdx);
      if (!old_matches_.empty() && result_0 != old_matches_.end()) {
        best_matches.push_back(nn_matches[i][0]);
      } else if (!old_matches_.empty() && result_1 != old_matches_.end()) {
        best_matches.push_back(nn_matches[i][1]);
      }
    }
  }
  // Get the best percentage of matches.
  sort(best_matches.begin(), best_matches.end());
  const int num_good_matches = best_matches.size() * best_percent_;
  best_matches.resize(num_good_matches);
  return best_matches;
}

void FeatureTracker::WriteEndingMatches() {
  for (std::pair<int, Match> match_pair : old_matches_) {
    if (match_pair.second.last_frame_) {
      o_file_ << match_pair.second.frame_age_ << std::endl;
    }
  }
}

void FeatureTracker::WriteFeatureTracks() {
  for (std::pair<int, Match> match_pair : old_matches_) {
    if (match_pair.second.last_frame_) {
      if (match_pair.second.track_history_.size() < min_track_length_) continue;
      for (const pair<int, Point>& p : match_pair.second.track_history_) {
        tracks_file_ << p.first << ","
                     << p.second.x << ","
                     << p.second.y << ", ";
      }
      tracks_file_ << std::endl;
    }
  }
}
