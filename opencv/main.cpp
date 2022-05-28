#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
double getDistance(const cv::Point2f &pt1, cv::Point2f &pt2) {
    double distance = std::sqrt(std::pow((pt1.x - pt2.x),2) + std::pow((pt1.y - pt2.y),2));
    return distance;
}
int main(){
    cv::Mat src = cv::imread("/home/dqs/桌面/Assessment/opencv/assets/src.jpg");
    cv::Mat gray,binary,median_img,src_open;
    cv::cvtColor(src,gray,cv::COLOR_BGR2GRAY);
    cv::threshold(gray,binary,0,255,cv::THRESH_OTSU);
    cv::medianBlur(binary,median_img,7);
    cv::Mat element=getStructuringElement(cv::MORPH_RECT,cv::Size(10,10));
    cv::morphologyEx(median_img,src_open,cv::MORPH_OPEN,element);//开运算
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hieracy;
    cv::findContours(src_open,contours,hieracy,cv::RETR_TREE,cv::CHAIN_APPROX_NONE);
    cv::Mat canvas = cv::Mat::zeros(cv::Size(src.cols,src.rows),CV_8UC3);
    std::vector<std::array<cv::Point2f, 4>> results;
    for(int i=0;i+1;i=hieracy[i][0]){//遍历轮廓
        std::vector<cv::Point> result;
        cv::approxPolyDP(contours[i],result,cv::arcLength(contours[i],true)*0.02,true);//多边形拟合
        int num=result.size();
        cv::RotatedRect rRect = minAreaRect(contours[i]);

        if(num == 4  && (abs(rRect.size.height/rRect.size.width)-1.0*17/9)<=0.1 &&rRect.size.height*rRect.size.width>10000 ){
            std::array<cv::Point2f,4> pts;
            for(int i=0;i<=4;i++) pts[i]=result[i];
            cv::Point2f center = (pts[0]+pts[1]+pts[2]+pts[3])/4;
            bool clockwise = (pts[0]-center).cross(pts[1]-pts[0]) < 0;  // 是否逆时针
            if (clockwise) {
                auto length1 = getDistance(pts[0], pts[1]);
                auto length2 = getDistance(pts[0], pts[3]);
                if (length1 < length2) {
                    auto tmp = pts[0];
                    for (int i = 3; i > 0; --i) {
                        pts[(i+1)%4] = pts[i];
                    }
                    pts[1] = tmp;
                }
            } else {
                auto length1 = getDistance(pts[0], pts[1]);
                auto length2 = getDistance(pts[0], pts[3]);
                if (length1 > length2) {
                    auto tmp = pts[0];
                    for (int i = 1; i < 4; ++i) {
                        pts[i-1] = pts[i];
                    }
                    pts[3] = tmp;
                }
                auto tmp = pts[1];
                pts[1] = pts[3];
                pts[3] = tmp;
                clockwise = true;
            }
            for (int i = 0; i < 4; ++i) {
                cv::Point2i pt1 = pts[i];
                cv::Point2i pt2 = pts[(i+1)%4];
                cv::line(src, pt1, pt2, cv::Scalar(255, 0, 0), 2);
            }
            results.push_back(pts);
        }
            

    }
    std::cout<<results.size();
    cv::Mat dst;
    cv::Size size(300,500);
    std::array<cv::Point2f,4>dst_pts = {cv::Point2f(0,0), cv::Point2f(0,size.height), cv::Point2f(size.width,size.height), cv::Point2f(size.width,0.)};
    for (auto it = results.begin();it!=results.end();it++){
        cv::Mat trans = cv::getPerspectiveTransform(*it,dst_pts);
        cv::warpPerspective(src,dst,trans,size,cv::INTER_CUBIC);
        cv::imshow("src",dst);
        cv::waitKey();
    }
    
    //cv::imwrite("/home/dqs/桌面/Assessment/opencv/assets/1.jpg",src);
    cv::waitKey();
}
