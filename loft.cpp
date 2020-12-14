#include <iostream>
#include <stdlib.h>
#include "opencv2/opencv.hpp"


#include "frame.cpp"
#include "helper.cpp"

using namespace std;
using namespace cv;


int main(int argc, char ** argv) {
    //if(argc < 2) {
        //cout << "Usage: ./loft <path/to/mp4>" << endl;
        //return -1;
    //}
    //VideoCapture cap(argv[1]);
    VideoCapture cap("vod.mp4");

    /*
     * The next scope will resize the frames if the frame
     * is bigger than my global::max_W.
     */
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

    cap.set(CAP_PROP_POS_FRAMES, global::start_frame); // Start at the correct location

    cout << "Num Frames: " << cap.get(CAP_PROP_FRAME_COUNT) << endl;

    vector<Frame> frames; // This full list of frames

    for(int i = 0; i < global::num_frames; i++) {
        cout << "** Processing frame " << i << " **" << endl;
        Frame f; // frame obj to store
        cap >> f.img; // grab frame
        f.mask = f.img;
        f.start(); // Starts extracting the kps and everything magical
        frames.push_back(f);
    }

    for(size_t i = 1; i < frames.size(); i++) {
        // matches is NOT a vector of points... It's a vetor where x = queryIdx and y = trainIdx. I use this type here because it is easy not because it is good.
        vector<Point2f> matches = match_frames(frames.at(i-1), frames.at(i), i-1);
        frames.at(i).draw_matches(matches);

    }


    /*
     * Start rendering process.
     * @TODO This can move this into the loop above if we really are having problems with speed.
     */
    for(int i = 0; i < global::num_frames; i++){
        cout << "Showing frame: " << i << endl;

        // If you want to see full frame uncomment the next 3 lines:
        //Mat resized;
        //resize(frames.at(i).mask, resized, Size(global::W, global::H));
        //imshow("Frame", resized);

        imshow("miniMap", frames.at(i).miniMapMask); // Renders the minimap mask

        char c=(char)waitKey(0);
        if(c==27)
            break;
    }
    destroyAllWindows();

    return 0;
}
