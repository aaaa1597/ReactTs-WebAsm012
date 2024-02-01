#include <opencv2/opencv.hpp>
#include "../src/MainProcess.h"

int main(int argc, char *argv[]) {
  /* 画像の読み込み */
  std::string infilepath = "../../QR_sample6.png";
  cv::Mat inputImage = cv::imread(infilepath);

  /* 画像が正しく読み込まれたかを確認 */
  if (inputImage.empty()) {
    std::cerr << "画像を読み込めませんでした" << std::endl;
    return -1;
  }

  /* 計測開始 */
  std::chrono::steady_clock::time_point stime = std::chrono::steady_clock::now();

  cv::Mat outputImage;
  bool ret = detect(inputImage, outputImage);

  /* 計測終了 */
  std::chrono::steady_clock::time_point etime = std::chrono::steady_clock::now();
  long elapsedtime = std::chrono::duration_cast<std::chrono::milliseconds>(etime - stime).count() % 1000;
  std::cout << "elapsedtime: " << elapsedtime << "ms." << std::endl;

  /* Output file. */
  int path_i = infilepath.find_last_of("/") + 1;
  int ext_i = infilepath.find_last_of(".");
  std::string pathname= infilepath.substr(0, path_i);
  std::string filename= infilepath.substr(path_i, ext_i-path_i);
  std::string extname = infilepath.substr(ext_i, infilepath.size()-ext_i);
  std::string outfile = pathname + "out" + filename + extname;
  cv::imwrite(outfile, outputImage);

  return 0;
}