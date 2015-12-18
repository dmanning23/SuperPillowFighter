//////////////////////////////////////////////////////////////////////////////////////////
//File Name:		GeometryTypes.h
// 
//Author:			Kevin Cecelski
//
//Date Created:		8/14/2004
//
//Purpose:			This declares some structures needed for manipulating
//					and rendering geometry.
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GEOM_TYPES_H_
#define _GEOM_TYPES_H_
#include "..\MathLibrary\spfMath.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

// Used to store joint and weight information.
struct JointT {
    int		tiJointId;		// Id of the joint.
	float	tfWeight;		// Amount of influence this joint has on this vertex.
};

// A vertex used for rendering and animation.
struct VertexT {
	GLfloat			tfTex[2];		// The texture coordinates for this vertex.
	Vec4DT			tNormal;		// This vertex's normal.
	Vec4DT			tPosition;		// Position of this vertex.
	JointT			tuJoints[3];	// The joints that influence this vertex.
};

// A structure used to store a model's appearance.
struct GeometryT {
	unsigned int	tuNumVerts;		// Number of vertices in the mesh.
	int				tiNumIndices;	// The number of indices for the mesh.
	VertexT			*tpVertices;	// The vertices for this mesh.
	int				*tpIndices;		// The indices for this mesh.
	unsigned int	tuTexID;		// The id of the bitmap used to texture this model.
	int				tiVertID;		// The id of the vertex buffer for this object.
	GLfloat			tfRGBAColor[4];	// The color for this model.
};

#endif