#include"view.h"

static vector<vector<GLfloat>> vSets;
static vector<vector<GLint>> fSets;
GLfloat roate = 0.0;// set rote of roate ying yu bu hao  bu zhuang le 设置旋转速率
GLfloat rote = 0.0;//shezhi旋转角度
GLfloat anglex = 0.0;//X 轴旋转
GLfloat angley = 0.0;//Y 轴旋转
GLfloat anglez = 0.0;//Z 轴旋转
GLfloat oldx;//当左键按下时记录鼠标坐标  
GLfloat oldy;
GLint WinW = 400;
GLint WinH = 400;

void setLightRes() {
    GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    glEnable(GL_LIGHT0);   //使用指定灯光
	glEnable(GL_LIGHTING); //启用光源
}

//初始化
void init() {
	//cout << "void init()" << endl;
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 800);
    glutCreateWindow("Mesh Simplification");
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    setLightRes();
    glEnable(GL_DEPTH_TEST);
}

void display()
{
	//cout << "void display()" << endl;
    glColor3f(1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);                            
    glLoadIdentity();                                     
    glTranslatef(0.0f, 0.0f, -5.0f);                                                              
    setLightRes();
    glPushMatrix();

    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(rote, 0.0f, 1.0f, 0.0f);
	glRotatef(anglex, 1.0, 0.0, 0.0);
	glRotatef(angley, 0.0, 1.0, 0.0);
	glRotatef(anglez, 0.0, 0.0, 1.0);


    Draw();//绘制obj模型
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int width, int height)
{
	//cout << "void reshape()" << endl;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

//移动鼠标360观察模型
void moseMove(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			roate = 0;
			rote = 0;
			oldx = x;//当左键按下时记录鼠标坐标  
			oldy = y;
			//cout << "left" << endl;
		}

	}
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			roate += 1.0f;
			cout << "right" << endl;
		}

	}

}
void changeViewPoint(int x, int y)
{
	GLint deltax = oldx - x;
	GLint deltay = oldy - y;
	anglex += 360 * (GLfloat)deltax / (GLfloat)WinW;//根据屏幕上鼠标滑动的距离来设置旋转的角度  
	angley += 360 * (GLfloat)deltay / (GLfloat)WinH;
	anglez += 360 * (GLfloat)deltay / (GLfloat)WinH;
	oldx = x;//记录此时的鼠标坐标，更新鼠标坐标  
	oldy = y;//若是没有这两句语句，滑动是旋转会变得不可控  
	glutPostRedisplay();
	glutPostRedisplay();

}

void myIdle()
{
    glutPostRedisplay();
}



void readObj(string filename)
{
    std::ifstream file(filename);
    std::string line;
while (getline(file, line))
{
    if (line.substr(0, 2) == "vt")
    {

    }
    else if (line.substr(0, 2) == "vn")
    {

    }
    else if (line.substr(0, 1) == "v")
    {
        vector<GLfloat> Point;
        GLfloat x, y, z;
        std::istringstream s(line.substr(2));
        s >> x; s >> y; s >> z;
        Point.push_back(x);
        Point.push_back(y);
        Point.push_back(z);
        vSets.push_back(Point);

    }
    else if (line.substr(0, 1) == "f")
    {
        vector<GLint> vIndexSets;
        GLint u, v, w;
        std::istringstream vtns(line.substr(2));
        vtns >> u; vtns >> v; vtns>> w;
        vIndexSets.push_back(u-1);
        vIndexSets.push_back(v-1);
        vIndexSets.push_back(w-1);
        fSets.push_back(vIndexSets);
    }
    else if (line.substr(0, 1) == "#") {}
}
	//cout << "All data read" << endl;	
file.close();
}



void Draw(){
	
	//cout << "start drawing " << vSets.size() << " vertexes " << fSets.size() << " faces" << endl;
    glBegin(GL_TRIANGLES);//开始绘制
    for (int i = 0; i < fSets.size(); i++) {
        GLfloat VN[3];
        //三个顶点
        GLfloat SV1[3];  // 三个分量代表三维坐标
        GLfloat SV2[3];
        GLfloat SV3[3];

        if ((fSets[i]).size() != 3) {
            cout << "the fSetsets_Size is not correct" << endl;
        }
        else {
				// 每一个面包含三个索引点
                GLint firstVertexIndex = (fSets[i])[0];//取出顶点索引
                GLint secondVertexIndex = (fSets[i])[1];
                GLint thirdVertexIndex = (fSets[i])[2];
				//第一个顶点
                SV1[0] = (vSets[firstVertexIndex])[0]; // x
                SV1[1] = (vSets[firstVertexIndex])[1]; // y
                SV1[2] = (vSets[firstVertexIndex])[2]; // z
				//第二个顶点
                SV2[0] = (vSets[secondVertexIndex])[0]; 
                SV2[1] = (vSets[secondVertexIndex])[1];
                SV2[2] = (vSets[secondVertexIndex])[2];
				//第三个顶点
                SV3[0] = (vSets[thirdVertexIndex])[0]; 
                SV3[1] = (vSets[thirdVertexIndex])[1];
                SV3[2] = (vSets[thirdVertexIndex])[2];


                GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
				//(x1-x2,y1-y2,z1-z2)  vect1 = sv1-sv2
                vec1[0] = SV1[0] - SV2[0];
                vec1[1] = SV1[1] - SV2[1];
                vec1[2] = SV1[2] - SV2[2];

                //(x1-x3,y1-y3,z1-z3)  vect2 = sv1-sv3
                vec2[0] = SV1[0] - SV3[0];
                vec2[1] = SV1[1] - SV3[1];
                vec2[2] = SV1[2] - SV3[2];

                //vect3 = crossProduct(vect1, vect2)
                vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
                vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
                vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

                GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

                VN[0] = vec3[0] / D;
                VN[1] = vec3[1] / D;
                VN[2] = vec3[2] / D;
           
                glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量

                glVertex3f(SV1[0], SV1[1], SV1[2]);//绘制三角面片
                glVertex3f(SV2[0], SV2[1], SV2[2]);
                glVertex3f(SV3[0], SV3[1], SV3[2]);    
        }
    }
    glEnd();
}

void show(){
    glutInit(NULL, NULL);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(moseMove);
    glutMotionFunc(changeViewPoint);
    glutIdleFunc(myIdle);
    glutMainLoop();
}
