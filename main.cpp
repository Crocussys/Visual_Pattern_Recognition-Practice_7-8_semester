#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

Mat frame;
vector<Point> ps;
int b1, g1, r1, b2, g2, r2;

static void on_trackbar(int, void*){
    Mat new_frame, edges;
    Scalar bottom(b1, g1, r1);
    Scalar top(b2, g2, r2);

    cvtColor(frame, new_frame, COLOR_BGR2HSV);
    imshow("cvtColor", new_frame);

    inRange(new_frame, bottom, top, new_frame);
    imshow("inRange", new_frame);
    imshow("bottom line", Mat(50, 50, CV_8UC3, bottom));
    imshow("top line", Mat(50, 50, CV_8UC3, top));

    Canny(new_frame, edges, 0, 0);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    vector<Point> approx;

    findContours(edges, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

    for(size_t i = 0; i < contours.size(); i++){
        approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);
        if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
            continue;
        drawContours(frame, contours, (int)i, Scalar(0, 255, 0), 2, LINE_8, hierarchy, 0);
        Moments m = moments(approx, true);
        Point p(m.m10/m.m00, m.m01/m.m00);
        ps.push_back(p);
        // if(approx.size() > 3 && approx.size() < 7){
        //     putText(frame, "Rectangle", p, 0, 0.5, Scalar(0, 255, 0));
        // }
    }
    for(int i = 0; i < ps.size(); i++){
        circle(frame, ps[i], 3, Scalar(0, 255, 0), -1);
    }
    imshow("Frame", frame);
}

int main()
{
    VideoCapture cap("../Visual_Pattern_Recognition-Practice_7-8_semester/video1.mp4");
    if(!cap.isOpened()){
        cout << "Error" << endl;
        return -1;
    }
    VideoWriter out("../Visual_Pattern_Recognition-Practice_7-8_semester/output.mp4", cap.get(CAP_PROP_FOURCC), cap.get(CAP_PROP_FPS), Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)));
    namedWindow("Frame", WINDOW_AUTOSIZE);
    b1 = 95;
    g1 = 12;
    r1 = 138;
    b2 = 255;
    g2 = 50;
    r2 = 255;
    createTrackbar("b1", "Frame", &b1, 255, on_trackbar);
    createTrackbar("g1", "Frame", &g1, 255, on_trackbar);
    createTrackbar("r1", "Frame", &r1, 255, on_trackbar);
    createTrackbar("b2", "Frame", &b2, 255, on_trackbar);
    createTrackbar("g2", "Frame", &g2, 255, on_trackbar);
    createTrackbar("r2", "Frame", &r2, 255, on_trackbar);
    bool start = true;
    while(true){
        cap >> frame;
        if(frame.empty()) break;
        on_trackbar(b1, 0);
        out.write(frame);
        char c = (char) waitKey(30);
        if (c == 27) break;
        if (c == 32 || start){
            while(true){
                char c = (char) waitKey(30);
                if (c == 32) break;
            }
            start = false;
        }
    }
    cap.release();
    out.release();
    destroyAllWindows();
    return 0;
}
