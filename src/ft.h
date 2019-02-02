#ifndef FT_FT_H_
#define FT_FT_H_

#include <unordered_map>
#include <fstream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>


using namespace std;
using namespace cv;

class Match {
public:
  int frame_age_;
  Point last_image_point_;
  Point image_point_;
  bool last_frame_ = true;
  Match(int frame_age, 
	Point last_image_point, 
	Point image_point) : frame_age_(frame_age),
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
		 const string& o_file
		);
  void AddImage(const String &filename);
private:
  Ptr<cv::Feature2D> feature_finder;
  long frame_num_;
  std::string detector_;
  bool bayered_;
  bool draw_;
  Mat last_image_;
  Mat last_image_descs_;
  cv::NormTypes matcher_params_;
  std::ofstream o_file_;
  std::vector<KeyPoint> last_image_points_;
  std::unordered_map<int, Match> last_image_matchs_;
  double inlier_threshold_, nn_match_ratio_, best_percent_;
  std::vector<DMatch> MatchFeatureDescs(const cv::Mat &descs_1, 
					const cv::Mat &descs_2);
  void WriteEndingMatches();
};

#endif
