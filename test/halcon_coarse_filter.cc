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

void evaluate(char* filepath, char* filename,int face){
   Mat mat;
   mat = imread(filepath);
   char model[100];
   sprintf(model, "resnet18-256-384x512_20200831_095306_%d.onnx", face);
   printf("Choose model %s\n",model);
   string  halcon_score = nievaluate( model, mat);
   printf("tichu result is %s \n", halcon_score.c_str());
   Json::Reader reader;
   Json::Value value;
   if (!reader.parse(halcon_score.c_str(), value)) //解析json内容
        return;
   int isTichu = value["isTichu"].asInt();
   printf("Tichu %d\n", isTichu);
   char* move_to_path;
   if (isTichu == 1){
     printf(">>>>Move image.\n"); 
     move_to_path = (char*) "/home/ubuntu/work/images/test/halcon/NG";
   }else{
     move_to_path = (char*) "/home/ubuntu/work/images/test/halcon/OK";
   }
   char newFile[100];
   sprintf(newFile, "%s/%d/%s", move_to_path,face, filename);
   printf("Move to %s \n", newFile);
   if (rename(filepath, newFile))
     printf("Move image failed!\n");
   else
     printf("Move image succeed!\n");
}

static void trave_dir(char* path, int face) {
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
	    evaluate(fullfilename, dp->d_name, face);
        } else {
            printf("dir: %s/%s/\n", path, dp->d_name);
            trave_dir(p, face);
        }
    }
    closedir(d);

    return;
}


int main()
{
  try{
    initEnv("/home/ubuntu/work", (char*) "10000",1,0);
  }catch(char *){
    
  }

  for(int face = 0; face < 6; face++){
    if(face != 1)
      continue;
    char samplepath[100];
    sprintf(samplepath,  "/home/ubuntu/work/images/defects/%d", face);
    printf("Scan %s\n", samplepath);
    trave_dir(samplepath, face);
  }
 return 0;
}
