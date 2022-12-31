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
  int ii= initEnv("/home/ubuntu/work","10000",1,1);



for(int i=0;i<3;i++)
{

  Mat m2;
  m2 = imread("./images/4-20221108165050587-1323.jpg");


 char* a;
string  halcon_score = nievaluate("resnet18-256-384x512_20200831_095306_4.onnx", m2);
cout << halcon_score << std::endl;
//printf("111233333333333333%s",halcon_score.c_str());
}


}
