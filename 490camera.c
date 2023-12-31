


/* Feel free to read from this struct's members, but don't write to them. */
typedef struct camCamera camCamera;
struct camCamera {
	float projection[6];
	int projectionType;
	isoIsometry isometry;
};



/*** Projections ***/

#define camORTHOGRAPHIC 0
#define camPERSPECTIVE 1
#define camPROJL 0
#define camPROJR 1
#define camPROJB 2
#define camPROJT 3
#define camPROJF 4
#define camPROJN 5

const float camVulkan[4][4] = {
    {1.0, 0.0, 0.0, 0.0}, 
    {0.0, -1.0, 0.0, 0.0}, 
    {0.0, 0.0, 0.5, 0.5}, 
    {0.0, 0.0, 0.0, 1.0}};

/* Sets the projection type, to either camORTHOGRAPHIC or camPERSPECTIVE. */
void camSetProjectionType(camCamera *cam, int projType) {
	cam->projectionType = projType;
}

/* Sets all six projection parameters. */
void camSetProjection(camCamera *cam, const float proj[6]) {
	vecCopy(6, proj, cam->projection);
}

/* Sets one of the six projection parameters. */
void camSetOneProjection(camCamera *cam, int i, float value) {
	cam->projection[i] = value;
}


void camGetOrthographic(const camCamera *cam, float proj[4][4]) {
	float l = cam->projection[camPROJL], r = cam->projection[camPROJR], b = cam->projection[camPROJB], t = cam->projection[camPROJT], n = cam->projection[camPROJN], f = cam->projection[camPROJF];
	float temp[4][4];
	mat44Zero(temp);
	temp[0][0] = 2.0/(r - l);
	temp[0][3] = (-r-l)/(r-l);
	temp[1][1] = 2.0/(t-b);
	temp[1][3] = (-t-b)/(t-b);
	temp[2][2] = (-2.0)/(n-f);
	temp[2][3] = (n + f)/(n - f);
	temp[3][3] = 1.0;
	mat444Multiply(camVulkan, temp, proj);
}


void camGetPerspective(const camCamera *cam, float proj[4][4]) {
	float l = cam->projection[camPROJL], r = cam->projection[camPROJR], b = cam->projection[camPROJB], t = cam->projection[camPROJT], n = cam->projection[camPROJN], f = cam->projection[camPROJF];
	float temporaryMatrix[4][4];
	mat44Zero(temporaryMatrix);
	temporaryMatrix[0][0] = (-2.0*n)/(r-l);
	temporaryMatrix[0][2] = (r+l)/(r-l);
	temporaryMatrix[1][1] = (-2.0 * n)/(t - b);
	temporaryMatrix[1][2] = (t + b)/(t - b);
	temporaryMatrix[2][2] = (n + f)/(n - f);
	temporaryMatrix[2][3] = (-2.0 * n * f)/(n - f);
	temporaryMatrix[3][2] = -1.0;
	mat444Multiply(camVulkan, temporaryMatrix, proj);
}



/*** Convenience functions for projection ***/

/* Sets the six projection parameters, based on the width and height of the 
viewport and three other parameters. The camera looks down the center of the 
viewing volume. For perspective projection, fovy is the full (not half) 
vertical angle of the field of vision, in radians. focal > 0 is the distance 
from the camera to the 'focal' plane (where 'focus' is used in the sense of 
attention, not optics). ratio expresses the far and near clipping planes 
relative to focal: far = -focal * ratio and near = -focal / ratio. Reasonable 
values are fovy = M_PI / 6.0, focal = 10.0, and ratio = 10.0, so that 
far = -100.0 and near = -1.0. For orthographic projection, the projection 
parameters are set to produce the orthographic projection that, at the focal 
plane, is most similar to the perspective projection just described. You must 
re-invoke this function after each time you resize the viewport. */
void camSetFrustum(
        camCamera *cam, float fovy, float focal, float ratio, float width, 
        float height) {
	cam->projection[camPROJF] = -focal * ratio;
	cam->projection[camPROJN] = -focal / ratio;
	float tanHalfFovy = tan(fovy * 0.5);
	if (cam->projectionType == camPERSPECTIVE)
		cam->projection[camPROJT] = -cam->projection[camPROJN] * tanHalfFovy;
	else
		cam->projection[camPROJT] = focal * tanHalfFovy;
	cam->projection[camPROJB] = -cam->projection[camPROJT];
	cam->projection[camPROJR] = cam->projection[camPROJT] * width / height;
	cam->projection[camPROJL] = -cam->projection[camPROJR];
}

/* Returns the homogeneous 4x4 product of the camera's projection and the 
camera's inverse isometry (regardless of whether the camera is in orthographic 
or perspective mode). */
void camGetProjectionInverseIsometry(const camCamera *cam, float homog[4][4]) {
	float inverseHomogeneous[4][4], proj[4][4];
	isoGetInverseHomogeneous(&cam->isometry, inverseHomogeneous);
	if(cam->projectionType == camORTHOGRAPHIC){
		camGetOrthographic(cam, proj);
	} 
	else{
		camGetPerspective(cam, proj);
	}
	mat444Multiply(proj, inverseHomogeneous, homog);
}



/*** Convenience functions for isometry ***/

/* Sets the camera's isometry, in a manner suitable for third-person viewing. 
The camera is aimed at the world coordinates target. The camera itself is 
displaced from that target by a distance rho, in the direction specified by the 
spherical coordinates phi and theta (as in vec3Spherical). Under normal use, 
where 0 < phi < pi, the camera's up-direction is world-up, or as close to it as 
possible. */
void camLookAt(
        camCamera *cam, const float target[3], float rho, float phi, 
		float theta) {
	float z[3], y[3], yStd[3] = {0.0, 1.0, 0.0}, zStd[3] = {0.0, 0.0, 1.0};
	float rot[3][3], trans[3];
	vec3Spherical(1.0, phi, theta, z);
	vec3Spherical(1.0, M_PI / 2.0 - phi, theta + M_PI, y);
	mat33BasisRotation(yStd, zStd, y, z, rot);
	isoSetRotation(&(cam->isometry), rot);
	vecScale(3, rho, z, trans);
	vecAdd(3, target, trans, trans);
	isoSetTranslation(&(cam->isometry), trans);
}

/* Sets the camera's isometry, in a manner suitable for first-person viewing. 
The camera is positioned at the world coordinates position. From that position, 
the camera's sight direction is described by the spherical coordinates phi and 
theta (as in vec3Spherical). Under normal use, where 0 < phi < pi, the camera's 
up-direction is world-up, or as close to it as possible. */
void camLookFrom(
        camCamera *cam, const float position[3], float phi, float theta) {
	float negZ[3], y[3],  yStd[3] = {0.0, 1.0, 0.0};
	float negZStd[3] = {0.0, 0.0, -1.0}, rot[3][3];
	vec3Spherical(1.0, phi, theta, negZ);
	vec3Spherical(1.0, M_PI / 2.0 - phi, theta + M_PI, y);
	mat33BasisRotation(yStd, negZStd, y, negZ, rot);
	isoSetRotation(&(cam->isometry), rot);
	isoSetTranslation(&(cam->isometry), position);
}


