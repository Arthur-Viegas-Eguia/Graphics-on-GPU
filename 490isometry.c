


/* Describes an isometry as a rotation followed by a translation. Can be used 
to describe the position and orientation of a rigid body. If the position is 
the translation, and the columns of the rotation are the local coordinate axes 
in global coordinates, then the isometry takes local coordinates to global. */

/* Feel free to read from, but not write to, this struct's members. */
typedef struct isoIsometry isoIsometry;
struct isoIsometry {
	float translation[3];
	float rotation[3][3];
};

/* Sets the rotation. */
void isoSetRotation(isoIsometry *iso, const float rot[3][3]) {
	vecCopy(9, (float *)rot, (float *)(iso->rotation));
}

/* Sets the translation. */
void isoSetTranslation(isoIsometry *iso, const float transl[3]) {
	vecCopy(3, transl, iso->translation);
}

/* Applies the rotation and translation to a point. The output CANNOT safely 
alias the input. */
void isoTransformPoint(
        const isoIsometry *iso, const float p[3], float isoP[3]) {
	mat331Multiply(iso->rotation, p, isoP);
	vecAdd(3, isoP, iso->translation, isoP);
}

/* Applies the inverse of the isometry to a point. If you transform a point and 
then untransform the result, then you recover the original point. Similarly, if 
you untransform a point and then transform the result, then you recover the 
original point. The output CANNOT safely alias the input. */
void isoUntransformPoint(
        const isoIsometry *iso, const float isoP[3], float p[3]) {
			float tempP[3];
			vecSubtract(3, isoP, iso->translation, tempP);
			mat331TransposeMultiply(iso->rotation, tempP, p);
}

/* Applies the rotation to a direction vector (typically unit). The output 
CANNOT safely alias the input. */
void isoRotateDirection(
        const isoIsometry *iso, const float d[3], float rotD[3]) {
	mat331Multiply(iso->rotation, d, rotD);
}

/* Applies the inverse rotation to a direction vector (typically unit). The 
output CANNOT safely alias the input. */
void isoUnrotateDirection(
        const isoIsometry *iso, const float rotD[3], float d[3]) {
	mat331TransposeMultiply(iso->rotation, rotD, d);
}

/* Fills homog with the homogeneous version of the isometry. */
void isoGetHomogeneous(const isoIsometry *iso, float homog[4][4]) {
	mat44Isometry(iso->rotation, iso->translation, homog);
}

/* Fills homog with the homogeneous version of the inverse isometry. That is, 
the product of this matrix and the one from isoGetHomogeneous is the identity 
matrix. */
void isoGetInverseHomogeneous(const isoIsometry *iso, float homogInv[4][4]) {
	float rot[3][3], translation[3];
	mat33Transpose(iso->rotation, rot);
	mat331Multiply(rot, iso->translation, translation);
	vecScale(3, -1, translation, translation);
	mat44Isometry(rot, translation, homogInv);
}


