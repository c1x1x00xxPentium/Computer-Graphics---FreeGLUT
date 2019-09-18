//#include <iostream>
//#include <math.h>
//#include <Windows.h>
//#include <stdlib.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>

#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

using namespace std;


//Point class for taking the points
class Point {
public:
	float x, y;
	void setxy(float x2, float y2)
	{
		x = x2; y = y2;
	}
	//operator overloading for '=' sign
	const Point & operator=(const Point &rPoint)
	{
		x = rPoint.x;
		y = rPoint.y;
		return *this;
	}

};

int factorial(int n)
{
	if (n <= 1)
		return(1);
	else
		n = n * factorial(n - 1);
	return n;
}

float binomial_coff(float n, float k)
{
	float ans;
	ans = factorial(n) / (factorial(k)*factorial(n - k));
	return ans;
}

Point abc[20];
int SCREEN_HEIGHT = 500;
int points = 0;
int clicks = 0;

void myInit() {
	glClearColor(0.5, 0.5, 0.5, 0.0);
	//glShadeModel(GL_FLAT);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(8);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 500.0);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glutSwapBuffers();

	//glClearColor(0.5, 0.5, 0.5, 0.0);
	//	glColor3f(0.0, 0.0, 0.0);
	//	glPointSize(5);
	//	glMatrixMode(GL_PROJECTION);
	//	glLoadIdentity();
	//	gluOrtho2D(0.0, 640.0, 0.0, 500.0);
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	//glutSwapBuffers();
	//glFlush();
}

void drawDot(int x, int y) {
	//glClear(GL_COLOR_BUFFER_BIT);
	//glViewport(0, 0, (GLsizei)x, (GLsizei)y);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//glPointSize(10.0);
	glBegin(GL_POINTS);
	//glColor3f(1.0, 0.0, 0.0);
	glVertex2i(x, y);
	glEnd();

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glBegin(GL_POINTS);
	//glColor3f(1.0, 0.0, 0.0);
	//glVertex2i(x, y);
	//glEnd();
	//glutSwapBuffers();
	glFlush();
}

void drawLine(Point p1, Point p2) {

	//glClear(GL_COLOR_BUFFER_BIT);
	//glViewport(0, 0, (GLsizei)x, (GLsizei)y);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

	//glPointSize(10.0);
	glBegin(GL_LINES);
	glLineWidth(3);
	//glColor3f(0.0, 1.0, 0.0);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glutSwapBuffers();
	glFlush();
}

//Calculate the bezier point
Point drawBezier(Point PT[], double t) {
	Point P;
	P.x = pow((1 - t), 3) * PT[0].x + 3 * t * pow((1 - t), 2) * PT[1].x + 3 * (1 - t) * pow(t, 2)* PT[2].x + pow(t, 3)* PT[3].x;
	P.y = pow((1 - t), 3) * PT[0].y + 3 * t * pow((1 - t), 2) * PT[1].y + 3 * (1 - t) * pow(t, 2)* PT[2].y + pow(t, 3)* PT[3].y;

	return P;
}

//Calculate the bezier point [generalized]
Point drawBezierGeneralized(Point PT[], double t) {
	Point P;
	P.x = 0; P.y = 0;
	for (int i = 0; i < clicks; i++)
	{
		P.x = P.x + binomial_coff((float)(clicks - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (clicks - 1 - i)) * PT[i].x;
		P.y = P.y + binomial_coff((float)(clicks - 1), (float)i) * pow(t, (double)i) * pow((1 - t), (clicks - 1 - i)) * PT[i].y;
	}
	std::cout << "P.X = " << P.x << std::endl << "P.Y = " << P.y;
	std::cout << std::endl << std::endl;
	std::cout << "==============================\n==============================" << std::endl;
	return P;
}


void myMouse(int button, int state, int x, int y) {
	// If left button was clicked
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Store where mouse was clicked, Y is backwards.
		clicks += 1;
		abc[points].setxy((float)x, (float)(SCREEN_HEIGHT - y));
		points++;

		// Draw the red  dot.
		drawDot(x, SCREEN_HEIGHT - y);

		// If (click-amout) points are drawn do the curve.
		//if (points == clicks)
		if( points > 2)
		{
			glColor3f(0.0, 1.0, 0.0);
			// Drawing the control lines
			//for (int k = 0; k < clicks - 1; k++) { drawLine(abc[k], abc[k + 1]); }
			for (int k = 0; k < clicks - 1; k++) { glLineWidth(3); drawLine(abc[k], abc[k + 1]); }

			Point p1 = abc[0];
			// Draw each segment of the curve.Make t increment in smaller amounts for a more detaile curve.
			for (double t = 0.0; t <= 1.0; t += 0.02)
			{
				Point p2 = drawBezierGeneralized(abc, t);
				cout << "p1.X = " << p1.x << "  ,p1.Y = " << p1.y << endl;
				cout << "p2.X = " << p2.x << "  ,p2.Y = " << p2.y << endl;
				cout << endl;
				glColor3f(1.0, 0.0, 0.0);
				glLineWidth(8);
				drawLine(p1, p2);
				glLineWidth(3);
				p1 = p2;
			}
			glColor3f(0.0, 0.0, 0.0);

			//points = 0;
		}
		else if(points == 2){
			for (int k = 0; k < clicks - 1; k++) { glLineWidth(3); drawLine(abc[k], abc[k + 1]); }
		}
		//else if(points % 2 == 0 || points >= 2){drawLine(abc[points], abc[points + 1]);}
	}
}


int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Bezier Curve - Grafkom 13-Sept-2019 ++ Sugiyanto G(c14170075)");
	myInit();
	glutMouseFunc(myMouse);
	glutDisplayFunc(myDisplay);
	glutMainLoop();

	return 0;
}