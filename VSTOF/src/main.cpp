#include "ofMain.h"
#include "ofApp.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//========================================================================
int main(int argc, LPSTR argv[]){
	ofSetupOpenGL(640,480,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp(argc,argv));
}