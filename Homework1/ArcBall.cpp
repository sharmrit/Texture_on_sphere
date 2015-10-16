

#include <windows.h>											
#include <GL.h>												
#include <GLU.h>												
#include <GLAUX.H>												
#include <glut.h>
#include "math.h"                                              

#include "ArcBall.h"                                            

//Arcball sphere constants:
//Diameter is       2.0f
//Radius is         1.0f
//Radius squared is 1.0f

void ArcBall_t::_mapToSphere(const Point2fT* NewPt, Vector3fT* NewVec) const
{
    Point2fT TempPt;
    GLfloat length;

    TempPt = *NewPt;

    TempPt.s.X  =        (TempPt.s.X * this->AdjustWidth)  - 1.0f;
    TempPt.s.Y  = 1.0f - (TempPt.s.Y * this->AdjustHeight);

    length      = (TempPt.s.X * TempPt.s.X) + (TempPt.s.Y * TempPt.s.Y);

    if (length > 1.0f)
    {
        GLfloat norm;

        norm    = 1.0f / FuncSqrt(length);

        NewVec->s.X = TempPt.s.X * norm;
        NewVec->s.Y = TempPt.s.Y * norm;
        NewVec->s.Z = 0.0f;
    }
    else    
    {
        //Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
        NewVec->s.X = TempPt.s.X;
        NewVec->s.Y = TempPt.s.Y;
        NewVec->s.Z = FuncSqrt(1.0f - length);
    }
}


ArcBall_t::ArcBall_t(GLfloat NewWidth, GLfloat NewHeight)
{
    //Clear initial values
    this->StVec.s.X     =
    this->StVec.s.Y     = 
    this->StVec.s.Z     = 

    this->EnVec.s.X     =
    this->EnVec.s.Y     = 
    this->EnVec.s.Z     = 0.0f;


    this->setBounds(NewWidth, NewHeight);
}

//Mouse down
void    ArcBall_t::click(const Point2fT* NewPt)
{
    this->_mapToSphere(NewPt, &this->StVec);
}

void    ArcBall_t::drag(const Point2fT* NewPt, Quat4fT* NewRot)
{
    this->_mapToSphere(NewPt, &this->EnVec);

    if (NewRot)
    {
        Vector3fT  Perp;

        Vector3fCross(&Perp, &this->StVec, &this->EnVec);

        if (Vector3fLength(&Perp) > Epsilon)    //if its non-zero
        {
            NewRot->s.X = Perp.s.X;
            NewRot->s.Y = Perp.s.Y;
            NewRot->s.Z = Perp.s.Z;
            NewRot->s.W= Vector3fDot(&this->StVec, &this->EnVec);
        }
        else                                    
        {
          
            NewRot->s.X = 
            NewRot->s.Y = 
            NewRot->s.Z = 
            NewRot->s.W = 0.0f;
        }
    }
}

