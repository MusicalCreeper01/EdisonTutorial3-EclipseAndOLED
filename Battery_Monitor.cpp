#include "mraa.hpp"
#include "sparkfun/oled/Edison_OLED.h"
#include "sparkfun/gpio/gpio.h"
#include "math.h"

#include <iostream>
using namespace std;

edOLED oled;

int toint(std::string s);

void SetupOLED();
void DrawOLED();

int getBatPrecentage();
std::string getCmdOutput(const char * mStr);

int toint(std::string s)
{
    return atoi(s.c_str());
}

int main() {
	SetupOLED();
	while (1) {
		DrawOLED();
	}
	return 0;
}

void SetupOLED (){
	oled.begin();
	oled.clear(PAGE);
	oled.display();
}

void DrawOLED(){
	oled.clear(PAGE);

	int width = oled.getLCDWidth() - (oled.getLCDWidth()/3);
	int height = oled.getLCDHeight()/2;

	int batWidth = ((getBatPrecentage() * (width-2)) / 100);

	int connectorwidth = oled.getLCDWidth()/8;

	for (int x = 0	; x <= width	; ++x){
		for (int y = 0; y <= height; ++y){
			if((x == 0 || y == 0 || y == height || x == width) || x < batWidth)
				oled.pixel(((oled.getLCDWidth() - width - connectorwidth)/2)+x, ((oled.getLCDHeight()/2)-(height/2))+y);
		}
	}

	for (int x = 0; x <= connectorwidth; ++x){
		for (int y = 0; y <= height/2; ++y){
			oled.pixel(((oled.getLCDWidth() - width - connectorwidth)/2)+width+x, ((oled.getLCDHeight()/2)-(height/2))+(((height/2)/2)+y));
		}
	}
	oled.display();
}


int getBatPrecentage(){
	std::string batteryPercent = getCmdOutput("battery-voltage | grep level | awk '{ print $4 }' | sed 's/%//'");
	return toint(batteryPercent);
}

std::string getCmdOutput(const char * mStr){
    std::string result, file;
    FILE* pipe{popen(mStr, "r")};
    char buffer[256];
    while(fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        file = buffer;
        result += file.substr(0, file.size() - 1);
    }
    pclose(pipe);
    return result;
}
