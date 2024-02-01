#include <opencv2/opencv.hpp>
#include "../src/MainProcess.h"

void printMatInfo(const cv::Mat& mat, const std::string &str = "");
template <typename ... Args> std::string format(const std::string& fmt, Args ... args );
cv::Point CrossLineLine(const cv::Point &p0, const cv::Point &p1, const cv::Point &p2, const cv::Point &p3);

int main(int argc, char *argv[]) {
  std::string file = "../../QR_sample6.png";
  /* 画像の読み込み */
  cv::Mat inputImage = cv::imread(file);
  cv::Mat outputImage;
  inputImage.copyTo(outputImage);

  /* 画像が正しく読み込まれたかを確認 */
  if (inputImage.empty()) {
    std::cerr << "画像を読み込めませんでした" << std::endl;
    return -1;
  }

  cv::QRCodeDetector qrcode_detector;

  /* 計測開始 */
  std::chrono::steady_clock::time_point stime = std::chrono::steady_clock::now();

  std::vector<std::string> decoded_info;
  std::vector<cv::Point> points;
//  std::vector<cv::Mat> straight_codes;  /* 第4引数はQRコードのビットパターンが設定される */

  /* 検出 & デコード */
  qrcode_detector.detectAndDecodeMulti(inputImage, decoded_info, points/*,straight_codes*/);
//for(int lpct = 0; lpct < straight_codes.size(); lpct++)
//  printMatInfo(straight_codes[lpct], format("straight_codes[%d]", lpct));

  /* 結果描画 */
  for(size_t lpct = 0; lpct < decoded_info.size(); lpct++) {
    /* 枠線描画 */
    cv::line(outputImage, cv::Point(points[lpct*4+0].x, points[lpct*4+0].y), cv::Point(points[lpct*4+1].x, points[lpct*4+1].y), cv::Scalar(0,0,255), 5);
    cv::line(outputImage, cv::Point(points[lpct*4+1].x, points[lpct*4+1].y), cv::Point(points[lpct*4+2].x, points[lpct*4+2].y), cv::Scalar(0,0,255), 5);
    cv::line(outputImage, cv::Point(points[lpct*4+2].x, points[lpct*4+2].y), cv::Point(points[lpct*4+3].x, points[lpct*4+3].y), cv::Scalar(0,0,255), 5);
    cv::line(outputImage, cv::Point(points[lpct*4+3].x, points[lpct*4+3].y), cv::Point(points[lpct*4+0].x, points[lpct*4+0].y), cv::Scalar(0,0,255), 5);
    /* 頂点描画 */
    cv::circle(outputImage, cv::Point(points[lpct*4+0].x, points[lpct*4+0].y), 10, cv::Scalar(255,   0, 255));
    cv::circle(outputImage, cv::Point(points[lpct*4+1].x, points[lpct*4+1].y), 10, cv::Scalar(255, 255,   0));
    cv::circle(outputImage, cv::Point(points[lpct*4+2].x, points[lpct*4+2].y), 10, cv::Scalar(  0, 255, 255));
    cv::circle(outputImage, cv::Point(points[lpct*4+3].x, points[lpct*4+3].y), 10, cv::Scalar(  0, 255,   0));
    /* 交点描画 */
    cv::Point center = CrossLineLine(points[lpct*4+0], points[lpct*4+1], points[lpct*4+2], points[lpct*4+3]);
    cv::circle(outputImage, center, 10, cv::Scalar( 0, 0,255), -1);
    /* 交点補助線描画 */
    cv::line(outputImage, cv::Point(points[lpct*4+0].x, points[lpct*4+0].y), cv::Point(points[lpct*4+2].x, points[lpct*4+2].y), cv::Scalar(255,7,85));
    cv::line(outputImage, cv::Point(points[lpct*4+1].x, points[lpct*4+1].y), cv::Point(points[lpct*4+3].x, points[lpct*4+3].y), cv::Scalar(255,7,85));
    /* 頂点番号描画 */
    cv::putText(outputImage, "0", cv::Point(points[lpct*4+0].x, points[lpct*4+0].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,   0, 255));
    cv::putText(outputImage, "1", cv::Point(points[lpct*4+1].x, points[lpct*4+1].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255,   0));
    cv::putText(outputImage, "2", cv::Point(points[lpct*4+2].x, points[lpct*4+2].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(  0, 255, 255));
    cv::putText(outputImage, "3", cv::Point(points[lpct*4+3].x, points[lpct*4+3].y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(  0, 255,   0));
    /* URL文字列描画 */
    cv::putText(outputImage, decoded_info[lpct], cv::Point(points[lpct*4+3].x, points[lpct*4+3].y), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(  0, 255,   0), 3, 8, false);
  }

 /* 計測終了 */
  std::chrono::steady_clock::time_point etime = std::chrono::steady_clock::now();
  long elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(etime - stime).count() % 1000;
  std::cout << "elapsedtime: " << elapsedtime << "ms." << std::endl;

  /* Output file. */
  int path_i = file.find_last_of("/") + 1;
  int ext_i = file.find_last_of(".");
  std::string pathname = file.substr(0, path_i);
  std::string filename = file.substr(path_i, ext_i-path_i);
  std::string extname = file.substr(ext_i, file.size()-ext_i);
  std::string outfile = pathname + "out" + filename + extname;
  cv::imwrite(outfile, outputImage);

  return 0;
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