#include <opencv2/opencv.hpp>
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"
#include <iostream>

using namespace std;
using namespace cv;

Mat icon, Background, Background_buff, Background_origin;
Mat src_pic;

int MenuSelect = 0;
int x_1, x_2, x_3 =0;
int y_1, y_2, y_3= 0;
int drawing_point = false;
int point_number = 0;

void InverseIcon(int MenuSelect)
{
    int sx, sy;
    sy = (MenuSelect - 1) / 4;
    sx = (MenuSelect - 1) % 4;
    Rect r(sx * 100, sy * 100, 95, 100);
    Mat D = icon(r);  
    bitwise_not(D, D);
    imshow("Menu", icon);
}

void onMouseBackground(int event, int x, int y, int flags, void* param)
{

    if (event == CV_EVENT_LBUTTONDOWN && MenuSelect == 3) {
        
        point_number += 1;

        switch (point_number) {
        case 1: {
            x_1 = x;
            y_1 = y;
            circle(Background_buff, Point(x_1, y_1), 5, Scalar(0, 0, 255), 5);
            putText(Background_buff, "1", Point(x_1, y_1), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2);
            imshow("Background", Background_buff);
        }break;

        case 2: {
            x_2 = x;
            y_2 = y;
            circle(Background_buff, Point(x_2, y_2), 5, Scalar(0, 0, 255), 5);
            putText(Background_buff, "2", Point(x_2, y_2), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2);
            imshow("Background", Background_buff);
        }break;

        case 3: {
            x_3 = x;
            y_3 = y;
            circle(Background_buff, Point(x_3, y_3), 5, Scalar(0, 0, 255), 5);
            putText(Background_buff, "3", Point(x_3, y_3), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2);
            imshow("Background", Background_buff);
            
        }break;
        }
        imshow("Background", Background_buff);
    }
}


void onMouseMenu(int event, int x, int y, int flags, void* param)
{
    OpenFileDialog* openFileDialog = new OpenFileDialog();
    SaveFileDialog* saveFileDialog = new SaveFileDialog();

    int sx, sy;
    if (event == EVENT_LBUTTONDOWN) {
        sx = x / 100; sy = y / 100;

        if (MenuSelect != 0) {
            InverseIcon(MenuSelect);
        }
        MenuSelect = sy * 4 + sx + 1;
        InverseIcon(MenuSelect);

        switch (MenuSelect) {
        case 1: if (openFileDialog->ShowDialog()) {
            Background = imread(openFileDialog->FileName);

            Background_buff = Background.clone();
            Background_origin = Background.clone();

            imshow("Background", Background_buff);
          
        }
              break;

        case 2: if (saveFileDialog->ShowDialog()) {
            imwrite(saveFileDialog->FileName, Background);
        }
              break;

        case 3: setMouseCallback("Background", onMouseBackground, &icon);
            point_number = 0;

            Background = Background_origin.clone();
            Background_buff = Background_origin.clone();

            imshow("Background", Background);
            break;

        case 4: break;

        case 5:  if (openFileDialog->ShowDialog()) {
            src_pic = imread(openFileDialog->FileName);
            imshow("src_pic", src_pic);

            Point2f srcTri[3], dstTri[3];
            Mat warp_mat(2, 3, CV_32FC1);

            srcTri[0] = Point2f(0, 0);
            srcTri[1] = Point2f(src_pic.cols - 1.0f, 0);
            srcTri[2] = Point2f(0, src_pic.rows - 1.0f);

            dstTri[0] = Point2f(x_1, y_1);
            dstTri[1] = Point2f(x_2, y_2);
            dstTri[2] = Point2f(x_3, y_3);

            warp_mat = getAffineTransform(srcTri, dstTri);
            warpAffine(src_pic, Background, warp_mat, Background.size(), INTER_LINEAR, BORDER_TRANSPARENT);
            imshow("Background", Background);
        }
              break;

        case 6: if (openFileDialog->ShowDialog()) {
            VideoCapture cap(openFileDialog->FileName);

            if (!cap.isOpened()) {
                cout << "동영상을 읽을 수 없음" << endl;
            }


            for (;;) {
                Mat frame;
                cap >> frame;
                if (frame.empty()) {
                    break;
                }
                if (waitKey(20) >= 0) break;

                Point2f srcTri[3], dstTri[3];
                Mat warp_mat(2, 3, CV_32FC1);

                srcTri[0] = Point2f(0, 0);
                srcTri[1] = Point2f(frame.cols - 1.0f, 0);
                srcTri[2] = Point2f(0, frame.rows - 1.0f);

                dstTri[0] = Point2f(x_1, y_1);
                dstTri[1] = Point2f(x_2, y_2);
                dstTri[2] = Point2f(x_3, y_3);

                warp_mat = getAffineTransform(srcTri, dstTri);
                warpAffine(frame, Background, warp_mat, Background.size(), INTER_LINEAR, BORDER_TRANSPARENT);
                imshow("Background", Background);
            }
        }
            break;

        case 7:VideoCapture cap(0);

            if (!cap.isOpened()) {
                cout << "동영상을 읽을 수 없음" << endl;
            }

            for (;;) {
                Mat frame;
                cap >> frame;
                
                if (waitKey(20) >= 0) break;

                Point2f srcTri[3], dstTri[3];
                Mat warp_mat(2, 3, CV_32FC1);

                srcTri[0] = Point2f(0, 0);
                srcTri[1] = Point2f(frame.cols - 1.0f, 0);
                srcTri[2] = Point2f(0, frame.rows - 1.0f);

                dstTri[0] = Point2f(x_1, y_1);
                dstTri[1] = Point2f(x_2, y_2);
                dstTri[2] = Point2f(x_3, y_3);

                warp_mat = getAffineTransform(srcTri, dstTri);
                warpAffine(frame, Background, warp_mat, Background.size(), INTER_LINEAR, BORDER_TRANSPARENT);
                imshow("Background", Background);
            } 
            break;
        }
        
        
        
    }
}

int main()
{
    icon = imread("MENU.jpg", IMREAD_COLOR);
    imshow("Menu", icon);


    setMouseCallback("Menu", onMouseMenu, &icon);
    setMouseCallback("Background", onMouseBackground, &icon);


    while (1) {
        if (waitKey(0) == 'q')
            break;
    }
    
    waitKey(0);
    return 1;
}
