#pragma once

#include "DLL.h"

void FROSTAPI gluLookAt(double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz);
void FROSTAPI gluOrtho2D(double left, double right, double bottom, double top);
void FROSTAPI frustum(double left, double right, double bottom, double top, double nearval, double farval);
void FROSTAPI gluPerspective(double fovy, double aspect, double zNear, double zFar);
void FROSTAPI gluGLPerspective(double fovy, double aspect, double zNear, double zFar);
void FROSTAPI gluPickMatrix(double x, double y, double width, double height, int viewport[4]);

int FROSTAPI gluProject(double objx, double objy, double objz, const double model[16], const double proj[16], const int viewport[4], double * winx, double * winy, double * winz);
int FROSTAPI gluUnProject(double winx, double winy, double winz, const double model[16], const double proj[16], const int viewport[4], double * objx, double * objy, double * objz);