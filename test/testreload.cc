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
  system("cp ~/work/test/models/10000/0_config.json.original ~/work/test/models/10000/0_config.json");
  system("cat ~/work/test/models/10000/0_config.json| awk 'NR==58'");
  try{
     int init_result = initEnv("/home/ubuntu/work/test", (char*) "10000", 1, 0);
     printf("Init result: %d\n", init_result);
  }catch(char *){
    
  }

  
  string s = "./0-20220927111223546-10861.jpg";
 for(int j=0; j<1; j++){
 for (int i = 0; i < 1; i++)

 {
  Mat m2;
  m2 = imread(s);
  printf("file = %s \n", s.c_str());
  printf("rows=%d \n", m2.rows);
  printf("element type=%d \n", m2.type());
  printf("channels=%d \n", m2.channels());
  printf("depth=%d \n", m2.depth());
  string  halcon_score = nievaluate("resnet18-256-384x512_20200831_095306_0.onnx", m2);
  printf("tichu result is %s \n", halcon_score.c_str());
 }
 usleep(1000);
 }


 
  system("cp ~/work/test/models/10000/0_config.json.test ~/work/test/models/10000/0_config.json");
  system("cat ~/work/test/models/10000/0_config.json| awk 'NR==58'");
  int init_result = initEnv("/home/ubuntu/work/test", (char*) "10000", 1, 0);
  printf("Init result: %d \n", init_result);
  Mat img = imread(s);
  string evaluate_result = nievaluate("resnet18-256-384x512_20200831_095306_0.onnx",img);
  printf("Reload config, tichu result is %s \n", evaluate_result);
 return 0;
}
