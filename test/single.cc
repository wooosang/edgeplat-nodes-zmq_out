#include <string>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.hpp>


using namespace cv;
using namespace std;


int initEnv(string base_dir, char* prd_id,int logOut,int computeMode);
int nievaluate(const string md_prd_id, cv::Mat &img);

int main()
{

 //   int ii =  loadEngine("33223");   

  try{
  initEnv("/home/ubuntu/work", "10000",1,0);
  //initEnv("/home/ubuntu/work", "10000", 1, 0);
  }catch(char *){
    
  }


  string s = "/home/ubuntu/work/test/0/0-1662877459-559.jpg";
  Mat m2;
  m2 = imread(s);
  printf("ss=%d \n", m2.rows);
  int  halcon_score = nievaluate("resnet18-256-384x512_20200831_095306_0.onnx", m2);
  printf("tichu result is %d \n", halcon_score);

 return 1;
 // return estimateResult;
}
