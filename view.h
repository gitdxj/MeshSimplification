#include <GL/glut.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <sstream> 
#include <fstream>
using namespace std;

void setLightRes();
void init();
void display();
void reshape(int width, int height);
void moseMove(int button, int state, int x, int y);
void changeViewPoint(int x, int y);
void myIdle();
void readObj(string filename);
void Draw();
void show();
//安置光源

