#include <iostream>
#include "global.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Frame {
    public:
        Mat img;
        Mat mask;
        vector<KeyPoint> kps;
        Rect miniMap;

    Frame() {
    }

    void start() {
        extrcat_kps();
        draw_kps();
        draw_minimap();
    }

    void draw_minimap() {
        miniMap = Rect(global::start_W, global::start_H, global::real_W - global::start_W, global::real_H - global::start_H);
        rectangle(mask, miniMap, Scalar(0,0,255), 3);
    }

    void draw_kps() {
        for(const auto kp: kps) {
            circle(
                mask,
                kp.pt,
                2,
                Scalar(0,255,0),
                -1,
                2,
                0);
        }
    }

    void extrcat_kps() {
        vector<Point2f> feat;
        Mat img_gray;
        cvtColor(img, img_gray, COLOR_BGR2GRAY);
        goodFeaturesToTrack(
            img_gray, // in array
            feat, // out array
            10000, // max corners
            0.01, // quality level
            10, // min distance
            Mat(), // mask
            3, // block size
            false, // harris detector?
            0.04 // k
        );

        for(const auto f : feat) {
            KeyPoint k(f, 1.f);
            kps.push_back(k);
        }
    }
};
