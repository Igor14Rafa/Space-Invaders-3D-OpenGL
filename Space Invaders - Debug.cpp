///////////////////////////////////////////////////////////////////////////////////        
// spaceTravel.cpp
//
// This program draws a conical spacecraft that can travel and an array of 
// fixed spherical asteroids. The view in the left viewport is from a fixed
// camera; the view in the right viewport is from the spacecraft.
// There is approximate collision detection.
// 
// User-defined constants: 
// ROW is the number of rows of  asteroids.
// COLUMN is the number of columns of asteroids.
// FILL_PROBABILITY is the percentage probability that a particular row-column slot
// will be filled with an asteroid.
//
// Interaction:
// Press the left/right arrow keys to turn the craft.
// Press the up/down arrow keys to move the craft.
//
//Igor Rafael Santos da Silva 
/////////////////////////////////////////////////////////////////////////////////// 


#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

#define ROWS 8  // Number of rows of asteroids.
#define COLUMNS 3 // Number of columns of asteroids.
#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will be 
                             // filled with an asteroid. It should be an integer between 0 and 100.

using namespace std;
							 
// Globals.
static long font = (long)GLUT_BITMAP_TIMES_ROMAN_24; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0, angleBullet = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 0, xValBullet = 0, zValBullet = 0; // Co-ordinates for the spacecraft and the bullet.
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static unsigned int spacecraft; // Display lists base index.
static GLuint bullet;     //Display for bullet test
static int numShots = 0;
static int bulletsQuant = 1000000;
int countBullets = 0;
bool isShotting = 0; 

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
} 

class Bullet
{
public:
   Bullet();
   Bullet(float x, float y, float z, unsigned char colorR,
          unsigned char colorG, unsigned char colorB);
   float getCenterX() {
         return centerX;
   }
   float getCenterY() {
         return centerY;
   }          
   float getCenterZ() {
         return centerZ;
   }          
   float getRadius() {
         return radius;
   }
   void draw();
   
private:
   float centerX, centerY, centerZ, radius;
   unsigned char color[3];
};

Bullet::Bullet()
{
   centerX = 0.0;
   centerY = 0.0;
   centerZ = 0.0; 
   radius = 0.0; 
   color[0] = 0;
   color[1] = 0;
   color[2] = 0;
}

Bullet::Bullet(float x, float y, float z, unsigned char colorR, 
               unsigned char colorG, unsigned char colorB)
{
   centerX = x;
   centerY = y;
   centerZ = z; 
   radius = 0.6;
   color[0] = colorR;
   color[1] = colorG;
   color[2] = colorB;
}

void Bullet::draw()
{
      glPushMatrix();
      glTranslatef(centerX, centerY, centerZ);
      glColor3ubv (color); 
      glutSolidSphere(radius, 10, 10);
      glPopMatrix();
}

Bullet spacecraftBulletList[100000];
int spacecraftBulletNums = 0;

// Asteroid class.
class Asteroid
{
public:
   Asteroid();
   Asteroid(float x, float y, float z, float r, unsigned char colorR, 
		    unsigned char colorG, unsigned char colorB);
   float getCenterX() { return centerX; }
   float getCenterY() { return centerY; }
   float getCenterZ() { return centerZ; }
   float getRadius()  { return radius; }
   void draw();
   void setRadius(float r) {radius = r;}

private:
   float centerX, centerY, centerZ, radius;
   unsigned char color[3];
};

// Asteroid default constructor.
Asteroid::Asteroid()
{
   centerX = 0.0;
   centerY = 0.0;
   centerZ = 0.0; 
   radius = 0.0; // Indicates no asteroid exists in the position.
   color[0] = 0;
   color[1] = 0;
   color[2] = 0;
}

// Asteroid constructor.
Asteroid::Asteroid(float x, float y, float z, float r, unsigned char colorR, 
				   unsigned char colorG, unsigned char colorB)
{
   centerX = x;
   centerY = y;
   centerZ = z; 
   radius = r;
   color[0] = colorR;
   color[1] = colorG;
   color[2] = colorB;
}
	
// Function to draw asteroid.
void Asteroid::draw()
{
   if (radius > 0.0) // If asteroid exists.
   {
      glPushMatrix();
      glTranslatef(centerX, centerY, centerZ);
      glColor3ubv(color);
      glutSolidSphere(radius, (int)radius*6, (int)radius*6); //First Sphere
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);//Front Cone
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX, centerY + 2.0, centerZ);
      glColor3ubv(color);
      glutSolidSphere(radius, (int)radius*6, (int)radius*6);//Second Sphere
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);
      glRotatef((GLfloat) (100 % 360), 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);//RigthCone
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);
      glRotatef((GLfloat) (-100 % 360), 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);//Left Cone
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);//Up Cone
      glRotatef((GLfloat) (-100 % 360), 1.0, 0.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);//Up Cone
      glRotatef((GLfloat) (50 % 360), 0.0, 0.0, 1.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);
      glPopMatrix();                        
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);
      glRotatef((GLfloat) (100 % 360), 1.0, 0.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);//Low Cone
      glPopMatrix();      
   }
}

Asteroid arrayAsteroids[ROWS][COLUMNS]; // Global array of asteroids.

// Initialization routine.
void setup(void) 
{
   int i, j;
   float asteroids_radius = 3.0;
   float spacecraft_height = 5.0;

   spacecraft = glGenLists(1);
   glNewList(spacecraft, GL_COMPILE);
	  glPushMatrix();
	  glTranslatef(0.0, 0.0, 35.0);
	  glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the z-axis initially.
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidCone(spacecraft_height*0.6, spacecraft_height*2, spacecraft_height, spacecraft_height*2);//Front 5, 10, 4.5, 50
	  glPopMatrix();
	  glPushMatrix();
	  glTranslatef(0.0, 0.0, 33.0);
	  glRotatef(90.0, 0.0, 1.0, 0.0); 
	  glRotatef(90.0, 0.0, 0.0, 1.0);
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidCone(spacecraft_height*0.8, spacecraft_height*2, spacecraft_height*0.6, spacecraft_height*10);//Left Wing
	  glPopMatrix();
	  glPushMatrix();
	  glTranslatef(0.0, 0.0, 33.0);
	  glRotatef(-90.0, 0.0, 1.0, 0.0); 
	  glRotatef(90.0, 0.0, 0.0, 1.0);
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidCone(spacecraft_height*0.8, spacecraft_height*2, spacecraft_height*0.6, spacecraft_height*10);//Right Wing
	  glPopMatrix();
   glEndList();
   
   bullet = glGenLists(10000000);
   glNewList(bullet, GL_COMPILE);
	  glPushMatrix();
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidSphere(0.6,10,10);//Front 5, 10, 4.5, 50
	  glPopMatrix();
   glEndList();

   // Initialize global arrayAsteroids.
   for (j = 0; j<COLUMNS; j++)
      for (i = 0; i<ROWS; i++)
	     if (rand()%100 < FILL_PROBABILITY) 
         // If rand()%100 >= FILL_PROBABILITY the default constructor asteroid remains in the slot 
		 // which indicates that there is no asteroid there because the default's radius is 0.
		 {
			// Position the asteroids depending on if there is an even or odd number of columns
			// so that the spacecraft faces the middle of the asteroid field.
			if (COLUMNS%2) // Odd number of columns.
	           arrayAsteroids[i][j] = Asteroid( 30.0*(-COLUMNS/2 + j), 0.0, -40.0 - 30.0*i, asteroids_radius, 
			                                    rand()%256, rand()%256, rand()%256);
			else // Even number of columns.
			   arrayAsteroids[i][j] = Asteroid( 15 + 30.0*(-COLUMNS/2 + j), 0.0, -40.0 - 30.0*i, asteroids_radius, 
			                                    rand()%256, rand()%256, rand()%256);
		 }
   //Add GLEnable for lighting	 
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   //Finish Lighting GLEnable
   glEnable(GL_DEPTH_TEST);
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1, 
						     float x2, float y2, float z2, float r2)
{
   return ( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) <= (r1+r2)*(r1+r2) );
}

// Function to check if the spacecraft collides with an asteroid when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int asteroidCraftCollision(float x, float z, float a)
{
   int i,j;

   // Check for collision with each asteroid.
   for (j = 0; j < COLUMNS; j++)
      for (i = 0; i < ROWS; i++)
		 if (arrayAsteroids[i][j].getRadius() > 0) // If asteroid exists.
            if (checkSpheresIntersection(x + 20 * sin((PI/180.0) * a), 0.0, 
		         z + 20 * cos((PI/180.0) * a), 7.0,
		         arrayAsteroids[i][j].getCenterX(), arrayAsteroids[i][j].getCenterY(), 
		         arrayAsteroids[i][j].getCenterZ(), arrayAsteroids[i][j].getRadius())){
                                                    
                 arrayAsteroids[i][j].setRadius(0.0);//Asteroid is deleted
                 return 1;
                 }   
		       
   return 0;
}

void addBullet(void){
   glNewList(bullet + numShots, GL_COMPILE);
	  glPushMatrix();
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidSphere(0.6,10,10);
	  glPopMatrix();
   glEndList();
     
   glPushMatrix();
   glTranslatef(0.0, 0.0, 35.0);
   glTranslatef(isShotting ? xValBullet : xVal, 0.0, isShotting ? zValBullet : zVal);
   glRotatef(angleBullet, 0.0, 1.0, 0.0);
   glCallList(bullet + numShots);
   glPopMatrix();
}

void idle(void){
     if(zValBullet > -300.0 && isShotting){
         int temp_x = xValBullet, temp_z = zValBullet, tempAngle = angle;
         
         temp_x = xValBullet - sin(angleBullet * PI/180.0);
         temp_z = zValBullet - cos(angleBullet * PI/180.0);
         tempAngle = angleBullet;
         
         if (tempAngle > 360.0) tempAngle -= 360.0;
         if (tempAngle < 0.0) tempAngle += 360.0;
    
         xValBullet = temp_x;
         zValBullet = temp_z;
         angleBullet = tempAngle;
         glutPostRedisplay();                                  
     }
     if(asteroidCraftCollision(xValBullet,zValBullet,1.0)){
           printf("Alien Destroyed\n");                                                
           zValBullet = 0.0;
           xValBullet = 0.0;
           isShotting = 0;
           glDeleteLists(bullet + numShots, 1);
     }
}
     
// Drawing routine.
void drawScene(void)
{ 
   //Adding Lighting
   GLfloat mat_specular[] = { 10.0, 10.0, 10.0, 10.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 10.0, 10.0, 0.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); //Adding modifications from desenho3D
   glEnable(GL_COLOR_MATERIAL); //Adding modifications from desenho3D   
   //End Lighting
 
   int i, j;
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Begin left viewport.
   glViewport (0, 0, width,  height); 
   glLoadIdentity();

   // Write text in isolated (i.e., before gluLookAt) translate block.
   glPushMatrix();
   glColor3f(1.0, 0.0, 0.0);
   glRasterPos3f(-28.0, 25.0, -30.0);
   glRasterPos3f(-28.0, 23.0, -30.0);
   if (isCollision) writeBitmapString((void*)font, "Cannot - will crash!");
   glPopMatrix();

   // Fixed camera.
//   gluLookAt(0.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt(xVal - 100 * sin( (PI/180.0) * angle), 
	         30.0, 
			 	zVal + 50 * cos( (PI/180.0) * angle),
				xVal + 2 * sin( (PI/180.0) * angle),
			 	0.0,
            zVal + 2 * cos( (PI/180.0) * angle),
				0.0, 1.0, 0.0);
 
   // Draw all the asteroids in arrayAsteroids.
   for (j=0; j<COLUMNS; j++)	  
      for (i=0; i<ROWS; i++)
         arrayAsteroids[i][j].draw();

   // Draw spacecraft.
   glPushMatrix();
   glTranslatef(xVal, 0.0, zVal);
   glRotatef(angle, 0.0, 1.0, 0.0);
   glCallList(spacecraft);
   glPopMatrix();
   
   addBullet();
   
   printf("xVal - %0.2f zVal - %0.2f xBullet - %0.2f zBullet - %0.2f\n", xVal, zVal, xValBullet, zValBullet);

   glutSwapBuffers();
}


// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);
   glMatrixMode(GL_MODELVIEW);

   // Pass the size of the OpenGL window.
   width = w;
   height = h;
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
      case 32:
          isShotting = 1;
          numShots++;
          addBullet();
         break;     
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;
   float x_increment = 5.0;

   // Compute next position.
   if (key == GLUT_KEY_PAGE_DOWN) tempAngle = angle + 5.0; //turn clockwise
   if (key == GLUT_KEY_PAGE_UP) tempAngle = angle - 5.0; //turn counter-clockwise
   
   if(key == GLUT_KEY_LEFT) //Modification on the left movement 
   {
	  tempxVal = xVal - x_increment - cos(angle * PI/180.0); 
	  tempzVal = zVal - sin(angle * PI/180.0);
   }
   if(key == GLUT_KEY_RIGHT) //Modification on the right movement 
   {
	  tempxVal = xVal + x_increment + cos(angle * PI/180.0); 
	  tempzVal = zVal + sin(angle * PI/180.0);
   }  
   if( key == GLUT_KEY_UP)
   {
	  tempxVal = xVal - sin(angle * PI/180.0); 
	  tempzVal = zVal - cos(angle * PI/180.0);
   }
   if( key == GLUT_KEY_DOWN)
   {
	  tempxVal = xVal + sin(angle * PI/180.0); 
	  tempzVal = zVal + cos(angle * PI/180.0);
   }

   // Angle correction.
   if (tempAngle > 360.0) tempAngle -= 360.0;
   if (tempAngle < 0.0) tempAngle += 360.0;

   // Move spacecraft to next position only if there will not be collision with an asteroid.
   if (!asteroidCraftCollision(tempxVal, tempzVal, tempAngle))
   {
      isCollision = 0;
      xVal = tempxVal;
	  zVal = tempzVal;
	  angle = tempAngle;
   }
   else isCollision = 1;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the left/right arrow keys to turn the craft." << endl
        << "Press the up/down arrow keys to move the craft." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize(880, 550);
   glutInitWindowPosition(200, 80); 
   glutCreateWindow("Space Travel.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutIdleFunc(idle);
   glutMainLoop(); 

   return 0;  
}

