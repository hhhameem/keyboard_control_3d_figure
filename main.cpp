#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
int drawcone = 0;
int drawcircle = 0;
double angle;
int wp,rap,upap,mrp = 0;


double camera_look_x, camera_look_y, camera_look_z;


double translate_x = 0;
double increment_x = 0.5;

double translate_y = 0;
double increment_y = 0.5;

double translate_z = 0;
double increment_z = 0.5;

int move_sphere_x = 0;
int move_sphere_y = 0;
int move_sphere_z = 0;

int rotate_sphere_clockwise = 0;

int scale_x=0;
int scale_y=0;
int scale_multiple_x = 1;
int scale_multiple_y = 1;

int camera_up_x, camera_up_y, camera_up_z;

struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}


void drawSphere(double radius)
{
	struct point points[100][100];
	int i,j;
	double h,r, t1, t2,shade;
	//generate points
	for(t1 = 0, i = 0; t1 <= 1; i++, t1 += 0.01)
	{
		h=radius*sin((pi/2)*t1);
		r=radius*cos((pi/2)*t1);
		for(t2 = 0, j = 0; t2 <= 1; j++, t2 += 0.01)
		{
			points[i][j].x = r*cos(2*pi*t2);
			points[i][j].y = r*sin(2*pi*t2);
			points[i][j].z = h;
		}
	}
	int num_of_stacks = i - 1;
	int num_of_points = j - 1;
	//draw quads using generated points
	for(t1 = 0, i = 0; i < num_of_stacks; i++, t1 += 0.01)
	{
	    if(i < num_of_points/2)
            shade = 2*t1;
        else shade = 2*(1.0 - t1);
        glColor3f(shade,0,shade);
        //glColor3f(t1,t1,t1);
		for(t2 = 0, j = 0; j <= num_of_points; j++, t2 += 0.01)
		{
		    //if (i == 5 && j == 5) glColor3f(1, 0, 0);
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    glColor3f(shade,0,0);
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

                glColor3f(0,shade,0);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;
        case 'w':
            wp+=1;
            rotate_sphere_clockwise = 1;
            break;
        case 's':
            wp-=1;
            rotate_sphere_clockwise = 2;
            break;
        case 'y':
            scale_multiple_x+=1;
            scale_x = 1;
            break;
        case 'u':
            scale_multiple_x-=1;
            scale_x = 2;
            break;
        case 'i':
            scale_multiple_y+=1;
            scale_y = 1;
            break;
        case 'o':
            scale_multiple_y-=1;
            scale_y = 2;
            break;
		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
            if(increment_y<0)
                increment_y+=1;
            else if(increment_y>0)
                increment_y-=1;
			move_sphere_y = 1;
			break;
		case GLUT_KEY_UP:		// up arrow key
			if(increment_y<0)
                increment_y-=1;
            else if(increment_y>0)
                increment_y+=1;
			move_sphere_y = 2;
			break;

		case GLUT_KEY_RIGHT:
		    if(increment_x<0)
                increment_x-=1;
            else if(increment_x>0)
                increment_x+=1;
			move_sphere_x = 1;
			break;
		case GLUT_KEY_LEFT:
		    if(increment_x<0)
                increment_x+=1;
            else if(increment_x>0)
                increment_x-=1;
			move_sphere_x = 2;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
		    camera_up_x = 0;
            camera_up_y = 0;
            camera_up_z = 1;
			break;

		case GLUT_KEY_HOME:
            camera_up_x = 1;
            camera_up_y = 0;
            camera_up_z = 0;
			break;
		case GLUT_KEY_END:
		    camera_up_x = 0;
            camera_up_y = 1;
            camera_up_z = 0;
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
                if(increment_x<0)
                    increment_x+=1;
                else if(increment_x>0)
                    increment_x-=1;
                move_sphere_z = 2;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN){
                if(increment_x<0)
                    increment_x-=1;
                else if(increment_x>0)
                    increment_x+=1;
                move_sphere_z = 1;
			}
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,
           		camera_look_x,camera_look_y,camera_look_z,
                    camera_up_x,camera_up_y,camera_up_z);
	//gluLookAt(0,0,200,	0,0,0,	camera_up_x,camera_up_y,camera_up_z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects
    drawAxes();
    drawGrid();
    if (rotate_sphere_clockwise == 1)
        glRotatef(wp*angle,1,0,0);
    else if(rotate_sphere_clockwise == 2)
        glRotatef(wp*angle,1,0,0);
    if(move_sphere_x == 1)
        glTranslatef(translate_x, 0, 0);
    else if (move_sphere_x == 2)
        glTranslatef(-translate_x, 0, 0);
    else if (move_sphere_y == 1)
        glTranslatef(0, translate_y, 0);
    else if (move_sphere_y == 2)
        glTranslatef(0, -translate_y, 0);
    else if (move_sphere_z == 1)
        glTranslatef(0, 0, translate_z);
    else if (move_sphere_z == 2)
        glTranslatef(0, 0, -translate_z);
    else if(scale_y == 1)
        glScalef(1,scale_multiple_y, 1);
    else if (scale_y == 2)
        glScalef(1,scale_multiple_y, 1);
    else if(scale_x == 1)
        glScalef(scale_multiple_x, 1, 1);
    else if (scale_x == 2)
        glScalef(scale_multiple_x, 1, 1);
    drawSphere(30);


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
    angle += 0.5;
    translate_x +=increment_x;
    translate_y +=increment_y;
    translate_z +=increment_z;
    if (translate_x >= 100 || translate_x <= -100) {
        increment_x = -increment_x;
    }
    else if (translate_y >= 100 || translate_y <= -100) {
        increment_y = -increment_y;
    }
    else if (translate_z >= 100 || translate_z <= -100) {
        increment_z = -increment_z;
    }
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	camera_up_x = camera_up_y = 0;
	camera_up_z = 1;
	camera_look_x = camera_look_y = camera_look_z = 0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
