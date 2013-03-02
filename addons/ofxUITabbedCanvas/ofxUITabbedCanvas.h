#pragma once

#include "ofxUI.h"

class ofxUIXmlCanvas : public ofxUICanvas {
public:
	ofxUIXmlCanvas();
	void saveSettingsToXml(ofxXmlSettings& xml);
	void loadSettingsFromXml(ofxXmlSettings& xml);
};

class ofxUITab : public ofxUIXmlCanvas {
protected:
	ofxUILabel* title;
	bool visible;
	bool enabled, enableable;
public:
	ofxUITab(string tabName = "", bool enableable = true)
	:visible(false)
	,enabled(false)
	,enableable(enableable)
	{
		title = addLabel(tabName, OFX_UI_FONT_LARGE);
		addSpacer();
	}
	
	void setTabName(const string& tabName) {title->setLabel(tabName);}
	string getTabName() const {return title->getLabel();}
	bool& getVisible() {return visible;}
	bool& getEnabled() {return enabled;}
	bool getEnableable() {return enableable;}
};

class ofxUITabbedCanvas : public ofxUIXmlCanvas {
protected:
	int currentTab;
	float tabWidth, enableWidth;
	bool visible;
	bool saveStatus, loadStatus;
	vector<ofxUITab*> tabs;
	vector<ofxUILabelToggle*> tabToggles;
	vector<ofxUIToggle*> enableToggles;
	ofxUIImageButton *saveButton, *loadButton;
public:
	ofxUITabbedCanvas(float tabWidth = 100, float enableWidth = 10)
	:currentTab(0)
	,saveStatus(false)
	,loadStatus(false)
	,tabWidth(tabWidth)
	,enableWidth(enableWidth)
	,visible(true) {
        loadButton = new ofxUIImageButton(0, 0, 32, 32, &loadStatus, "../../../../resources/Images/open.png", "Load");
        saveButton = new ofxUIImageButton(0, 0, 32, 32, &saveStatus, "../../../../resources/Images/save.png", "Save");
        addWidgetRight(loadButton);
        addWidgetRight(saveButton);
	}
	void add(ofxUITab* tab) {
		tab->disableAppEventCallbacks();
		tab->disableMouseEventCallbacks();
		tab->disableKeyEventCallbacks();
		tab->disableWindowEventCallbacks();
		if(tabs.empty()) {
			tab->getVisible() = true;
		}
		tabs.push_back(tab);
		ofxUILabelToggle* tabToggle = new ofxUILabelToggle(tab->getTabName(), &tab->getVisible(), tabWidth, 0, 0, 0, OFX_UI_FONT_SMALL, true);
        addWidgetDown(tabToggle);
		tabToggles.push_back(tabToggle);
		if(tab->getEnableable()) {
			ofxUIToggle* enableToggle = new ofxUIToggle("", &tab->getEnabled(), enableWidth, tabToggle->getRect()->height);
			addWidgetRight(enableToggle);
			enableToggles.push_back(enableToggle);
		}
		autoSizeToFitWidgets();
	}
	ofxUITab* at(int i) {
		return tabs[i];
	}
	int getTabIndex(string name) {
		for(int i = 0; i < tabToggles.size(); i++) {
			ofxUILabelToggle *tabToggle = tabToggles[i];		
			if(tabToggle->getName() == name) {
				return i;
			}
		}
		return -1;
	}
	void select(string name) {
		int tabIndex = getTabIndex(name);
		if(tabIndex != -1) {
			currentTab = tabIndex;
			for(int i = 0; i < tabToggles.size(); i++) {	
				tabToggles[i]->setValue(i == tabIndex);
			}
		}
	}
	void triggerEvent(ofxUIWidget *child) {
		select(child->getName());
		ofxUICanvas::triggerEvent(child);
	} 
	ofxUITab* getCurrent() {
		return tabs[currentTab];
	}
	// this should be done differently instead of overriding update()
	void update() {
		if (saveStatus) {
			ofFileDialogResult result = ofSystemSaveDialog("settings.xml", "Save settings.");
			saveSettings(result.getPath());
			saveButton->setValue(false);
		}
		if (loadStatus) {
			ofFileDialogResult result = ofSystemLoadDialog("Load settings.", false);
			loadSettings(result.getPath());
			loadButton->setValue(false);
		}
		if (!visible || tabs.empty()) return;
		getCurrent()->update();
	}
	void draw() {
		if(visible) {
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
			
			ofPushStyle();
			ofPushMatrix();
			ofNoFill();
			ofTranslate(getRect()->width, 0);
			tabs[currentTab]->draw();
			ofPopMatrix();
			ofPopStyle();
			
			ofxUICanvas::draw();
		}
	}
	void keyPressed(int key) {
		if(key == '\t') visible = !visible;
		if (!visible) return;
		ofxUICanvas::keyPressed(key);
		if (tabs.empty()) return;
		getCurrent()->keyPressed(key);
	}
	void keyReleased(int key) {
		if (!visible) return;
		ofxUICanvas::keyReleased(key);
		if (tabs.empty()) return;
		getCurrent()->keyReleased(key);
	}
	void mouseMoved(int x, int y) {
		if (!visible) return;
		ofxUICanvas::mouseMoved(x, y);
		if (tabs.empty()) return;
		getCurrent()->mouseMoved(x - getRect()->width, y);
	}
	void mouseDragged(int x, int y, int tabToggle) {
		if (!visible) return;
		ofxUICanvas::mouseDragged(x, y, tabToggle);
		if (tabs.empty()) return;
		getCurrent()->mouseDragged(x - getRect()->width, y, tabToggle);
	}
	void mousePressed(int x, int y, int tabToggle) {
		if (!visible) return;
		ofxUICanvas::mousePressed(x, y, tabToggle);
		if (tabs.empty()) return;
		getCurrent()->mousePressed(x - getRect()->width, y, tabToggle);
	}	
	void mouseReleased(int x, int y, int tabToggle) {
		if (!visible) return;
		ofxUICanvas::mouseReleased(x, y, tabToggle);
		if (tabs.empty()) return;
		getCurrent()->mouseReleased(x - getRect()->width, y, tabToggle);
	}	
	void loadSettings(const string &filename);
	void saveSettings(const string &filename);
};