

vector<Point2f> match_frames(Frame f1, Frame f2, int frame_num) {
    cout << "** Matching Frames: " << frame_num << ":" << frame_num + 1 << endl;
    BFMatcher bf(NORM_HAMMING, false);

    vector<vector<DMatch>> matches;
    bf.knnMatch(f1.des, f2.des, matches, 2);

    vector<Point2f> ret;

    for(const auto m : matches) {
      if(m.at(0).distance < 0.75*m.at(1).distance) {
        Point2f pt(m.at(0).queryIdx, m.at(0).trainIdx);
        if(m.at(0).distance < 32) {
          ret.push_back(pt);
        }
      }
    }

    cout << "kps matched: " << ret.size() << " / " << max(f1.kps.size(), f2.kps.size()) << endl;

    return ret;
}
