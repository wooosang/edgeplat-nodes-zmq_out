#include <string>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.hpp>
#include <unistd.h>


using namespace cv;
using namespace std;


int initEnv(string base_dir, char* prd_id,int logOut,int computeMode);
string nievaluate(const string md_prd_id, cv::Mat &img);

int main()
{

 //   int ii =  loadEngine("33223");   

  try{
     initEnv("/home/ubuntu/work", "10000", 1, 0);
  }catch(char *){
    
  }

 for(int j=0; j<1; j++){
 for (int i = 0; i < 1; i++)

 {
//  string s = "/home/ubuntu/work/test/samples/1/" + to_string(i) + ".jpg";
  string s = "/home/ubuntu/work/edgeplat/defect_checker/test/0-20220927111223546-10861.jpg";
//  string s = "/home/ubuntu/work/edgeplat/defect_checker/test/4-20220920211416162-337.jpg";
  printf("file = %s \n", s.c_str());
  Mat m2;
  m2 = imread(s);
  printf("rows=%d \n", m2.rows);
  printf("element type=%d \n", m2.type());
  printf("channels=%d \n", m2.channels());
  printf("depth=%d \n", m2.depth());
  string  halcon_score = nievaluate("resnet18-256-384x512_20200831_095306_0.onnx", m2);
//  string  halcon_score = nievaluate("resnet18-256-384x512_20200831_095306_1.onnx", matGray);
  printf("tichu result is %s \n", halcon_score.c_str());
//  imwrite("./test.jpg", m2);
//  imwrite("./test.jpg", matGray);
 }
 usleep(1000);
 }
 return 1;
 // return estimateResult;
}
