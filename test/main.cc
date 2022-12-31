#include<string>
#include <cv.h>
#include <opencv2/opencv.hpp>
//#include <opencv.hpp>
#include <unistd.h>

using namespace std;
using namespace cv;
int nreleaseEngines();
int initEnv(string base_dir, char* prd_id,int logOupt,int computeMode);
string  nievaluate(const string md_prd_id, cv::Mat &img);


int main()
{
  int i =nreleaseEngines();
  int ii= initEnv("/home/ubuntu/work","10000",1,0);



for(int i=0;i<1;i++)
{

  Mat m2;
  m2 = imread("./images/0_p/0-20221101102944915314-4.jpg");


 char* a;
string  halcon_score = nievaluate("resnet18-256-384x512_20200831_095306_0.onnx", m2);
printf("111233333333333333%s",halcon_score.c_str());
}


}
