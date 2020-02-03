#include "kinect.h"
#include <memory>

void Kinect::init() {
    //myPlayer.loadMovie("/Users/dariux881/Movies/video_past/antorossi.mov");
    
    //width = myPlayer.width;
    //height = myPlayer.height;
    
   // isDevConnected = false;
    
    //myImage.allocate(width, height);
    //tmpImage = new uchar[width*height];
}

void Kinect::createImagePart(int x_s, int y_s, int w_, int h_, int value) {
    int xy;

    for( int w=x_s; w<x_s+w_; w++ ) {
        for( int h=y_s; h<y_s+h_; h++ ) {
            xy = w*width + h;

            //tmpImage[xy] = value;
        }
    }
}

void Kinect::open(int id) {
    isDevConnected = true;

   // myPlayer.setFrame(0);
   // myPlayer.play();
}

bool Kinect::isConnected() {
    return isDevConnected;
}

void Kinect::setRegistration(bool rec) {
}

void Kinect::setCameraTiltAngle(int g) {
}

void Kinect::update() {
   // uchar* newFrame;
   // int nChannels;
   /*
    if( LOAD_FROM_FILE ) {
        myPlayer.update();

        newFrame = myPlayer.getPixels();
        nChannels = myPlayer.getPixelsRef().getNumChannels();

        for(int i=0; i<width; i++)
            for(int j=0; j<height; j++)
                tmpImage[j*width + i] = newFrame[(j*width + i)*nChannels];
    }

    myImage.setFromPixels(tmpImage, width, height);*/
}

bool Kinect::isFrameNew() {
    return true;
}

/*uchar* Kinect::getDepthPixels() {
    return tmpImage;
}
*/
void Kinect::drawDepth(int x_s, int y_s, int w_, int h_) {
   // myImage.draw(x_s, y_s, w_, h_);
}

int Kinect::getFrameNumber() {
    //return myPlayer.getCurrentFrame();
}

void Kinect::close() {
    /*
    cout << "current frame: " << myPlayer.getCurrentFrame() << endl;
    
    myPlayer.stop();
    myPlayer.close();*/
}
