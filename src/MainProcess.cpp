#include <opencv2/opencv.hpp>

void printMatInfo(const cv::Mat& mat, const std::string &str = "");
template <typename ... Args> std::string format(const std::string& fmt, Args ... args );
cv::Point CrossLineLine(const cv::Point &p0, const cv::Point &p1, const cv::Point &p2, const cv::Point &p3);

void ConvertColor(const cv::Mat &inmat, cv::Mat &outmat, int ccc) {
  cv::cvtColor(inmat, outmat, ccc);
  return;
}

bool detect(const cv::Mat &inMat, cv::Mat &outMat) {
  cv::QRCodeDetector qrcode_detector;
  inMat.copyTo(outMat);

  std::vector<cv::Point> points;
  bool ret = qrcode_detector.detect(inMat, points);
  if(!ret) return ret;

  /* 枠線描画 */
  cv::line(outMat, cv::Point(points[0].x, points[0].y), cv::Point(points[1].x, points[1].y), cv::Scalar(0,0,255), 5);
  cv::line(outMat, cv::Point(points[1].x, points[1].y), cv::Point(points[2].x, points[2].y), cv::Scalar(0,0,255), 5);
  cv::line(outMat, cv::Point(points[2].x, points[2].y), cv::Point(points[3].x, points[3].y), cv::Scalar(0,0,255), 5);
  cv::line(outMat, cv::Point(points[3].x, points[3].y), cv::Point(points[0].x, points[0].y), cv::Scalar(0,0,255), 5);
  /* 頂点描画 */
  cv::circle(outMat, cv::Point(points[0].x, points[0].y), 10, cv::Scalar(255,   0, 255));
  cv::circle(outMat, cv::Point(points[1].x, points[1].y), 10, cv::Scalar(255, 255,   0));
  cv::circle(outMat, cv::Point(points[2].x, points[2].y), 10, cv::Scalar(  0, 255, 255));
  cv::circle(outMat, cv::Point(points[3].x, points[3].y), 10, cv::Scalar(  0, 255,   0));
  /* 交点描画 */
  cv::Point center = CrossLineLine(points[0], points[1], points[2], points[3]);
  cv::circle(outMat, center, 10, cv::Scalar( 0, 0,255), -1);
  /* 交点補助線描画 */
  cv::line(outMat, cv::Point(points[0].x, points[0].y), cv::Point(points[2].x, points[2].y), cv::Scalar(255,7,85));
  cv::line(outMat, cv::Point(points[1].x, points[1].y), cv::Point(points[3].x, points[3].y), cv::Scalar(255,7,85));
  /* 頂点番号描画 */
  cv::putText(outMat, "0", cv::Point(points[0].x, points[0].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,   0, 255));
  cv::putText(outMat, "1", cv::Point(points[1].x, points[1].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255,   0));
  cv::putText(outMat, "2", cv::Point(points[2].x, points[2].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(  0, 255, 255));
  cv::putText(outMat, "3", cv::Point(points[3].x, points[3].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(  0, 255,   0));

  return ret;
}

/**********************/
/* 2線分の交点を求める */
/* p0 ------ p1 */
/* |          | */
/* |          | */
/* |          | */
/* p3 ------ p2 */
/**********************/
cv::Point CrossLineLine(const cv::Point &p0, const cv::Point &p1, const cv::Point &p2, const cv::Point &p3) {
  double s1 = ((p1.x-p3.x)*(p0.y-p3.y)-(p1.y-p3.y)*(p0.x-p3.x)) / 2.0;
  double s2 = ((p1.x-p3.x)*(p3.y-p2.y)-(p1.y-p3.y)*(p3.x-p2.x)) / 2.0;
  double x = p0.x+(p2.x-p0.x) * s1 / (s1+s2);
  double y = p0.y+(p2.y-p0.y) * s1 / (s1+s2);

  cv::Point ret = {(int)x, (int)y};
  return ret;
};


/********************************************************/
/* printMatInfo                                         */
/*   cv::Matの情報を表示する関数(cv::Matは分かりづらいから) */
/********************************************************/
void printMatInfo(const cv::Mat& mat, const std::string &str) {
  std::cout << str << "(row:" << mat.rows << " col:" << mat.cols;
  /* 要素の型 */
  std::cout << " type:" << (
      mat.type() == CV_8UC1  ? "CV_8UC1" :
      mat.type() == CV_8UC2  ? "CV_8UC2" :
      mat.type() == CV_8UC3  ? "CV_8UC3" :
      mat.type() == CV_8UC4  ? "CV_8UC4" :
      mat.type() == CV_8SC1  ? "CV_8SC1" :
      mat.type() == CV_8SC2  ? "CV_8SC2" :
      mat.type() == CV_8SC3  ? "CV_8SC3" :
      mat.type() == CV_8SC4  ? "CV_8SC4" :
      mat.type() == CV_16UC1 ? "CV_16UC1" :
      mat.type() == CV_16UC2 ? "CV_16UC2" :
      mat.type() == CV_16UC3 ? "CV_16UC3" :
      mat.type() == CV_16UC4 ? "CV_16UC4" :
      mat.type() == CV_16SC1 ? "CV_16SC1" :
      mat.type() == CV_16SC2 ? "CV_16SC2" :
      mat.type() == CV_16SC3 ? "CV_16SC3" :
      mat.type() == CV_16SC4 ? "CV_16SC4" :
      mat.type() == CV_32SC1 ? "CV_32SC1" :
      mat.type() == CV_32SC2 ? "CV_32SC2" :
      mat.type() == CV_32SC3 ? "CV_32SC3" :
      mat.type() == CV_32SC4 ? "CV_32SC4" :
      mat.type() == CV_32FC1 ? "CV_32FC1" :
      mat.type() == CV_32FC2 ? "CV_32FC2" :
      mat.type() == CV_32FC3 ? "CV_32FC3" :
      mat.type() == CV_32FC4 ? "CV_32FC4" :
      mat.type() == CV_64FC1 ? "CV_64FC1" :
      mat.type() == CV_64FC2 ? "CV_64FC2" :
      mat.type() == CV_64FC3 ? "CV_64FC3" :
      mat.type() == CV_64FC4 ? "CV_64FC4" :
      mat.type() == CV_16FC1 ? "CV_16FC1" :
      mat.type() == CV_16FC2 ? "CV_16FC2" :
      mat.type() == CV_16FC3 ? "CV_16FC3" :
      mat.type() == CV_16FC4 ? "CV_16FC4" :
      "other"
      );

  /* 深度 */
  std::cout << " depth:" << (
      mat.depth() == CV_8U  ? "CV_8U" :
      mat.depth() == CV_8S  ? "CV_8S" :
      mat.depth() == CV_16U ? "CV_16U" :
      mat.depth() == CV_16S ? "CV_16S" :
      mat.depth() == CV_32S ? "CV_32S" :
      mat.depth() == CV_32F ? "CV_32F" :
      mat.depth() == CV_64F ? "CV_64F" :
      mat.depth() == CV_16F ? "CV_16F" :
      "other"
      );

  /* チャンネル数 */
  std::cout << " channels:" << mat.channels();

  /* バイト列が連続しているか */
  std::cout << " continuous:" <<
      (mat.isContinuous() ? "true" : "false") << ")" << std::endl;

  /* matの中身 */
  std::cout << mat << std::endl;
  return;
}

/*********************************************/
/* format                                    */
/*   C++20じゃないからformat関数を自作した。 */
/*********************************************/
template <typename ... Args>
std::string format(const std::string& fmt, Args ... args ) {
    size_t len = std::snprintf( nullptr, 0, fmt.c_str(), args ... );
    std::vector<char> buf(len + 1);
    std::snprintf(&buf[0], len + 1, fmt.c_str(), args ... );
    return std::string(&buf[0], &buf[0] + len);
}
