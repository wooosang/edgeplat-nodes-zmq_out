#include <string>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.hpp>
#include <stdio.h>
#include <sys/dir.h>
#include <dirent.h>
#include <sys/stat.h>
#include <jsoncpp/json/json.h>

using namespace cv;
using namespace std;

/**
sudo apt-get install libjsoncpp-dev
*/

int initEnv(string base_dir, char* prd_id,int logOut,int computeMode);
string nievaluate(const string md_prd_id, cv::Mat &img);

#define MAX_PATH_LEN (256)

char* basepath = (char*) "/home/ubuntu/work/images/test/halcon";

void evaluate(char* filepath, char* filename, const char* label, int face){
   Mat mat;
   mat = imread(filepath);
   char model[100];
   sprintf(model, "resnet18-256-384x512_20200831_095306_%d.onnx", face);
   printf("Choose model %s\n",model);
   string  evaluate_result = nievaluate( model, mat);
   printf("tichu result is %s \n", evaluate_result.c_str());
   Json::Reader reader;
   Json::Value value;
   if (!reader.parse(evaluate_result.c_str(), value)) //解析json内容
        return;
   int isTichu = value["isTichu"].asInt();
   printf("Lable: %s, tichu %d\n", label, isTichu);
   printf("Label is Ok: %d\n", strcmp("OK",label));
   char move_to_path[100];
   if (isTichu == 1 && strcmp("NG", label) == 0)
     return;
   if(isTichu == 0 && strcmp("OK", label) == 0)
     return;

   if(isTichu == 0)   
   {
     sprintf(move_to_path, "%s/%s/%d", basepath, (char*) "FP", face);
   }else{
     sprintf(move_to_path, "%s/%s/%d", basepath, (char*) "FN", face);
   }
   char newFile[100];
//   printf("Copy to path: %s\n", move_to_path);
   sprintf(newFile, "%s/%s", move_to_path, filename);
   printf("Copy to %s \n", newFile);
   std::ifstream  src(filepath, std::ios::binary);
    std::ofstream  dst(newFile,   std::ios::binary);
    dst << src.rdbuf();
//   if (copy("", ""))
//   if (copy(filepath, newFile))
//     printf("Copy image failed!\n");
//   else
//     printf("Copy image succeed!\n");
}

static void trave_dir(char* path,const char* label, int face) {
    DIR *d = NULL;
    struct dirent *dp = NULL; /* readdir函数的返回值就存放在这个结构体中 */
    struct stat st;    
    char p[MAX_PATH_LEN] = {0};
    
    if(stat(path, &st) < 0 || !S_ISDIR(st.st_mode)) {
        printf("invalid path: %s\n", path);
        return;
    }

    if(!(d = opendir(path))) {
        printf("opendir[%s] error: %m\n", path);
        return;
    }

    while((dp = readdir(d)) != NULL) {
        /* 把当前目录.，上一级目录..及隐藏文件都去掉，避免死循环遍历目录 */
        if((!strncmp(dp->d_name, ".", 1)) || (!strncmp(dp->d_name, "..", 2)))
            continue;
        snprintf(p, sizeof(p) - 1, "%s/%s", path, dp->d_name);
        stat(p, &st);
        if(!S_ISDIR(st.st_mode)) {
	    char fullfilename[100];
            sprintf(fullfilename, "%s/%s", path, dp->d_name);
	    printf("Ready to evaluate file: %s\n",fullfilename);
	    evaluate(fullfilename, dp->d_name, label, face);
        } else {
            printf("dir: %s/%s/\n", path, dp->d_name);
            trave_dir(p, label, face);
        }
    }
    closedir(d);

    return;
}


int main()
{
  try{
    initEnv("/home/ubuntu/work", (char*) "10000.uac", 1, 0);
  }catch(char *){
    
  }
  string labels[2] = {"NG"};
//  string labels[2] = {"OK","NG"};

//  string basepath = "/home/ubuntu/work/images/samples";
  string basepath = "/home/ubuntu/work/images/test/dataset";
  for(auto label : labels){
    //string label = labels[i];
    for(int face = 4; face < 5; face++){
      char samplepath[100];
      sprintf(samplepath,  "%s/%s/%d", basepath.c_str(), label.c_str(), face);
      printf("Scan %s\n", samplepath);
      trave_dir(samplepath, label.c_str(), face);
    }
  }
 return 0;
}
