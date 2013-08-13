// 
// testApp.cpp - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "testApp.h"

ramExpansion expansion;
ramLowPassFilter lowpass;
ramGhost ghost;
ramPendulum pendulum;
ramUpsideDown upsideDown;
ramTimeShifter timeShifter;

bool bDrawActor;
bool bDrawName;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	/// ram setup
	// ------------------
	ramInitialize(10000);
	
	ramGetGUI().addPanel( &expansion );
	ramGetGUI().addPanel( &lowpass );
	ramGetGUI().addPanel( &ghost );
	ramGetGUI().addPanel( &pendulum );
	ramGetGUI().addPanel( &upsideDown );
	ramGetGUI().addPanel( &timeShifter );
	
	bDrawName = false;
	bDrawActor = true;
}

//--------------------------------------------------------------
void testApp::update()
{
	
}	

//--------------------------------------------------------------
void testApp::draw()
{
	ramBeginCamera();
	
	if (getNumNodeArray() > 0)
	{
		// original data
		const ramNodeArray &NA = getNodeArray(0);
		
		
		// filtering
		// each of filter can be enable/disable from GUI
		const ramNodeArray &f1 = lowpass.update(NA);
		const ramNodeArray &f2 = expansion.update(f1);
		const ramNodeArray &f3 = upsideDown.update(f2);
		const ramNodeArray &f4 = timeShifter.update(f3);
		const ramNodeArray &f5 = ghost.update(f4);
		const ramNodeArray &f6 = pendulum.update(f5);
		
		
		// draw filterd actor with blue
		ofSetColor(ramColor::BLUE_LIGHT);
		for (int n=0; n<NA.getNumNode(); n++)
		{
			const ramNode& node = f6.getNode(n);
			node.beginTransform();
			ofNoFill();
			ofDrawBox(5);
			ofDrawAxis(5);
			node.endTransform();
			
			if (bDrawName)
				node.drawNodeName();
		}
		
		
		// draw lines between original and filterd with gray
		ofSetColor(ramColor::LIGHT_GRAY_ALPHA);
		ramDrawNodeCorresponds(NA, f3);
	}
	
	ramEndCamera();
	
	ofSetColor(ramColor::RED_DEEP);
	ofDrawBitmapString("press [h] to hide original actor", ofGetWidth()/2, 20);
	ofDrawBitmapString("press [n] to draw node name", ofGetWidth()/2, 40);
}


#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
	if (bDrawActor)
	{
		// draw basic actor with yellow
		ofSetColor(ramColor::YELLOW_DEEP);
		ofFill();
		ramDrawBasicActor(actor);
	}
}

//--------------------------------------------------------------
void testApp::drawRigid(const ramRigidBody &rigid)
{
	
}


#pragma mark - ram Events
//--------------------------------------------------------------
void testApp::onActorSetup(const ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::onActorExit(const ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::onRigidSetup(const ramRigidBody &rigid)
{
	
}

//--------------------------------------------------------------
void testApp::onRigidExit(const ramRigidBody &rigid)
{
	
}


#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if (key == 'h')
	{
		bDrawActor ^= true;
	}
	
	if (key == 'n')
	{
		bDrawName ^= true;
	}
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
}
