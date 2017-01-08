//Working fine!!!
//Adds sound and background

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

#define ALIEN_TRANSLATE_DELAY 500000 //Delay for the alien's movement
#define BULLETSQUANT 1000000000 //Num of bullets
#define ALIEN_SHOT_DELAY 10000 //Delay for the alien's shot
#define PLAYER_SHOT_DELAY 100 //Delay for the spacecraft's shot
#define ROWS 3  // Number of rows of Aliens.
#define COLUMNS 3 // Number of columns of Aliens.
#define FILL_PROBABILITY 100 // Percentage probability that a particular row-column slot will be 
                             // filled with an Alien. It should be an integer between 0 and 100.

using namespace std;
							 
// Globals.
static long font = (long)GLUT_BITMAP_TIMES_ROMAN_24; // Font selection.
static int width, height; // Size of the OpenGL window.
static float angle = 0.0; // Angle of the spacecraft.
static float xVal = 0, zVal = 0; // Co-ordinates for the spacecraft 
static int isCollision = 0; // Is there collision between the spacecraft and an Alien?
static int game_over = 0;   //Flag for game over indication

//Using for controlling the spacecraft
static float xValSpacecraftBullet = 0, zValSpacecraftBullet = 0, angleSpacecraftBullet = 0.0; //Co-ordinates for the spacecraft's bullet
static unsigned int spacecraft; // Display lists base index.
int playerShotDelay = 0;        //Control the player's shot
static GLuint bulletSpacecraft;     //Display for spacecraft bullet
static int shotsSpacecraft = 0;     //Counter for the spacecraft shots
bool isShotting = 0;                //Using as a flag for the player's shot
float spacecraft_height = 5.0;
float maxZAlien =  (-40.0 - (30.0*ROWS));
float maxZSpacecraft = abs(maxZAlien) + abs(35.0);

//Using for cotrolling the aliens.
static float xValAlienBullet = 0, zValAlienBullet = 0, angleAlienBullet = 0.0; //Co-ordinates for the alien's bullet
static GLuint bulletAlien;
static int shotsAliens = 0;
bool alienShotting = 0;
int alienShotDelay = 0;
int alienMovementDelay = 0;
vector<float> axis_alien;
vector<float> axis_alien_shot;
float actual_alien_radius = 0.0;
float aliens_radius = 3.0;
float alien_translate = 0.1; //0.1
float alien_rotate = 0.0;
float translate_increment = 4.0;
float lower_translate = -8.0;
float upper_translate = 8.0;
int alienMovements = 0;


int NumAliens = ROWS * COLUMNS;
int points = 0;

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
} 

// Alien class.
class Alien
{
public:
   Alien();
   Alien(float x, float y, float z, float r, unsigned char colorR, 
		    unsigned char colorG, unsigned char colorB);
   float getCenterX() { return centerX; }
   float getCenterY() { return centerY; }
   float getCenterZ() { return centerZ; }
   float getRadius()  { return radius; }
   void setCenterX(float x) { centerX = x; }
   void setCenterY(float y) { centerY = y; }
   void setCenterZ(float z) { centerZ = z; }
   void setRadius(float r) {radius = r;}
   void draw();

private:
   float centerX, centerY, centerZ, radius;
   unsigned char color[3];
};

// Alien default constructor.
Alien::Alien()
{
   centerX = 0.0;
   centerY = 0.0;
   centerZ = 0.0; 
   radius = 0.0; // Indicates no Alien exists in the position.
   color[0] = 0;
   color[1] = 0;
   color[2] = 0;
}

// Alien constructor.
Alien::Alien(float x, float y, float z, float r, unsigned char colorR, 
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
	
// Function to draw Alien.
void Alien::draw()
{
   if (radius > 0.0) // If Alien exists.
   {
      glPushMatrix();
      glTranslatef(centerX, centerY, centerZ);
      glRotatef(alien_rotate, 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidSphere(radius, (int)radius*6, (int)radius*6); //First Sphere
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);
      glRotatef(alien_rotate, 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);//Front Cone
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX, centerY + 2.0, centerZ);
      glRotatef(alien_rotate, 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidSphere(radius, (int)radius*6, (int)radius*6);//Second Sphere
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);
      glRotatef((GLfloat) (100 % 360), 0.0, 1.0, 0.0);
      glRotatef(alien_rotate, 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);//RigthCone
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);
      glRotatef((GLfloat) (-100 % 360), 0.0, 1.0, 0.0);
      glRotatef(alien_rotate, 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);//Left Cone
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);//Up Cone
      glRotatef((GLfloat) (-100 % 360), 1.0, 0.0, 0.0);
      glRotatef(alien_rotate, 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);//Up Cone
      glRotatef((GLfloat) (50 % 360), 0.0, 0.0, 1.0);
      glRotatef(alien_rotate, 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);
      glPopMatrix();                        
      glPushMatrix();
      glTranslatef(centerX + 0.2, centerY + 2.5, centerZ);
      glRotatef((GLfloat) (100 % 360), 1.0, 0.0, 0.0);
      glRotatef(alien_rotate, 0.0, 1.0, 0.0);
      glColor3ubv(color);
      glutSolidCone(radius, 10.0, 5, 20);//Low Cone
      glPopMatrix();      
   }
}

Alien arrayAliens[ROWS][COLUMNS]; // Global array of Aliens.

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
   
   bulletSpacecraft = glGenLists(BULLETSQUANT);
   glNewList(bulletSpacecraft, GL_COMPILE);
	  glPushMatrix();
	  glColor3f (1.0, 1.0, 1.0); 
      glutSolidSphere(0.6,10,10);//Front 5, 10, 4.5, 50
	  glPopMatrix();
   glEndList();
   
   bulletAlien = glGenLists(BULLETSQUANT);
   glNewList(bulletAlien, GL_COMPILE);
      glPushMatrix();
      glColor3f(0.0, 1.0, 1.0);
      glutSolidSphere(0.6, 10, 10);
      glPopMatrix();
   glEndList();

   // Initialize global arrayAliens.
   for (j = 0; j < COLUMNS; j++)
      for (i = 0; i < ROWS; i++)
	     if (rand()%100 < FILL_PROBABILITY) 
         // If rand()%100 >= FILL_PROBABILITY the default constructor Alien remains in the slot 
		 // which indicates that there is no Alien there because the default's radius is 0.
		 {
			// Position the Aliens depending on if there is an even or odd number of columns
			// so that the spacecraft faces the middle of the Alien field.
			if (COLUMNS%2) // Odd number of columns.
	           arrayAliens[i][j] = Alien(30.0*(-COLUMNS/2 + j), 0.0, 
                                               -40.0 - 30.0*i, aliens_radius, 
                                               rand()%256, rand()%256, rand()%256);
			else // Even number of columns.
			   arrayAliens[i][j] = Alien(15 + 30.0*(-COLUMNS/2 + j), 0.0, 
                                               -40.0 - 30.0*i, aliens_radius, 
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

// Function to check if the spacecraft collides with an Alien when the center of the base
// of the craft is at (x, 0, z) and it is aligned at an angle a to to the -z direction.
// Collision detection is approximate as instead of the spacecraft we use a bounding sphere.
int AlienCraftCollision(float x, float z, float a)
{
   int i, j;

   // Check for collision with each Alien.
   for (i = 0; i < COLUMNS; i++)
      for (j = 0; j < ROWS; j++)
		 if (arrayAliens[i][j].getRadius() > 0) // If Alien exists.
            if (checkSpheresIntersection(x + 20 * sin((PI/180.0) * a), 0.0, 
		         z + 20 * cos((PI/180.0) * a), 7.0,
		         arrayAliens[i][j].getCenterX(), arrayAliens[i][j].getCenterY(), 
		         arrayAliens[i][j].getCenterZ(), arrayAliens[i][j].getRadius())){
                                                    
                 arrayAliens[i][j].setRadius(0.0);//Alien is deleted
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
     register int row = 0; 
	  register int cols = 0;
	  row = (int)(rand() % ROWS);
     cols = (int)(rand() % COLUMNS);
     while(NumAliens > 1 && arrayAliens[row][cols].getRadius() == 0.0){
			row = (int)(rand() % ROWS);
			cols = (int)(rand() % COLUMNS);
	  } 
     actual_alien_radius = arrayAliens[row][cols].getRadius();
     axis[0] = arrayAliens[row][cols].getCenterX();
     axis[1] = arrayAliens[row][cols].getCenterY();
     axis[2] = arrayAliens[row][cols].getCenterZ();     
     return axis;
}

void updateAlienCenterX(){
     for(int i = 0; i < ROWS; i++){
		  for(int j = 0; j < COLUMNS; j++){
			  arrayAliens[i][j].setCenterX(arrayAliens[i][j].getCenterX() + alien_translate);
		  }
     }		   
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
     alienMovementDelay++;
     
     //Translate the spacecraft's bullet.
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
     //Calculate the alien's translations. 
     //
     if(alienMovementDelay > ALIEN_TRANSLATE_DELAY){
		   if(alien_translate > upper_translate){ 
			  alien_translate = -0.1;
			  if(alienMovements == 0){
				 alienMovements = 1;
				 upper_translate *= 2;
				 lower_translate *= 2;
              }
		   }
		   
		   if(alien_translate < lower_translate) alien_translate = 0.1;
		        
		   alien_translate >= 0.0 ? alien_translate += translate_increment : 
		   				   	  	  	alien_translate -= translate_increment;
		   updateAlienCenterX();
		   alienMovementDelay = 0;
		   glutPostRedisplay();
	   }		  
     
     //Verify if is necessary add another bullet to the screen
     //
     if(zValSpacecraftBullet == (-40.0 - (30.0*ROWS))){ 
           zValSpacecraftBullet = zVal;
           xValSpacecraftBullet = xVal;
           isShotting = 0;
           playerShotDelay = 0;
           glDeleteLists(bulletSpacecraft + shotsSpacecraft, 1);
           glutPostRedisplay();
     }
     
     //Check if the spacecraft's shot collides with an alien 
     //  
     if(AlienCraftCollision(xValSpacecraftBullet, zValSpacecraftBullet, 1.0)){
           printf("Alien Destroyed\n");                                                
           zValSpacecraftBullet = zVal;
           xValSpacecraftBullet = xVal;
           isShotting = 0;
           glDeleteLists(bulletSpacecraft + shotsSpacecraft, 1);
           NumAliens--;
           points+=10;
           glutPostRedisplay();
           
     }
     //Translate an alien's bullet.
     //
     if(alienShotDelay == ALIEN_SHOT_DELAY){
                       
           shotsAliens++;            
           int alienShot_increment = 3;            
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
     if(zValAlienBullet > maxZSpacecraft || alienShotDelay > ALIEN_SHOT_DELAY){
           alienShotting = 0;            
           alienShotDelay = 0;
           vector<float> axis = alienPosition();
           xValAlienBullet = 0;
           zValAlienBullet = 0;
           angleAlienBullet = 0;
           glDeleteLists(bulletAlien + shotsAliens, 1);
           alienShot(axis[0], axis[1], axis[2]);
           glutPostRedisplay();
     }
     //Function for the collision with the spacecraft (Debbuging - Fix the collision with the spacecraft) 
     if(checkSpheresIntersection(xVal, 0.0, maxZSpacecraft, 2.0, xValAlienBullet, 0.0, zValAlienBullet, 1.0) == 1){
           glDeleteLists(spacecraft, 1);
           game_over = 1;
           shotsSpacecraft = 0;
           glutPostRedisplay();                                      
     }
}

//Function that draw an integer on the screen. Is used to draw the pontuation.  
void drawInteger(int value){
	 static bool f = true;
     char *c0 = "0";
     char *c1 = "1";
     char *c2 = "2";
     char *c3 = "3";
     char *c4 = "4";
     char *c5 = "5";
     char *c6 = "6";
     char *c7 = "7";
     char *c8 = "8";
     char *c9 = "9";
     glPushMatrix();
     glPushAttrib(GL_LINE_BIT);
     glColor3f(1.0, 0.0, 0.0);
     glRasterPos3f(-28.0, 23.0, -30.0);

     int res[10], i;
     for(i = 0; value > 0; i++){
          res[i] = value%10;
          value = value/10;
     }
     if(i > 0){
          for(i = i - 1; i >= 0; i--){
              if(res[i] == 0)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c0); // (font, string)
              if(res[i] == 1)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c1); // (font, string)
              if(res[i] == 2)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c2); // (font, string)
              if(res[i] == 3)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c3); // (font, string)
              if(res[i] == 4)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c4); // (font, string)
              if(res[i] == 5)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c5); // (font, string)
              if(res[i] == 6)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c6); // (font, string)
              if(res[i] == 7)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c7); // (font, string)
              if(res[i] == 8)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c8); // (font, string)
              if(res[i] == 9)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c9); // (font, string)
          }
     }else{
         char *c = "0";
         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // (font, string)
     }     
     f = false;     
     glPopAttrib();
     glPopMatrix();

}     
// Drawing routine.
void drawScene(void)
{ 
   //Adding Lighting
   GLfloat specular_light[] = {0.0, 0.0, 0.0, 0.0};
   GLfloat diffuse_light[] = {0.0, 0.0, 0.7, 1.0};
   GLfloat ambient_light[] = {1.0, 1.0, 1.0, 1.0};   
   GLfloat specularity[] = {0.0, 0.0, 0.0, 0.0};
   GLint spec_material = 5;
   GLfloat mat_shininess[] = {100.0};
   GLfloat light_position[] = {100.0, 100.0, 100.0, 0.0};
   glClearColor (0.0, 0.0, 0.0, 1.0);
   
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, specularity);
   glMateriali(GL_FRONT, GL_SHININESS, spec_material);
//   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
   
//   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
//   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
//   glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 
   glEnable(GL_COLOR_MATERIAL);    
   //End Lighting
 
   int i, j;
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Begin viewport.
   glViewport (0, 0, width,  height); 
   glLoadIdentity();
   
   glPushMatrix();
   glColor3f(1.0, 0.0, 0.0);
   glRasterPos3f(-28.0, 26.0, -30.0);
   writeBitmapString((void*)font, "PONTUATION");
   drawInteger(points);
   glPopMatrix();
   
   glPushMatrix();
   glColor3f(1.0, 1.0, 1.0);
   glRasterPos3f(-5.0, 26.0, -30.0);
   if (!NumAliens) writeBitmapString((void*)font, "YOU WIN!!!!!");
   glPopMatrix();

   glPushMatrix();
   glColor3f(1.0, 1.0, 1.0);
   glRasterPos3f(-5.0, 26.0, -30.0);
   if (game_over) writeBitmapString((void*)font, "YOU LOOSE!!!!!");
   glPopMatrix();   

   //The gluLookAt values are calculated to make the camera "follow" the spacecraft.
	gluLookAt(xVal - 100 * sin((PI/180.0) * (angle)), 
	         30.0, 
			 	zVal + 50 * cos((PI/180.0) * (angle)),
				xVal + 2 * (sin((PI/180.0) * (angle))), 
			 	0.0,
            zVal + 2 * (cos((PI/180.0) * (angle))),
				0.0, 1.0, 0.0);
 
   // Draw all the Aliens in arrayAliens.
   for (j = 0; j < COLUMNS; j++)	  
      for (i = 0; i < ROWS; i++)
         arrayAliens[i][j].draw();

   // Draw spacecraft.
   glPushMatrix();
   glTranslatef(xVal, 0.0, zVal);
   glRotatef(angle, 0.0, 1.0, 0.0);
   glCallList(spacecraft);
   glPopMatrix();
   
   if(!game_over) addBullet();
   
   if(NumAliens == 1){
	   for(register int i = 0; i < ROWS; i++){
		   for(register int j = 0; j < COLUMNS; j++){
			   if(arrayAliens[i][j].getRadius() != 0.0){
				   axis_alien_shot[0] = arrayAliens[i][j].getCenterX();
				   axis_alien_shot[1] = arrayAliens[i][j].getCenterY();
				   axis_alien_shot[2] = arrayAliens[i][j].getCenterZ();
			   }
		   }
	   }
   }
   
   if(NumAliens >= 2) axis_alien_shot = alienPosition();
   axis_alien = axis_alien_shot;
   shotsAliens++;
   alienShot(axis_alien_shot[0], axis_alien_shot[1], axis_alien_shot[2]);
//   printf("xAlienShot -> %0.2f zAlienShot -> %0.2f xVal -> %0.2f zVal -> %0.2f\n", 
//          xValAlienBullet, zValAlienBullet, xVal, zVal);
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

   // Angle correction.
   if (tempAngle > 360.0) tempAngle -= 360.0;
   if (tempAngle < 0.0) tempAngle += 360.0;

   // Move spacecraft to next position only if there will not be collision with an Alien.
   if (!AlienCraftCollision(tempxVal, tempzVal, tempAngle))
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

// Function to output interaction instructions.
void printInteraction(void)
{
 	 printf("Press space to shot\n");
 	 printf("Left/Right arrow -> move left/right\n");
 	 printf("Each alien cost 10 points\n");
 	 printf("Good Luck\n");
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
   glutInitWindowSize(600, 400);
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

