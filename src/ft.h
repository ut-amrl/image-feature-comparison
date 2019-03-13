#ifndef SRC_FT_H_
#define SRC_FT_H_

#include <inttypes.h>
#include <unordered_map>
#include <fstream>
#include <utility>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

class Match {
 public:
  int frame_age_;
  cv::Point last_image_point_;
  cv::Point image_point_;
  // Track history, saved as a vector of pairs of image numbers and pixel
  // locations.
  std::vector<std::pair<int, cv::Point>> track_history_;
  bool last_frame_;
  Match(int frame_age,
        cv::Point last_image_point,
        cv::Point image_point,
        int frame_num) :
      frame_age_(frame_age),
      last_image_point_(last_image_point),
      image_point_(image_point),
      last_frame_(true) {
    track_history_.push_back(std::make_pair(frame_num, image_point));
  }
};

class FeatureTracker {
 public:
  FeatureTracker(double inlier_threshold,
                 double nn_match_ratio,
                 double best_percent_,
                 int min_track_length,
                 const std::string &detector,
                 bool draw,
                 bool bayered,
                 const std::string& o_file,
                 const std::string& tracks_file);
  void AddImage(const cv::String &filename);

 private:
  cv::Ptr<cv::Feature2D> feature_finder;
  cv::Ptr<cv::FastFeatureDetector> keypoint_finder;
  uint64_t frame_num_;
  std::string detector_;
  bool bayered_;
  bool draw_;
  cv::Mat last_image_;
  cv::Mat old_descriptors_;
  cv::NormTypes matcher_params_;
  std::ofstream o_file_;
  std::ofstream tracks_file_;
  std::vector<cv::KeyPoint> old_keypoints_;
  // Lookup for matches. Key: descriptor index from old image, value: Match.
  std::unordered_map<int, Match> old_matches_;
  double inlier_threshold_, nn_match_ratio_, best_percent_;
  int min_track_length_;
  std::vector<cv::DMatch> MatchFeatureDescs(const cv::Mat &descs_1,
                                            const cv::Mat &descs_2);
  void WriteEndingMatches();
  void WriteFeatureTracks();
};

#endif  // SRC_FT_H_
