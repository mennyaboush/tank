#include "GLUT.H"
#include <math.h>

const int WH = 100;
const int WW = 50;
const double PI = 4 * atan(1);

double angle = 0;
double bx = 0.8;
double dx = -0.001;
double sizeOfWeel = 0.03;
bool hasPress = false;
bool isFiring = false;
bool bomb = false;
int bombing = 0;
double  ballX ,ballY , bx0,by0,vx, vy, a = -0.15, t;// the speed of the fire bulet 

void init()
{
	glClearColor(0, 0, 1, 0); // set background color
	glOrtho(-1, 1, -1, 1, 1, -1); // set coordinate system
}
double groundFunc(double x)
{
	return    0.1*sin(x * 6);
}
void DrawGround()
{
	double x, y;
	glColor3d(1, 1, 1);
	glBegin(GL_LINE_STRIP);
	for (x = -1; x <= 1; x += 0.01)
	{
		y = groundFunc(x);
		glVertex2d(x, y);
	}
	glEnd();
}

void DrawWheel()
{
	double alpha, teta = PI / 60;
	double x, y;
	glLineWidth(5);
	glBegin(GL_LINE_LOOP);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
	}
	glEnd();
	teta = PI / 4;
	glLineWidth(1);
	glBegin(GL_LINES);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
		glVertex2d(0,0);
	}
	glEnd();

}

void drawBarrel()
{
	glBegin(GL_LINE_LOOP);
	glVertex2d(-0.09, 0.04);
	glVertex2d(-0.25, 0.04);
	glVertex2d(-0.25, 0.06);
	glVertex2d(-0.09, 0.06);
	glEnd();
}
void drawTrapez()
{
	double delta = 0.01;
	glBegin(GL_LINE_LOOP);

	glVertex2d(-3*sizeOfWeel,-sizeOfWeel);
	glVertex2d(5 * sizeOfWeel, -sizeOfWeel);

	glVertex2d(6 * sizeOfWeel, sizeOfWeel+delta);
	glVertex2d(-5 * sizeOfWeel, sizeOfWeel + delta);
	glEnd();
	
}
void drowTank()
{
	glColor3d(1, 1, 1);
	
	//draw weels.
	glPushMatrix();		
		glPushMatrix();
		glTranslated(-2*sizeOfWeel, 0, 0);
		glScaled(sizeOfWeel, sizeOfWeel, 1);
		glRotated(800 * angle, 0, 0, 1);
		DrawWheel();
		glPopMatrix();
			
		glPushMatrix();
		glScaled(sizeOfWeel, sizeOfWeel, 1);
		glRotated(800 * angle, 0, 0, 1);
		DrawWheel();
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(sizeOfWeel*2, 0, 0);
		glScaled(sizeOfWeel, sizeOfWeel, 1);
		glRotated(800 * angle, 0, 0, 1);
		DrawWheel();
		glPopMatrix();

		glPushMatrix();
		glTranslated(4*sizeOfWeel, 0, 0);
		glScaled(sizeOfWeel, sizeOfWeel, 1);
		glRotated(800 * angle, 0, 0, 1);
		DrawWheel();
		glPopMatrix();
		
		//draw bottom trapeze
		glLineWidth(3);
		drawTrapez();
	
		//draw midel trapeze
		glPushMatrix();
		glLineWidth(2);
		glTranslated(0,2*sizeOfWeel,0);
		glScaled(0.7, -0.5, 1);
		drawTrapez();
		glPopMatrix();

		//draw upper trapeze
		glPushMatrix();
		glTranslated(0.02,  3*sizeOfWeel, 0);
		glScaled(0.4, -0.3, 1);
		drawTrapez();
		glPopMatrix();

		//draw barrel.
		glPushMatrix();
		glRotated(-35, 0, 0, 1);
		drawBarrel();
		glPopMatrix();

	glPopMatrix();

	
}
void drawFire()
{
	glPushMatrix();
	glTranslated(ballX+0.16, ballY+0.04, 0);
	glRotated(800 * angle, 0, 0, 1);
	glTranslated(-ballX, -ballY, 0);
	glPointSize(7);
	glBegin(GL_POINTS);
	glVertex2d(ballX, ballY);
	glEnd();
	glPopMatrix();
}
void fire(double bx , double by)
{
	bomb = false;
	isFiring = true;

	bx0 = bx -0.2;
	by0 = by +0.2;
	vx = -0.3;
	vy = 0.35;
	}

void display()
{
	double  by;
	double beta;
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	glLoadIdentity(); // start the transformations from the beginning

	DrawGround();
	
	glPushMatrix();
	by = 0.05 + groundFunc(bx); // 0.05 is the radius of the wheel

		beta = atan(0.6*cos(6*bx)); // beta is in radian

		glTranslated(bx, by,0);
		glRotated(beta * 180 / PI, 0, 0, 1);
		if (dx > 0)
			glScaled(-1, 1, 1);
		if (hasPress)
		{
			fire(bx,by);
			hasPress = false;
		}
		
		drowTank();
	glPopMatrix();
	if (isFiring)
		drawFire();
	if (bomb && bombing < 200)
	{
		bombing++;
		glPointSize(bombing);
		glColor3d(1, 1, 102/255);
		glBegin(GL_POINTS);
		glVertex2d(ballX,ballY);
		glEnd();
		glColor3d(1,1,1);
	}

	glutSwapBuffers();
}
void idle()
{
	angle+=0.001;
	if (isFiring)
	{
		t += 0.01;
		ballX = bx0 + vx * t;
		ballY = by0 + vy * t + a * t*t;

	}
	else
	{
		bx += dx;
	}

	if (groundFunc(ballX)  > ballY)//check hit in ground
	{
		isFiring = false;
		bomb = true;
	}
	glutPostRedisplay(); // indirect call to display
}

void keyBordFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 32:
		hasPress = true;
		break;
	default:
		break;
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Basic graphics app");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyBordFunc);

	init();

	glutMainLoop();
}