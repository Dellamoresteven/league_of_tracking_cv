#include <iostream>
#include <stdlib.h>
#include "opencv2/opencv.hpp"


#include "frame.cpp"

using namespace std;
using namespace cv;


int main(int argc, char ** argv) {
    //if(argc < 2) {
        //cout << "Usage: ./loft <path/to/mp4>" << endl;
        //return -1;
    //}
    //VideoCapture cap(argv[1]);
    VideoCapture cap("vod.mp4");
    {
        using namespace global;
        W = cap.get(CAP_PROP_FRAME_WIDTH);
        H = cap.get(CAP_PROP_FRAME_HEIGHT);
        if(W > max_W) {
            float d = 1.0 * max_W / W;
            H = int(d * H);
            W = max_W;
        }
        real_W = cap.get(CAP_PROP_FRAME_WIDTH);
        real_H = cap.get(CAP_PROP_FRAME_HEIGHT);
    }

    cap.set(CAP_PROP_POS_FRAMES, global::start_frame);

    cout << "Num Frames: " << cap.get(CAP_PROP_FRAME_COUNT) << endl;

    vector<Frame> frames;

    for(int i = 0; i < global::num_frames; i++) {
        cout << "** Processing frame " << i << " **" << endl;
        Frame f;
        cap >> f.img;
        f.mask = f.img;
        f.start();
        frames.push_back(f);
    }

    for(int i = 0; i < global::num_frames; i++){
        cout << "Showing frame: " << i << endl;
        Mat resized;
        resize(frames.at(i).mask, resized, Size(global::W, global::H));
        imshow("Frame", resized);
        char c=(char)waitKey(0);
        if(c==27)
            break;
    }
    destroyAllWindows();

    return 0;
}
