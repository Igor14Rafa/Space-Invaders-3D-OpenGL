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
#include <time.h>
#include <vector>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

#define ALIEN_SHOT_DELAY 50000 //Delay for the alien's shot
#define PLAYER_SHOT_DELAY 100 //Delay for the spacecraft's shot
#define ROWS 1  // Number of rows of asteroids.
#define COLUMNS 5 // Number of columns of asteroids.
#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will be 
                             // filled with an asteroid. It should be an integer between 0 and 100.

using namespace std;
							 
// Globals.
static long font = (long)GLUT_BITMAP_TIMES_ROMAN_24; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 0; // Co-ordinates for the spacecraft 
static int isCollision = 0; // Is there collision between the spacecraft and an asteroid?
static int game_over = 0;   //Flag for game over indication

static long int bulletsQuant = 1000000000;  //Using for the spacecraft bullets display lists

//Using for controlling the spacecraft
static float xValSpacecraftBullet = 0, zValSpacecraftBullet = 0, angleSpacecraftBullet = 0.0; //Co-ordinates for the spacecraft's bullet
static unsigned int spacecraft; // Display lists base index.
int playerShotDelay = 0;        //Control the player's shot
static GLuint bulletSpacecraft;     //Display for spacecraft bullet
static int shotsSpacecraft = 0;     //Counter for the spacecraft shots
bool isShotting = 0;                //Using as a flag for the player's shot
float spacecraft_height = 5.0;


//Using for cotrolling the aliens.
static float xValAlienBullet = 0, zValAlienBullet = 0, angleAlienBullet = 0.0; //Co-ordinates for the alien's bullet
static GLuint bulletAlien;
static int shotsAliens = 0;
bool alienShotting = 0;
int alienShotDelay = 0;
vector<float> axis_alien;
float actual_alien_radius = 0.0;
float asteroids_radius = 3.0;



int NumAliens = ROWS*COLUMNS;
int points = 0;

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
} 

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

   spacecraft = glGenLists(1);
   glNewList(spacecraft, GL_COMPILE);
	  glPushMatrix();
	  glTranslatef(0.0, 0.0, 35.0);
	  glRotatef(180.0, 0.0, 1.0, 0.0); // To make the spacecraft point down the z-axis initially.
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidCone(spacecraft_height*0.6, 
                    spacecraft_height*2.0, 
                    (GLint)spacecraft_height, 
                    (GLint)spacecraft_height*2);//Front 5, 10, 4.5, 50
	  glPopMatrix();
	  glPushMatrix();
	  glTranslatef(0.0, 0.0, 33.0);
	  glRotatef(90.0, 0.0, 1.0, 0.0); 
	  glRotatef(90.0, 0.0, 0.0, 1.0);
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidCone(spacecraft_height*0.8, 
                    spacecraft_height*2.0, 
                    (GLint)(spacecraft_height*0.6), 
                    (GLint)(spacecraft_height*10));//Left Wing
	  glPopMatrix();
	  glPushMatrix();
	  glTranslatef(0.0, 0.0, 33.0);
	  glRotatef(-90.0, 0.0, 1.0, 0.0); 
	  glRotatef(90.0, 0.0, 0.0, 1.0);
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidCone(spacecraft_height*0.8, 
                    spacecraft_height*2.0, 
                    (GLint)(spacecraft_height*0.6), 
                    (GLint)(spacecraft_height*10));//Right Wing
	  glPopMatrix();
   glEndList();
   
   bulletSpacecraft = glGenLists(bulletsQuant);
   glNewList(bulletSpacecraft, GL_COMPILE);
	  glPushMatrix();
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidSphere(0.6,10,10);//Front 5, 10, 4.5, 50
	  glPopMatrix();
   glEndList();
   
   bulletAlien = glGenLists(bulletsQuant);
   glNewList(bulletAlien, GL_COMPILE);
      glPushMatrix();
      glColor3f(0.0, 1.0, 1.0);
      glutSolidSphere(0.6, 10, 10);
      glPopMatrix();
   glEndList();

   // Initialize global arrayAsteroids.
   for (j = 0; j < COLUMNS; j++)
      for (i = 0; i < ROWS; i++)
	     if (rand()%100 < FILL_PROBABILITY) 
         // If rand()%100 >= FILL_PROBABILITY the default constructor asteroid remains in the slot 
		 // which indicates that there is no asteroid there because the default's radius is 0.
		 {
			// Position the asteroids depending on if there is an even or odd number of columns
			// so that the spacecraft faces the middle of the asteroid field.
			if (COLUMNS%2) // Odd number of columns.
	           arrayAsteroids[i][j] = Asteroid(30.0*(-COLUMNS/2 + j), 0.0, 
                                               -40.0 - 30.0*i, asteroids_radius, 
                                               rand()%256, rand()%256, rand()%256);
			else // Even number of columns.
			   arrayAsteroids[i][j] = Asteroid(15 + 30.0*(-COLUMNS/2 + j), 0.0, 
                                               -40.0 - 30.0*i, asteroids_radius, 
                                               rand()%256, rand()%256, rand()%256);
		 }
   //Add GLEnable for lighting	 
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

// Function to check if two spheres centered at (x1,y1,z1) and (x2,y2,z2) with
// radius r1 and r2 intersect.
int checkSpheresIntersection(float x1, float y1, float z1, float r1, 
						     float x2, float y2, float z2, float r2)
{
   return ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2) <= (r1+r2)*(r1+r2));
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

void addBullet(void){ //Adds a bullet for the spacecraft
   glNewList(bulletSpacecraft + shotsSpacecraft, GL_COMPILE);
	  glPushMatrix();
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidSphere(0.6,10,10);
	  glPopMatrix();
   glEndList();
     
   glPushMatrix();
   glTranslatef(0.0, 0.0, 32.0);
   glTranslatef(isShotting ? xValSpacecraftBullet : xVal, 0.0, 
                isShotting ? zValSpacecraftBullet : zVal);
   glRotatef(angleSpacecraftBullet, 0.0, 1.0, 0.0);
   glCallList(bulletSpacecraft + shotsSpacecraft);
   glPopMatrix();
}


vector<float> alienPosition(){//Returns an aliens's coordinate.
     srand(time(NULL));
     vector<float> axis(3);
     int row = (int)(rand()%ROWS);
     int cols = (int)(rand()%COLUMNS);
     actual_alien_radius = arrayAsteroids[row][cols].getRadius();
     axis[0] = arrayAsteroids[row][cols].getCenterX();
     axis[1] = arrayAsteroids[row][cols].getCenterY();
     axis[2] = arrayAsteroids[row][cols].getCenterZ();     
     return axis;
}     

void alienShot(float alien_x, float alien_y, float alien_z){//Adds a alien's shot
   glNewList(bulletAlien + shotsAliens, GL_COMPILE);
	  glPushMatrix();
	  glColor3f (0.0, 1.0, 1.0); 
      glutSolidSphere(0.6,10,10);
	  glPopMatrix();
   glEndList();
     
   glPushMatrix();
   glTranslatef(alien_x, alien_y, alien_z);
   glTranslatef(xValAlienBullet, 0.0, zValAlienBullet);
   glRotatef(angleSpacecraftBullet, 0.0, 1.0, 0.0);
   glCallList(bulletAlien + shotsAliens);
   glPopMatrix();
   
}

void idle(void){//Idle function for the Glut. Updates the shots for spacecraft and aliens

     alienShotDelay++;//Controling the alien's shot delay
     playerShotDelay++;//Controlling the player's shot
     
     //Make a translation for a spacecraft's shot.
     //
     if(zValSpacecraftBullet > -300 && isShotting){ 
           float temp_x = xValSpacecraftBullet, temp_z = zValSpacecraftBullet, tempAngle = angle;
         
           temp_x = xValSpacecraftBullet - sin(angleSpacecraftBullet * PI/180.0);
           temp_z = zValSpacecraftBullet - cos(angleSpacecraftBullet * PI/180.0);
           tempAngle = angleSpacecraftBullet;
         
           if (tempAngle > 360.0) tempAngle -= 360.0;
           if (tempAngle < 0.0) tempAngle += 360.0;
    
           xValSpacecraftBullet = temp_x;
           zValSpacecraftBullet = temp_z;
           angleSpacecraftBullet = tempAngle;
           glutPostRedisplay();                                  
     }
     
     //Verify if is necessary add another bullet to the screen
     //
     if(zValSpacecraftBullet == -300.0){
           zValSpacecraftBullet = zVal;
           xValSpacecraftBullet = xVal;
           isShotting = 0;
           playerShotDelay = 0;
           glDeleteLists(bulletSpacecraft + shotsSpacecraft, 1);
           glutPostRedisplay();
     }
     
     //Check if the spacecraft shot collides with an alien 
     //  
     if(asteroidCraftCollision(xValSpacecraftBullet,zValSpacecraftBullet,1.0)){
           printf("Alien Destroyed\n");                                                
           zValSpacecraftBullet = zVal;
           xValSpacecraftBullet = xVal;
           isShotting = 0;
           glDeleteLists(bulletSpacecraft + shotsSpacecraft, 1);
           NumAliens--;
           points+=10;
           glutPostRedisplay();
           
     }
     //Make a translation for an alien's shot. (Debugging)
     //
     if(alienShotDelay == ALIEN_SHOT_DELAY){
                       
           shotsAliens++;            
           int alienShot_increment = 8;            
           alienShotting = 1;
           
           alienShotDelay = 0;                                              
                                              
           float temp_x_alien = xValAlienBullet, temp_z_alien = zValAlienBullet, tempAngle_alien = angle;
         
           temp_x_alien = xValAlienBullet + sin(angleAlienBullet * PI/180.0);
           temp_z_alien = zValAlienBullet + alienShot_increment + cos(angleAlienBullet * PI/180.0);
           tempAngle_alien = angleAlienBullet;
         
           if (tempAngle_alien > 360.0) tempAngle_alien -= 360.0;
           if (tempAngle_alien < 0.0) tempAngle_alien += 360.0;
    
           xValAlienBullet = temp_x_alien;
           zValAlienBullet = temp_z_alien;
           angleAlienBullet = tempAngle_alien;
                      
           glutPostRedisplay();           
     }
          
     //Updates the alien's shot (Debugging)
     //
     if(zValAlienBullet > 800.0 || alienShotDelay > ALIEN_SHOT_DELAY){
           alienShotting = 0;            
           alienShotDelay = 0;
           vector<float> axis = alienPosition();
           xValAlienBullet = 0;
           zValAlienBullet = 0;
           angleAlienBullet = 0;
//           glDeleteLists(bulletAlien + shotsAliens, 1);
           alienShot(axis[0], axis[1], axis[2]);
           glutPostRedisplay();
     }
     //Function for the collision with the spacecraft (Debbuging) 
//     if(checkSpheresIntersection(xVal, 0.0, zVal, 2.0, xValAlienBullet, 0.0, zValAlienBullet, actual_alien_radius) == 1){
//           glDeleteLists(spacecraft, 1);
//           glDeleteLists(bulletSpacecraft, bulletsQuant);
//           game_over = 1;
//           shotsSpacecraft = 0;
//           glutPostRedisplay();                                      
//     }
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
   
   glPushMatrix();
   glColor3f(1.0, 1.0, 1.0);
   glRasterPos3f(-5.0, 25.0, -30.0);
   glRasterPos3f(-5.0, 23.0, -30.0);
   if (!NumAliens) writeBitmapString((void*)font, "YOU WIN!!!!!");
   glPopMatrix();

   glPushMatrix();
   glColor3f(1.0, 1.0, 1.0);
   glRasterPos3f(-5.0, 25.0, -30.0);
   glRasterPos3f(-5.0, 23.0, -30.0);
   if (game_over) writeBitmapString((void*)font, "YOU LOOSE!!!!!");
   glPopMatrix();   

   // Fixed camera.
//   gluLookAt(0.0, 10.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt(xVal - 100 * sin((PI/180.0) * angle), 
	         30.0, 
			 	zVal + 50 * cos((PI/180.0) * angle),
				xVal + 2 * sin((PI/180.0) * angle),
			 	0.0,
            zVal + 2 * cos((PI/180.0) * angle),
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
   vector<float> axis = alienPosition();
   axis_alien = axis;
   shotsAliens++;
   alienShot(axis[0], axis[1], axis[2]);
//   alienShot(arrayAsteroids[0][0].getCenterX(), arrayAsteroids[0][0].getCenterY(), arrayAsteroids[0][0].getCenterZ());
   printf("xAlienShot -> %0.2f zAlienShot -> %0.2f xVal -> %d zVal -> %d   %d\n", 
          xValAlienBullet, zValAlienBullet, alienShotDelay, shotsAliens, alienShotDelay);
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
          if(!game_over){  
             isShotting = 1;
             shotsSpacecraft++;
             addBullet();
          }   
         break;     
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   float tempxVal = xVal, tempzVal = zVal, tempAngle = angle;
   float x_increment = 0.0;

   // Compute next position.
   if(key == GLUT_KEY_PAGE_DOWN) tempAngle = angle + 5.0; //turn clockwise
   if(key == GLUT_KEY_PAGE_UP) tempAngle = angle - 5.0; //turn counter-clockwise
   
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
   if(key == GLUT_KEY_UP)
   {
	  tempxVal = xVal - sin(angle * PI/180.0); 
	  tempzVal = zVal - cos(angle * PI/180.0);
   }
   if(key == GLUT_KEY_DOWN)
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
		if(!isShotting){
	      isCollision = 0;
	      xVal = tempxVal;
		   zVal = tempzVal;
		   zValSpacecraftBullet = tempzVal;
		   xValSpacecraftBullet = tempxVal;
		   angleSpacecraftBullet = tempAngle;
		   angle = tempAngle;
		}
		else{
	      isCollision = 0;
	      xVal = tempxVal;
		   zVal = tempzVal;
		   angle = tempAngle;			
		}
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
   glutInitWindowSize(400, 400);
   glutInitWindowPosition(0, 0); 
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

