#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Frame {
    public:
        Mat img;
        Mat mask;
        vector<KeyPoint> kps;

    Frame() {
        extrcat_kps();
    }

    void extrcat_kps() {
        vector<Point2f> feat;
        Mat img_gray;
        //cvtColor(img, img_gray, COLOR_BGR2GRAY);
        goodFeaturesToTrack(
            img, // in array
            feat, // out array
            10000, // max corners
            0.01, // quality level
            10, // min distance
            Mat(), // mask
            3, // block size
            false, // harris detector?
            0.04 // k
        );
        //for(const auto f : feat) {
            //KeyPoint k(f, 1.f);
            //kps.push_back(k);
        //}
    }
};
