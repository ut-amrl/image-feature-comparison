#ifndef SRC_FT_H_
#define SRC_FT_H_

#include <inttypes.h>
#include <unordered_map>
#include <fstream>

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
  bool last_frame_ = true;
  Match(int frame_age,
        cv::Point last_image_point,
        cv::Point image_point) : frame_age_(frame_age),
                             last_image_point_(last_image_point),
                             image_point_(image_point) {}
};

class FeatureTracker {
 public:
  FeatureTracker(double inlier_threshold,
                 double nn_match_ratio,
                 double best_percent_,
                 const std::string &detector,
                 bool draw,
                 bool bayered,
                 const std::string& o_file);
  void AddImage(const cv::String &filename);

 private:
  cv::Ptr<cv::Feature2D> feature_finder;
  cv::Ptr<cv::FastFeatureDetector> keypoint_finder;
  uint64_t frame_num_;
  std::string detector_;
  bool bayered_;
  bool draw_;
  cv::Mat last_image_;
  cv::Mat last_image_descs_;
  cv::NormTypes matcher_params_;
  std::ofstream o_file_;
  std::vector<cv::KeyPoint> old_keypoints_;
  std::unordered_map<int, Match> old_matches_;
  double inlier_threshold_, nn_match_ratio_, best_percent_;
  std::vector<cv::DMatch> MatchFeatureDescs(const cv::Mat &descs_1,
                                            const cv::Mat &descs_2);
  void WriteEndingMatches();
};

#endif  // SRC_FT_H_
