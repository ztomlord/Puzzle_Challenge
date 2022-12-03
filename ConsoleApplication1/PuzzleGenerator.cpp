#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;
using namespace cv;

RNG rng(12345);

bool debugVisuals = false;
String puzzlePath = "../Resources/";
String puzzleName = "jigsaw_5x5.JPG";

int main()
{
    //std::string image_path = samples::findFile("jigsaw_5x5.JPG");
    Mat img = imread(puzzlePath+puzzleName, IMREAD_GRAYSCALE);
    if (img.empty())
    {
        //std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    if (debugVisuals) { imshow("Original", img); }
    
    Mat inverted = 255 - img;
    
    if (debugVisuals) { imshow("Inverted", inverted); }

    Mat thresh;
    threshold(inverted, thresh, 125, 255, THRESH_BINARY);
    if (debugVisuals) { imshow("Thresholded", thresh); }

    Mat dilate;
    int closingSize = 1;
    Mat element = getStructuringElement(MORPH_RECT,Size(2 * closingSize + 1,2 * closingSize + 1),
        Point(closingSize, closingSize));
    morphologyEx(thresh, dilate, MORPH_DILATE, element, Point(-1, -1), 1);
    if (debugVisuals) { imshow("Closed", dilate); }
    
    //waitKey(0);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(dilate, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE);

    double aveSize = 0;
    for (int i = 0; i < contours.size(); i++)
    { aveSize += contours[i].size(); }
    aveSize = aveSize / (double)contours.size();

    //remove any unreasonably large contours (likely image edge)
    vector<vector<Point>> contoursFinal;
    for (int i = 0; i < contours.size(); i++)
    {
        if (contours[i].size() < aveSize * 2) 
        { 
            contoursFinal.push_back(contours[i]); 
        }
    }

    Mat drawing = Mat::zeros(dilate.size(), CV_8UC3);
    for (size_t i = 0; i < contoursFinal.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(drawing, contoursFinal, (int)i, color, 1, LINE_8);
    }
    cout << "Pieces: " << contoursFinal.size() << endl;
    if (debugVisuals) { imshow("Contours", drawing); }

    int windowOffset = 250;
    vector<Mat> puzzlePieces;
    //isolate the puzzle pieces into their own images and floodfill their interiors
    int padding = 5;
    for (int i = 0; i < contoursFinal.size(); i++)
    {
        int minX, minY, maxX, maxY;
        for (int ii = 0; ii < contoursFinal[i].size(); ii++)
        {
            Point pt = contoursFinal[i][ii];
            if (ii == 0 || pt.x < minX) { minX = pt.x; }
            if (ii == 0 || pt.x > maxX) { maxX = pt.x; }
            if (ii == 0 || pt.y < minY) { minY = pt.y; }
            if (ii == 0 || pt.y > maxY) { maxY = pt.y; }
        }

        int width_px = maxX - minX;
        int height_px = maxY - minY;

        if (width_px + 2 * padding > windowOffset) { windowOffset = width_px + 2 * padding; }

        for (int ii = 0; ii < contoursFinal[i].size(); ii++)
        {
            contoursFinal[i][ii].x += (padding - minX);
            contoursFinal[i][ii].y += (padding - minY);
        }
        Mat singlePiece = Mat::zeros(Size(width_px+ 2 * padding,height_px + 2 * padding), CV_8UC3);
        vector<vector<Point>> contourList;
        contourList.push_back(contoursFinal[i]);
        Scalar color = Scalar(255,255,255);
        drawContours(singlePiece, contourList, 0, color, 1, LINE_8);
        floodFill(singlePiece, Point(width_px / 2, height_px / 2), color);
        threshold(singlePiece, singlePiece, 125, 255, THRESH_BINARY);

        Mat pieceFinal;
        morphologyEx(singlePiece, pieceFinal, MORPH_ERODE, element, Point(-1, -1), 1);
        puzzlePieces.push_back(pieceFinal);
        if (debugVisuals) {
            imshow("piece pre erode", singlePiece);
            imshow("piece final", pieceFinal);
            moveWindow("piece pre erode", 0, 0);
            moveWindow("piece final", windowOffset, 0);
            waitKey(0);
            destroyWindow("piece pre erode");
            destroyWindow("piece final");
        }
    }

    //add edge noise/ blurr (todo)

    //add random rotation
    for (int i = 0; i < puzzlePieces.size(); i++)
    {
        Mat rotated;
        rotate(puzzlePieces[i], rotated, rng.uniform(0, 3));
        
        if (debugVisuals)
        {
            imshow("piece pre rotate", puzzlePieces[i]);
            imshow("piece post rotate", rotated);
            moveWindow("piece pre rotate", 0, 0);
            moveWindow("piece post rotate", windowOffset, 0);
            waitKey(0);
            destroyWindow("piece pre rotate");
            destroyWindow("piece post rotate");
        }
        puzzlePieces[i] = rotated;
    }

    //shuffle puzzle pieces
    auto rng2 = std::default_random_engine{};
    shuffle(begin(puzzlePieces), end(puzzlePieces), rng2);

    //save puzzle pieces to output file
    for (int i = 0; i < puzzlePieces.size(); i++)
    {
        imwrite(puzzlePath + "piece_" + to_string(i)+".JPG", puzzlePieces[i]);
    }


    
    if (debugVisuals) { waitKey(0); }
    return 0;
}