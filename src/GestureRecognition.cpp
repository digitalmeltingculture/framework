//
//  GestureRecognition.cpp
//  PaSt
//
//  Created by Dario Di Mauro on 04/09/14.
//
//

#include "GestureRecognition.h"

string GestureRecognition::userMove = "userMove";
string GestureRecognition::userCount = "user.count.";
string GestureRecognition::userNotEmpty = "user.notempty";
string GestureRecognition::gesturePosition = "gesture.position.";
string GestureRecognition::gestureArea = "gesture.area.";

GestureRecognition* GestureRecognition::instance = NULL;

//--------------------------------------------------
GestureRecognition::GestureRecognition() {
    previousDetectedHands   = 0;
    
    minAngularVariation 	= Configuration::getInstance()->minAngularVariation; 	// In degree
    maxAngularVariation 	= Configuration::getInstance()->maxAngularVariation; 	// In degree
    minAngularVariation_2	= Configuration::getInstance()->minAngularVariation_2; 	// In degree
    maxAngularVariation_2	= Configuration::getInstance()->maxAngularVariation_2; 	// In degree
    minTimeElapsed 			= Configuration::getInstance()->minTimeElapsed; 		// In ms
    maxTimeElapsed 			= Configuration::getInstance()->maxTimeElapsed;		    // In ms

    minAngle = maxAngle = 0;
    angleHistory = new History(20);
    angleHistory->initWeights(angleHistory->GAMMA, 0.1f);
    
    centerX = new History(10);
    centerY = new History(10);
    
    //oldLeaderPosition = ofPoint(0);
    
    /*
     0 - No gesture
     1 - left 2 right (l2r)
     2 - right 2 left (r2l)
     3 - zoom in (zi)
     4 - zoom out (zo)
     */
    gesture_dataset_type = 4;
    user_code = "007";

    switch (gesture_dataset_type) {
        case 0:
            break;
        case 1:
            myfile.open (string( string("/Users/dariux881/Desktop/dataset/") + user_code + string("/l2r.csv") ).data() );
            break;
        case 2:
            myfile.open (string( string("/Users/dariux881/Desktop/dataset/") + user_code + string("/r2l.csv") ).data());
            break;
        case 3:
            myfile.open (string( string("/Users/dariux881/Desktop/dataset/") + user_code + string("/zi.csv") ).data());
            break;
        case 4:
            myfile.open (string( string("/Users/dariux881/Desktop/dataset/") + user_code + string("/zo.csv") ).data());
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------
GestureRecognition* GestureRecognition::getInstance() {
    if( !instance )
        instance = new GestureRecognition();
    return instance;
}

//--------------------------------------------------
void GestureRecognition::closeDatasetFile() {
   // if( myfile!= NULL )
   //     myfile.close();
}

//--------------------------------------------------
void GestureRecognition::myRegisterObserver( Observer* obs) {
    if( obs )
        observers.push_back(obs);
}

//--------------------------------------------------
void GestureRecognition::myNotifyAll(GestureType_t* data) {
    vector<Observer*>::iterator it;
    
    for(it = observers.begin(); it!=observers.end(); it++)
        (*it)->onObservableUpdate(data);
    
    if( data->type.compare(NO_GESTURE) )
        createDataset(gesture_dataset_type);
    else
        createDataset(0);
    //historyHands.clear();
}

//--------------------------------------------------
void GestureRecognition::resetImage() {
  /*  minDistanceToCentroid = GR_RESIZE_WIDTH;
    cout << "reset leader image" << endl;
    
//    processIdle();
    
    leaderBox.size.width = GR_RESIZE_WIDTH;
    leaderBox.size.height = GR_RESIZE_HEIGHT;*/
}

//--------------------------------------------------
/*void GestureRecognition::updateImage(ofxCvGrayscaleImage newImage, ofPoint centroid) {
    GestureType_t *debug;
    
    if( leaderImage.width!=newImage.width || leaderImage.height!=newImage.height)
        leaderImage.allocate(newImage.width, newImage.height);
    
    leaderImage.setFromPixels(newImage.getPixelsRef());
    
    ofxCvContourFinder    singleBlob;
    singleBlob.findContours(leaderImage, 10, leaderImage.width*leaderImage.height, 1, false);
    
    if( singleBlob.nBlobs>0 ) {
        leaderBlob = singleBlob.blobs[0];
        leaderBlob.centroid = centroid;
    }
    else
        return;
    
    if( oldLeaderPosition.x!=0 && oldLeaderPosition.y!=0 ) {
        leaderBlob.centroid.x = Utils::lowPass(leaderBlob.centroid.x, oldLeaderPosition.x, 0.85f, 1);
        leaderBlob.centroid.y = Utils::lowPass(leaderBlob.centroid.y, oldLeaderPosition.y, 0.85f, 1);
    }
    
    calculateMaxDistance();
    
//    debug = new GestureType_t();
//    debug->type  = NO_GESTURE;
//    debug->ts    = 0;
//    debug->img   = &leaderImage;
//    debug->hand  = UNDEFINED_HAND;
//    notifyAll(debug);
//    
//    delete debug;
}
*/
//--------------------------------------------------
/*
 La funzione effettua una stima delle posizioni delle mani. Se le braccia non sono alzate
 non verrà restituito nessun valore possibile
 */
/*
vector<ofPoint*> GestureRecognition::detectHands(ofxCvBlob blob) {
    vector<ofPoint>::iterator   itPts;
    float 						tmpDist1, tmpDist2,
                                distance1, distance2;
    int							ptIdx, minIdx1, minIdx2, idx;
    int                         limits[4];
    vector<ofPoint*>            retVect;
    ofPoint                     center = ofPoint(box.center.x, box.center.y);
    
    distance1 = distance2 = GR_RESIZE_WIDTH; // Valore fittizio. Sarà aggiornato alla prima iterazione
    minIdx1 = minIdx2 = -1;
    ptIdx = 0;
    
    cvCircle(leaderImage.getCvImage(), cvPoint(directionPoint1.x, directionPoint1.y), 4, cvScalar(100, 0, 0), 2, 8 );
    cvCircle(leaderImage.getCvImage(), cvPoint(directionPoint2.x, directionPoint2.y), 4, cvScalar(100, 0, 0), 2, 8 );
    cvCircle(leaderImage.getCvImage(), cvPoint(center.x, center.y), 3, cvScalar(100, 0, 0), 2, 8 );
    
    // Cerco i punti del contorno che sono in direzione della persona. Si fa
    //   una stima del punto centrale di petto e schiena
    for(itPts = blob.pts.begin(); itPts != blob.pts.end(); itPts++) {
        tmpDist1 = Utils::squaredEuclideanDistance(directionPoint1, *itPts);
        tmpDist2 = Utils::squaredEuclideanDistance(directionPoint2, *itPts);
        
        // inizializzo distanze e punti la prima volta
        if( ptIdx==0 ) {
            distance1 = tmpDist1;
            minIdx1 = ptIdx;
            distance2 = tmpDist2;
            minIdx2 = ptIdx;
        }
        
        ptIdx++;
        
        if( tmpDist1<distance1 ) {
            distance1 = tmpDist1;
            minIdx1 = ptIdx;
        }
        
        if( tmpDist2<distance2 ) {
            distance2 = tmpDist2;
            minIdx2 = ptIdx;
        }
    }
    
    // Controllo che siano stati rilevati entrambi i punti
    if( minIdx1==-1 || minIdx2==-1) {
        return retVect;
    }
    
    // ------------------------------------------------------
    //   Devo scorrere un vettore fissati due indici intermedi
     
    //      + 0 +_+_+_* k1 +_+_+_+_+ k2 +_+_+_+ n +
     
    //    il primo for:
    //            k1 -> k2
    //    il secondo for:
    //            k2 -> n + k1 (con il modulo sugli indici)
    //   ------------------------------------------------------
    if( minIdx1<minIdx2) {
        limits[0] = minIdx1;
        limits[1] = minIdx2;
        limits[2] = minIdx2;
        limits[3] = blob.pts.size() + minIdx1;
    }
    else {
        limits[0] = minIdx2;
        limits[1] = minIdx1;
        limits[2] = minIdx1;
        limits[3] = blob.pts.size() + minIdx2;
    }
    
    // I due for innestati successivi scorrono il vettore dei punti una sola volta.
    // La complessità è O(n)
    
    // Uso distance2 come indice per il vettore limits
    for( distance2=0; distance2<2; distance2++) {
        distance1 = 0;
        tmpDist1  = 0;
        ofPoint *tmpOfPoint = new ofPoint();
        
        // Scorro parte del vettore del perimetro partizionando
        //   il perimetro dal risultato precedente, in modo da cercare
        //   separatamente le due mani
        for( ptIdx=limits[(int)distance2]; ptIdx < limits[(int)distance2 + 2]; ptIdx++) {
            idx = ptIdx%blob.pts.size();
            
            tmpDist1  = Utils::squaredEuclideanDistance(center, blob.pts[idx]);
            
            if( tmpDist1>distance1 ) {
                distance1 = tmpDist1;
                
                tmpOfPoint->x = blob.pts[idx].x;
                tmpOfPoint->y = blob.pts[idx].y;
                tmpOfPoint->z = 0;
            }
        }

        // Controllo se ho trovato una mano
        if( distance1>minDistanceToCentroid ) {
            retVect.push_back(tmpOfPoint);
            
            cvCircle(leaderImage.getCvImage(), cvPoint(tmpOfPoint->x, tmpOfPoint->y), 7, cvScalar(50, 0, 0), 2, 8 );
        }
        else {
            delete tmpOfPoint;
        }
    }
    
	return retVect;
}
*/
//--------------------------------------------------
void GestureRecognition::calculateMaxDistance() {
/*    ofPoint tmpOfPoint, center;
    char    versus;
    double  angle;
    
    calculateEllipse(leaderBlob.pts, box, false);

    previousDetectedHands = handVect.size();
    handVect = detectHands(leaderBlob);
    
    // +++ WARNING +++
    //Determino la situazione in base al numero di mani rilevate. Evito
    // di passare da un gesto all'altro senza NO_GESTURE intermedio,
    // quindi richiedo che il numero delle mani rilevate sia pari al precedente
    // o 0
    
    if( (handVect.size()==1 ) &&  // Una sola mano rilevata
       (previousDetectedHands==1 || previousDetectedHands==0) ) { // Prima ho rilevato una o nessuna mano
        tmpOfPoint = *(handVect[0]);
        center = ofPoint(box.center.x, box.center.y);
        
        versus = analyzeHand(&tmpOfPoint, center, box, sqrt(minDistanceToCentroid), bodyMinorAxis + bodyMinorAxis*0.2f );
        
        if( previousDetectedHands==0 ) {
            if( tmpOfPoint.y>=-box.size.width/4 && tmpOfPoint.y<=box.size.width/4 ) {
                //cout << "la prima volta mani troppo centrali" << endl;
                return;
            }
            
            versusGesture = versus;
            
            if( versus==UNDEFINED_HAND ) {
                undefinedHand.push_back(tmpOfPoint);
            }
            else {
                pushInHistory(tmpOfPoint, versus);
            }
        }
        else {
            if( versus==versusGesture ) {
                if( versus==UNDEFINED_HAND ) {
                    undefinedHand.push_back(tmpOfPoint);
                }
                else {
                    pushInHistory(tmpOfPoint, versus);
                }
            }
            else if( versus==UNDEFINED_HAND ) {
                // Questa mano è undefined ma prima non lo era
                declareUndefined(tmpOfPoint, versusGesture);
            }
            else { // versus!=versusGesture && versus!=UNDEFINED_HAND
                if( versusGesture==UNDEFINED_HAND ) {
                    versusGesture = versus;
                    declareUndefined(tmpOfPoint, versus);
                }
                else {
                    // Problema!! la mano riconosciuta non è la stessa
                    previousDetectedHands = 0;
                    return;
                }
            }
        }

//        cout << handToString(versusGesture) << endl;
        
        manageSingleHandGesture(tmpOfPoint);
    }
    else if( ( handVect.size()==2 ) && // Due mani rilevate
            (previousDetectedHands==2 || previousDetectedHands==0) ) {// Prima ho rilevato due o nessuna mano
        
        vector<ofPoint*> tmp;
        center = ofPoint(box.center.x, box.center.y);
        tmp = analyzeTwoHands(handVect[0], handVect[1], center, box, bodyMinorAxis + bodyMinorAxis*0.2f);

        if( tmp[0]->x==0 && tmp[0]->y==0 && tmp[1]->x==0 && tmp[1]->y==0 ) {
            cout << "vettori nn validi" << endl;
//            return;
        }
        else {
            pushInHistoryTwoHands(*tmp[0], *tmp[1]);
            manageTwoHandsGesture(tmp, ofPoint(0));
        }
    }
    else { // istanti in cui non c'e' gesto. Nessuna mano rilevata
        processIdle();
    }

    CvPoint cent;
    CvSize  size;
    
    cent.x = box.center.x;
    cent.y = box.center.y;
    
    size.width = box.size.width/2;
    size.height = box.size.height/2;
    
    if( size.width<=0 || size.height<=0 )
        return;
    
    cvEllipse(leaderImage.getCvImage(), cent, size,
              box.angle, 0, 360,
              CV_RGB(125,125,125), 1, CV_AA, 0);
*/
}

//--------------------------------------------------
/* Gestisci il periodo in cui non ci sono gesti in corso
 */
void GestureRecognition::processIdle() {
   /* if( startTrackingTime1 != 0 ) // Fine del gesto
        startTrackingTime1 = 0;
    if( startTrackingTime2 != 0 ) // Fine del gesto
        startTrackingTime2 = 0;
    
    handVect.clear();
    
    GestureType_t *debug = new GestureType_t();
    debug->type  = NO_GESTURE;
    debug->ts    = 0;
    debug->img   = &leaderImage;
    debug->hand  = UNDEFINED_HAND;
    notifyAll(debug);

    delete debug;
    
    pushInHistoryTwoHands(ofPoint(0), ofPoint(0));
    createDataset();
//        historyHands.clear();
    versusGesture = UNDEFINED_HAND;
    orientationGesture = ORIENTATION_UNDEFINED;

    // Calcolo l'ellisse attorno alla persona
    calculateEllipse(leaderBlob.pts, box, true);
	*/
}

//--------------------------------------------------
/*
bool GestureRecognition::calculateEllipse( vector <ofPoint>  & contour, CvBox2D32f & box, bool updateAngle ) {
    ofPoint start, end, end2;
    float tmpAngle;
    bool  ellipseRight;
    
    ellipseRight = fitEllipse(contour, box);
    
    if( ellipseRight ) {
        centerX->addElement(box.center.x);
        centerY->addElement(box.center.y);
        
        box.center.x = centerX->getMedianValue();
        box.center.y = centerY->getMedianValue();
    }
    
    if( updateAngle && ellipseRight ) {
        // Aggiorno l'angolo dell'ellisse solo se è calcolata come corretta
        angleHistory->addElement(box.angle);
    }
    tmpAngle = angleHistory->getMedianValue();
    
    if( tmpAngle==0) {
        // Vale all'inizio. La storia è vuota, quindi prendo per buono il primo valore ricevuto
        angleHistory->initHistory(box.angle);
        tmpAngle = angleHistory->getMedianValue();
    }
    
    box.angle = tmpAngle;
    
    minDistanceToCentroid = pow((box.size.width) + ((box.size.width)*0.25f),2);
    bodyMinorAxis = ((int)box.size.height/4);
    
    start = ofPoint(box.center.x, box.center.y);
    
    end2.x = start.x;
    end2.y = start.y - bodyMinorAxis;
    end.x = start.x;
    end.y = start.y + bodyMinorAxis;
    
    directionPoint1 = Utils::rotatePoint(Utils::degreeToRadians(box.angle+90), end, start);
    directionPoint2 = Utils::rotatePoint(Utils::degreeToRadians(box.angle+90), end2, start);

    return true;
}
*/
//--------------------------------------------------
/*void GestureRecognition::declareUndefined(ofPoint newPoint, char versus) {
    list<ofPoint>::iterator undIt;
    
    for(undIt=undefinedHand.begin(); undIt!=undefinedHand.end(); undIt++) {
        pushInHistory(*undIt, versus);
    }
    
    pushInHistory(newPoint, versus);
    
    undefinedHand.clear();
}
*/
//--------------------------------------------------
/*void GestureRecognition::pushInHistory(ofPoint p, char versus) {
    if( versus==LEFT_HAND ) {
//        historyHands.push_back(make_pair(p, ofPoint(0)) );
        pushInHistoryTwoHands(p, ofPoint(0));
    }
    else {
//        historyHands.push_back(make_pair(ofPoint(0), p) );
        pushInHistoryTwoHands(ofPoint(0), p);
    }
}
*/
//--------------------------------------------------
/*void  GestureRecognition::pushInHistoryTwoHands(ofPoint left, ofPoint right) {
    historyHands.push_back(make_pair(left, right));
}
*/
//--------------------------------------------------
void GestureRecognition::createDataset(int label) {
   /* list<pair<ofPoint, ofPoint> >::iterator histIt;
    
    for( histIt=historyHands.begin(); histIt!=historyHands.end(); histIt++ ) {
        myfile << (*histIt).first.x << "," << (*histIt).first.y << "," << (*histIt).second.x << "," << (*histIt).second.y << ","<< label << endl;
    }*/
}

//--------------------------------------------------
/*char GestureRecognition::analyzeHand(ofPoint *hand, ofPoint centroid, CvBox2D box, float minDistance, float bodyThickness) {
    char versus = UNDEFINED_HAND;
    
    cvCircle(leaderImage.getCvImage(), cvPoint(hand->x, hand->y), 3, cvScalar(100, 0, 0), 2, 8 );
    
    *hand = Utils::rotatePoint(Utils::degreeToRadians(-box.angle), *hand, centroid) - centroid;

//    cout << "HAND: " << *hand << endl;
    
    if( hand->y < -minDistance ) {
        if( hand->x < -bodyThickness ) {
            versus = RIGHT_HAND;
            
            orientationGesture = ORIENTATION_REAR;
        }
        else if( hand->x > bodyThickness ) {
            versus = LEFT_HAND;
            
            orientationGesture = ORIENTATION_FRONT;
        }
    }
    else if( hand->y > minDistance ) {
        if( hand->x < -bodyThickness ) {
            versus = LEFT_HAND;
            
            orientationGesture = ORIENTATION_REAR;
        }
        else if( hand->x > bodyThickness ) {
            versus = RIGHT_HAND;
            
            orientationGesture = ORIENTATION_FRONT;
        }
    }
    
    return versus;
}
*/
//--------------------------------------------------
/*vector<ofPoint*> GestureRecognition::analyzeTwoHands(ofPoint *h1, ofPoint *h2, ofPoint centroid, CvBox2D box, float bodyThickness) {
    vector<ofPoint*> result(2);
    
    ofPoint *tmp1 = new ofPoint(Utils::rotatePoint(Utils::degreeToRadians(-box.angle), *h1, centroid));
    ofPoint *tmp2 = new ofPoint(Utils::rotatePoint(Utils::degreeToRadians(-box.angle), *h2, centroid));
    
    *tmp1 = *tmp1 - centroid;
    *tmp2 = *tmp2 - centroid;

    result[0] = new ofPoint(0);
    result[1] = new ofPoint(0);
    
    if( (tmp1->y>=-box.size.width/4) && (tmp1->y<=box.size.width/4) ) {
        //cout << "punti scartati perche centrali" << endl;
        return result;
    }
    
    if( tmp1->x>bodyThickness && tmp2->x>bodyThickness ) {
        if( tmp1->y>0 ) {
            result[0] = tmp2;
            result[1] = tmp1;
            
            cvCircle(leaderImage.getCvImage(), cvPoint(h2->x, h2->y), 3, cvScalar(100, 0, 0), 2, 8 );
        }
        else {
            result[0] = tmp1;
            result[1] = tmp2;
            
            cvCircle(leaderImage.getCvImage(), cvPoint(h1->x, h1->y), 3, cvScalar(100, 0, 0), 2, 8 );
        }
    }
    else if( tmp1->x< -bodyThickness && tmp2->x<-bodyThickness ){
        // Ruoto i punti di 180 gradi per orientare verso l'alto
        *tmp1 = Utils::rotatePoint(Utils::degreeToRadians(180), *tmp1, ofPoint(0));
        *tmp2 = Utils::rotatePoint(Utils::degreeToRadians(180), *tmp2, ofPoint(0));
        
        if( tmp1->y>0 ) {
            result[0] = tmp2;
            result[1] = tmp1;
            
            //cout << "verso il basso, tmp1 sinistro" << endl;
            cvCircle(leaderImage.getCvImage(), cvPoint(h2->x, h2->y), 3, cvScalar(100, 0, 0), 2, 8 );
        }
        else {
            result[0] = tmp1;
            result[1] = tmp2;
            
            //cout << "verso il basso, tmp1 destro" << endl;
            cvCircle(leaderImage.getCvImage(), cvPoint(h1->x, h1->y), 3, cvScalar(100, 0, 0), 2, 8 );
        }
    }
    else {
        cout << "mani laterali" << endl;
    }
    
    if( result[0]->x==0 && result[0]->y==0 && result[1]->x==0 && result[1]->y==0 ) {
//        cout << *tmp1 <<"  |  " << *tmp2 << endl;
//        cout << *h1 <<"  |  " << *h2 << endl;
    }
    
    return result;
}
*/
//--------------------------------------------------
/*void GestureRecognition::manageSingleHandGesture(ofPoint tmpOfPoint) {
    if( startTrackingTime1==0 ) {
        // La prima volta che calcolo un punto oltre la distanza minima
        //		( inizio del gesto? )
        startTrackingTime1 = (double)cvGetTickCount();
        
        startGesturePoint.x = tmpOfPoint.x;
        startGesturePoint.y = tmpOfPoint.y;
        startGesturePoint.z = 0;
        
        cout << "--------------------------------------" << endl;
    }
    else {
        // Ho già calcolato il punto di partenza. Aggiorno la posizione finale
        endGesturePoint.x = tmpOfPoint.x;
        endGesturePoint.y = tmpOfPoint.y;
        endGesturePoint.z = 0;
        
        //Controllo il gesto
        if( trackBrowseGesture() ) {// C'è stato un gesto. Azzero il tempo di calcolo
//            startTrackingTime1 = 0;
        }
    }
}
*/
//--------------------------------------------------
/*
 Gestisce i gesti con due mani. Controlla l'angolo che si crea tra mani e centroide
 e come varia nel tempo
 */
/*void GestureRecognition::manageTwoHandsGesture(vector<ofPoint*> hands, ofPoint center) {
    float           tmpDistance, delta, twoHandsAngle;
    double          deltaTime;
    GestureType_t   result;
    
    // DEBUG - Disegna linee tra mani e centroide
//    cvLine(leaderImage.getCvImage(), cvPoint(hands[1].x, hands[1].y),
//           cvPoint(singleBlob.blobs[0].centroid.x, singleBlob.blobs[0].centroid.y), cvScalar(100, 0, 0), 2, 8, 0 );
//    cvLine(leaderImage.getCvImage(), cvPoint(hands[0].x, hands[0].y),
//           cvPoint(singleBlob.blobs[0].centroid.x, singleBlob.blobs[0].centroid.y), cvScalar(100, 0, 0), 2, 8, 0 );
    
    if( startTrackingTime2==0 ) { // La prima volta che calcolo un punto oltre la distanza minima (inizio del gesto?)
        startTrackingTime2 = (double)cvGetTickCount();
        
        cout << *hands[0] << "\t\t" << *hands[1] << "\t\t" << center << endl;
        
        twoHandsAngle = Utils::angleBetweenVectors(ofPoint(hands[1]->x - center.x, 
                                                   hands[1]->y - center.y), 
                                           ofPoint(hands[0]->x - center.x,
                                                   hands[0]->y - center.y));
        
        minAngle = maxAngle = twoHandsAngle;
        minAngleTime = maxAngleTime = startTrackingTime2;
        cout << "--------------------------------------" << endl;
        
        return;
    }
    
//    // Filtro l'angolo per eliminare outliers
//    twoHandsAngle = Utils::lowPass(abs(Utils::angleBetweenVectors(*hands[1], *hands[0])), 
//                           twoHandsAngle,
//                           0.8f, 1);

    // Filtro l'angolo per eliminare outliers
    twoHandsAngle = abs(Utils::angleBetweenVectors(*hands[1], *hands[0]));
    
    // Aggiorno angoli minimo e massimo, salvando anche l'istante di acquisizione
    //  L'ampiezza servirà per rilevare il gesto.
    if( twoHandsAngle<minAngle ) {
        minAngle = twoHandsAngle;
        minAngleTime = stopTrackingTime2 = (double)cvGetTickCount();
    }
    else if( twoHandsAngle>maxAngle ) {
        maxAngle = twoHandsAngle;
        maxAngleTime = stopTrackingTime2 = (double)cvGetTickCount();
    }
    
    // Variazione degli angoli e tempo
    delta = maxAngle-minAngle;
    deltaTime = (stopTrackingTime2-startTrackingTime2)/((double)cvGetTickFrequency()*1000.);

    // Se la variazione dell'angolo rispetta limiti di ampiezza e intervallo di tempo
    //  il gesto è valido
    if( (delta>minAngularVariation_2 && delta < maxAngularVariation_2) &&
            (deltaTime>minTimeElapsed && deltaTime<maxTimeElapsed) ) {
        if( minAngleTime < maxAngleTime )
            result.type = ZOOM_IN;
        else
            result.type = ZOOM_OUT;
        
        result.ts   = std::time(0);
        result.img  = &leaderImage;
        result.hand = BOTH_HANDS;
        
        notifyAll(&result);
        
        // Il gesto è stato notificato.
        startTrackingTime2 = 0;
    }
    else {
        
    }
//    else if ( delta <= minAngularVariation_2 ) {
//        cout << "troppa poca variazione: "<< delta << endl;
//    }
//    else if ( delta >= maxAngularVariation_2 ) {
//        cout << "troppa variazione: "<< delta << endl;
//    }
//    else if ( deltaTime <= minTimeElapsed ) {
//        cout << "troppo veloce: "<< deltaTime << endl;
//    }
//    else if ( deltaTime >= maxTimeElapsed ) {
//        cout << "troppo lento: "<< deltaTime << endl;
//    }

}
*/
//--------------------------------------------------
/*
 Traccia il gesto con una mano. Confronta le posizioni con quelle precedenti e
 analizza l'angolo ottenuto durante il movimento. Se questo rispetta determinate
 situazioni, allora viene interpretato come gesto
 *//*
bool GestureRecognition::trackBrowseGesture() {
	float 	angleVariation;
    string	gestureType;
    GestureType_t result;
    double  elapsedTime;
    
    stopTrackingTime1 = (double)cvGetTickCount() - startTrackingTime1;
    
    // Operazioni di correzione per elaborare i punti in sistemi di
    // riferimento differenti
    angleVariation = Utils::angleBetween3Points(ofPoint(startGesturePoint.x, startGesturePoint.y), 
                                                ofPoint(box.center.x, box.center.y),
                                                ofPoint(endGesturePoint.x, endGesturePoint.y));
    
//    cout << "\t\tstart point " << startGesturePoint << endl;
//    cout << "\t\tend gesture point " << endGesturePoint << endl;
    
    // calcolo il tempo e la variazione dell'angolo - analizzo il gesto
    gestureType    = NO_GESTURE;
	elapsedTime    = stopTrackingTime1/((double)cvGetTickFrequency()*1000.);
    
	if (  (abs(angleVariation) > minAngularVariation && abs(angleVariation) < maxAngularVariation ) &&
        (elapsedTime > minTimeElapsed && elapsedTime < maxTimeElapsed ) ) {
		// E' stato rilevato un gesto. Serve individuare quale
        if( orientationGesture==ORIENTATION_FRONT ) {
            ( angleVariation<0 )? gestureType = ARM_LEFT_TO_RIGHT : gestureType=ARM_RIGHT_TO_LEFT;
        }
        else if( orientationGesture==ORIENTATION_REAR ) {
            ( angleVariation<0 )? gestureType = ARM_RIGHT_TO_LEFT : gestureType=ARM_LEFT_TO_RIGHT;
        }
        else { // orientation undefined
        }
	}
    else if ( abs(angleVariation) <= minAngularVariation ) {
        cout << "troppa poca variazione: "<< angleVariation << endl;
    }
    else if ( abs(angleVariation) >= maxAngularVariation ) {
        cout << "troppa variazione: "<< angleVariation << endl;
    }
    else if ( elapsedTime <= minTimeElapsed ) {
        cout << "troppo veloce: "<< elapsedTime << endl;
    }
    else if ( elapsedTime >= maxTimeElapsed ) {
        cout << "troppo lento: "<< elapsedTime << endl;
    }

    if( gestureType==ARM_LEFT_TO_RIGHT ) {
        result.type = gestureType;
        result.ts   = std::time(0);
        result.img  = &leaderImage;
        
        ofPoint tmp;
        tmp = (*historyHands.begin()).first;
        if(tmp.x==0 && tmp.y==0)
            result.hand = RIGHT_HAND;
        else
            result.hand = LEFT_HAND;

        notifyAll(&result);
        return true;
    }
    else if( gestureType==ARM_RIGHT_TO_LEFT ) {
        result.type = gestureType;
        result.ts   = std::time(0);
        result.img  = &leaderImage;

        ofPoint tmp;
        tmp = (*historyHands.begin()).first;
        if(tmp.x==0 && tmp.y==0)
            result.hand = RIGHT_HAND;
        else
            result.hand = LEFT_HAND;

        notifyAll(&result);
        return true;
	}
    
    return false;
}
*/
//--------------------------------------------------
/*
 Stampa la mano
 */
void GestureRecognition::printHand(char versus) {
    cout << handToString(versus) << endl;    
}

/*
 Traduce la mano in stringa
 */
string GestureRecognition::handToString(char versus) {
    if( versus==LEFT_HAND )
        return "left hand";
    else if( versus==RIGHT_HAND )
        return "right hand";
    else if( versus==UNDEFINED_HAND )
        return "undefined";
    else if( versus==BOTH_HANDS )
        return "both hands";
    else if( versus==NO_HANDS )
        return "no hands";
}

//--------------------------------------------------
/*
 Ritorna l'ellisse costruita su un insieme di punti
 */
/*bool GestureRecognition::fitEllipse( vector<ofPoint> & contour, CvBox2D32f& box ) {
    CvPoint2D32f* pointArray;
    CvBox2D32f    tmpBox;
    
    if( contour.size()<6 )
        return false;
    
    // Alloc memory for contour point set.
    pointArray = (CvPoint2D32f*)malloc( contour.size()*sizeof(CvPoint2D32f) );
    
    for(unsigned int i = 0; i < contour.size(); i++) {
    	pointArray[i].x = contour[i].x;
    	pointArray[i].y = contour[i].y;
    }
    
	// Fit ellipse
    cvFitEllipse(pointArray, contour.size(), &tmpBox);
    free(pointArray);
    
    if( (tmpBox.size.width > leaderBox.size.width + leaderBox.size.width*0.15f)   || 
        (tmpBox.size.height > leaderBox.size.height + leaderBox.size.height*0.15f) ) {
        // ellisse calcolata più grande di quella mantenuta
        leaderBox.size.width = Utils::lowPass(leaderBox.size.width, tmpBox.size.width, 0.9f, 0);
        leaderBox.size.height = Utils::lowPass(leaderBox.size.height, tmpBox.size.height, 0.9f, 0);
        
        return false;
    }
    else if( leaderBox.size.width==GR_RESIZE_WIDTH && leaderBox.size.height==GR_RESIZE_HEIGHT ) {
        // le dimensioni del leader non sono note. Le aggiorno
        tmpBox.center.x = Utils::lowPass(tmpBox.center.x, leaderBlob.centroid.x, 0.9f, 0);
        tmpBox.center.y = Utils::lowPass(tmpBox.center.y, leaderBlob.centroid.y, 0.9f, 0);
        
        leaderBox = tmpBox;
        box = leaderBox;
    }
    else {
        // Le dimensioni del leader sono note. 
        // Quelle calcolate sono più piccole. Aggiorno quelle attuali
        leaderBox.size.width = Utils::lowPass(leaderBox.size.width, tmpBox.size.width, 0.8f, 0);
        leaderBox.size.height = Utils::lowPass(leaderBox.size.height, tmpBox.size.height, 0.8f, 0);
    }
    
    box.center.x = Utils::lowPass(tmpBox.center.x, box.center.x, 0.95f, 0);
    box.center.y = Utils::lowPass(tmpBox.center.y, box.center.y, 0.95f, 0);
//    box.center = tmpBox.center;
    
    box.size = leaderBox.size;
    box.angle = tmpBox.angle;
    
    return true;
}
*/
