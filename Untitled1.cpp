#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <cmath>
 

using namespace std;

const int d=10;
float xx,yy,dx,dy,tatta,tang,m,PI = 3.1415;	//player incremental position
float ui = 5;								//unit increment of player
float cartx = 600, carty = 600;
bool keybuffer[256];

int map[d+1][d+1] = {{1,1,1,1,1,1,1,1,1,1},
			   	 {1,0,0,0,0,0,0,1,0,1},
			   	 {1,0,0,0,0,0,0,1,0,1},
			   	 {1,0,0,0,0,1,1,1,0,1},
			   	 {1,0,0,0,0,0,0,0,0,1},
			   	 {1,0,0,0,0,0,0,0,0,1},
			   	 {1,0,0,1,0,0,0,0,0,1},
			   	 {1,0,0,0,0,0,0,0,0,1},
			   	 {1,0,0,0,0,0,0,0,0,1},
				 {1,1,1,1,1,1,1,1,1,1}};

float gusx = cartx/10; //GridUnitSize


void bashyohead(float nut){
	float vx,vy=0,hx,hy,cotg;
	int rem,dir;
	tang = tan(nut);
	dir = (cos(nut)/abs(cos(nut)));
	rem = (int)xx%(int)gusx;
	vx = xx-rem+gusx*(1+cos(nut)/abs(cos(nut)))/2;
	for(short int i = 0; i<10 ;i++){
		tang = abs(tang)>58?57*tang/abs(tang):tang;
		vy = yy + tang*(vx - xx);
		vy = abs(vy)>600?599:vy;
		if (map[(int) (vy/gusx)][(int) (vx/gusx -(1-cos(nut)/abs(cos(nut)))/2)]==1){break;}
		vx+=gusx*dir;
		vx = abs(vx)>600?599:vx;
		vx = vx<0?0.1:vx;
	}
		
	cotg = 1/tan(nut);
	
	dir = (sin(nut)/abs(sin(nut)));
	rem = (int)yy%(int)gusx;
	hy = yy-rem+gusx*(1+sin(nut)/abs(sin(nut)))/2;
	for(short int i = 0; i<10 ;i++){
		cotg = abs(cotg)>58?57*(cotg)/abs(cotg):(cotg);
		hx = xx + (cotg)*(hy - yy);
		hx = hx>600?599:hx;
		if (map[(int) (hy/gusx -(1-sin(nut)/abs(sin(nut)))/2)][(int) (hx/gusx)]==1){break;}
		hy+=gusx*dir;
		hy = hy>600?599:hy;
		hy = hy<0?0.1:hy;
	}
		
	if (sqrt((xx-hx)*(xx-hx)+(yy-hy)*(yy-hy))>sqrt((xx-vx)*(xx-vx)+(yy-vy)*(yy-vy))){
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(vx,vy);
	glVertex2f(xx,yy);
	glEnd();	
	}
	else{
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex2f(hx,hy);
	glVertex2f(xx,yy);
	glEnd();
	}

}

void rays (){
	for (int o = 0; o<600; o++){
	bashyohead(tatta - PI/4 + o*0.0026167);
	}
}

void keybuf(unsigned char key, int x, int y){
	keybuffer[(int)key] = true;
	
	if (keybuffer['w'] == true){yy+=ui*sin(tatta);xx+=ui*cos(tatta);}
	if (keybuffer['a'] == true){tatta+=0.1;if (tatta>(2.5)*PI){tatta=PI/2;}tang=tan(tatta);}
	if (keybuffer['s'] == true){yy-=ui*sin(tatta);xx-=ui*cos(tatta);}
	if (keybuffer['d'] == true){tatta-=0.1;if (tatta<(-1.5)*PI){tatta=PI/2;}tang=tan(tatta);}
	
	glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y){
	keybuffer[key] = false;
	glutPostRedisplay();
}

void playerhead(){
	glColor3f(1.0, 1.0, 1.0);
	glPointSize(3);
	glBegin(GL_POINTS);
	glVertex2i(xx+5*ui*cos(tatta),yy+5*ui*sin(tatta));
	glEnd();
}

void Grid(){
	
	glClearColor(0.0, 0.4, 0.4, 0.0);
	glColor4f(0,0,0,1);
	
	
	glLineWidth(1);
	float t = (cartx/d);
	for (float i=0; i<=cartx; i=i+t){
		glBegin(GL_LINES);
		glVertex2d(i, carty);
		glVertex2d(i, 0);
		glEnd();
	}
	t=(carty/d);
	for (float i=0; i<=carty; i=i+t){
		glBegin(GL_LINES);
		glVertex2d(cartx, i);
		glVertex2d(0, i);
		glEnd();
	}
	glFlush();
}
//veggie function

void fillmap(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(0.4, 0.4, 0.4);
	float xt=(cartx/d);
	float yt=(carty/d);
	for (int y=0;y<d;y++){
		for (int x=0;x<d;x++){
			if (map[x][y]==1){
				glBegin(GL_QUADS);
				glVertex2d(xt*y, yt*x);
				glVertex2d(xt*y + xt, yt*x);
				glVertex2d(xt*y+xt, yt*x+yt);
				glVertex2d(xt*y, yt*x+yt);
				glEnd();
			}
		}
	}
	
	glFlush();
}

void Player(){
	float psize = 5; //actual size will be 2 times of this
	glColor3f(1.0, 1.6, 0.0);
	glPointSize(6);
	glBegin(GL_POINTS);
	glVertex2i(xx,yy);
	glEnd();
	rays();
	glutPostRedisplay();
}

void Mousetrack(int mx, int my){
	float mxx=mx, myy=my;
	xx=mxx;
	yy=abs(carty-myy);
	printf("mxx = %.2f, myy = %.2f",mxx,myy);
	glutPostRedisplay();
}

void PlayerMovs(){
	Player();
	playerhead();
}

void hoop(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fillmap();
	Grid();
	PlayerMovs();
	glutSwapBuffers();

}

int main(int argc, char** argv)
{ 
	xx=100;yy=100;
	tatta = PI/2;tang = tan(tatta);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(cartx, carty);
	glutCreateWindow("tootte");
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, cartx, 0, carty);
	glutDisplayFunc(hoop);
	glutKeyboardFunc(keybuf);
	glutKeyboardUpFunc(keyup);
	//glutPassiveMotionFunc(MouseLook);
	
	glutMainLoop();
 
 return 0;
}


