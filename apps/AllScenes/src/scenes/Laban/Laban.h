#pragma once

#include "ramMain.h"

class Laban : public ramBaseScene
{
public:
	
	float threshold, lineWidth, lineLength, scale, ticks;
	bool showLines, showPlanes;
	
	vector<ofColor> labanColors;
	vector<ofVec3f> labanDirections;
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		threshold = .1;
		lineWidth = 2;
		scale = 20;
		ticks = 3;
		lineLength = 100;
		showLines = true;
		showPlanes = false;
		panel->addSlider("Threshold", 0, .5, &threshold, 300, 20);
		panel->addToggle("Show lines", &showLines, 20, 20);
		panel->addSlider("Line width", 0, 10, &lineWidth, 300, 20);
		panel->addSlider("Line length", 0, 1000, &lineLength, 300, 20);
		panel->addToggle("Show planes", &showPlanes, 20, 20);
		panel->addSlider("Scale", 0, 1000, &scale, 300, 20);
		panel->addSlider("Ticks", 0, 10, &ticks, 300, 20);
	}
	
	void setup()
	{
		for(int x = -1; x <= +1; x++)
		{
			for(int y = -1; y <= +1; y++)
			{
				for(int z = -1; z <= +1; z++)
				{
					if(!(x == 0 && y == 0 && z == 0))
					{
						ofVec3f cur(x, y, z);
						cur.normalize();
						labanDirections.push_back(cur);
						float red = ofMap(x, -1, 1, 0, 255);
						float green = ofMap(y, -1, 1, 0, 255);
						float blue = ofMap(z, -1, 1, 0, 255);
						labanColors.push_back(ofColor(red, green, blue));
					}
				}
			}
		}
	}
	
	void update()
	{
	}
	
	void draw()
	{	
	}
	
	void drawActor(ramActor &actor)
	{	
		for (int i=0; i<actor.getNumNode(); i++)
		{
			ramNode &node = actor.getNode(i);
			ofSetColor(255);
			ofSetLineWidth(lineWidth);
			if(node.hasParent())
			{
				ofVec3f start = node.getGlobalPosition();
				ofVec3f end = node.getParent()->getGlobalPosition();
				ofVec3f direction = (end - start);
				direction.normalize();
				if(showLines)
				{
					int nearestChoice;
					float nearestDistance;
					for(int i = 0; i < labanDirections.size(); i++)
					{
						float distance = direction.distance(labanDirections[i]);
						if(i == 0 || distance < nearestDistance)
						{
							nearestChoice = i;
							nearestDistance = distance;
						}
					}
					if(nearestDistance < threshold)
					{
						ofPushStyle();
						ofSetColor(128);
						ofLine(start, start + direction * lineLength);
						ofSetColor(labanColors[nearestChoice]);
						ofLine(start, start + labanDirections[nearestChoice] * lineLength);
						ofPopStyle();
					}
				}
				if(showPlanes)
				{
					if(abs(direction.x) < threshold)
					{
						ofPushMatrix();
						ofTranslate(start);
						ofDrawGrid(scale, ticks, false, true, false, false);
						ofPopMatrix();
					}
					if(abs(direction.y) < threshold)
					{
						ofPushMatrix();
						ofTranslate(start);
						ofDrawGrid(scale, ticks, false, false, true, false);
						ofPopMatrix();
					}
					if(abs(direction.z) < threshold)
					{
						ofPushMatrix();
						ofTranslate(start);
						ofDrawGrid(scale, ticks, false, false, false, true);
						ofPopMatrix();
					}
				}
			}
		}
		ofPopStyle();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
	}
	
	const string getName() { return "Laban"; }
	
};
