//Meeshaan Shah
//final
//asteroids game

#ifndef MAC //if not MAC
#include <GL\glut.h>
#else // MAC includes below
#include <GLUT/glut.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

//set up constants
const int numStars = 100;			//number of stars
const int numAsteroids = 6;			//number of asteroids
//for coordinate limits
float xmin = -50.0;
float xmax = 50.0;
float ymin = -50.0;
float ymax = 50.0;

//set up variables
float shipAngle = 0;
float bulletAngle = 0;

float frameStart = 0;				//time variables
float frameEnd = 0;

float bulletX = 0.0;				//bullet coord
float bulletY = 0.0;
float starX[numStars];				//arrays for star coords
float starY[numStars];
float asteroidX[numAsteroids];		//arrays for asteroid coords
float asteroidY[numAsteroids];

bool isBulletAlive = false;			//to detect if bullet exists
bool isAsteroidAlive[numAsteroids];	//to detect if asteroids exist

char scoreStr[5];	//to store scores to transfer to strings
int score = 0;		//score 

int numAstDest = 0;	//for how many asteroids are destroyed

//create a random number in a specified range
float getRandom(int range)
{	
	float randNum;
	randNum = (rand() % range);
	return randNum;
}

//collision detection for ship and asteroids
bool hasCollided(float asteroidX, float asteroidY)	
{
	//bounding boxes for ship and asteroids
	int asteroidWidth = 11;
	int asteroidHeight = 10;
	int shipWidth = 6;
	int shipHeight = 6;

	//gets x and y difference b/n ship and asteroid and compares bounding boxes
	if ((abs(0.0 - asteroidX) < (shipWidth + asteroidWidth)/2) && (abs(0.0 - asteroidY) < (shipHeight + asteroidHeight)/2)) 
	{
		//printf("Collision\n");
		return true;
	}
	else
	{
		//printf("No Collision\n");
		return false;
	}
}

//set up stars to have random coord
void setStarCoords()
{
	for (int i = 0; i < numStars; ++i)
	{
		starX[i] = (getRandom(100) - xmax);
		starY[i] = (getRandom(100) - ymax);
	}
}

//set up asteroids to have random coordinates
void setAsteroidCoords()
{
	float x; 
	float y;

	for (int i = 0; i < numAsteroids; ++i)
	{
		x = (getRandom(100) - xmax);
		y = (getRandom(100) - ymax);
		
		//if collision occurs set up coords again
		if (hasCollided(x, y))
		{
			--i;
		}
		else
		{
			asteroidX[i] = x;
			asteroidY[i] = y;
		}
	}
}

//draws a score to screen as a score
void renderString(float x, float y, float z, void *font, char *string)
{  
	glColor3f(1.0,0.0,0.0);
    char *c;
    glRasterPos3f(x,y,z);
    for (c = string; *c != '\0'; ++c) 
	{
        glutBitmapCharacter(font, *c);
    }
}

//check if coordinates are in bounds
bool isInBounds(float x, float y) 
{		
	if ((x > xmin) && (x < xmax) && (y > ymin) && (y < ymax))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

//check if bullet hits asteroid
void asteroidHit() 
{		
	if (isBulletAlive)
	{
	//checks if bullet hits asteroids bounding box
	for (int i = 0; i < numAsteroids; ++i)
		{
		if((isAsteroidAlive[i] == true) && (bulletX > (asteroidX[i] - 6.0)) && (bulletX < (asteroidX[i] + 5.0)) && (bulletY > (asteroidY[i] - 5.0)) && (bulletY < (asteroidY[i] + 5.0))) 
			{
			isAsteroidAlive[i] = false;	//marks asteroid as destroyed
			isBulletAlive = false;		//mark bullet as destroyed
			score += 10;				//increase score
			++numAstDest;				//records number of asteroids destroyed
			}
		}
	}	
}

//sets all asteroids to alive
void setAstAlive()
{
	for (int i = 0; i < numAsteroids; ++i)
	{
		isAsteroidAlive[i] = true;
	}
}

// reshape function for whene screen is moved or resized
void reshape(GLint width, GLint height)
{
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
  
   glOrtho(xmin, xmax, ymin, ymax, -1.0, 1.0);   //sets coord space
   glMatrixMode(GL_MODELVIEW);
}

//draws star into coord space using star array
void drawStar()
{
    glPointSize(2.0);	//sets size of stars
	glLoadIdentity();

	for (int i = 0; i < numStars; ++i)
	{
		glBegin(GL_POINTS);
			glVertex2f(starX[i], starY[i]);
		glEnd();
	}
}

//moving star field
void moveStar(float starSpeed, float time)
{
	for (int i = 0; i < numStars; ++i)
	{
		//check if stars are in coord space and then move them
		if (starY[i] > ymin) 
		{
			starY[i] = (starSpeed * time) + starY[i];
		} 
		//if star moves to the bottom redraw it on top of the screen with random x coord
		else if (starY[i] < ymin)		
		{
			starX[i] = getRandom(100) - xmax;
			starY[i] = ymax;
		}			
	}
}

//draws the ship
void drawShip(GLfloat shipX, GLfloat shipY) 
{
	glLoadIdentity();
	glRotatef(shipAngle, 0.0, 0.0, 1.0 );   //rotates the ship set by keyboard functions 

	glBegin(GL_POLYGON);						//draws spaceship in middle of screen
		glVertex2f( shipX, shipY + 3.0 );
		glVertex2f( shipX + 2.0, shipY - 3.0 );
	glColor3f(0.0, 1.0, 1.0);					//for color blend
		glVertex2f( shipX, shipY - 2.0);
		glVertex2f( shipX - 2.0, shipY - 3.0);
	glEnd();
}

//draws bullet
void drawBullet()
{
	glPointSize(4.0);

	glBegin(GL_POINTS);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(bulletX,bulletY);
	glEnd();
}

//moves bullet using set speed and time
void moveBullet(float bulletSpeed, float time)
{
	float angleRadians = 0;
	float pi = 3.14159;
	
	//check if bullet is in coord space
	if (isInBounds(bulletX, bulletY))
	{
		angleRadians = ((bulletAngle + 90)/180) * pi;		//convert to radians
		bulletX = ((bulletSpeed * time) * cos(angleRadians)) + bulletX;		//set direction angle based on angle of ship
		bulletY = ((bulletSpeed * time) * sin(angleRadians)) + bulletY;		
	}
	else
	{
		isBulletAlive = false;
	}
}

//draws asteroids to screen
void drawAsteroid()
{
	glLoadIdentity();
	for (int i = 0; i < numAsteroids; ++i) 
	{
		if (isAsteroidAlive[i] == true)
		{
			glBegin(GL_POLYGON);
			glColor3f(0.5,0.5,0.5);
				glVertex2f( asteroidX[i], asteroidY[i] + 5.0 );
				glVertex2f( asteroidX[i] + 5.0, asteroidY[i] + 2.0 );
			glColor3f(0.2,0.2,0.2);		//color blend
				glVertex2f( asteroidX[i] + 4.0, asteroidY[i] - 3.0 );
				glVertex2f( asteroidX[i] - 3.0, asteroidY[i] - 5.0 );
				glVertex2f( asteroidX[i] - 4.0, asteroidY[i] - 1.0 );
				glVertex2f( asteroidX[i] - 6.0, asteroidY[i] );
			glEnd();
		}
	}	
}

//creates each new bullet
void createBullet()	
{
	bulletAngle = shipAngle;		//sets angle of bullet when shot
	bulletX = 0.0;					//sets bullet back to origin
	bulletY = 0.0;
	isBulletAlive = true;
}

/* reverse:  reverse string s in place */
//for use in the itoa function
//http://en.wikibooks.org/wiki/C_Programming/C_Reference/stdlib.h/itoa
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

/* itoa:  convert n to characters in s */
//got from wikibooks since there is no itoa equivalent in c
//http://en.wikibooks.org/wiki/C_Programming/C_Reference/stdlib.h/itoa
 void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

//display function
void display()
{
	frameStart = glutGet(GLUT_ELAPSED_TIME);			//gets time from last time function was called records when frame began
	float frameTime = (frameStart - frameEnd) / 1000;	//calculates difference of each frame in ms

	glClear(GL_COLOR_BUFFER_BIT);   // clear window 
	glColor3f(1.0, 1.0, 1.0);        // white objects, for stars

	drawStar(); 
	moveStar(-3.0,frameTime);
	
	//draws and moves bullet if it exists
	if (isBulletAlive)
		{
			moveBullet(80.0,frameTime);
			drawBullet();
		}

	glColor3f(1.0, 1.0, 1.0); 
	drawShip(0.0,0.0);

	drawAsteroid();
	asteroidHit();

	renderString(-45,45, 0.0, GLUT_BITMAP_HELVETICA_12, "Score: " );
	itoa(score, scoreStr);			//converts score to string and puts in character array
	renderString(-38,45, 0.0, GLUT_BITMAP_HELVETICA_12, scoreStr );

	glFlush();	//push to screen

	if (numAstDest == numAsteroids) //tests if all asteroids are destroyed and reinitialises if so
	{
	   setAsteroidCoords();
	   setAstAlive();
	   numAstDest = 0;
	}

	frameEnd = frameStart;			//gets time from last time function was called records it as time frame ends
}

// init to start program
void init()
{	
	srand(time(NULL));		//set for random number use
   	setStarCoords();		
   	setAsteroidCoords();		
   	setAstAlive();
   	glClearColor(0.0, 0.0, 0.0, 0.0);   //set background to black
}

//for keyboard functions
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		//creates bullet when spacebar is pressed
		case 32:	
			if (!isBulletAlive)
			{
				createBullet();
			}
			break;

		default:	
			break;
	}
}

//special keyboard functions
void spKeyboard(int key, int x, int y)
{
	//rotation for the ship
	switch (key)
	{
		//right
		case GLUT_KEY_RIGHT:	
			shipAngle = shipAngle - 5.0;
			break;
		//left
		case GLUT_KEY_LEFT:		
			shipAngle = shipAngle + 5.0;
			break;
		default:				
			break;
	}
}

//main
int main(int argc, char** argv)
{	 
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("My Sexy Window");
   
   init();
   
   //callbacks	
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(spKeyboard);
   glutKeyboardFunc(keyboard);
   glutIdleFunc(display);			//for animations 
   glutMainLoop();

   return 0;
}