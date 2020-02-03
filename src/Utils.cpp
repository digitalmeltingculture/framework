#include "Utils.h"

//#include <opencv2/opencv.hpp>

/*
int Utils::OneNearestNeighborsIndex(ofPoint p, ofxCvContourFinder allOtherPoints) {
    std::vector<float> distances;

    distances = searchDistances(p, allOtherPoints);

//    for(uint i=0; i<distances.size(); i++)
//        cout<<i<<" - "<<distances[i]<<endl;

    return indexOfMinValue(allOtherPoints.nBlobs, distances);
}
*/

int Utils::indexOfMinValue(int dim, std::vector<float> values) {
    float minVal;
    int   idx, minIdx;

    idx = 0;
    minIdx = 0;
    minVal = values[idx];

    for(idx=1; idx<dim; idx++) {

        if( values[idx]<minVal ) {
            minVal = values[idx];
            minIdx = idx;
        }
    }

    return minIdx;
}

/*
float Utils::squaredEuclideanDistance(ofPoint a, ofPoint b) {
    return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}
*/
/*
std::vector<float> Utils::searchDistances(ofPoint p, ofxCvContourFinder allOtherPoints) {
    ofxCvBlob blob;
    ofPoint   tmp;
    int       b;
    std::vector<float> distances(allOtherPoints.nBlobs);

    for(b=0; b<allOtherPoints.nBlobs; b++) {
        blob = allOtherPoints.blobs[b];

        tmp.x = blob.centroid.x;
        tmp.y = blob.centroid.y;

        distances[b] = squaredEuclideanDistance(p, tmp);
    }

    return distances;
}
*/
/**
 Calcola l'angolo tra 2 vettori in gradi
 */
/*
float Utils::angleBetweenVectors(ofPoint a, ofPoint b) {
	float scalarProduct,
		  mod1, mod2;

    a.normalize();
    b.normalize();
    
	scalarProduct = a.dot(b);
	mod1 = sqrt(squaredEuclideanDistance(a, ofPoint(0, 0)));
	mod2 = sqrt(squaredEuclideanDistance(b, ofPoint(0, 0)));

    mod1 = radiansToDegree(acos(scalarProduct/(mod1*mod2)));
    
    if( mod1<0.0f )
        mod1 = 360.0f - mod1;
    
	return mod1;
}
*/
/**
 Calcola l'angolo tra 3 punti, assumendo il secondo parametro come
  punto centrale. L'angolo è calcolato in gradi
 */
/*
float Utils::angleBetween3Points(ofPoint a, ofPoint central, ofPoint b) {
	float angle;
	ofPoint vector1, vector2;

	vector1 = ofPoint( a.x - central.x, a.y - central.y );
	vector2 = ofPoint( b.x - central.x, b.y - central.y );

	angle = angleBetweenVectors( vector1, vector2  );

	if( (vector1.cross(vector2)).z < 0)
		return -1*angle;

	return angle;
}
*/
/**
 Ruoto il punto p di angle (in rad) rispetto al centro center
 */
/*
ofPoint Utils::rotatePoint(double angle, ofPoint p, ofPoint center) {
    
    ofPoint final = ofPoint(cos(angle) * (p.x-center.x) - sin(angle) * (p.y-center.y) + center.x,
                                sin(angle) * (p.x-center.x) + cos(angle) * (p.y-center.y) + center.y);
    
    //Blocco già commentato in file originale
    //--------------------------------
    //double cosA = cos(angle);
    //double sinA = sin(angle);
    
    //ofPoint translP = ofPoint(p.x - center.x, p.y - center.y);
    //ofPoint rotatedP = ofPoint(translP.x*cosA - translP.y * sinA, translP.x*sinA+translP.y*cosA);
    
    //ofPoint finalP = ofPoint(rotatedP.x + center.x, rotatedP.y + center.y);
    
    //cout << "Dario: " <<  final << endl;
    //cout << "Davide: " << finalP << endl;
    
    //return finalP;
    //------------------------------------
     
    return final;
}
*/
/**
 Converto i gradi in radianti
 */
float Utils::degreeToRadians(float value) {
	return value * TO_RAD;
}

/**
 Converto i radianti in gradi
 */
float Utils::radiansToDegree(float value) {
	return value * TO_DEG;
}

/**
 Filtro lowpass. Threshold indica la soglia oltre la quale il filtro è attivo (prima si ritorna
 il nuovo valore. newValue e oldValue sono i valori da prendere in considerazione. smoothFactor
 indica il peso da dare alla variazione (newValue-oldValue).
 */
float Utils::lowPass(float newValue, float oldValue, float smoothFactor, float threshold) {
    
   // if( abs((newValue-oldValue))<threshold )
        return newValue;
   // else
   //     return oldValue + (newValue - oldValue) * smoothFactor;
}

/**
 Inverte i valori a e b
 */
void Utils::swap(float *a, float *b){
    float x;
    x = *a;
    *a = *b;
    *b = x;
}

/*
 modificare il metodo per leggere solo le roi e 
 */
/*
uchar* Utils::rangeContrastStretch(IplImage* src, IplImage* dst, double minPixVal, double maxPixVal, double finalMinVal, double finalMaxVal){
    double scale;
    uchar *srcData, *dstData;
    
    int xStart = 0, yStart = 0;
    int xEnd=src->width, yEnd=src->height;
    
    if(src->roi){
        xStart = src->roi->xOffset;
        yStart = src->roi->yOffset;
        
        xEnd = xStart + src->roi->width;
        yEnd = yStart + src->roi->height;
    }
    
    if( !dst )
        dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
    cvZero(dst);
    
    scale = (finalMaxVal-finalMinVal)/(maxPixVal-minPixVal);
    
    for(int i=yStart; i<yEnd; i++){
        srcData = (uchar*)src->imageData + i*src->widthStep;
        dstData = (uchar*)dst->imageData + i*dst->widthStep;
        
        for(int j=xStart; j< xEnd; j++){
            if( srcData[j]>=minPixVal && srcData[j]<=maxPixVal ){
                dstData[j]= (srcData[j]-minPixVal) * scale + finalMinVal;
            }
        }
    }

    return (uchar*)dst->imageData;
}
*/

