//#include "ofMain.h"
//#include "ofxOpenCv.h"


#define LOAD_FROM_FILE true

class Kinect {

    public:
        void init();
        void setRegistration(bool);
        void open(int);
        bool isConnected();
        void setCameraTiltAngle(int);
        void update();
        bool isFrameNew();
        void close();
       // uchar* getDepthPixels();
        void drawDepth(int x_s, int y_s, int width, int height);

        int getFrameNumber();
        int width;
        int height;

    private:
        void createImagePart(int x_s, int y_s, int width, int height, int value);

        bool isDevConnected;

       // ofVideoPlayer myPlayer;

       // ofxCvGrayscaleImage myImage;
      //  uchar *tmpImage;
};

