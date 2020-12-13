#include <iostream>
#include <stdlib.h>
#include "opencv2/opencv.hpp"


#include "frame.cpp"

using namespace std;
using namespace cv;

namespace global {
    int W = 0;
    int H = 0;
    int start_W = 500;
    int start_H = 500;
    int max_W = 1500;
    int start_frame = 110000;
    int num_frames = 500;
}

int main(int argc, char ** argv) {
    if(argc < 2) {
        cout << "Usage: ./loft <path/to/mp4>" << endl;
        return -1;
    }
    VideoCapture cap(argv[1]);
    {
        using namespace global;
        W = cap.get(CAP_PROP_FRAME_WIDTH);
        H = cap.get(CAP_PROP_FRAME_HEIGHT);
        if(cap.get(CAP_PROP_FRAME_WIDTH) > max_W) {
            float d = max_W / W;
            H = int(d * H);
            W = max_W;
        }
    }

    cap.set(CAP_PROP_POS_FRAMES, global::start_frame);

    cout << "Num Frames: " << cap.get(CAP_PROP_FRAME_COUNT) << endl;

    vector<Frame> frames;

    for(int i = 0; i < global::num_frames; i++) {
        cout << "** Processing frame " << i << " **" << endl;
        Frame f;
        cap >> f.img;
        frames.push_back(f);
    }

    for(int i = 0; i < global::num_frames; i++){
        cout << "Showing frame: " << i << endl;
        imshow("Frame", frames.at(i).img);
        char c=(char)waitKey(0);
        if(c==27)
            break;
    }
    destroyAllWindows();

    return 0;
}
