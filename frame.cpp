#include <iostream>
#include "global.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Frame {
    public:
        Mat img; // full frame nothing on it
        Mat mask; // full frame with mask applied
        Mat miniMap; // only the minimap with no mask
        Mat miniMapMask; // only minimap with mask
        vector<KeyPoint> kps; // keypoints found in ONLY the minimap. (does not search whole image)
        Mat des; // descriptor for our kps
        Rect miniMapRect; // Bounding rect of minimap

    Frame() {
        // Get the bounding rect around the minimap from the global namespace
        miniMapRect = Rect(global::start_W, global::start_H, global::real_W - global::start_W, global::real_H - global::start_H);
    }

    void start() {
        /*
         * Here we are extracting the minimap from the real img. We do this so when we
         * start searching for kps we are only searching through the minimap, which is
         * much faster.
         */
        img(miniMapRect).copyTo(miniMap);
        miniMapMask = miniMap; // copying the real minimap to my mask

        extrcat_kps(); // get the kps out of the minimap
        //draw_kps(); // Draw the kps on the minimap
        draw_minimap(); // Only useful if you are drawing the entire img
        extract_des();
    }

    void extract_des() {
        auto orb = ORB::create();
        orb->compute(miniMap, kps, des); // compute the des for each kp
    }

    void draw_minimap() {
        rectangle(mask, miniMapRect, Scalar(0,0,255), 3); // draws a rect on the full image mask
    }

    void draw_kps() {
        /*
         * Draw circles for each kps that was found on ONLY the minimap
         */
        for(const auto kp: kps) {
            circle(
                miniMapMask,
                kp.pt,
                2,
                Scalar(0,255,0),
                -1,
                2,
                0);
        }
    }

    void draw_matches(vector<Point2f> matches) {
        for(const auto m: matches) {
            circle(
                miniMapMask,
                kps[m.x].pt,
                2,
                Scalar(0,255,0),
                -1,
                2,
                0);
        }
    }

    void extrcat_kps() {
        vector<Point2f> feat; // features vector
        Mat img_gray; // gray scale image so "goodFeaturesToTrack" can do its thing
        cvtColor(miniMap, img_gray, COLOR_BGR2GRAY); // convert minimap to grayscale
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

        /* 
         * Convert the features we found to keyPoints
         */
        for(const auto f : feat) {
            KeyPoint k(f, 1.f);
            kps.push_back(k);
        }
    }
};
