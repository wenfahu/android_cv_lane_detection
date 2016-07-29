//
// Created by wenfahu on 16-7-26.
//
#include "com_example_wenfahu_lane_droid_MainActivity.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "linefinder.h"
#include <vector>
#include <string>
#include <android/log.h>

#define PI 3.1415926
#define MAX_KERNEL_LENGTH 5

#define  LOG_TAG    "native"

#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


using namespace cv;
using namespace std;


JNIEXPORT void JNICALL Java_com_example_wenfahu_lane_1droid_MainActivity_process
  (JNIEnv *, jobject, jlong matAddr){

    Mat frame = *(Mat*)matAddr;
    // Rect roi(0,frame.cols/3,frame.cols-1,frame.rows - frame.cols/3);
    // Mat image = frame(roi);
    int houghVote = 200;
    if(!frame.data){
        LOGD("NO IMAGE DATA");
        exit(1);
    }
    Mat image;
    for(int i=1; i<MAX_KERNEL_LENGTH; i=i+2){
        GaussianBlur(frame, image, Size(i, i), 0, 0);
    }
    Mat contours;
    Canny(image, contours, 150, 350);
    Mat contoursInv;
    threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);
    vector<Vec2f> lines;
    if (houghVote < 1 or lines.size() > 2){
        houghVote = 200;
    } else {
        houghVote += 25;
    }
    while(lines.size() < 5 && houghVote > 0){
        HoughLines(contours, lines, 5, 1.5*PI/180, houghVote);
        houghVote -= 5;
    }
    // cout<<"houghVote: " << houghVote <<endl;
    Mat result(contours.rows, contours.cols, CV_8U, Scalar(255));
    image.copyTo(result);
    vector<Vec2f>::const_iterator it = lines.begin();
    Mat hough(image.size(), CV_8U, Scalar(0));
    while(it != lines.end()){
        float rho = (*it)[0];
        float theta = (*it)[1];
        if(theta > 0.52 && theta < 1.48 || theta < 2.61 && theta > 1.66){
            Point pt1(rho/cos(theta), 0); // point of intersection of the line with first row
            Point pt2((rho-result.rows*sin(theta))/cos(theta),result.rows); // point of interseaction of the line with last row
            line(result, pt1, pt2, Scalar(255), 8);
            line(hough, pt1, pt2, Scalar(255), 8);
        }
        // cout<<"line : (" << rho <<", " <<theta<<")"<<endl;
        ++it;
    }

    LineFinder ld;
    ld.setLineLengthAndGap(60, 10);
    ld.setMinVote(4);

    vector<Vec4i> li = ld.findLines(contours);
    Mat houghP(image.size(), CV_8U, Scalar(0));
    ld.setShift(0);
    ld.drawDetectedLines(houghP);

    bitwise_and(houghP, hough, houghP); // bitwise AND of the two hough images
    Mat houghPinv(image.size(), CV_8U, Scalar(0));
    Mat dst(image.size(), CV_8U, Scalar(0));
    threshold(houghP, houghPinv, 150, 255, THRESH_BINARY_INV);

    Canny(houghPinv, contours, 160, 350);
    li = ld.findLines(contours);

    ld.setLineLengthAndGap(5,2);
    ld.setMinVote(1);
    //ld.setShift(frame.cols/3);
    ld.setShift(0);
    ld.drawDetectedLines(frame);
  }

JNIEXPORT void JNICALL Java_com_example_wenfahu_lane_1droid_MainActivity_processOpt
  (JNIEnv *, jobject, jlong matAddr){
    Mat frame = *(Mat*)matAddr;
    if(!frame.data){
        LOGE("read image no data");
        exit(1);
    }
    Mat image;
    for(int i=1; i<MAX_KERNEL_LENGTH; i=i+2){
        GaussianBlur(frame, image, Size(i, i), 0, 0);
    }

    Mat contours;
    Canny(image, contours, 100, 350);

    LineFinder ld;
    ld.setLineLengthAndGap(100 , 20);
    ld.setShift(0);
    ld.setMinVote(80);

    vector<Vec4i> lines = ld.findLines(contours);

    vector<Vec4i>::const_iterator it = lines.begin();
    while(it != lines.end()){
        Point pt1((*it)[0], (*it)[1]);
        Point pt2((*it)[2], (*it)[3]);
        float theta = abs(atan2(pt1.y - pt2.y, pt1.x - pt2.x));
        if((theta > 0.35 && theta < 1.48) || (theta < 2.79 && theta > 1.66)){
            line(frame, pt1, pt2, Scalar(255), 8);
            // cout<<"line : (" << pt1 <<", " <<pt2<<")"<<endl;
        }
        ++it;
    }
  }
