

#include <windows.h>												
#include <GL.h>													
#include <GLU.h>												
#include <GLAUX.h>												
#include "Utility.h"												
#include "imageloader.h"	
#include "math.h"												  
#include "ArcBall.h"												

#ifndef CDS_FULLSCREEN												
#define CDS_FULLSCREEN 4											
#endif																

GL_Window*	g_window;
Keys*		g_keys;

GLUquadricObj *quadratic;											
GLuint _textureId; //The id of the textur
GLUquadric *quad;

const float PI2 = 2.0*3.1415926535f;								

Matrix4fT   Transform   = {  1.0f,  0.0f,  0.0f,  0.0f,				
                             0.0f,  1.0f,  0.0f,  0.0f,
                             0.0f,  0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  0.0f,  1.0f };

Matrix3fT   LastRot     = {  1.0f,  0.0f,  0.0f,					
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

Matrix3fT   ThisRot     = {  1.0f,  0.0f,  0.0f,					
                             0.0f,  1.0f,  0.0f,
                             0.0f,  0.0f,  1.0f };

ArcBallT    ArcBall(640.0f, 480.0f);				                
Point2fT    MousePt;												
bool        isClicked  = false;										
bool        isRClicked = false;										
bool        isDragging = false;					                    

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

BOOL Initialize (GL_Window* window, Keys* keys)						
{
	g_window	= window;
	g_keys		= keys;

	// Start Of User Initialization
    isClicked   = false;								            
    isDragging  = false;							                

	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);							
	glClearDepth (1.0f);											
	glDepthFunc (GL_LEQUAL);										
	glEnable (GL_DEPTH_TEST);										
	glShadeModel (GL_FLAT);											
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);				
  
	glEnable(GL_NORMALIZE);

	quadratic=gluNewQuadric();										
	gluQuadricNormals(quadratic, GLU_SMOOTH);					
	gluQuadricTexture(quadratic, GL_TRUE);							

	glEnable(GL_LIGHT0);											
	glEnable(GL_LIGHTING);											

	glEnable(GL_COLOR_MATERIAL);									


	Image* image = loadBMP("TennisBallColorMap.bmp");
	_textureId = loadTexture(image);
	delete image;

	return TRUE;													
}

void Deinitialize (void)											
{
	gluDeleteQuadric(quadratic);
}


float zoom=1;
void Update (DWORD milliseconds)									
{
	if (g_keys->keyDown [VK_ESCAPE] == TRUE)						
		TerminateApplication (g_window);							

	if (g_keys->keyDown [VK_F1] == TRUE)							
		ToggleFullscreen (g_window);								

    if (isRClicked)													
    {
		Matrix3fSetIdentity(&LastRot);								// Reset Rotation
		Matrix3fSetIdentity(&ThisRot);								// Reset Rotation
        Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);		// Reset Rotation
		
	
    }

		if(GetAsyncKeyState(VK_UP))
		{
			zoom =zoom+0.01;
		}
		else if(GetAsyncKeyState(VK_DOWN))
		{
			zoom=zoom-0.01;
		}
		
	
    if (!isDragging)											
    {
        if (isClicked)												// First Click
        {
			isDragging = true;										
			LastRot = ThisRot;										
			ArcBall.click(&MousePt);								
        }
    }
    else
    {
        if (isClicked)												
        {
            Quat4fT     ThisQuat;

            ArcBall.drag(&MousePt, &ThisQuat);						
            Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);		
            Matrix3fMulMatrix3f(&ThisRot, &LastRot);				
            Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);	
        }
        else														
            isDragging = false;
    }
}




void Draw (void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);				
	glLoadIdentity();												
	glTranslatef(0.0f,0.0f,-6.0f);									

    glPushMatrix();													
    glMultMatrixf(Transform.M);										
	

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if(zoom!=0)
	{
		gluSphere(quadratic,zoom*1.0f,100,100);
	}
	else
	{
		gluSphere(quadratic,1.0f,100,100);
	}
	glPopMatrix();													

	glFlush ();														
}

