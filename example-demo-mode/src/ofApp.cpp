#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup()
{
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetLogLevel("ofThread", OF_LOG_ERROR);
    doDrawInfo = true;    
    doPrintInfo = false;
    doPresetChange = false;
    //allows keys to be entered via terminal remotely (ssh)
    consoleListener.setup(this);
    

    presets = sessionConfig.getAllPresets();
    
    currentPreset = 0;
    //sessionConfig.preset = presets[currentPreset];
    sessionConfig.preset = SessionConfig::PRESET_720P_30FPS_TEXTURE;
    
    //pass in the settings and it will start the camera
    videoGrabber.setup(sessionConfig);
    
    
   
   /* DemoManualControls*  manualControlDemo = new DemoManualControls();
    manualControlDemo->setup(&videoGrabber);
    manualControlDemo->name = "MANUAL CONTROLS";
    demos.push_back(manualControlDemo);*/
    
    DemoEnhancement* enhancementDemo = new DemoEnhancement();
    enhancementDemo->setup(&videoGrabber);
    enhancementDemo->name = "IMAGE ENHANCEMENT";
    demos.push_back(enhancementDemo);
    
    DemoFilters* filterDemo = new DemoFilters();
    filterDemo->setup(&videoGrabber);
    filterDemo->name = "FILTERS";
    demos.push_back(filterDemo);
    
    DemoExposurePresets* exposurePresetDemo = new DemoExposurePresets();
    exposurePresetDemo->setup(&videoGrabber);
    exposurePresetDemo->name = "EXPOSURE PRESETS";
    demos.push_back(exposurePresetDemo);
    
    DemoMirrorMode* mirrorModeDemo = new DemoMirrorMode();
    mirrorModeDemo->setup(&videoGrabber);
    mirrorModeDemo->name = "MIRROR MODE";
    demos.push_back(mirrorModeDemo);
    
    DemoRotation* rotationDemo = new DemoRotation();
    rotationDemo->setup(&videoGrabber);
    rotationDemo->name = "ROTATION";
    demos.push_back(rotationDemo);
    
    DemoZoomCrop* zoomCropDemo = new DemoZoomCrop();
    zoomCropDemo->setup(&videoGrabber);
    zoomCropDemo->name = "ZOOM/CROP";
    demos.push_back(zoomCropDemo);
    
    DemoExposureMode* exposureModeDemo = new DemoExposureMode();
    exposureModeDemo->setup(&videoGrabber);
    exposureModeDemo->name = "EXPOSURE MODES";
    demos.push_back(exposureModeDemo);
    

    
    /*
     DEMOS TODO
     
    - Manual mode, shutter speed, disable sharpen/saturation, White Balance/Metering
    - Recording mode
    - Saving/Loading States
     
    
    */
    
    doNextDemo = false;
    currentDemoID =0;
    currentDemo = demos[currentDemoID];
}


//--------------------------------------------------------------
void ofApp::update()
{
    if (doPresetChange) 
    {
        if((unsigned int)currentPreset+1 < presets.size())
        {
            currentPreset++;
        }else
        {
            currentPreset = 0;
        }
        SessionConfig* settings = new SessionConfig();
        
        settings->preset = presets[currentPreset];
        sessionConfig = *settings;
        
        videoGrabber.setup(sessionConfig);
        doPresetChange = false;
        doPrintInfo = true;
    }
    
    if (doNextDemo)
    {
        if((unsigned int) currentDemoID+1 < demos.size())
        {
            currentDemoID++;
        }else
        {
            currentDemoID = 0;
        }
        currentDemo = demos[currentDemoID];
        doNextDemo = false;
        doPrintInfo = true;
    }else
    {
        currentDemo->update();
    }
    
}


//--------------------------------------------------------------
void ofApp::draw()
{
    if(videoGrabber.isTextureEnabled())
    {
        //draws at camera resolution
        videoGrabber.draw();
        
        //draw a smaller version via the getTextureReference() method
        int drawWidth = videoGrabber.getWidth()/4;
        int drawHeight = videoGrabber.getHeight()/4;
        videoGrabber.getTextureReference().draw(videoGrabber.getWidth()-drawWidth, videoGrabber.getHeight()-drawHeight, drawWidth, drawHeight);
    }
    
    if (doDrawInfo || doPrintInfo) 
    {
        stringstream info;
        info << "\n";
        info << "App FPS: " << ofGetFrameRate() << "\n";
        info << "Camera Resolution: "   << videoGrabber.getWidth() << "x" << videoGrabber.getHeight()	<< " @ "<< videoGrabber.getFrameRate() <<"FPS"<< "\n";
        info << "\n";
        info << "DEMO: " << currentDemo->name << "\n";
        info << "\n";
        info << currentDemo->infoString;
        info << "\n";
        info << "Press p for next Size/FPS/Mode Preset" << "\n";
        info << "Press r to reset camera settings" << "\n";
        info << "Press SPACE for next Demo" << "\n";
        
        if (doDrawInfo) 
        {
            int x = 100;
            if(videoGrabber.getWidth()<1280)
            {
                x = videoGrabber.getWidth();
            }
            ofDrawBitmapStringHighlight(info.str(), x, 40, ofColor(ofColor::black, 10), ofColor::yellow);
        }
        if (doPrintInfo) 
        {
            ofLogVerbose() << info.str();
            doPrintInfo = false;
        }
    }
    ofColor circleColor;
    if (videoGrabber.isRecording()) 
    {
        circleColor = ofColor::green;
    }else
    {
        circleColor = ofColor::red;
    }
    ofPushStyle();
        ofSetColor(circleColor, 90);
        ofCircle(ofPoint(ofGetWidth() - 200, 40), 20);
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key)
{
    ofLog(OF_LOG_VERBOSE, "%c keyPressed", key);
    switch (key) 
    {
        case ' ':
        {
            doNextDemo = true;
            break;
        }
        case 'd':
        {
            doDrawInfo = !doDrawInfo;
            break;
        }
        case 'i':
        {
            doPrintInfo = !doPrintInfo;
            break;
        }
        case 'p' :
        {
            doPresetChange = true;
            break;
        }
        case 'r' :
        {
            videoGrabber.getCameraSettings().resetCameraToDefaultSettings();
            break;
        }
            
        case 'S' :
        {
            videoGrabber.saveCameraSettingsToFile();
            break;
        }
        case 'L' :
        {
            //videoGrabber.cameraSettings.loadCameraSettingsFromFile();
            break;
        }
        case '8' :
        {
            videoGrabber.startRecording();
            break;
        }
        case '9' :
        {
            videoGrabber.stopRecording();
            break;
        }
        /*
        case '5' :
        {
            videoGrabber.rotateCounterClockwise();
            break;
        }
        case '6' :
        {
            videoGrabber.rotateClockwise();
            break;
        }
        case '7' :
        {
            videoGrabber.toggleLED();
            break;
        }
        case '8' :
        {
            videoGrabber.startRecording();
            break;
        }
        case '9' :
        {
            videoGrabber.stopRecording();
            break;
        }
        */
        default:
        {
            currentDemo->onKey(key);
            break;
        }
            
    }
}

void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
    keyPressed((int)e.character);
}