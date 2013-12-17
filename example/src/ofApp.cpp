#include "ofMain.h"

#include "ofxExternalTimeSync.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp
{
public:
	
	ofxExternalTimeSync<ofVideoPlayer> video;
	ofxOscReceiver osc_in;
	
	float ext_time;
	
	void setup()
	{
		ofSetFrameRate(60);
		ofSetVerticalSync(true);
		ofBackground(0);
		
		video.loadMovie("../../../../../examples/video/videoPlayerExample/bin/data/movies/fingers.mov");
		osc_in.setup(9999);
		
		ext_time = 0;
	}
	
	void update()
	{
		while (osc_in.hasWaitingMessages())
		{
			ofxOscMessage m;
			osc_in.getNextMessage(&m);
			
			float t = m.getArgAsFloat(0);
			video.setTime(t);
			
			ext_time = t;
		}
		
		video.update();
	}
	
	void draw()
	{
		video.draw(0, 0);
		
		stringstream ss;
		ss << "OSC time: " << ext_time << endl;
		ss << "movie time: " << (video.getPosition() * video.getDuration()) << endl;
		ss << endl;
		
		ss << "video.isJamSyncing(): " << video.isJamSyncing() << endl;
		ss << "video.getTimeDiff(): " << video.getTimeDiff() << endl;
		ss << "video.isPlaying(): " << video.isPlaying() << endl;
		ss << "video.getSpeed(): " << video.getSpeed() << endl;
		
		ofDrawBitmapString(ss.str(), 320, 10);
	}

	void keyPressed(int key)
	{
	}

	void keyReleased(int key)
	{
	}
	
	void mouseMoved(int x, int y)
	{
	}

	void mouseDragged(int x, int y, int button)
	{
	}

	void mousePressed(int x, int y, int button)
	{
	}

	void mouseReleased(int x, int y, int button)
	{
	}
	
	void windowResized(int w, int h)
	{
	}
};


int main(int argc, const char** argv)
{
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp);
	return 0;
}
