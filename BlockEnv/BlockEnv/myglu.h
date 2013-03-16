#pragma once

void gluLookAt(double eyex, double eyey, double eyez, double centerx, double centery, double centerz, double upx, double upy, double upz);
void gluOrtho2D(double left, double right, double bottom, double top);
void frustum(double left, double right, double bottom, double top, double nearval, double farval);
void gluPerspective(double fovy, double aspect, double zNear, double zFar);
void gluGLPerspective(double fovy, double aspect, double zNear, double zFar);
void gluPickMatrix(double x, double y, double width, double height, int viewport[4]);

int gluProject(double objx, double objy, double objz, const double model[16], const double proj[16], const int viewport[4], double * winx, double * winy, double * winz);
int gluUnProject(double winx, double winy, double winz, const double model[16], const double proj[16], const int viewport[4], double * objx, double * objy, double * objz);