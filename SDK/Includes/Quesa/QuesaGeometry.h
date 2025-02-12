/*! @header QuesaGeometry.h
        Declares the Quesa geometry objects.
        
	@ignore	_Nullable
	@ignore _Nonnull
	@ignore	_Null_unspecified
 */
/*  NAME:
        QuesaGeometry.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Copyright (c) 1999-2019, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
#ifndef QUESA_GEOMETRY_HDR
#define QUESA_GEOMETRY_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"
#include "QuesaSet.h"

// Disable QD3D header
#if defined(__QD3DGEOMETRY__)
#error
#endif

#define __QD3DGEOMETRY__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *	@enum
 *      TQ3GeneralPolygonShapeHint
 *	@discussion
 *		A general polygon has a shape hint that may be used by the renderer to
 *		optimize drawing.
 *	@constant	kQ3GeneralPolygonShapeHintComplex	The general polygon has more
 *							than one contour, or is self-intersecting, or it is not
 *							known whether it is concave or convex.
 *	@constant	kQ3GeneralPolygonShapeHintConcave	There is exactly one contour, which
 *							is concave.
 *	@constant	kQ3GeneralPolygonShapeHintConvex	There is exactly one contour, which
 *							is convex.
 */
typedef enum TQ3GeneralPolygonShapeHint {
    kQ3GeneralPolygonShapeHintComplex           = 0,
    kQ3GeneralPolygonShapeHintConcave           = 1,
    kQ3GeneralPolygonShapeHintConvex            = 2,
    kQ3GeneralPolygonShapeHintSize32            = 0xFFFFFFFF
} TQ3GeneralPolygonShapeHint;


/*!
 *	@enum
 *      Nurb Limits
 *	@discussion
 *		Miscellaneous limits for NURB curves and patches.
 *	@constant	kQ3NURBCurveMaxOrder	Maximum order for NURB curves.
 *	@constant	kQ3NURBPatchMaxOrder	Maximum order for NURB patches. Note that this was
 *                                      set to 11 in QD3DGeometry.h, however a Develop article
 *                                      on NURBs indicated it should be 16.
 */
enum {
	kQ3NURBCurveMaxOrder = 16,
	kQ3NURBPatchMaxOrder = 16
};


/*!
 *	@enum
 *      TQ3PolyhedronEdgeMasks
 *	@discussion
 *		These are flags indicating which edges of a polyhedral triangle should be
 *		rendered.
 *	@constant	kQ3PolyhedronEdgeNone		No Edge.
 *	@constant	kQ3PolyhedronEdge01			Render the edge between vertex 0 and vertex 1.
 *	@constant	kQ3PolyhedronEdge12			Render the edge between vertex 1 and vertex 2.
 *	@constant	kQ3PolyhedronEdge20			Render the edge between vertex 2 and vertex 0.
 *	@constant	kQ3PolyhedronEdgeAll		Render all the edges.
 */
typedef enum TQ3PolyhedronEdgeMasks {
    kQ3PolyhedronEdgeNone                       = 0,
    kQ3PolyhedronEdge01                         = (1 << 0),
    kQ3PolyhedronEdge12                         = (1 << 1),
    kQ3PolyhedronEdge20                         = (1 << 2),
    kQ3PolyhedronEdgeAll                        = (kQ3PolyhedronEdge01 | kQ3PolyhedronEdge12 | kQ3PolyhedronEdge20),
    kQ3PolyhedronEdgeSize32                     = 0xFFFFFFFF
} TQ3PolyhedronEdgeMasks;





/*!
	@enum	Geometry&nbsp;Property&nbsp;Types
	
	@abstract	Object properties that may be set on geometries.
	
	@constant	kQ3GeometryPropertyNonCartoon
						This property indicates to the cartoon renderer or to the
						quantized per-pixel lighting mode of the OpenGL renderer
						that this geometry should be rendered normally.
						
						Data type: TQ3Boolean.  Default value: kQ3False.
	@constant	kQ3GeometryPropertyLayerShifts
						This property, attached to the naked geometry within a TriMesh,
						can be used to provide small shifts toward or away from the
						camera to a vertex shader.
						
						Data type: TQ3LayerShifts (variable length).
*/
enum
{
	kQ3GeometryPropertyNonCartoon                   = Q3_OBJECT_TYPE('n', 'c', 'a', 'r'),
	kQ3GeometryPropertyLayerShifts     				= Q3_OBJECT_TYPE('l', 'y', 's', 'f')
};




//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
 *	@struct
 *      TQ3BoxData
 *	@discussion
 *		Data describing the state of a box object.
 *
 *	@field		origin				Origin of the box (one of the corners).
 *	@field		orientation			Orientation vector of the box.
 *	@field		majorAxis			Major axis of the box.
 *	@field		minorAxis			Minor axis of the box.
 *	@field		faceAttributeSet	Array of attribute set objects for the 6 faces.
 *									This field may be NULL, or individual sets in the
 *									array may be NULL.
 *	@field		boxAttributeSet		Attribute set object holding attributes that apply
 *									to all of the faces.  May be NULL.
 */
typedef struct TQ3BoxData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorAxis;
    TQ3Vector3D                                 minorAxis;
    TQ3AttributeSet _Nullable * _Nullable       faceAttributeSet;
    TQ3AttributeSet _Nullable                   boxAttributeSet;
} TQ3BoxData;


/*!
 *	@struct
 *      TQ3ConeData
 *	@discussion
 *		Data describing the state of a cone object.  The orientation, major radius,
 *		and minor radius vectors need not be orthogonal, though they should be
 *		independent.  Normally these vectors (in that order) should form a right-handed system.
 *		If they form a left-handed system, then the cone is "inside out", i.e., the front face
 *		is inside.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and <code>vMax</code>
 *		allow partial cones, e.g., a cone with a wedge cut out of it or with its tip cut off.
 *		This feature was not implemented in QuickDraw 3D.  See the discussion of
 *		<code>TQ3EllipseData</code> for notes on the interpretation of <code>uMin</code> and
 *		<code>uMax</code>.
 *
 *		If <code>vMax</code> < 1, so that the tip is cut off, the top disk is considered part of
 *		an interior face, rather than a top cap.
 *
 *	@field		origin					The center of the base of the cone.
 *	@field		orientation				Vector from the origin to the tip of the cone.
 *	@field		majorRadius				A vector from the origin to a point on the perimeter
 *										of the base.
 *	@field		minorRadius				A vector from the origin to a point on the perimeter
 *										of the base.
 *	@field		uMin					Minimum value of the u parameter, which goes around
 *										the base.  Typically 0.
 *	@field		uMax					Maximum value of the u parameter, which goes around
 *										the base.  Typically 1.
 *	@field		vMin					Minimum value of the v parameter, which goes from
 *										base to tip.  Typically 0.
 *	@field		vMax					Minimum value of the v parameter, which goes from
 *										base to tip.  Typically 1.
 *	@field		caps					End cap masks, determining whether there is a bottom cap,
 *										and in the case of partial cones, determining whether
 *										there is an interior face.
 *	@field		interiorAttributeSet	Interior attributes.  Unused by QD3D.
 *	@field		faceAttributeSet		Attributes that affect the face but not the bottom.
 *										May be nullptr.
 *	@field		bottomAttributeSet		Attributes that affect the bottom end cap.  May be nullptr.
 *	@field		coneAttributeSet		Attributes for all parts of the cone.  May be nullptr.
 */
typedef struct TQ3ConeData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3EndCap                                   caps;
    TQ3AttributeSet _Nullable                   interiorAttributeSet;
    TQ3AttributeSet _Nullable                   faceAttributeSet;
    TQ3AttributeSet _Nullable                   bottomAttributeSet;
    TQ3AttributeSet _Nullable                   coneAttributeSet;
} TQ3ConeData;


/*!
 *	@struct
 *      TQ3CylinderData
 *	@discussion
 *		Data describing the state of a cylinder object.  The orientation, major radius,
 *		and minor radius vectors need not be orthogonal, though they should be
 *		independent.  Ordinarily, the vectors (orientation, major axis, minor axis)
 *		should form a right-handed coordinate system.  If they form a left-handed system,
 *		then the cylinder is "inside out", i.e., the front face is inside.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and <code>vMax</code>
 *		allow partial cylinders, e.g., a cylinder with a wedge cut out of it.
 *		This feature was not implemented in QuickDraw 3D.  See the discussion of
 *		<code>TQ3EllipseData</code> for notes on the interpretation of <code>uMin</code> and
 *		<code>uMax</code>.
 *
 *	@field		origin					The center of the base of the cylinder.
 *	@field		orientation				Vector from the origin to the center of the opposite end.
 *	@field		majorRadius				A vector from the origin to a point on the perimeter
 *										of the base.
 *	@field		minorRadius				A vector from the origin to a point on the perimeter
 *										of the base.
 *	@field		uMin					Starting value of the u parameter, which goes around
 *										the base counterclockwise.  Typically 0.
 *	@field		uMax					Ending value of the u parameter, which goes around
 *										the base counterclockwise.  Typically 1.
 *	@field		vMin					Minimum value of the v parameter, which goes from the
 *										base to the other end.  Typically 0.
 *	@field		vMax					Maximum value of the v parameter, which goes from the
 *										base to the other end.  Typically 1.
 *	@field		caps					End cap masks, determining whether the cylinder is
 *										closed on one end, the other, or both.
 *	@field		interiorAttributeSet	Interior attributes.  May be nullptr.  Unused by QD3D.
 *	@field		topAttributeSet			Attributes that affect the top end cap.  May be nullptr.
 *	@field		faceAttributeSet		Attributes that affect the face but not the bottom or top.
 *										May be nullptr.
 *	@field		bottomAttributeSet		Attributes that affect the bottom end cap.  May be nullptr.
 *	@field		cylinderAttributeSet	Attributes for all parts of the cylinder.  May be nullptr.
 */
typedef struct TQ3CylinderData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3EndCap                                   caps;
    TQ3AttributeSet _Nullable                   interiorAttributeSet;
    TQ3AttributeSet _Nullable                   topAttributeSet;
    TQ3AttributeSet _Nullable                   faceAttributeSet;
    TQ3AttributeSet _Nullable                   bottomAttributeSet;
    TQ3AttributeSet _Nullable                   cylinderAttributeSet;
} TQ3CylinderData;


/*!
 *	@struct
 *      TQ3DiskData
 *	@discussion
 *		Data describing the state of a disk object (a filled ellipse).  The major radius
 *		and minor radius vectors need not be orthogonal, though they should be independent.
 *		The direction of the front face is the cross product of the major and minor axes.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and <code>vMax</code>
 *		allow partial disks, e.g., a slice of pie (partial range of u values) or a washer
 *		(partial range of v values).  This feature was never implemented in QuickDraw 3D,
 *		although it was planned.  See the discussion of <code>TQ3EllipseData</code> for notes
 *		on the interpretation of <code>uMin</code> and <code>uMax</code>.
 *
 *		The u and v limits here do not have anything to do with the uv parametrization used by
 *		shaders.
 *
 *	@field		origin					The center of the disk.
 *	@field		majorRadius				A vector from the origin to a point on the perimeter
 *										of the disk.
 *	@field		minorRadius				A vector from the origin to a point on the perimeter
 *										of the disk.
 *	@field		uMin					Starting value of the u parameter, which goes around
 *										the perimeter counterclockwise.  Typically 0, must be
 *										in the range [0, 1].
 *	@field		uMax					Ending value of the u parameter, which goes around
 *										the perimeter counterclockwise.  Typically 1, must be
 *										in the range [0, 1].
 *	@field		vMin					Minimum value of the v parameter, which goes from the
 *										perimeter to the origin.  Typically 0, must be in the
 *										range [0, 1].
 *	@field		vMax					Maximum value of the v parameter, which goes from the
 *										perimeter to the origin.  Typically 1, must be in the
 *										range [0, 1].
 *	@field		diskAttributeSet		Attributes for the disk.  May be nullptr.
 */
typedef struct TQ3DiskData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3AttributeSet _Nullable                   diskAttributeSet;
} TQ3DiskData;


/*!
 *	@struct
 *      TQ3EllipseData
 *	@discussion
 *		Data describing the state of an ellipse.  The major radius and minor radius
 *		vectors need not be orthogonal, though they should be independent.
 *
 *		You can make a partial ellipse by using values other than 0 and 1 for the
 *		<code>uMin</code> and <code>uMax</code> fields.  The ellipse travels
 *		counterclockwise from the <code>uMin</code> to the <code>uMax</code> value.
 *		It is permissible for <code>uMin</code> to be greater than <code>uMax</code>.
 *		For example, if <code>uMin</code> = 7/8 and <code>uMax</code> = 1/8, then the
 *		ellipse covers a range of 45 degrees, including the zero angle.
 *
 *	@field		origin					Center of the ellipse.
 *	@field		majorRadius				A vector from the origin to a point on the curve.
 *	@field		minorRadius				Another vector from the origin to a point on the curve.
 *	@field		uMin					Starting value of the u parameter.  Typically 0.
 *	@field		uMax					Ending value of the u parameter.  Typically 1.
 *	@field		ellipseAttributeSet		Attributes for the ellipse.  May be nullptr.
 */
typedef struct TQ3EllipseData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    TQ3AttributeSet _Nullable                   ellipseAttributeSet;
} TQ3EllipseData;


/*!
 *	@struct
 *      TQ3EllipsoidData
 *	@discussion
 *		Data describing the state of an ellipsoid.  The major axis, minor axis, and
 *		orientation vectors need not be orthogonal, but should be independent, i.e.,
 *		not coplanar.
 *
 *		The ellipsoid can be expressed by a parametric equation
 *
 *		f(u,v) = origin - cos(&pi;v)orientation + sin(&pi;v)(cos(2&pi;u)majorRadius + sin(2&pi;u)minorRadius)
 *
 *		where u and v range from 0 to 1.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and <code>vMax</code>
 *		allow partial ellipsoids, e.g., an ellipsoid with a wedge cut out of it.
 *		Partial ellipsoids were not implemented in QuickDraw 3D.
  *
 *	@field		origin					Center of the ellipsoid.
 *	@field		orientation				A vector from the origin to a point on the ellipsoid.
 *	@field		majorRadius				A vector from the origin to a point on the ellipsoid.
 *	@field		minorRadius				A vector from the origin to a point on the ellipsoid.
 *	@field		uMin					Minimum value of the u parameter, which goes around
 *										the perimeter (the ellipse determined by the major and
 *										minor axes).  Typically 0.
 *	@field		uMax					Maximum value of the u parameter.  Typically 1.
 *	@field		vMin					Minimum value of the v parameter, which goes from the
 *										south pole (origin - orientation) to the north pole
 *										(origin + orientation).  Typically 0.
 *	@field		vMax					Minimum value of the v parameter.  Typically 1.
 *	@field		caps					Style of caps to be used on partial ellipsoids.
 *	@field		interiorAttributeSet	Interior attributes, used for end or interior caps.
 *										May be nullptr.
 *	@field		ellipsoidAttributeSet	Attributes for the ellipsoid surface.  May be nullptr.
 */
typedef struct TQ3EllipsoidData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3EndCap                                   caps;
    TQ3AttributeSet _Nullable                   interiorAttributeSet;
    TQ3AttributeSet _Nullable                   ellipsoidAttributeSet;
} TQ3EllipsoidData;


/*!
 *	@struct
 *      TQ3GeneralPolygonContourData
 *	@discussion
 *		An ordered list of vertices forming a contour of a general polygon.
 *		Used within the <code>TQ3GeneralPolygonData</code> structure.
 *	@field		numVertices				Number of vertices.  Must be at least 3.
 *	@field		vertices				Array of vertices.
 */
typedef struct TQ3GeneralPolygonContourData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 * _Nonnull vertices;
} TQ3GeneralPolygonContourData;


/*!
 *	@struct
 *      TQ3GeneralPolygonData
 *	@discussion
 *		Data describing a general polygon.  A general polygon is a closed figure
 *		defined by one or more coplanar closed curves called contours.  If there are
 *		holes, the even-odd rule determines which parts are inside.
 *	@field		numContours				Number of contours in the following array.
 *										Must be at least 1.
 *	@field		contours				Array of contours.
 *	@field		shapeHint				Information about the shape of the general polygon,
 *										which may be used by the renderer to optimize
 *										drawing.
 *	@field		generalPolygonAttributeSet		Set of attributes for the polygon.
 *										May be nullptr.
 */
typedef struct TQ3GeneralPolygonData {
    TQ3Uns32                                    numContours;
    TQ3GeneralPolygonContourData                * _Nullable contours;
    TQ3GeneralPolygonShapeHint                  shapeHint;
    TQ3AttributeSet _Nullable                   generalPolygonAttributeSet;
} TQ3GeneralPolygonData;


/*!
 *	@struct
 *      TQ3LineData
 *	@discussion
 *		Data describing a line.
 *	@field		vertices				Array of two vertices, the ends of the line.
 *	@field		lineAttributeSet		Set of attributes for the line.  May be nullptr.
 */
typedef struct TQ3LineData {
    TQ3Vertex3D                                 vertices[2];
    TQ3AttributeSet _Nullable                   lineAttributeSet;
} TQ3LineData;


/*!
 *	@struct
 *      TQ3MarkerData
 *	@discussion
 *		Data describing a bitmap marker, a 2-dimensional image drawn on top of a scene
 *		at a specified location.
 *	@field		location				Location of the marker, in local coordinates.
 *	@field		xOffset					Horizontal offset from the <code>location</code> to the
 *										upper left corner of the marker, in pixels.
 *	@field		yOffset					Vertical offset from the <code>location</code> to the
 *										upper left corner of the marker, in pixels.
 *	@field		bitmap					A bitmap.  Each bit corresponds to a pixel in the image.
 *	@field		markerAttributeSet		Marker attributes, which can for instance modify the color
 *										or transparency of the 1 bits.  May be nullptr.
 */
typedef struct TQ3MarkerData {
    TQ3Point3D                                  location;
    TQ3Int32                                    xOffset;
    TQ3Int32                                    yOffset;
    TQ3Bitmap                                   bitmap;
    TQ3AttributeSet _Nullable                   markerAttributeSet;
} TQ3MarkerData;


/*!
 *	@typedef
 *      TQ3MeshVertex
 *	@discussion
 *      Opaque pointer representing a vertex of a Mesh.
 */
typedef struct OpaqueTQ3MeshVertex              *TQ3MeshVertex;


/*!
 *	@typedef
 *      TQ3MeshContour
 *	@discussion
 *      Opaque pointer representing a contour of a Mesh, one of the closed
 *		paths that bounds a face.
 */
typedef struct OpaqueTQ3MeshContour             *TQ3MeshContour;


/*!
 *	@typedef
 *      TQ3MeshFace
 *	@discussion
 *      Opaque pointer representing a face of a Mesh.  This is a polygonal
 *		figure, normally planar, which may contain holes.
 */
typedef struct OpaqueTQ3MeshFace                *TQ3MeshFace;


/*!
 *	@typedef
 *      TQ3MeshEdge
 *	@discussion
 *      Opaque pointer representing an edge of a Mesh, a straight line
 *		segment that connects two vertices.
 */
typedef struct OpaqueTQ3MeshEdge                *TQ3MeshEdge;


/*!
 *	@typedef
 *      TQ3MeshComponent
 *	@discussion
 *      Opaque pointer representing a connected component of a Mesh.
 */
typedef struct OpaqueTQ3MeshComponent           *TQ3MeshComponent;


/*!
 *	@struct
 *      TQ3MeshIterator
 *	@discussion
 *      This structure is used for iterating through various parts of a
 *		Mesh.  You should consider it opaque, because the meanings of the
 *		fields are not documented.
 */
typedef struct TQ3MeshIterator {
    void                                        * _Null_unspecified var1;
    void                                        * _Null_unspecified var2;
    void                                        * _Null_unspecified var3;
    struct {
        void                                    * _Null_unspecified field1;
        char                                    field2[4];
    } var4;
} TQ3MeshIterator;


#if QUESA_ALLOW_QD3D_EXTENSIONS

// Mesh data (all not available in QD3D)
/*!
 *	@struct
 *      TQ3MeshCornerData
 *	@discussion
 *		Data describing a corner for a mesh vertex.
 *
 *		Note that a <code>TQ3MeshCornerData</code> is an external data structure
 *		for use with the <code>Q3Mesh_Set/Get/EmptyData</code> functions.
 *
 *		<em>This structure is not available in QD3D.</em>
 *	@field		numFaces				Number of faces.  Must be at least 1.
 *	@field		faceIndices				Indices of the faces.
 *	@field		cornerAttributeSet		Set of attributes for the corner.  <em>Should not be nullptr.</em>
 */
typedef struct TQ3MeshCornerData {
    TQ3Uns32                                    numFaces;
    TQ3Uns32                  			        * _Nonnull faceIndices;
    TQ3AttributeSet _Nonnull                    cornerAttributeSet;
} TQ3MeshCornerData;

/*!
 *	@struct
 *      TQ3MeshVertexData
 *	@discussion
 *		Data describing a vertex within a mesh.
 *
 *		Note that a <code>TQ3MeshVertexData</code> is an external data structure
 *		for use with the <code>Q3Mesh_Set/Get/EmptyData</code> functions.
 *		In contrast, a <code>TQ3MeshVertex</code> is a pointer to an opaque internal
 *		data structure, which is most likely not a <code>TQ3MeshVertexData</code>.
 *
 *		<em>This structure is not available in QD3D.</em>
 *  @field		point					Location of the vertex.
 *	@field		numCorners				Number of corners in the following array.  May be 0
 *										if you do not want to specify any corners.
 *	@field		corners					Pointer to an array of corners.  May be nullptr if you
 *										also specify 0 for <code>numCorners</code>.
 *	@field		attributeSet			Attribute set for the vertex.
 */
typedef struct TQ3MeshVertexData {
    TQ3Point3D                                  point;
    TQ3Uns32                                    numCorners;
    TQ3MeshCornerData                           * _Nullable corners;
    TQ3AttributeSet  _Nullable                  attributeSet;
} TQ3MeshVertexData;

/*!
 *	@struct
 *      TQ3MeshEdgeData
 *	@discussion
 *		Data describing an edge within a mesh.
 *		Used within the <code>TQ3MeshData</code> structure.
 *
 *		Note that a <code>TQ3MeshEdgeData</code> is an external data structure
 *		for use with the <code>Q3Mesh_Set/Get/EmptyData</code> functions.
 *		In contrast, a <code>TQ3MeshEdge</code> is a pointer to an opaque internal
 *		data structure, which is most likely not a <code>TQ3MeshEdgeData</code>.
 *
 *		<em>This structure is not available in QD3D.</em>
 *	@field		vertexIndices			Indices of the two vertices that are ends of the edge.
 *	@field		edgeAttributeSet		Set of attributes for the edge.  May be nullptr.
 */
typedef struct TQ3MeshEdgeData {
    TQ3Uns32                                    vertexIndices[2];
    TQ3AttributeSet _Nullable                   edgeAttributeSet;
} TQ3MeshEdgeData;

/*!
 *	@struct
 *      TQ3MeshContourData
 *	@discussion
 *		Data describing a contour within a mesh face.
  *
 *		Note that a <code>TQ3MeshContourData</code> is an external data structure
 *		for use with the <code>Q3Mesh_Set/Get/EmptyData</code> functions.
 *		In contrast, a <code>TQ3MeshContour</code> is a pointer to an opaque internal
 *		data structure, which is most likely not a <code>TQ3MeshContourData</code>.
 *
 *		<em>This structure is not available in QD3D.</em>
*	@field		numVertices				Number of vertices.  Must be at least 3.
 *	@field		vertexIndices			Indices of the vertices of the contour.
 */
typedef struct TQ3MeshContourData {
    TQ3Uns32                                    numVertices;
    TQ3Uns32                                    * _Nullable vertexIndices;
} TQ3MeshContourData;

/*!
 *	@struct
 *      TQ3MeshFaceData
 *	@discussion
 *		Data describing a face within a mesh.
 *
 *		Note that a <code>TQ3MeshFaceData</code> is an external data structure
 *		for use with the <code>Q3Mesh_Set/Get/EmptyData</code> functions.
 *		In contrast, a <code>TQ3MeshFace</code> is a pointer to an opaque internal
 *		data structure, which is most likely not a <code>TQ3MeshFaceData</code>.
 *
 *		<em>This structure is not available in QD3D.</em>
 *	@field		numContours				Number of contours.  Must be at least 1.
 *	@field		contours				Contours of the face.
 *	@field		faceAttributeSet		Set of attributes for the face.  May be nullptr.
 */
typedef struct TQ3MeshFaceData {
    TQ3Uns32                                    numContours;
    TQ3MeshContourData                          * _Nullable contours;
    TQ3AttributeSet _Nullable                   faceAttributeSet;
} TQ3MeshFaceData;

/*!
 *	@struct
 *      TQ3MeshData
 *	@discussion
 *		Data describing a mesh.
 *
 *		Note that a <code>TQ3MeshData</code> is an external data structure
 *		for use with the <code>Q3Mesh_Set/Get/EmptyData</code> functions.
 *
 *		<em>This structure is not available in QD3D.</em>
 *	@field		numVertices				Number of vertices in the following array.
 *	@field		vertices				Pointer to array of vertices of the mesh.
 *	@field		numEdges				Number of edges in the following array.  May be 0
 *										if you do not want to specify any edges.
 *	@field		edges					Pointer to an array of edges.  May be nullptr if you
 *										also specify 0 for <code>numEdges</code>.
 *	@field		numFaces				Number of faces in the mesh.
 *	@field		faces					Pointer to an array of faces.
 *	@field		meshAttributeSet		Set of attributes for the mesh.  May be nullptr.
 */
typedef struct TQ3MeshData {
    TQ3Uns32									numVertices;
    TQ3MeshVertexData							* _Nullable vertices;
    TQ3Uns32									numEdges;
    TQ3MeshEdgeData								* _Nullable edges;
    TQ3Uns32									numFaces;
    TQ3MeshFaceData								* _Nullable faces;
    TQ3AttributeSet _Nullable                   meshAttributeSet;
} TQ3MeshData;

#endif // QUESA_ALLOW_QD3D_EXTENSIONS


/*!
 *	@struct
 *      TQ3NURBCurveData
 *	@discussion
 *      Data defining a NURBS curve, a 3-dimensional curve represented by
 *		a nonuniform rational B-spline equation.
 *	@field		order							The order of the curve, one more than the
 *												degree of the polynomials defining the curve.
 *												Must be greater than one.
 *	@field		numPoints						Number of control points.  Must be greater than
 *												or equal to the order.
 *	@field		controlPoints					Array of rational 4-dimensional control points.
 *	@field		knots							Array of knots that define the curve.  The number
 *												of knots must equal the sum of <code>order</code>
 *												and <code>numPoints</code>.  The values must be
 *												nondecreasing.
 *	@field		curveAttributeSet				Set of attributes for the curve.  May be nullptr.
 */
typedef struct TQ3NURBCurveData {
    TQ3Uns32                                    order;
    TQ3Uns32                                    numPoints;
    TQ3RationalPoint4D                          * _Nonnull controlPoints;
    float                                       * _Nonnull knots;
    TQ3AttributeSet _Nullable                   curveAttributeSet;
} TQ3NURBCurveData;


// NURBPatch data
/*!
 *	@struct		TQ3NURBPatchTrimCurveData
 *	@discussion
 *		Curve that trims a NURB patch.  Note that this is similar to TQ3NURBCurveData,
 *		but lacks an attribute set.
 *	@field		order							The order of the curve, one more than the
 *												degree of the polynomials defining the curve.
 *												Must be greater than one.
 *	@field		numPoints						Number of control points.  Must be greater than
 *												or equal to the order.
 *	@field		controlPoints					Array of rational 4-dimensional control points.
 *	@field		knots							Array of knots that define the curve.  The number
 *												of knots must equal the sum of <code>order</code>
 *												and <code>numPoints</code>.  The values must be
 *												nondecreasing.
 */
typedef struct TQ3NURBPatchTrimCurveData {
    TQ3Uns32                                    order;
    TQ3Uns32                                    numPoints;
    TQ3RationalPoint3D                          * _Nonnull controlPoints;
    float                                       * _Nonnull knots;
} TQ3NURBPatchTrimCurveData;

/*!
 *	@struct		TQ3NURBPatchTrimLoopData
 *	@discussion
 *		Data describing curves that trim a NURB patch.
 *	@field		numTrimCurves			Number of curves in the following array.
 *	@field		trimCurves				Pointer to an array of curves.
 */
typedef struct TQ3NURBPatchTrimLoopData {
    TQ3Uns32                                    numTrimCurves;
    TQ3NURBPatchTrimCurveData                   * _Nonnull trimCurves;
} TQ3NURBPatchTrimLoopData;

/*!
 *	@struct		TQ3NURBPatchData
 *	@discussion
 *		Data describing a NURB patch, a surface defined by a ratio of B-spline surfaces.
 *	@field		uOrder					Order of the NURB patch in the u parametric direction.
 *										The order is one greater than the degree of the
 *										polynomial functions involved, and must be
 *										greater than one.
 *	@field		vOrder					Order of the NURB patch in the v parametric direction.
 *										The order is one greater than the degree of the
 *										polynomial functions involved, and must be
 *										greater than one.
 *	@field		numRows					Number of control points in the u parametric equation.
 *										Must be greater than 1.
 *	@field		numColumns				Number of control points in the v parametric equation.
 *										Must be greater than 1.
 *	@field		controlPoints			Array of rational 4-dimensional control points.
 *										They are listed first in the direction of increasing u
 *										and then in the direction of increasing v.
 *										The number of control points is <code>numRows</code> times
 *										<code>numColumns</code>.
 *	@field		uKnots					Array of knots in the u parametric direction.  The
 *										number of these knots is the sum of <code>uOrder</code>
 *										and <code>numColumns</code>.  The values must be
 *										nondecreasing.
 *	@field		vKnots					Array of knots in the v parametric direction.  The
 *										number of these knots is the sum of <code>vOrder</code>
 *										and <code>numRows</code>.  The values must be
 *										nondecreasing.
 *	@field		numTrimLoops			Number of trim loops in the following array.
 *										Currently this should be 0.
 *	@field		trimLoops				Pointer to an array of trim loop structures.
 *										Currently this should be nullptr.
 *	@field		patchAttributeSet		Set of attributes for the patch.  May be nullptr.
 */
typedef struct TQ3NURBPatchData {
    TQ3Uns32                                    uOrder;
    TQ3Uns32                                    vOrder;
    TQ3Uns32                                    numRows;
    TQ3Uns32                                    numColumns;
    TQ3RationalPoint4D                          * _Nonnull controlPoints;
    float                                       * _Nonnull uKnots;
    float                                       * _Nonnull vKnots;
    TQ3Uns32                                    numTrimLoops;
    TQ3NURBPatchTrimLoopData                    * _Nullable trimLoops;
    TQ3AttributeSet _Nullable                   patchAttributeSet;
} TQ3NURBPatchData;


/*!
 *	@struct		TQ3PixmapMarkerData
 *	@discussion
 *		Data describing a pixmap marker, a 2-dimensional color image drawn on top of a scene
 *		at a specified location.
 *	@field		position				Location of the marker, in local coordinates.
 *	@field		xOffset					Horizontal offset from the <code>position</code> to the
 *										upper left corner of the marker, in pixels.
 *	@field		yOffset					Vertical offset from the <code>position</code> to the
 *										upper left corner of the marker, in pixels.
 *	@field		pixmap					A pixmap.
 *	@field		pixmapMarkerAttributeSet		A set of attributes for the marker.  May be nullptr.
 */
typedef struct TQ3PixmapMarkerData {
    TQ3Point3D                                  position;
    TQ3Int32                                    xOffset;
    TQ3Int32                                    yOffset;
    TQ3StoragePixmap                            pixmap;
    TQ3AttributeSet _Nullable                   pixmapMarkerAttributeSet;
} TQ3PixmapMarkerData;


/*!
 *	@struct		TQ3PointData
 *	@discussion
 *		Data describing a point object.
 *	@field		point					Location of the point.
 *	@field		pointAttributeSet		Set of attributes for the point.  May be nullptr.
 */
typedef struct TQ3PointData {
    TQ3Point3D                                  point;
    TQ3AttributeSet _Nullable                   pointAttributeSet;
} TQ3PointData;


/*!
 *	@struct		TQ3PolygonData
 *	@discussion
 *		Data describing a simple convex polygon.
 *	@field		numVertices				Number of vertices.  Must be at least 3.
 *	@field		vertices				Pointer to an array of vertices.
 *	@field		polygonAttributeSet		Set of attributes for the polygon.  May be nullptr.
 */
typedef struct TQ3PolygonData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 * _Nonnull vertices;
    TQ3AttributeSet _Nullable                   polygonAttributeSet;
} TQ3PolygonData;


// Polyhedron data
/*!
 *	@typedef	TQ3PolyhedronEdge
 *	@discussion
 *		A combination of masks of type TQ3PolyhedronEdgeMasks indicating which edges of
 *		a triangle are to be rendered.
 */
typedef TQ3Uns32                                TQ3PolyhedronEdge;

/*!
 *	@struct		TQ3PolyhedronEdgeData
 *	@discussion
 *		Data describing an edge within a polyhedron.
 *	@field		vertexIndices			Indices of the two vertices that are ends of the edge.
 *	@field		triangleIndices			Indices of the two triangles that contain the edge.
 *	@field		edgeAttributeSet		Set of attributes for the edge.  May be nullptr.
 */
typedef struct TQ3PolyhedronEdgeData {
    TQ3Uns32                                    vertexIndices[2];
    TQ3Uns32                                    triangleIndices[2];
    TQ3AttributeSet _Nullable                   edgeAttributeSet;
} TQ3PolyhedronEdgeData;

/*!
 *	@struct		TQ3PolyhedronTriangleData
 *	@discussion
 *		Data describing a triangle within a polyhedron.
 *	@field		vertexIndices			Indices of the 3 vertices of the triangle.
 *	@field		edgeFlag				Flags indicating which edges of the triangle should
 *										be rendered.  See <code>TQ3PolyhedronEdgeMasks</code>.
 *	@field		triangleAttributeSet	Set of attributes for the triangle.  May be nullptr.
 */
typedef struct TQ3PolyhedronTriangleData {
    TQ3Uns32                                    vertexIndices[3];
    TQ3PolyhedronEdge                           edgeFlag;
    TQ3AttributeSet _Nullable                   triangleAttributeSet;
} TQ3PolyhedronTriangleData;

/*!
 *	@struct		TQ3PolyhedronData
 *	@discussion
 *		Data describing a polyhedron.
 *	@field		numVertices				Number of vertices in the following array.
 *	@field		vertices				Pointer to array of vertices of the polyhedron.
 *	@field		numEdges				Number of edges in the following array.  May be 0
 *										if you do not want to specify any edges.
 *	@field		edges					Pointer to an array of edges.  May be nullptr if you
 *										also specify 0 for <code>numEdges</code>.
 *	@field		numTriangles			Number of triangles (faces) in the polygon.
 *	@field		triangles				Pointer to an array of triangles.
 *	@field		polyhedronAttributeSet	Set of attributes for the polyhedron.  May be nullptr.
 */
typedef struct TQ3PolyhedronData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 * _Nullable vertices;
    TQ3Uns32                                    numEdges;
    TQ3PolyhedronEdgeData                       * _Nullable edges;
    TQ3Uns32                                    numTriangles;
    TQ3PolyhedronTriangleData                   * _Nonnull triangles;
    TQ3AttributeSet _Nullable                   polyhedronAttributeSet;
} TQ3PolyhedronData;


/*!
 *	@struct		TQ3PolyLineData
 *	@discussion
 *		Data describing a connected but not closed curve made up of several straight line
 *		segments.
 *	@field		numVertices				Number of vertices of the curve (one more than the
 *										number of line segments).  Must be at least 2.
 *	@field		vertices				Pointer to an array of vertices.
 *	@field		segmentAttributeSet		Pointer to an array of attribute sets, one for each
 *										segment.  If you do not want to assign attributes
 *										to any segment, this pointer may be nullptr.
 *	@field		polyLineAttributeSet	Set of attributes for the whole curve.  May be nullptr.
 */
typedef struct TQ3PolyLineData {
    TQ3Uns32                                    numVertices;
    TQ3Vertex3D                                 * _Nonnull vertices;
    TQ3AttributeSet _Nullable                   * _Nullable segmentAttributeSet;
    TQ3AttributeSet _Nullable                   polyLineAttributeSet;
} TQ3PolyLineData;


/*!
 *	@struct		TQ3TorusData
 *	@discussion
 *		Data describing a generalized torus.  A torus is a surface formed by rotating an
 *		ellipse about an axis that is in the same plane as the ellipse but does not pass
 *		through the ellipse.
 *
 *		To express the torus as a parametric equation, we first define the
 *		elliptical central axis by the equation
 *
 *		axis(u) = cos(2&pi;u) majorRadius + sin(2&pi;u) minorRadius
 *
 *		where majorRadius and minorRadius are independent vectors, and u varies
 *		from 0 to 1.  Then we define the torus surface by
 *
 *		f(u,v) = origin + axis(u) - sin(2&pi;v) orientation -
 *				(cos(2&pi;v) * ratio * |orientation| / |axis(u)|) axis(u)
 *
 *		where v also varies from 0 to 1 and vertical bars indicate length of a
 *		vector.
 *
 *		The kind of torus usually encountered in elementary mathematics, with circular
 *		cross sections, would be one where the <code>orientation</code>, <code>majorRadius</code>,
 *		and <code>minorRadius</code> vectors are mutually orthogonal, where
 *		<code>majorRadius</code> and <code>minorRadius</code> have the same length, and where
 *		<code>ratio</code> is 1.
 *
 *		The values <code>uMin</code>, <code>uMax</code>, <code>vMin</code>, and
 *		<code>vMax</code> allow one to create partial tori, e.g., a torus with
 *		a wedge cut out of it.  QuickDraw 3D did not implement this feature.
 *
 *	@field		origin					Center of rotation.
 *	@field		orientation				Vector whose direction is the axis of rotation, and
 *										whose length is the length of the
 *										radius of the cross-sectional ellipse
 *										in the direction of the axis of rotation.
 *	@field		majorRadius				Vector from the origin to the center of the ellipse.
 *	@field		minorRadius				Vector from the origin to the center of a different
 *										cross-sectional ellipse.
 *	@field		ratio					The ratio between the radius of the
 *										cross-sectional ellipse in the
 *										direction of <code>majorRadius</code>,
 *										and the length of
 *										<code>orientation</code>.
 *	@field		uMin					Minimum value in the u parametric
 *										direction (the long way around.)
 *										Normally 0, must be in interval [0, 1].
 *	@field		uMax					Maximum value in the u parametric
 *										direction (the long way around.)
 *										Normally 1, must be in interval [0, 1].
 *	@field		vMin					Minimum value in the v parametric
 *										direction (the short way around.)
 *										Normally 0, must be in interval [0, 1].
 *	@field		vMax					Maximum value in the v parametric
 *										direction (the short way around.)
 *										Normally 1, must be in interval [0, 1].
 *	@field		caps					Cap style.  Should be kQ3EndCapNone.
 *
 *	@field		interiorAttributeSet	Interior attribute set.  Not currently used.
 *
 *	@field		torusAttributeSet		Overall attribute set.
 */
typedef struct TQ3TorusData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Vector3D                                 majorRadius;
    TQ3Vector3D                                 minorRadius;
    float                                       ratio;
    float                                       uMin;
    float                                       uMax;
    float                                       vMin;
    float                                       vMax;
    TQ3EndCap                                   caps;
    TQ3AttributeSet _Nullable                   interiorAttributeSet;
    TQ3AttributeSet _Nullable                   torusAttributeSet;
} TQ3TorusData;


/*!
 *	@struct		TQ3TriangleData
 *	@discussion
 *		Data defining a single triangle.
 *	@field		vertices				The 3 vertices of the triangle.
 *	@field		triangleAttributeSet	Set of attributes for the triangle.  May be nullptr.
 */
typedef struct TQ3TriangleData {
    TQ3Vertex3D                                 vertices[3];
    TQ3AttributeSet _Nullable                   triangleAttributeSet;
} TQ3TriangleData;


/*!
 *	@struct		TQ3TriGridData
 *	@discussion
 *		Data describing a TriGrid object.  A TriGrid is defined by a grid of points in
 *		3-dimensional space.  Each set of 4 adjacent points (in the mth and (m+1)st row and
 *		nth and (n+1)st column) defines a quadrilateral that can be subdivided into
 *		triangles in 2 natural ways.  This subdivision is done in an alternating way,
 *		such that the vertex in the first row and first column belongs to two triangles.
 *	@field		numRows					Number of rows of vertices.  Should be at least 2.
 *	@field		numColumns				Number of columns of vertices.  Should be at least 2.
 *	@field		vertices				Pointer to an array of vertices, listed in rectangular
 *										order, first by increasing columns and then by
 *										increasing rows.  The number of vertices should be
 *										<code>numRows</code> times <code>numColumns</code>.
 *	@field		facetAttributeSet		Pointer to an array of triangle attributes.  May be
 *										nullptr, but otherwise should point to an array of
 *										2&times;(<code>numRows</code>-1)&times;(<code>numColumns</code>-1)
 *										attribute sets.
 *	@field		triGridAttributeSet		Set of attributes for the whole TriGrid object.
 *										May be nullptr.	
 */
typedef struct TQ3TriGridData {
    TQ3Uns32                                    numRows;
    TQ3Uns32                                    numColumns;
    TQ3Vertex3D                                 * _Nonnull vertices;
    TQ3AttributeSet _Nullable                   * _Nullable facetAttributeSet;
    TQ3AttributeSet _Nullable                   triGridAttributeSet;
} TQ3TriGridData;


// TriMesh data
/*!
 *	@struct		TQ3TriMeshTriangleData
 *	@discussion
 *		Data defining a triangle within a TriMesh.
 *	@field		pointIndices			Array of 3 indices into the TriMesh array of points.
 */
typedef struct TQ3TriMeshTriangleData {
    TQ3Uns32                                    pointIndices[3];
} TQ3TriMeshTriangleData;

/*!
 *	@struct		TQ3TriMeshEdgeData
 *	@discussion
 *		Data defining an edge within a TriMesh.
 *	@field		pointIndices			Array of 2 indices into the TriMesh array of points,
 *										the ends of the edge.
 *	@field		triangleIndices			Array of 2 indices into the TriMesh array of triangles,
 *										the triangles containing the edge.
 */
typedef struct TQ3TriMeshEdgeData {
    TQ3Uns32                                    pointIndices[2];
    TQ3Uns32                                    triangleIndices[2];
} TQ3TriMeshEdgeData;

/*!
 *	@struct		TQ3TriMeshAttributeData
 *	@discussion
 *		A structure holding an array of attribute data of a particular type.
 *	@field		attributeType			Type of the attribute.
 *	@field		data					Pointer to an array of attribute data.  The number of
 *										members in the array is determined by context in the
 *										enclosing <code>TQ3TriMeshData</code> structure; for
 *										instance, if these are vertex attributes, the number
 *										of members is the number of vertices.  The size of each
 *										member depends on the attribute type.
 *	@field		attributeUseArray		For non-custom attribute types, this must be nullptr.
 *										For custom attribute types, it can point to an array
 *										of 1-byte 0s and 1s, with 1s indicating which items
 *										have the custom attribute.
 */
typedef struct TQ3TriMeshAttributeData {
    TQ3AttributeType                            attributeType;
    void                                        * _Nullable data;
    char                                        * _Nullable attributeUseArray;
} TQ3TriMeshAttributeData;

/*!
 *	@struct		TQ3TriMeshData
 *	@discussion
 *		Structure describing a TriMesh object, which is an object composed of vertices, edges,
 *		and triangular faces.  The main difference between a TriMesh and a Polyhedron is that
 *		attribute data for vertices, edges, and faces are not stored in attribute sets, but
 *		in arrays of attribute data.  This is normally more efficient, so long as you are
 *		going to be assigning an attribute to every vertex, or every edge, or every face.
 *	@field		triMeshAttributeSet		Set of attributes for the whole object.  May be nullptr.
 *	@field		numTriangles			Number of triangles in the following array.
 *	@field		triangles				Pointer to an array of triangle data.
 *	@field		numTriangleAttributeTypes	Number of triangle attribute types listed in the
 *											following array.
 *	@field		triangleAttributeTypes		Pointer to an array of attribute data for triangles (faces).
 *											May be nullptr, if <code>numTriangleAttributeTypes</code> is 0.
 *	@field		numEdges				Number of edges in the following array.
 *	@field		edges					Pointer to an array of edge data.  If you do not wish to
 *										specify any edges, you can set this to nullptr, and set
 *										<code>numEdges</code> to 0.
 *	@field		numEdgeAttributeTypes	Number of edge attribute types in the following array.
 *	@field		edgeAttributeTypes		Pointer to an array of edge attribute types.  May be nullptr,
 *										provided that <code>numEdgeAttributeTypes</code> is 0.
 *	@field		numPoints				Number of points (vertices).
 *	@field		points					Pointer to an array of points.
 *	@field		numVertexAttributeTypes	Number of vertex attribute types in the following array.
 *	@field		vertexAttributeTypes	Pointer to an array of vertex attribute data.  May be nullptr,
 *										so long as <code>numVertexAttributeTypes</code> is 0.
 *	@field		bBox					Bounding box of the TriMesh.
 */
typedef struct TQ3TriMeshData {
    TQ3AttributeSet _Nullable                   triMeshAttributeSet;

    TQ3Uns32                                    numTriangles;
    TQ3TriMeshTriangleData                      * _Nonnull triangles;

    TQ3Uns32                                    numTriangleAttributeTypes;
    TQ3TriMeshAttributeData                     * _Nullable triangleAttributeTypes;

    TQ3Uns32                                    numEdges;
    TQ3TriMeshEdgeData                          * _Nullable edges;

    TQ3Uns32                                    numEdgeAttributeTypes;
    TQ3TriMeshAttributeData                     * _Nullable edgeAttributeTypes;

    TQ3Uns32                                    numPoints;
    TQ3Point3D                                  * _Nullable points;

    TQ3Uns32                                    numVertexAttributeTypes;
    TQ3TriMeshAttributeData                     * _Nullable vertexAttributeTypes;

    TQ3BoundingBox                              bBox;
} TQ3TriMeshData;





#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning( disable : 4200 )
#endif
/*!
	@struct		TQ3LayerShifts
	@abstract	Data of kQ3GeometryPropertyLayerShifts.
	@field		numPoints		Number of points (vertices) in the TriMesh.
	@field		coords			Array of coordinate data.  The number of values
								here must be numPoints, or in other words
								the data size is 4 * numPoints bytes.
*/
typedef struct TQ3LayerShifts
{
	TQ3Uns32	numPoints;
	TQ3Float32	coords[];
} TQ3LayerShifts;
#ifdef _MSC_VER
	#pragma warning( pop )
#endif




//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
/*!
	@functiongroup	Mesh Iterator Macros
*/
/*!
 *	@function		Q3ForEachMeshComponent
 *	@discussion
 *		Macro to aid in iterating over components of a Mesh object.  Example:
 *
 *		<blockquote><pre><code>
 *		TQ3MeshIterator		iter;
 *		TQ3MeshComponent	comp;
 *		Q3ForEachMeshComponent( myMesh, comp, &amp;iter )
 *		{
 *		&nbsp;	DoSomething( comp );
 *		}
 *		</code></pre></blockquote>
 *	@param		_m						The mesh object.
 *	@param		_c						The component.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachMeshComponent(_m, _c, _i)                  \
        for ((_c) = Q3Mesh_FirstMeshComponent((_m), (_i));  \
             (_c) != nullptr;                                  \
             (_c) = Q3Mesh_NextMeshComponent((_i)))

/*!
 *	@function		Q3ForEachComponentVertex
 *	@discussion
 *		Macro to aid in iterating over vertices of a component of a Mesh object.
 *	@param		_c						The component.
 *	@param		_v						The vertex.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachComponentVertex(_c, _v, _i)                \
        for ((_v) = Q3Mesh_FirstComponentVertex((_c), (_i));\
             (_v) != nullptr;                                  \
             (_v) = Q3Mesh_NextComponentVertex((_i)))

/*!
 *	@function		Q3ForEachComponentEdge
 *	@discussion
 *		Macro to aid in iterating over edges of a component of a Mesh object.
 *	@param		_c						The component.
 *	@param		_e						The edge.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachComponentEdge(_c, _e, _i)                  \
        for ((_e) = Q3Mesh_FirstComponentEdge((_c), (_i));  \
             (_e) != nullptr;                                  \
             (_e) = Q3Mesh_NextComponentEdge((_i)))

/*!
 *	@function		Q3ForEachMeshVertex
 *	@discussion
 *		Macro to aid in iterating over vertices of a Mesh object.
 *	@param		_m						The mesh.
 *	@param		_v						The vertex.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachMeshVertex(_m, _v, _i)                     \
        for ((_v) = Q3Mesh_FirstMeshVertex((_m), (_i));     \
             (_v) != nullptr;                                  \
             (_v) = Q3Mesh_NextMeshVertex((_i)))

/*!
 *	@function		Q3ForEachMeshFace
 *	@discussion
 *		Macro to aid in iterating over faces of a Mesh object.
 *	@param		_m						The mesh.
 *	@param		_f						The face.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachMeshFace(_m, _f, _i)                       \
        for ((_f) = Q3Mesh_FirstMeshFace((_m), (_i));       \
             (_f) != nullptr;                                  \
             (_f) = Q3Mesh_NextMeshFace((_i)))

/*!
 *	@function		Q3ForEachMeshEdge
 *	@discussion
 *		Macro to aid in iterating over edges of a Mesh object.
 *	@param		_m						The mesh.
 *	@param		_e						The edge.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachMeshEdge(_m, _e, _i)                       \
        for ((_e) = Q3Mesh_FirstMeshEdge((_m), (_i));       \
             (_e) != nullptr;                                  \
             (_e) = Q3Mesh_NextMeshEdge((_i)))

/*!
 *	@function		Q3ForEachVertexEdge
 *	@discussion
 *		Macro to aid in iterating over edges incident to a vertex of a Mesh object.
 *	@param		_v						The vertex.
 *	@param		_e						The edge.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachVertexEdge(_v, _e, _i)                     \
        for ((_e) = Q3Mesh_FirstVertexEdge((_v), (_i));     \
             (_e) != nullptr;                                  \
             (_e) = Q3Mesh_NextVertexEdge((_i)))


/*!
 *	@function		Q3ForEachVertexVertex
 *	@discussion
 *		Macro to aid in iterating over vertices adjacent to a vertex of a Mesh object.
 *	@param		_v						The vertex.
 *	@param		_n						An adjacent vertex.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachVertexVertex(_v, _n, _i)                   \
        for ((_n) = Q3Mesh_FirstVertexVertex((_v), (_i));   \
             (_n) != nullptr;                                  \
             (_n) = Q3Mesh_NextVertexVertex((_i)))

/*!
 *	@function		Q3ForEachVertexFace
 *	@discussion
 *		Macro to aid in iterating over faces incident to a vertex of a Mesh object.
 *	@param		_v						The vertex.
 *	@param		_f						An incident face.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachVertexFace(_v, _f, _i)                     \
        for ((_f) = Q3Mesh_FirstVertexFace((_v), (_i));     \
             (_f) != nullptr;                                  \
             (_f) = Q3Mesh_NextVertexFace((_i)))

/*!
 *	@function		Q3ForEachFaceEdge
 *	@discussion
 *		Macro to aid in iterating over edges incident to a face of a Mesh object.
 *	@param		_f						The face.
 *	@param		_e						An incident edge.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachFaceEdge(_f, _e, _i)                       \
        for ((_e) = Q3Mesh_FirstFaceEdge((_f), (_i));       \
             (_e) != nullptr;                                  \
             (_e) = Q3Mesh_NextFaceEdge((_i)))

/*!
 *	@function		Q3ForEachFaceVertex
 *	@discussion
 *		Macro to aid in iterating over vertices incident to a face of a Mesh object.
 *	@param		_f						The face.
 *	@param		_v						An incident vertex.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachFaceVertex(_f, _v, _i)                     \
        for ((_v) = Q3Mesh_FirstFaceVertex((_f), (_i));     \
             (_v) != nullptr;                                  \
             (_v) = Q3Mesh_NextFaceVertex((_i)))
    
/*!
 *	@function		Q3ForEachFaceFace
 *	@discussion
 *		Macro to aid in iterating over faces adjacent to a face of a Mesh object.
 *	@param		_f						The face.
 *	@param		_n						An incident face.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachFaceFace(_f, _n, _i)                       \
        for ((_n) = Q3Mesh_FirstFaceFace((_f), (_i));       \
             (_n) != nullptr;                                  \
             (_n) = Q3Mesh_NextFaceFace((_i)))
          
/*!
 *	@function		Q3ForEachFaceContour
 *	@discussion
 *		Macro to aid in iterating over contours of a face of a Mesh object.
 *	@param		_f						The face.
 *	@param		_h						A contour of the face.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachFaceContour(_f, _h, _i)                    \
        for ((_h) = Q3Mesh_FirstFaceContour((_f), (_i));    \
             (_h) != nullptr;                                  \
             (_h) = Q3Mesh_NextFaceContour((_i)))

/*!
 *	@function		Q3ForEachContourEdge
 *	@discussion
 *		Macro to aid in iterating over edges of a contour of a Mesh object.
 *	@param		_h						The contour.
 *	@param		_e						An edge of the contour.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachContourEdge(_h, _e, _i)                    \
        for ((_e) = Q3Mesh_FirstContourEdge((_h), (_i));    \
             (_e) != nullptr;                                  \
             (_e) = Q3Mesh_NextContourEdge((_i)))

/*!
 *	@function		Q3ForEachContourVertex
 *	@discussion
 *		Macro to aid in iterating over vertices of a contour of a Mesh object.
 *	@param		_h						The contour.
 *	@param		_v						A vertex of the contour.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachContourVertex(_h, _v, _i)                  \
        for ((_v) = Q3Mesh_FirstContourVertex((_h), (_i));  \
             (_v) != nullptr;                                  \
             (_v) = Q3Mesh_NextContourVertex((_i)))

/*!
 *	@function		Q3ForEachContourFace
 *	@discussion
 *		Macro to aid in iterating over edges of a contour of a Mesh object.
 *	@param		_h						The contour.
 *	@param		_v						An edge of the contour.
 *	@param		_i						Address of a <code>TQ3MeshIterator</code>.
 */
#define Q3ForEachContourFace(_h, _f, _i)                    \
        for ((_f) = Q3Mesh_FirstContourFace((_h), (_i));    \
             (_f) != nullptr;                                  \
             (_f) = Q3Mesh_NextContourFace((_i)))





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
	@functiongroup	Geometry Functions
*/

/*!
 *  @function
 *      Q3Geometry_GetType
 *  @discussion
 *      Returns the type of the GeometryObject passed in.
 *
 *      This function is used to determine the type of geometry object the parameter
 *      is a reference to. If 'geometry' is invalid, <code>kQ3ObjectTypeInvalid</code> will be
 *		returned.
 *
 *  @param geometry         A reference to a geometry object.
 *  @result                 A constant enumerating the type of 'geometry'.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Geometry_GetType (
    TQ3GeometryObject _Nonnull             geometry
);



/*!
 *  @function
 *      Q3Geometry_GetAttributeSet
 *  @discussion
 *      Returns the attribute set associated with a geometric object.
 *
 *      This function fills the <code>TQ3AttributeSet*</code> parameter out with the
 *		attribute set of the passed in <code>TQ3GeometryObject</code> reference.
 *
 *		The reference count of the attribute set is incremented.
 *
 *		To decrease the reference count when finished with the attribute set, use
 *		<code>Q3ObjectDispose()</code>.
 *
 *  @param geometry         A reference to a geometry object.
 *  @param attributeSet     Receives a reference to the attribute set of the geometry.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Geometry_GetAttributeSet (
    TQ3GeometryObject _Nonnull             geometry,
    TQ3AttributeSet _Nullable              * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3Geometry_SetAttributeSet
 *  @discussion
 *      Sets the attribute set associated with a geometric object.
 *
 *      Applies the attribute set given by the <code>TQ3AttributeSet</code> parameter onto
 *		the <code>TQ3GeometryObject</code> reference passed in.
 *
 *  @param geometry         A reference to a geometry object.
 *  @param attributeSet     The attribute set to apply to the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Geometry_SetAttributeSet (
    TQ3GeometryObject _Nonnull    geometry,
    TQ3AttributeSet _Nullable      attributeSet
);



/*!
 *  @function
 *      Q3Geometry_Submit
 *  @discussion
 *      Submits a geometry object for drawing, picking, bounding, or writing.
 *
 *      Submits the passed in <code>TQ3GeometryObject</code> to the supplied view for
 *      drawing, picking, bounding, or writing.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param geometry         A reference to a (retained) geometry object.
 *  @param view             The view to submit the geometric object to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Geometry_Submit (
    TQ3GeometryObject _Nonnull             geometry,
    TQ3ViewObject _Nonnull                 view
);



/*!
 *	@function
 *		Q3Geometry_GetDecomposed
 *	@discussion
 *		Creates the decomposed form of the geometry, if any.
 *
 *      The result may be a geometry or a group.  If the geometry cannot be decomposed
 *      further, nullptr is returned.
 *
 *      The decomposition of a given geometry is not documented and is subject to change.
 *      Geometries may be decomposed to multiple objects; for example, the decomposition
 *      of a cone with a bottom cap may be a group containing a TriMesh and a Disk.
 *
 *		You are free to modify the group or geometry returned by this function, and
 *		it is your responsibility to dispose it when you are done with it.
 *
 *		This function must be called in a submitting loop. Depending on the geometry,
 *      the result may be affected by the current view state. E.g., the geometry may
 *      be affected by the current transforms and camera, or even by styles such as
 *      subdivision or orientation.
 *
 *		<em>This function is not available in QD3D.</em>
 *
 *	@param	geometry		A geometry object.
 *	@param	view			A view object
 *	@result					A new reference to a group or geometry object, or nullptr.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C( TQ3Object _Nullable )
Q3Geometry_GetDecomposed (
	TQ3GeometryObject _Nonnull			geometry,
	TQ3ViewObject _Nonnull				view
);

#endif



/*!
	@functiongroup	Box Functions
*/



/*!
 *  @function
 *      Q3Box_New
 *  @discussion
 *      Constructs a new Box geometry object.
 *
 *		If you pass nullptr, you will get a default box with orientation (1, 0, 0), major axis
 *		(0, 1, 0), minor axis (0, 0, 1), and origin (0, 0, 0).  <em>This behavior was
 *		not present in QuickDraw 3D.</em>
 *
 *  @param boxData          Instance data passed in specifying the parameters of the new box object,
 *							or nullptr.
 *  @result                 A reference to the new geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3Box_New (
    const TQ3BoxData              * _Nonnull boxData
);



/*!
 *  @function
 *      Q3Box_Submit
 *  @discussion
 *		Submits a box for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param boxData			A pointer to a <code>TQ3BoxData</code> structure specifying the box
 *							to be drawn.
 *  @param view				The view to submit the box to.
 *  @result					Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_Submit (
    const TQ3BoxData              * _Nonnull boxData,
    TQ3ViewObject _Nonnull                 view
);



/*!
 *  @function
 *      Q3Box_SetData
 *  @discussion
 *      Sets the properties of an existing box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param boxData          A pointer to the box data to apply to the box object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_SetData (
    TQ3GeometryObject _Nonnull               box,
    const TQ3BoxData              * _Nonnull boxData
);



/*!
 *  @function
 *      Q3Box_GetData
 *  @discussion
 *      Gets the properties of an existing box object.
 *
 *		Memory is allocated for the 'boxData' parameter, and <code>Q3Box_EmptyData</code>
 *		must be called to dispose of this memory.
 *
 *  @param box              A reference to a box geometry object.
 *  @param boxData          Receives the box object's description.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_GetData (
    TQ3GeometryObject _Nonnull               box,
    TQ3BoxData                    * _Nonnull boxData
);



/*!
 *  @function
 *      Q3Box_EmptyData
 *  @discussion
 *      Releases the memory allocated by a prior call to <code>Q3Box_GetData</code>.
 *
 *  @param boxData          A pointer to the box data allocated by <code>Q3Box_GetData()</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_EmptyData (
    TQ3BoxData                    * _Nonnull boxData
);



/*!
 *  @function
 *      Q3Box_SetOrigin
 *  @discussion
 *      Sets the origin of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param origin           The point to which to set 'box's origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_SetOrigin (
    TQ3GeometryObject _Nonnull               box,
    const TQ3Point3D              * _Nonnull origin
);



/*!
 *  @function
 *      Q3Box_SetOrientation
 *  @discussion
 *      Sets the orientation of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param orientation      The vector to which to set 'box's orientation.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_SetOrientation (
    TQ3GeometryObject _Nonnull               box,
    const TQ3Vector3D             * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Box_SetMajorAxis
 *  @discussion
 *      Sets the major axis of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param majorAxis        The vector to which to set 'box's major axis.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_SetMajorAxis (
    TQ3GeometryObject _Nonnull               box,
    const TQ3Vector3D             * _Nonnull majorAxis
);



/*!
 *  @function
 *      Q3Box_SetMinorAxis
 *  @discussion
 *      Sets the minor axis of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param minorAxis        The vector to which to set 'box's minor axis.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_SetMinorAxis (
    TQ3GeometryObject _Nonnull               box,
    const TQ3Vector3D             * _Nonnull minorAxis
);



/*!
 *  @function
 *      Q3Box_GetOrigin
 *  @discussion
 *      Gets the origin of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param origin           Receives the origin of the box object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_GetOrigin (
    TQ3GeometryObject _Nonnull               box,
    TQ3Point3D                    * _Nonnull origin
);



/*!
 *  @function
 *      Q3Box_GetOrientation
 *  @discussion
 *      Gets the orientation of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param orientation      Receives the orientation vector of the box.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_GetOrientation (
    TQ3GeometryObject _Nonnull               box,
    TQ3Vector3D                   * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Box_GetMajorAxis
 *  @discussion
 *      Gets the major axis of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param majorAxis        Receives the major axis of the box.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_GetMajorAxis (
    TQ3GeometryObject _Nonnull               box,
    TQ3Vector3D                   * _Nonnull majorAxis
);



/*!
 *  @function
 *      Q3Box_GetMinorAxis
 *  @discussion
 *      Gets the minor axis of a box object.
 *
 *  @param box              A reference to a box geometry object.
 *  @param minorAxis        Receives the minor axis of the box.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_GetMinorAxis (
    TQ3GeometryObject _Nonnull               box,
    TQ3Vector3D                   * _Nonnull minorAxis
);



/*!
 *  @function
 *      Q3Box_GetFaceAttributeSet
 *  @discussion
 *      Gets the attribute set associated with a certain face of a box object.
 *
 *      The <code>TQ3AttributeSet*</code> parameter is filled with the attribute set
 *      of the face indexed with 'faceIndex' of the box geometry object.
 *		faceIndex must be between 0 and 5 (inclusive).
 *
 *		The reference count of the returned <code>TQ3AttributeSet</code> is incremented.
 *
 *  @param box              A reference to a box geometry object.
 *  @param faceIndex        The index of the face of the box.
 *  @param faceAttributeSet Receives a reference to the attribute set of the box face, or
 *							nullptr if the face does not have an attribute set.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_GetFaceAttributeSet (
    TQ3GeometryObject _Nonnull            box,
    TQ3Uns32                              faceIndex,
    TQ3AttributeSet _Nullable             * _Nonnull faceAttributeSet
);



/*!
 *  @function
 *      Q3Box_SetFaceAttributeSet
 *  @discussion
 *      Sets the attribute set associated with a certain face of a box object.
 *
 *      This function applies the <code>TQ3AttributeSet</code> parameter to the attribute set
 *      of the face indexed with <code>faceIndex</code> of the box geometry object.
 *		<code>faceIndex</code> must be between 0 and 5 (inclusive).
 *
 *  @param box              A reference to a box geometry object.
 *  @param faceIndex        The index of the face of the box.
 *  @param faceAttributeSet An attribute set to be applied.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Box_SetFaceAttributeSet (
    TQ3GeometryObject _Nonnull             box,
    TQ3Uns32                               faceIndex,
    TQ3AttributeSet _Nonnull               faceAttributeSet
);



/*!
	@functiongroup	Cone Functions
*/



/*!
 *  @function
 *      Q3Cone_New
 *  @discussion
 *      Create a cone geometry object.
 *
 *		If you pass nullptr, you will get a default cone with orientation (1, 0, 0), major axis
 *		(0, 1, 0), minor axis (0, 0, 1), and origin (0, 0, 0).  <em>This behavior was
 *		not present in QuickDraw 3D.</em>
 *
 *  @param coneData         Pointer to data describing the cone, or nullptr.
 *  @result                 The new cone object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3Cone_New (
    const TQ3ConeData             * _Nullable coneData
);



/*!
 *  @function
 *      Q3Cone_Submit
 *  @discussion
 *      Submits a cone in immediate mode for rendering, bounding, picking, or writing.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param coneData         Pointer to data describing a cone.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_Submit (
    const TQ3ConeData             * _Nonnull coneData,
    TQ3ViewObject _Nonnull                 view
);



/*!
 *  @function
 *      Q3Cone_SetData
 *  @discussion
 *      Modify a cone object by supplying new data.
 *
 *  @param cone             The cone object.
 *  @param coneData         Pointer to data describing a cone.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_SetData (
    TQ3GeometryObject _Nonnull             cone,
    const TQ3ConeData             * _Nonnull coneData
);



/*!
 *  @function
 *      Q3Cone_GetData
 *  @discussion
 *      Get data describing a cone object.
 *
 *		This may allocate memory, which should be freed using <code>Q3Cone_EmptyData</code>.
 *
 *  @param cone             The cone object.
 *  @param coneData         Receives data describing the cone.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_GetData (
    TQ3GeometryObject _Nonnull               cone,
    TQ3ConeData                   * _Nonnull coneData
);



/*!
 *  @function
 *      Q3Cone_SetOrigin
 *  @discussion
 *      Change the origin of a cone object.
 *
 *  @param cone             The cone object.
 *  @param origin           The new origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_SetOrigin (
    TQ3GeometryObject _Nonnull               cone,
    const TQ3Point3D              * _Nonnull origin
);



/*!
 *  @function
 *      Q3Cone_SetOrientation
 *  @discussion
 *      Change the orientation vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param orientation      The new orientation vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_SetOrientation (
    TQ3GeometryObject _Nonnull               cone,
    const TQ3Vector3D             * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Cone_SetMajorRadius
 *  @discussion
 *      Change the major radius vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param majorRadius      New major radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_SetMajorRadius (
    TQ3GeometryObject _Nonnull             cone,
    const TQ3Vector3D             * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Cone_SetMinorRadius
 *  @discussion
 *      Change the minor radius vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param minorRadius      New minor radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_SetMinorRadius (
    TQ3GeometryObject _Nonnull             cone,
    const TQ3Vector3D             * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Cone_GetOrigin
 *  @discussion
 *      Get the origin of a cone object.
 *
 *  @param cone             The cone object.
 *  @param origin           Receives the origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_GetOrigin (
    TQ3GeometryObject _Nonnull               cone,
    TQ3Point3D                    * _Nonnull origin
);



/*!
 *  @function
 *      Q3Cone_GetOrientation
 *  @discussion
 *      Get the orientation vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param orientation      Receives the orientation vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_GetOrientation (
    TQ3GeometryObject _Nonnull               cone,
    TQ3Vector3D                   * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Cone_GetMajorRadius
 *  @discussion
 *      Get the major radius vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param majorRadius      Receives the major radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_GetMajorRadius (
    TQ3GeometryObject _Nonnull             cone,
    TQ3Vector3D                   * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Cone_GetMinorRadius
 *  @discussion
 *      Get the minor radius vector of a cone object.
 *
 *  @param cone             The cone object.
 *  @param minorRadius      Receives the minor radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_GetMinorRadius (
    TQ3GeometryObject _Nonnull             cone,
    TQ3Vector3D                   * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Cone_SetCaps
 *  @discussion
 *      Set the end cap flags of a cone object.
 *
 *      Currently, this simply determines whether the cone will have a bottom cap.
 *
 *  @param cone             The cone object.
 *  @param caps             End cap value (do not include <code>kQ3EndCapMaskTop</code>).
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_SetCaps (
    TQ3GeometryObject _Nonnull             cone,
    TQ3EndCap                     caps
);



/*!
 *  @function
 *      Q3Cone_GetCaps
 *  @discussion
 *      Get the end cap flags of a cone object.
 *
 *  @param cone             The cone object.
 *  @param caps             Receives the end cap flags.  See <code>TQ3EndCapMasks</code>,
 *							defined in Quesa.h, for values.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_GetCaps (
    TQ3GeometryObject _Nonnull             cone,
    TQ3EndCap                     * _Nonnull caps
);



/*!
 *  @function
 *      Q3Cone_SetBottomAttributeSet
 *  @discussion
 *      Change the bottom attribute set of a cone object.
 *
 *      Naturally, this only makes sense if the cone has a bottom end cap.
 *
 *  @param cone             	The cone object.
 *  @param bottomAttributeSet	New bottom attribute set.  May be nullptr.
 *  @result                 	Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_SetBottomAttributeSet (
    TQ3GeometryObject _Nonnull             cone,
    TQ3AttributeSet _Nullable              bottomAttributeSet
);



/*!
 *  @function
 *      Q3Cone_GetBottomAttributeSet
 *  @discussion
 *      Get the attribute set for the bottom cap of a cone object.
 *
 *  @param cone             	The cone object.
 *  @param bottomAttributeSet	Receives a reference to the bottom attribute set, or nullptr.
 *  @result                 	Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_GetBottomAttributeSet (
    TQ3GeometryObject _Nonnull             cone,
    TQ3AttributeSet _Nullable * _Nonnull   bottomAttributeSet
);



/*!
 *  @function
 *      Q3Cone_SetFaceAttributeSet
 *  @discussion
 *      Set the attribute set for the face (as opposed to the bottom cap) of a cone.
 *
 *  @param cone             The cone object.
 *  @param faceAttributeSet New face attribute set.  May be nullptr.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_SetFaceAttributeSet (
    TQ3GeometryObject _Nonnull         	    cone,
    TQ3AttributeSet _Nullable               faceAttributeSet
);



/*!
 *  @function
 *      Q3Cone_GetFaceAttributeSet
 *  @discussion
 *      Get the attribute set for the face (as opposed to the bottom cap) of a cone.
 *
 *  @param cone             The cone object.
 *  @param faceAttributeSet Receives the attribute set for the face, or nullptr.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_GetFaceAttributeSet (
    TQ3GeometryObject _Nonnull            cone,
    TQ3AttributeSet _Nullable * _Nonnull  faceAttributeSet
);



/*!
 *  @function
 *      Q3Cone_SetInteriorAttributeSet
 *  @discussion
 *      Set the attribute set for the interior of a cone object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param cylinder         A cone object.
 *  @param faceAttributeSet New attribute set for the interior of the cone.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_SetInteriorAttributeSet (
    TQ3GeometryObject _Nonnull             cone,
    TQ3AttributeSet _Nullable              interiorAttributeSet
);

#endif	// QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Cone_GetInteriorAttributeSet
 *  @discussion
 *      Get the attribute set for the interior of a cone object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param cylinder         A cone object.
 *  @param faceAttributeSet Receives the attribute set for the interior, or nullptr.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_GetInteriorAttributeSet (
    TQ3GeometryObject _Nonnull             cone,
    TQ3AttributeSet _Nullable  * _Nonnull  interiorAttributeSet
);

#endif	// QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Cone_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Cone_GetData</code>.
 *
 *  @param coneData         Data describing a cone, previously obtained with
 *							<code>Q3Cone_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cone_EmptyData (
    TQ3ConeData                   * _Nonnull coneData
);



/*!
	@functiongroup	Cylinder Functions
*/



/*!
 *  @function
 *      Q3Cylinder_New
 *  @discussion
 *      Create a new Cylinder object.
 *
 *		If you pass nullptr, you will get a default cylinder with orientation (1, 0, 0), major axis
 *		(0, 1, 0), minor axis (0, 0, 1), origin (0, 0, 0), and no end caps.  <em>This behavior was
 *		not present in QuickDraw 3D.</em>
 *
 *  @param cylinderData     Data describing a cylinder, or nullptr.
 *  @result                 Reference to a new Cylinder geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3Cylinder_New (
    const TQ3CylinderData         * _Nonnull cylinderData
);



/*!
 *  @function
 *      Q3Cylinder_Submit
 *  @discussion
 *		Submits a cylinder for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param cylinderData     Data describing a cylinder.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_Submit (
    const TQ3CylinderData         * _Nonnull cylinderData,
    TQ3ViewObject _Nonnull                 view
);



/*!
 *  @function
 *      Q3Cylinder_SetData
 *  @discussion
 *      Modify a cylinder object by supplying all new data.
 *
 *  @param cylinder         A cylinder object.
 *  @param cylinderData     Data describing a cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetData (
    TQ3GeometryObject _Nonnull             cylinder,
    const TQ3CylinderData         * _Nonnull cylinderData
);



/*!
 *  @function
 *      Q3Cylinder_GetData
 *  @discussion
 *      Get data describing a cylinder.
 *
 *      This operation may allocate memory, which should be freed using
 *		<code>Q3Cylinder_EmptyData</code>.
 *
 *  @param cylinder         A cylinder object.
 *  @param cylinderData     Receives data describing the cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetData (
    TQ3GeometryObject _Nonnull             cylinder,
    TQ3CylinderData               * _Nonnull cylinderData
);



/*!
 *  @function
 *      Q3Cylinder_SetOrigin
 *  @discussion
 *      Change the origin of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param origin           New point of origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetOrigin (
    TQ3GeometryObject _Nonnull               cylinder,
    const TQ3Point3D              * _Nonnull origin
);



/*!
 *  @function
 *      Q3Cylinder_SetOrientation
 *  @discussion
 *      Change the orientation vector of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param orientation      New orientation vector for the cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetOrientation (
    TQ3GeometryObject _Nonnull               cylinder,
    const TQ3Vector3D             * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Cylinder_SetMajorRadius
 *  @discussion
 *      Change the major radius vector of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param majorRadius      New major radius vector for the cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetMajorRadius (
    TQ3GeometryObject _Nonnull               cylinder,
    const TQ3Vector3D             * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Cylinder_SetMinorRadius
 *  @discussion
 *      Change the minor radius vector of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param minorRadius      New minor radius vector for the cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetMinorRadius (
    TQ3GeometryObject _Nonnull               cylinder,
    const TQ3Vector3D             * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Cylinder_GetOrigin
 *  @discussion
 *      Get the origin of a cylinder.
 *
 *  @param cylinder         A cylinder object.
 *  @param origin           Receives the origin of the cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetOrigin (
    TQ3GeometryObject _Nonnull               cylinder,
    TQ3Point3D                    * _Nonnull origin
);



/*!
 *  @function
 *      Q3Cylinder_GetOrientation
 *  @discussion
 *      Get the orientation vector of a cylinder.
 *
 *  @param cylinder         A cylinder object.
 *  @param orientation      Receives the orientation vector of the cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetOrientation (
    TQ3GeometryObject _Nonnull               cylinder,
    TQ3Vector3D                   * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Cylinder_GetMajorRadius
 *  @discussion
 *      Get the major radius vector of a cylinder.
 *
 *  @param cylinder         A cylinder object.
 *  @param majorRadius      Receives the major radius vector of the cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetMajorRadius (
    TQ3GeometryObject _Nonnull              cylinder,
    TQ3Vector3D                   * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Cylinder_GetMinorRadius
 *  @discussion
 *      Get the minor radius vector of a cylinder.
 *
 *  @param cylinder         A cylinder object.
 *  @param minorRadius      Receives the minor radius vector of the cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetMinorRadius (
    TQ3GeometryObject _Nonnull               cylinder,
    TQ3Vector3D                   * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Cylinder_SetCaps
 *  @discussion
 *      Set the cap flags of a cylinder object.  This determines whether the
 *		cylinder is closed off at one end or the other.  The end containing the
 *		origin is considered the bottom.
 *
 *  @param cylinder         A cylinder object.
 *  @param caps             The end cap flags.  See <code>TQ3EndCapMasks</code>,
 *							defined in Quesa.h, for values.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetCaps (
    TQ3GeometryObject _Nonnull    cylinder,
    TQ3EndCap                     caps
);



/*!
 *  @function
 *      Q3Cylinder_GetCaps
 *  @discussion
 *      Get the cap flags of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param caps             Receives the end cap flags.  See <code>TQ3EndCapMasks</code>,
 *							defined in Quesa.h, for values.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetCaps (
    TQ3GeometryObject _Nonnull               cylinder,
    TQ3EndCap                     * _Nonnull caps
);



/*!
 *  @function
 *      Q3Cylinder_SetTopAttributeSet
 *  @discussion
 *      Set the attribute set for the top cap of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param topAttributeSet  New attribute set for the top cap.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetTopAttributeSet (
    TQ3GeometryObject _Nonnull             cylinder,
    TQ3AttributeSet _Nullable              topAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_GetTopAttributeSet
 *  @discussion
 *      Get the attribute set for the top cap of a cylinder object.
 *
 *  @param cylinder         A cylinder object.
 *  @param topAttributeSet  Receives the attribute set for the top cap, or nullptr.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetTopAttributeSet (
    TQ3GeometryObject _Nonnull             cylinder,
    TQ3AttributeSet _Nullable * _Nonnull topAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_SetBottomAttributeSet
 *  @discussion
 *      Set the attribute set for the bottom cap of a cylinder object.
 *
 *  @param cylinder         	A cylinder object.
 *  @param bottomAttributeSet	New attribute set for the bottom cap.
 *  @result                		Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetBottomAttributeSet (
    TQ3GeometryObject _Nonnull             cylinder,
    TQ3AttributeSet _Nullable              bottomAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_GetBottomAttributeSet
 *  @discussion
 *      Get the attribute set for the bottom cap of a cylinder object.
 *
 *  @param cylinder         	A cylinder object.
 *  @param bottomAttributeSet	Receives the attribute set for the bottom cap, or nullptr.
 *  @result						Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetBottomAttributeSet (
    TQ3GeometryObject _Nonnull             cylinder,
    TQ3AttributeSet _Nullable               * _Nonnull bottomAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_SetFaceAttributeSet
 *  @discussion
 *      Set the attribute set for the face of a cylinder object (i.e., neither end).
 *
 *  @param cylinder         A cylinder object.
 *  @param faceAttributeSet New attribute set for the face of the cylinder.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetFaceAttributeSet (
    TQ3GeometryObject _Nonnull              cylinder,
    TQ3AttributeSet _Nullable               faceAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_GetFaceAttributeSet
 *  @discussion
 *      Get the attribute set for the face of a cylinder object (i.e., neither end).
 *
 *  @param cylinder         A cylinder object.
 *  @param faceAttributeSet Receives the attribute set for the face, or nullptr.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetFaceAttributeSet (
    TQ3GeometryObject _Nonnull             cylinder,
    TQ3AttributeSet _Nullable              * _Nonnull faceAttributeSet
);



/*!
 *  @function
 *      Q3Cylinder_SetInteriorAttributeSet
 *  @discussion
 *      Set the attribute set for the interior of a cylinder object (which only shows
 *		when the u parameter does not have the full range 0 to 1).
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param cylinder         A cylinder object.
 *  @param faceAttributeSet New attribute set for the interior of the cylinder.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_SetInteriorAttributeSet (
    TQ3GeometryObject _Nonnull             cylinder,
    TQ3AttributeSet _Nullable              interiorAttributeSet
);

#endif	// QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Cylinder_GetInteriorAttributeSet
 *  @discussion
 *      Get the attribute set for the interior of a cylinder object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param cylinder         A cylinder object.
 *  @param faceAttributeSet Receives the attribute set for the interior, or nullptr.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_GetInteriorAttributeSet (
    TQ3GeometryObject _Nonnull             cylinder,
    TQ3AttributeSet _Nullable              * _Nonnull interiorAttributeSet
);

#endif	// QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Cylinder_EmptyData
 *  @discussion
 *      Free memory allocated by <code>Q3Cylinder_GetData</code>.
 *
 *  @param cylinderData     Cylinder data previously obtained using <code>Q3Cylinder_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Cylinder_EmptyData (
    TQ3CylinderData               * _Nonnull cylinderData
);



/*!
	@functiongroup	Disk Functions
*/



/*!
 *  @function
 *      Q3Disk_New
 *  @discussion
 *      Create a new disk object.
 *
 *		If you pass nullptr instead of a data pointer, you will get a default disk, with origin
 *		(0, 0, 0), major radius (1, 0, 0), and minor radius (0, 1, 0).  <em>This behavior was
 *		not present in QuickDraw 3D.</em>
 *
 *  @param diskData         Data describing a disk object, or NULL.
 *  @result                 Reference to a new Disk geometry object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3Disk_New (
    const TQ3DiskData             * _Nullable diskData
);



/*!
 *  @function
 *      Q3Disk_Submit
 *  @discussion
 *		Submits a disk for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param diskData         Data describing a disk object.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_Submit (
    const TQ3DiskData             * _Nonnull diskData,
    TQ3ViewObject _Nonnull        view
);



/*!
 *  @function
 *      Q3Disk_SetData
 *  @discussion
 *      Modify a disk object by supplying a whole new set of data.
 *
 *  @param disk             A disk geometry object.
 *  @param diskData         Data describing a disk object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_SetData (
    TQ3GeometryObject _Nonnull               disk,
    const TQ3DiskData             * _Nonnull diskData
);



/*!
 *  @function
 *      Q3Disk_GetData
 *  @discussion
 *      Get the data of a Disk object.
 *
 *		This may allocate memory, which should be freed using <code>Q3Disk_EmptyData</code>.
 *
 *  @param disk             A disk geometry object.
 *  @param diskData         Data describing a disk object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_GetData (
    TQ3GeometryObject _Nonnull             disk,
    TQ3DiskData                   * _Nonnull diskData
);



/*!
 *  @function
 *      Q3Disk_SetOrigin
 *  @discussion
 *      Change the origin of a Disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param origin           New origin for the disk.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_SetOrigin (
    TQ3GeometryObject _Nonnull               disk,
    const TQ3Point3D              * _Nonnull origin
);



/*!
 *  @function
 *      Q3Disk_SetMajorRadius
 *  @discussion
 *      Change the major radius vector of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param majorRadius      New major radius vector for the disk object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_SetMajorRadius (
    TQ3GeometryObject _Nonnull               disk,
    const TQ3Vector3D             * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Disk_SetMinorRadius
 *  @discussion
 *      Change the minor radius vector of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param minorRadius      New minor radius vector for the disk object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_SetMinorRadius (
    TQ3GeometryObject _Nonnull               disk,
    const TQ3Vector3D             * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Disk_GetOrigin
 *  @discussion
 *      Get the origin of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param origin           Receives the origin of the disk.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_GetOrigin (
    TQ3GeometryObject _Nonnull               disk,
    TQ3Point3D                    * _Nonnull origin
);



/*!
 *  @function
 *      Q3Disk_GetMajorRadius
 *  @discussion
 *      Get the major radius vector of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param majorRadius      Receives the major radius vector of the disk.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_GetMajorRadius (
    TQ3GeometryObject _Nonnull               disk,
    TQ3Vector3D                   * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Disk_GetMinorRadius
 *  @discussion
 *      Get the minor radius vector of a disk object.
 *
 *  @param disk             A disk geometry object.
 *  @param minorRadius      Receives the minor radius vector of the disk.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_GetMinorRadius (
    TQ3GeometryObject _Nonnull               disk,
    TQ3Vector3D                   * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Disk_EmptyData
 *  @discussion
 *      Free memory allocated by <code>Q3Disk_GetData</code>.
 *
 *  @param diskData         Data describing a disk object, previously obtained by
 *							<code>Q3Disk_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Disk_EmptyData (
    TQ3DiskData                   * _Nonnull diskData
);



/*!
	@functiongroup	Ellipse Functions
*/



/*!
 *  @function
 *      Q3Ellipse_New
 *  @discussion
 *      Create a new ellipse geometry object.
 *
 *		If you pass nullptr, you will get a default ellipse with major axis (0, 1, 0),
 *		minor axis (0, 0, 1), and origin (0, 0, 0).  <em>This behavior was
 *		not present in QuickDraw 3D.</em>
 *
 *  @param ellipseData      Data describing an ellipse, or NULL.
 *  @result                 Reference to a new Ellipse geometry object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3Ellipse_New (
    const TQ3EllipseData          * _Nullable ellipseData
);



/*!
 *  @function
 *      Q3Ellipse_Submit
 *  @discussion
 *		Submits an ellipse for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param ellipseData      Data describing an ellipse.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_Submit (
    const TQ3EllipseData          * _Nonnull ellipseData,
    TQ3ViewObject _Nonnull        view
);



/*!
 *  @function
 *      Q3Ellipse_SetData
 *  @discussion
 *      Modify an ellipse object by supplying a full new set of data.
 *
 *  @param ellipse          An ellipse object.
 *  @param ellipseData      Data describing an ellipse.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetData (
    TQ3GeometryObject _Nonnull               ellipse,
    const TQ3EllipseData          * _Nonnull ellipseData
);



/*!
 *  @function
 *      Q3Ellipse_GetData
 *  @discussion
 *      Get the data of an Ellipse object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Ellipse_EmptyData</code>.
 *
 *  @param ellipse          An ellipse object.
 *  @param ellipseData      Receives data describing the ellipse object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetData (
    TQ3GeometryObject _Nonnull               ellipse,
    TQ3EllipseData                * _Nonnull ellipseData
);



/*!
 *  @function
 *      Q3Ellipse_SetOrigin
 *  @discussion
 *      Change the origin of an ellipse object.
 *
 *  @param ellipse          The ellipse object.
 *  @param origin           The new origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetOrigin (
    TQ3GeometryObject _Nonnull             ellipse,
    const TQ3Point3D              * _Nonnull origin
);



/*!
 *  @function
 *      Q3Ellipse_SetMajorRadius
 *  @discussion
 *      Change the major radius vector of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param majorRadius      New major radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetMajorRadius (
    TQ3GeometryObject _Nonnull             ellipse,
    const TQ3Vector3D             * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Ellipse_SetMinorRadius
 *  @discussion
 *      Change the minor radius vector of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param minorRadius      New minor radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_SetMinorRadius (
    TQ3GeometryObject _Nonnull               ellipse,
    const TQ3Vector3D             * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Ellipse_GetOrigin
 *  @discussion
 *      Get the origin of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param origin           Receives the origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetOrigin (
    TQ3GeometryObject _Nonnull               ellipse,
    TQ3Point3D                    * _Nonnull origin
);



/*!
 *  @function
 *      Q3Ellipse_GetMajorRadius
 *  @discussion
 *      Get the major radius vector of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param majorRadius      Receives the major radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetMajorRadius (
    TQ3GeometryObject _Nonnull           ellipse,
    TQ3Vector3D                   * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Ellipse_GetMinorRadius
 *  @discussion
 *      Get the minor radius vector of an Ellipse object.
 *
 *  @param ellipse          The Ellipse object.
 *  @param minorRadius      Receives the minor radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_GetMinorRadius (
    TQ3GeometryObject _Nonnull              ellipse,
    TQ3Vector3D                   * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Ellipse_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Ellipse_GetData</code>.
 *
 *  @param ellipseData      Data describing an ellipse, previously obtained with
 *							<code>Q3Ellipse_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipse_EmptyData (
    TQ3EllipseData                * _Nonnull ellipseData
);



/*!
	@functiongroup	Ellipsoid Functions
*/



/*!
 *  @function
 *      Q3Ellipsoid_New
 *  @discussion
 *      Create a new ellipsoid geometry object.
 *
 *		If you pass nullptr, you will get a default ellipsoid with orientation (1, 0, 0), major axis
 *		(0, 1, 0), minor axis (0, 0, 1), and origin (0, 0, 0).  <em>This behavior was
 *		not present in QuickDraw 3D.</em>
 *
 *  @param ellipsoidData    Data describing an ellipsoid, or NULL.
 *  @result                 Reference to a new Ellipsoid geometry object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable )
Q3Ellipsoid_New (
    const TQ3EllipsoidData        * _Nullable ellipsoidData
);



/*!
 *  @function
 *      Q3Ellipsoid_Submit
 *  @discussion
 *		Submits an ellipsoid for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param ellipsoidData    Data describing an ellipsoid.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_Submit (
    const TQ3EllipsoidData        * _Nonnull ellipsoidData,
    TQ3ViewObject _Nonnull        view
);



/*!
 *  @function
 *      Q3Ellipsoid_SetData
 *  @discussion
 *      Modify an ellipsoid object by supplying a full new set of data.
 *
 *  @param ellipsoid        An ellipsoid object.
 *  @param ellipsoidData    Data describing an ellipsoid.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetData (
    TQ3GeometryObject _Nonnull             ellipsoid,
    const TQ3EllipsoidData        * _Nonnull ellipsoidData
);



/*!
 *  @function
 *      Q3Ellipsoid_GetData
 *  @discussion
 *      Get the data of an Ellipse object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Ellipsoid_EmptyData</code>.
 *
 *  @param ellipsoid        An ellipsoid object.
 *  @param ellipsoidData    Receives data describing the ellipsoid object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetData (
    TQ3GeometryObject _Nonnull             ellipsoid,
    TQ3EllipsoidData              * _Nonnull ellipsoidData
);



/*!
 *  @function
 *      Q3Ellipsoid_SetOrigin
 *  @discussion
 *      Change the origin of an ellipsoid object.
 *
 *  @param ellipsoid        The ellipsoid object.
 *  @param origin           The new origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetOrigin (
    TQ3GeometryObject _Nonnull               ellipsoid,
    const TQ3Point3D              * _Nonnull origin
);



/*!
 *  @function
 *      Q3Ellipsoid_SetOrientation
 *  @discussion
 *      Change the orientation vector of an Ellipsoid object.
 *
 *  @param ellipsoid        An Ellipsoid object.
 *  @param orientation      New orientation vector for the Ellipsoid.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetOrientation (
    TQ3GeometryObject _Nonnull            ellipsoid,
    const TQ3Vector3D             * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Ellipsoid_SetMajorRadius
 *  @discussion
 *      Change the major radius vector of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param majorRadius      New major radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetMajorRadius (
    TQ3GeometryObject _Nonnull             ellipsoid,
    const TQ3Vector3D             * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Ellipsoid_SetMinorRadius
 *  @discussion
 *      Change the minor radius vector of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param minorRadius      New minor radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_SetMinorRadius (
    TQ3GeometryObject _Nonnull            ellipsoid,
    const TQ3Vector3D             * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Ellipsoid_GetOrigin
 *  @discussion
 *      Get the origin of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param origin           Receives the origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetOrigin (
    TQ3GeometryObject _Nonnull             ellipsoid,
    TQ3Point3D                    * _Nonnull origin
);



/*!
 *  @function
 *      Q3Ellipsoid_GetOrientation
 *  @discussion
 *      Get the orientation vector of an Ellipsoid.
 *
 *  @param ellipsoid        An Ellipsoid object.
 *  @param orientation      Receives the orientation vector of the Ellipsoid.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetOrientation (
    TQ3GeometryObject  _Nonnull              ellipsoid,
    TQ3Vector3D                   * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Ellipsoid_GetMajorRadius
 *  @discussion
 *      Get the major radius vector of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param majorRadius      Receives the major radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetMajorRadius (
    TQ3GeometryObject _Nonnull               ellipsoid,
    TQ3Vector3D                   * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Ellipsoid_GetMinorRadius
 *  @discussion
 *      Get the minor radius vector of an Ellipsoid object.
 *
 *  @param ellipsoid        The Ellipsoid object.
 *  @param minorRadius      Receives the minor radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_GetMinorRadius (
    TQ3GeometryObject _Nonnull     ellipsoid,
    TQ3Vector3D                    * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Ellipsoid_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Ellipsoid_GetData</code>.
 *
 *  @param ellipsoidData    Data describing an Ellipsoid, previously obtained with
 *							<code>Q3Ellipsoid_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Ellipsoid_EmptyData (
    TQ3EllipsoidData              * _Nonnull ellipsoidData
);



/*!
	@functiongroup	General Polygon Functions
*/



/*!
 *  @function
 *      Q3GeneralPolygon_New
 *  @discussion
 *      Create a new general polygon geometry object.
 *
 *  @param generalPolygonData	Data describing a general polygon.
 *  @result						Reference to a new General Polygon geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3GeneralPolygon_New (
    const TQ3GeneralPolygonData   * _Nonnull generalPolygonData
);



/*!
 *  @function
 *      Q3GeneralPolygon_Submit
 *  @discussion
 *		Submits a general polygon for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param generalPolygonData	Data describing a general polygon.
 *  @param view					A view object.
 *  @result						Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_Submit (
    const TQ3GeneralPolygonData   * _Nonnull generalPolygonData,
    TQ3ViewObject _Nonnull        view
);



/*!
 *  @function
 *      Q3GeneralPolygon_SetData
 *  @discussion
 *      Modify a general polygon object by supplying a full new set of data.
 *
 *  @param generalPolygon   	A general polygon object.
 *  @param generalPolygonData	Data describing a general polygon object.
 *  @result						Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetData (
    TQ3GeometryObject _Nonnull             generalPolygon,
    const TQ3GeneralPolygonData   * _Nonnull generalPolygonData
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetData
 *  @discussion
 *      Get the data of a General Polygon object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3GeneralPolygon_EmptyData</code>.
 *
 *  @param polygon          	A general polygon object.
 *  @param generalPolygonData	Receives data describing the general polygon object.
 *  @result						Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetData (
    TQ3GeometryObject _Nonnull               polygon,
    TQ3GeneralPolygonData         * _Nonnull generalPolygonData
);



/*!
  @function
      Q3GeneralPolygon_EmptyData
  @discussion
        Release memory allocated by <code>Q3GeneralPolygon_GetData</code>, or that you
		allocated so long as you used Quesa memory allocation functions to allocate
		the arrays.

		To be precise, this function:
		<ul>
			<li>Disposes the attribute set of each vertex</li>
			<li>Frees the array of vertices of each contour.</li>
			<li>Frees the array of contours.</li>
			<li>Disposes the overall attribute set.</li>
		</ul>
 @param generalPolygonData	Data describing a General Polygon, perhaps previously obtained with
  							<code>Q3GeneralPolygon_GetData</code>.
 @result					Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_EmptyData (
    TQ3GeneralPolygonData         * _Nonnull generalPolygonData
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetNumContours
 *  @discussion
 *      Get the number of contours in a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @result                 Number of contours in the general polygon. Zero if invalid parameter.
 */
Q3_EXTERN_API_C ( TQ3Uns32  )
Q3GeneralPolygon_GetNumContours (
    TQ3GeometryObject _Nonnull             generalPolygon
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetNumVertices
 *  @discussion
 *      Get the number of vertices in a specific contour in a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @result                 Number of vertices in the specified contour in the general polygon. Zero if invalid parameters.
 */
Q3_EXTERN_API_C ( TQ3Uns32  )
Q3GeneralPolygon_GetNumVertices (
    TQ3GeometryObject _Nonnull             generalPolygon,
    TQ3Uns32                      contourIndex
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetVertexPosition
 *  @discussion
 *      Get the 3D position of a vertex in a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @param pointIndex       A 0-based index into the array of vertices of a contour.
 *  @param position         Receives the position of the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetVertexPosition (
    TQ3GeometryObject _Nonnull             generalPolygon,
    TQ3Uns32                      contourIndex,
    TQ3Uns32                      pointIndex,
    TQ3Point3D                    * _Nonnull position
);



/*!
 *  @function
 *      Q3GeneralPolygon_SetVertexPosition
 *  @discussion
 *      Set the 3D position of a vertex in a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @param pointIndex       A 0-based index into the array of vertices of a contour.
 *  @param position         New position for the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetVertexPosition (
    TQ3GeometryObject _Nonnull    generalPolygon,
    TQ3Uns32                      contourIndex,
    TQ3Uns32                      pointIndex,
    const TQ3Point3D              * _Nonnull position
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetVertexAttributeSet
 *  @discussion
 *      Get the attribute set of a vertex of a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @param pointIndex       A 0-based index into the array of vertices of a contour.
 *  @param attributeSet     Receives a new reference to the vertex attribute set, or nullptr.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetVertexAttributeSet (
    TQ3GeometryObject _Nonnull    generalPolygon,
    TQ3Uns32                      contourIndex,
    TQ3Uns32                      pointIndex,
    TQ3AttributeSet _Nullable     * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3GeneralPolygon_SetVertexAttributeSet
 *  @discussion
 *      Assign an attribute set to a vertex of a general polygon.
 *
 *      The function increments the reference count of the attribute set.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param contourIndex		A 0-based index into the array of contours.
 *  @param pointIndex       A 0-based index into the array of vertices of a contour.
 *  @param attributeSet     Attribute set for the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetVertexAttributeSet (
    TQ3GeometryObject _Nonnull    generalPolygon,
    TQ3Uns32                      contourIndex,
    TQ3Uns32                      pointIndex,
    TQ3AttributeSet _Nullable     attributeSet
);



/*!
 *  @function
 *      Q3GeneralPolygon_SetShapeHint
 *  @discussion
 *      Set the shape hint of a general polygon.
 *
 *      The hint may allow a renderer to optimize drawing the polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param shapeHint        The shape hint.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_SetShapeHint (
    TQ3GeometryObject _Nonnull    generalPolygon,
    TQ3GeneralPolygonShapeHint    shapeHint
);



/*!
 *  @function
 *      Q3GeneralPolygon_GetShapeHint
 *  @discussion
 *      Get the shape hint of a general polygon.
 *
 *  @param generalPolygon   A general polygon object.
 *  @param shapeHint        Receives the shape hint.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GeneralPolygon_GetShapeHint (
    TQ3GeometryObject _Nonnull    generalPolygon,
    TQ3GeneralPolygonShapeHint    * _Nonnull shapeHint
);



/*!
	@functiongroup	Line Functions
*/



/*!
 *  @function
 *      Q3Line_New
 *  @discussion
 *      Create a new line geometry object.
 *
 *  @param lineData         Data describing a line.
 *  @result                 Reference to a new Line geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3Line_New (
    const TQ3LineData             * _Nonnull lineData
);



/*!
 *  @function
 *      Q3Line_Submit
 *  @discussion
 *		Submits a line for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param lineData         Data describing a line.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Line_Submit (
    const TQ3LineData             * _Nonnull lineData,
    TQ3ViewObject _Nonnull        view
);



/*!
 *  @function
 *      Q3Line_GetData
 *  @discussion
 *      Get the data of a Line object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Line_EmptyData</code>.
 *
 *  @param line             A Line object.
 *  @param lineData         Receives data describing the Line object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Line_GetData (
    TQ3GeometryObject _Nonnull               line,
    TQ3LineData                   * _Nonnull lineData
);



/*!
 *  @function
 *      Q3Line_SetData
 *  @discussion
 *      Modify a line object by supplying a full new set of data.
 *
 *  @param line             A line object.
 *  @param lineData         Data describing a line.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Line_SetData (
    TQ3GeometryObject _Nonnull               line,
    const TQ3LineData             * _Nonnull lineData
);



/*!
 *  @function
 *      Q3Line_GetVertexPosition
 *  @discussion
 *      Get the position of one of the ends of a line.
 *
 *  @param line             A line object.
 *  @param index            An index, 0 or 1, specifying an end of the line.
 *  @param position         Receives the position of the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Line_GetVertexPosition (
    TQ3GeometryObject _Nonnull    line,
    TQ3Uns32                      index,
    TQ3Point3D                    * _Nonnull position
);



/*!
 *  @function
 *      Q3Line_SetVertexPosition
 *  @discussion
 *      Change the position of one of the ends of a line object.
 *
 *  @param line             A line object.
 *  @param index            An index, 0 or 1, specifying an end of the line.
 *  @param position         New position of the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Line_SetVertexPosition (
    TQ3GeometryObject _Nonnull    line,
    TQ3Uns32                      index,
    const TQ3Point3D              * _Nonnull position
);



/*!
 *  @function
 *      Q3Line_GetVertexAttributeSet
 *  @discussion
 *      Get the attribute set of one of the ends of a line object.
 *
 *  @param line             A line object.
 *  @param index            An index, 0 or 1, specifying an end of the line.
 *  @param attributeSet     Receives a new reference to the attribute set, or nullptr.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Line_GetVertexAttributeSet (
    TQ3GeometryObject _Nonnull    line,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable     * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3Line_SetVertexAttributeSet
 *  @discussion
 *      Assign an attribute set to one of the ends of a line.
 *
 *      Any previous attribute set on the vertex is disposed.
 *		The new attribute set has its reference count incremented.
 *
 *  @param line             A line object.
 *  @param index            An index, 0 or 1, specifying an end of the line.
 *  @param attributeSet     The new attribute set.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Line_SetVertexAttributeSet (
    TQ3GeometryObject _Nonnull    line,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable     attributeSet
);



/*!
 *  @function
 *      Q3Line_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Line_GetData</code>.
 *
 *  @param lineData         Data describing a Line, previously obtained with
 *							<code>Q3Line_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Line_EmptyData (
    TQ3LineData                   * _Nonnull lineData
);



/*!
	@functiongroup	Marker Functions
*/



/*!
 *  @function
 *      Q3Marker_New
 *  @discussion
 *      Create a new marker geometry object.
 *
 *  @param markerData       Data describing a marker.
 *  @result                 Reference to a new Marker geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable )
Q3Marker_New (
    const TQ3MarkerData           * _Nonnull markerData
);



/*!
 *  @function
 *      Q3Marker_Submit
 *  @discussion
 *		Submits a marker for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param markerData       Data describing a marker.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_Submit (
    const TQ3MarkerData           * _Nonnull markerData,
    TQ3ViewObject _Nonnull                 view
);



/*!
 *  @function
 *      Q3Marker_SetData
 *  @discussion
 *      Modify a marker object by supplying a full new set of data.
 *
 *  @param geometry         A marker object.
 *  @param markerData       Data describing a marker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_SetData (
    TQ3GeometryObject _Nonnull                geometry,
    const TQ3MarkerData            * _Nonnull markerData
);



/*!
 *  @function
 *      Q3Marker_GetData
 *  @discussion
 *      Get the data of a Marker object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Marker_EmptyData</code>.
 *
 *  @param geometry         A Marker object.
 *  @param markerData       Receives data describing the Marker object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_GetData (
    TQ3GeometryObject _Nonnull               geometry,
    TQ3MarkerData                 * _Nonnull markerData
);



/*!
 *  @function
 *      Q3Marker_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Marker_GetData</code>.
 *
 *  @param markerData       Data describing a Marker, previously obtained with
 *							<code>Q3Marker_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_EmptyData (
    TQ3MarkerData                 * _Nonnull markerData
);



/*!
 *  @function
 *      Q3Marker_GetPosition
 *  @discussion
 *      Get the position of a Marker object.
 *
 *      The position corresponds to the <code>location</code> field of the
 *		<code>TQ3MarkerData</code> structure.
 *
 *  @param marker			A marker object.
 *  @param location         Receives the location of the marker object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_GetPosition (
    TQ3GeometryObject _Nonnull               marker,
    TQ3Point3D                    * _Nonnull location
);



/*!
 *  @function
 *      Q3Marker_SetPosition
 *  @discussion
 *      Set the position of a Marker object.
 *
 *      The position corresponds to the <code>location</code> field of the
 *		<code>TQ3MarkerData</code> structure.
 *
 *  @param marker			A marker object.
 *  @param location         New location of the marker object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_SetPosition (
    TQ3GeometryObject _Nonnull               marker,
    const TQ3Point3D              * _Nonnull location
);



/*!
 *  @function
 *      Q3Marker_GetXOffset
 *  @discussion
 *      Get the horizontal offset, in screen pixels, from the marker location to the
 *		upper left corner of the marker bitmap.
 *
 *  @param marker			A marker object.
 *  @param xOffset          Receives the horizontal offset in pixels.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_GetXOffset (
    TQ3GeometryObject _Nonnull               marker,
    TQ3Int32                      * _Nonnull xOffset
);



/*!
 *  @function
 *      Q3Marker_SetXOffset
 *  @discussion
 *      Set the horizontal offset, in screen pixels, from the marker location to the
 *		upper left corner of the marker bitmap.
 *
 *  @param marker			A marker object.
 *  @param xOffset          New horizontal offset in pixels.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_SetXOffset (
    TQ3GeometryObject _Nonnull    marker,
    TQ3Int32                      xOffset
);



/*!
 *  @function
 *      Q3Marker_GetYOffset
 *  @discussion
 *      Get the vertical offset, in screen pixels, from the marker location to the
 *		upper left corner of the marker bitmap.
 *
 *  @param marker			A marker object.
 *  @param yOffset          Receives the vertical offset in pixels.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_GetYOffset (
    TQ3GeometryObject _Nonnull    marker,
    TQ3Int32                      * _Nonnull yOffset
);



/*!
 *  @function
 *      Q3Marker_SetYOffset
 *  @discussion
 *      Set the vertical offset, in screen pixels, from the marker location to the
 *		upper left corner of the marker bitmap.
 *
 *  @param marker			A marker object.
 *  @param yOffset          New vertical offset in pixels.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_SetYOffset (
    TQ3GeometryObject _Nonnull    marker,
    TQ3Int32                      yOffset
);



/*!
 *  @function
 *      Q3Marker_GetBitmap
 *  @discussion
 *      Get the bitmap of a Marker object.
 *
 *      This returns a copy of the bitmap of the marker.  When you are done
 *      with it, use <code>Q3Bitmap_Empty</code> to release the memory.
 *
 *  @param marker			A marker object.
 *  @param bitmap           Receives a copy of the bitmap of the marker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_GetBitmap (
    TQ3GeometryObject _Nonnull             marker,
    TQ3Bitmap                     * _Nonnull bitmap
);



/*!
 *  @function
 *      Q3Marker_SetBitmap
 *  @discussion
 *      Change the bitmap of a Marker object.
 *
 *      This makes a copy of the bitmap data, so you can dispose of the
 *      original.
 *
 *  @param marker			A marker object.
 *  @param bitmap           New bitmap to be used by the marker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Marker_SetBitmap (
    TQ3GeometryObject _Nonnull            marker,
    const TQ3Bitmap               * _Nonnull bitmap
);



/*!
	@functiongroup	Mesh Functions
*/



/*!
 *  @function
 *      Q3Mesh_New
 *  @discussion
 *      Create a new empty Mesh geometry object.
 *
 *  @result                 Reference to a new Mesh geometry object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3Mesh_New (
    void
);



/*!
 *  @function
 *      Q3Mesh_SetData
 *  @discussion
 *      Sets the properties of an existing mesh object.
 *
 *      If successful, this function invalidates all preexisting references to mesh parts
 *		(<code>TQ3MeshVertex</code>, <code>TQ3MeshContour</code>, <code>TQ3MeshFace</code>, 
 *		<code>TQ3MeshCorner</code>, <code>TQ3MeshEdge</code>, <code>TQ3MeshComponent</code>)
 *		and mesh iterators (<code>TQ3MeshIterator</code>).
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param mesh             A reference to a mesh geometry object.
 *  @param meshData         A pointer to the mesh data to apply to the mesh object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetData (
    TQ3GeometryObject _Nonnull               mesh,
    const TQ3MeshData             * _Nonnull meshData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Mesh_GetData
 *  @discussion
 *      Gets the properties of an existing mesh object.
 *
 *		Memory is allocated for the 'meshData' parameter, and <code>Q3Mesh_EmptyData</code>
 *		must be called to dispose of this memory.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param mesh             A reference to a mesh geometry object.
 *  @param meshData         Receives the mesh object's description.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetData (
    TQ3GeometryObject _Nonnull               mesh,
    TQ3MeshData                   * _Nonnull meshData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Mesh_EmptyData
 *  @discussion
 *      Releases the memory allocated by a prior call to <code>Q3Mesh_GetData</code>.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param meshData         A pointer to the mesh data allocated by <code>Q3Mesh_GetData()</code>.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_EmptyData (
    TQ3MeshData                   * _Nonnull meshData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Mesh_VertexNew
 *  @discussion
 *      Create a new mesh vertex.
 *
 *  @param mesh             The mesh to create the vertex within.
 *  @param vertex           The data for the vertex.
 *  @result                 The new mesh vertex.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_VertexNew (
    TQ3GeometryObject _Nonnull               mesh,
    const TQ3Vertex3D             * _Nonnull vertex
);



/*!
 *  @function
 *      Q3Mesh_FaceNew
 *  @discussion
 *      Create a new mesh face.
 *
 *  @param mesh             The mesh to create the face within.
 *  @param numVertices      The number of vertices in the face.
 *  @param vertices         The vertices for the face.
 *  @param attributeSet     The attribute set for the face.
 *  @result                 The new mesh face.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nonnull  )
Q3Mesh_FaceNew (
    TQ3GeometryObject _Nonnull    mesh,
    TQ3Uns32                      numVertices,
    const TQ3MeshVertex _Nonnull  * _Nonnull vertices,
    TQ3AttributeSet _Nullable     attributeSet
);



/*!
 *  @function
 *      Q3Mesh_VertexDelete
 *  @discussion
 *      Delete a mesh vertex.
 *
 *  @param mesh             The mesh to remove the vertex from.
 *  @param vertex           The mesh vertex to delete.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_VertexDelete (
    TQ3GeometryObject _Nonnull    mesh,
    TQ3MeshVertex _Nonnull        vertex
);



/*!
 *  @function
 *      Q3Mesh_FaceDelete
 *  @discussion
 *      Delete a mesh face.
 *
 *  @param mesh             The mesh to remove the face from.
 *  @param face             The mesh face to delete.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_FaceDelete (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshFace _Nonnull                  face
);



/*!
 *  @function
 *      Q3Mesh_DelayUpdates
 *  @discussion
 *      Delay updates to a mesh.
 *
 *      Can be used before a mesh is edited to allow Quesa to defer any
 *      geometrical optimisations on the mesh. While updates are deferred,
 *      Quesa will select a more flexible internal mesh representation.
 *
 *  @param mesh             The mesh to delay updates on.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_DelayUpdates (
    TQ3GeometryObject _Nonnull            mesh
);



/*!
 *  @function
 *      Q3Mesh_ResumeUpdates
 *  @discussion
 *      Resume updates to a mesh.
 *
 *      Should be used after a preceding call to Q3Mesh_DelayUpdates to
 *      indicate to Quesa that an optimised internal representation of
 *      the mesh would be preferred to a more flexible form.
 *
 *  @param mesh             The mesh to resume updatse for.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_ResumeUpdates (
    TQ3GeometryObject _Nonnull            mesh
);



/*!
 *  @function
 *      Q3Mesh_FaceToContour
 *  @discussion
 *      Convert a mesh face to a contour.
 *
 *  @param mesh             The mesh to update.
 *  @param containerFace    The face containing the face which will be converted.
 *  @param face             The face to convert to a contour.
 *  @result                 The new mesh contour.
 */
Q3_EXTERN_API_C ( TQ3MeshContour _Nonnull  )
Q3Mesh_FaceToContour (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshFace _Nonnull                  containerFace,
    TQ3MeshFace _Nonnull                  face
);



/*!
 *  @function
 *      Q3Mesh_ContourToFace
 *  @discussion
 *      Convert a mesh face to a contour.
 *
 *  @param mesh             The mesh to update.
 *  @param contour          The contour to convert to a face.
 *  @result                 The new mesh face.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nullable  )
Q3Mesh_ContourToFace (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshContour _Nonnull               contour
);



/*!
 *  @function
 *      Q3Mesh_GetNumComponents
 *  @discussion
 *      Get the number of components of a mesh.
 *
 *  @param mesh             The mesh to query.
 *  @param numComponents    Receives the number of components of the mesh.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumComponents (
    TQ3GeometryObject _Nonnull               mesh,
    TQ3Uns32                      * _Nonnull numComponents
);



/*!
 *  @function
 *      Q3Mesh_GetNumEdges
 *  @discussion
 *      Get the number of edges in a mesh
 *
 *  @param mesh             The mesh to query.
 *  @param numEdges         Receives the number of edges in the mesh.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumEdges (
    TQ3GeometryObject _Nonnull               mesh,
    TQ3Uns32                      * _Nonnull numEdges
);



/*!
 *  @function
 *      Q3Mesh_GetNumVertices
 *  @discussion
 *      Get the number of vertices in a mesh
 *
 *  @param mesh             The mesh to query.
 *  @param numVertices      Receives the number of vertices in the mesh.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumVertices (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3Uns32                      * _Nonnull numVertices
);



/*!
 *  @function
 *      Q3Mesh_GetNumFaces
 *  @discussion
 *      Get the number of faces in a mesh
 *
 *  @param mesh             The mesh to query.
 *  @param numFaces         Receives the number of faces in the mesh.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumFaces (
    TQ3GeometryObject _Nonnull               mesh,
    TQ3Uns32                      * _Nonnull numFaces
);



/*!
 *  @function
 *      Q3Mesh_GetNumCorners
 *  @discussion
 *      Get the number of corners in a mesh
 *
 *  @param mesh             The mesh to query.
 *  @param numCorners       Receives the number of corners in the mesh.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetNumCorners (
    TQ3GeometryObject _Nonnull               mesh,
    TQ3Uns32                      * _Nonnull numCorners
);



/*!
 *  @function
 *      Q3Mesh_GetOrientable
 *  @discussion
 *      Get the mesh orientable state
 *
 *  @param mesh             The mesh to query.
 *  @param orientable       Receives the mesh orientable state.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetOrientable (
    TQ3GeometryObject _Nonnull               mesh,
    TQ3Boolean                    * _Nonnull orientable
);



/*!
 *  @function
 *      Q3Mesh_GetComponentNumVertices
 *  @discussion
 *      Get the number of vertices in a mesh component
 *
 *  @param mesh             The mesh to query.
 *  @param component        The component to query.
 *  @param numVertices      Receives the number of vertices in the component.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentNumVertices (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshComponent _Nonnull             component,
    TQ3Uns32                      * _Nonnull numVertices
);



/*!
 *  @function
 *      Q3Mesh_GetComponentNumEdges
 *  @discussion
 *      Get the number of edges in a mesh component
 *
 *  @param mesh             The mesh to query.
 *  @param component        The component to query.
 *  @param numEdges         Receives the number of edges in the component.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentNumEdges (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshComponent _Nonnull             component,
    TQ3Uns32                      * _Nonnull numEdges
);



/*!
 *  @function
 *      Q3Mesh_GetComponentBoundingBox
 *  @discussion
 *      Get the bounding box of a mesh component
 *
 *  @param mesh             The mesh to query.
 *  @param component        The component to query.
 *  @param boundingBox      Receives the bounding box of the component.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentBoundingBox (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshComponent _Nonnull              component,
    TQ3BoundingBox                * _Nonnull boundingBox
);



/*!
 *  @function
 *      Q3Mesh_GetComponentOrientable
 *  @discussion
 *      Get the orientable state of a mesh component
 *
 *  @param mesh             The mesh to query.
 *  @param component        The component to query.
 *  @param orientable       Receives the orientable state of the component.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetComponentOrientable (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshComponent _Nonnull              component,
    TQ3Boolean                    * _Nonnull orientable
);



/*!
 *  @function
 *      Q3Mesh_GetVertexCoordinates
 *  @discussion
 *      Get the coordinates of a mesh vertex.
 *
 *  @param mesh             The mesh to query.
 *  @param vertex           The vertex to query.
 *  @param coordinates      Receives the coordinates of the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexCoordinates (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshVertex _Nonnull                vertex,
    TQ3Point3D                    * _Nonnull coordinates
);



/*!
 *  @function
 *      Q3Mesh_GetVertexIndex
 *  @discussion
 *      Get the index of a mesh vertex.
 *
 *  @param mesh             The mesh to query.
 *  @param vertex           The vertex to query.
 *  @param index            Receives the index of the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexIndex (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshVertex _Nonnull                vertex,
    TQ3Uns32                      * _Nonnull index
);



/*!
 *  @function
 *      Q3Mesh_GetVertexOnBoundary
 *  @discussion
 *      Get the boundary state for a mesh vertex
 *
 *  @param mesh             The mesh to query.
 *  @param vertex           The vertex to query.
 *  @param onBoundary       Receives the vertex boundary state.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexOnBoundary (
    TQ3GeometryObject _Nonnull               mesh,
    TQ3MeshVertex _Nonnull                   vertex,
    TQ3Boolean                    * _Nonnull onBoundary
);



/*!
 *  @function
 *      Q3Mesh_GetVertexComponent
 *  @discussion
 *      Get the component of a mesh vertex.
 *
 *  @param mesh             The mesh to query.
 *  @param vertex           The vertex to query.
 *  @param component        Receives the component of the mesh vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexComponent (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshVertex _Nonnull                 vertex,
    TQ3MeshComponent _Nonnull              * _Nonnull component
);



/*!
 *  @function
 *      Q3Mesh_GetVertexAttributeSet
 *  @discussion
 *      Get the attribute set of a mesh vertex.
 *
 *  @param mesh             The mesh to query.
 *  @param vertex           The vertex to query.
 *  @param attributeSet     Receives the attribute set of the mesh vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshVertex _Nonnull                vertex,
    TQ3AttributeSet _Nullable              * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3Mesh_SetVertexCoordinates
 *  @discussion
 *      Set the coordinates of a mesh vertex.
 *
 *  @param mesh             The mesh to update.
 *  @param vertex           The vertex to update.
 *  @param coordinates      The new coordinates for the mesh vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetVertexCoordinates (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshVertex _Nonnull                vertex,
    const TQ3Point3D              * _Nonnull coordinates
);



/*!
 *  @function
 *      Q3Mesh_SetVertexAttributeSet
 *  @discussion
 *      Set the attribute set of a mesh vertex
 *
 *  @param mesh             The mesh to udate.
 *  @param vertex           The vertex to update.
 *  @param attributeSet     The new attribute set for the mesh vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshVertex _Nonnull                vertex,
    TQ3AttributeSet _Nullable             attributeSet
);



/*!
 *  @function
 *      Q3Mesh_GetFaceNumVertices
 *  @discussion
 *      Get the number of vertices in a mesh face.
 *
 *  @param mesh             The mesh to query.
 *  @param face             The face to query.
 *  @param numVertices      Receives the number of vertices in the mesh face.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceNumVertices (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshFace _Nonnull                  face,
    TQ3Uns32                      * _Nonnull numVertices
);



/*!
 *  @function
 *      Q3Mesh_GetFacePlaneEquation
 *  @discussion
 *      Get the plane equation of a mesh face.
 *
 *  @param mesh             The mesh to query.
 *  @param face             The face to query.
 *  @param planeEquation    Receives the plane equation of the mesh face.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFacePlaneEquation (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshFace  _Nonnull                 face,
    TQ3PlaneEquation              * _Nonnull planeEquation
);



/*!
 *  @function
 *      Q3Mesh_GetFaceNumContours
 *  @discussion
 *      Get the number of contours of a mesh face.
 *
 *  @param mesh             The mesh to query.
 *  @param face             The face to query.
 *  @param numContours      Receives the number of contours of the mesh face.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceNumContours (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshFace _Nonnull                   face,
    TQ3Uns32                      * _Nonnull numContours
);



/*!
 *  @function
 *      Q3Mesh_GetFaceIndex
 *  @discussion
 *      Get the index of a mesh face.
 *
 *  @param mesh             The mesh to query.
 *  @param face             The face to query.
 *  @param index            Receives the index of the mesh face.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceIndex (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshFace   _Nonnull                face,
    TQ3Uns32                      * _Nonnull index
);



/*!
 *  @function
 *      Q3Mesh_GetFaceComponent
 *  @discussion
 *      Get the component of a mesh face.
 *
 *  @param mesh             The mesh to query.
 *  @param face             The face to query.
 *  @param component        Receives the component of the mesh face.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceComponent (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshFace _Nonnull                  face,
    TQ3MeshComponent _Nonnull             * _Nonnull component
);



/*!
 *  @function
 *      Q3Mesh_GetFaceAttributeSet
 *  @discussion
 *      Get the attribute set of a mesh face.
 *
 *  @param mesh             The mesh to query.
 *  @param face             The face to query.
 *  @param attributeSet     Receives the attribute set of the mesh face.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetFaceAttributeSet (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshFace _Nonnull                   face,
    TQ3AttributeSet _Nullable              * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3Mesh_SetFaceAttributeSet
 *  @discussion
 *      Set the attribute set of a mesh face.
 *
 *  @param mesh             The mesh to update.
 *  @param face             The face to update.
 *  @param attributeSet     The new attribute set for the mesh face.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetFaceAttributeSet (
    TQ3GeometryObject _Nonnull            mesh,
    TQ3MeshFace _Nonnull                  face,
    TQ3AttributeSet _Nullable             attributeSet
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeVertices
 *  @discussion
 *      Get the vertices of a mesh edge.
 *
 *  @param mesh             The mesh to query.
 *  @param edge             The edge to query.
 *  @param vertex1          Receives the first vertex of the mesh edge.
 *  @param vertex2          Receives the second vertex of the mesh edge.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeVertices (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshEdge _Nonnull                  edge,
    TQ3MeshVertex _Nonnull                * _Nonnull vertex1,
    TQ3MeshVertex _Nonnull                * _Nonnull vertex2
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeFaces
 *  @discussion
 *      Get the faces of a mesh edge.
 *
 *  @param mesh             The mesh to query.
 *  @param edge             The edge to query.
 *  @param face1            Receives the first face of the mesh edge.
 *  @param face2            Receives the second face of the mesh edge.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeFaces (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshEdge _Nonnull                   edge,
    TQ3MeshFace _Nonnull                   * _Nonnull face1,
    TQ3MeshFace _Nonnull                   * _Nonnull face2
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeOnBoundary
 *  @discussion
 *      Get the boundary state of a mesh edge.
 *
 *  @param mesh             The mesh to query.
 *  @param edge             The edge to query.
 *  @param onBoundary       Receives the boundary state of the mesh edge.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeOnBoundary (
    TQ3GeometryObject _Nonnull               mesh,
    TQ3MeshEdge _Nonnull                     edge,
    TQ3Boolean                    * _Nonnull onBoundary
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeComponent
 *  @discussion
 *      Get the component of a mesh edge.
 *
 *  @param mesh             The mesh to query.
 *  @param edge             The edge to query.
 *  @param component        Receives the component of the mesh edge.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeComponent (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshEdge _Nonnull                   edge,
    TQ3MeshComponent _Nonnull              * _Nonnull component
);



/*!
 *  @function
 *      Q3Mesh_GetEdgeAttributeSet
 *  @discussion
 *      Get the attribute set of a mesh edge.
 *
 *  @param mesh             The mesh to query.
 *  @param edge             The edge to query.
 *  @param attributeSet     Receives the attribute set of the mesh edge.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetEdgeAttributeSet (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshEdge _Nonnull                   edge,
    TQ3AttributeSet _Nullable              * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3Mesh_SetEdgeAttributeSet
 *  @discussion
 *      Set the attribute set for a mesh edge.
 *
 *  @param mesh             The mesh to update.
 *  @param edge             The edge to update.
 *  @param attributeSet     The new attribute set for the mesh edge.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetEdgeAttributeSet (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshEdge _Nonnull                   edge,
    TQ3AttributeSet _Nullable              attributeSet
);



/*!
 *  @function
 *      Q3Mesh_GetContourFace
 *  @discussion
 *      Get the face of a mesh contour.
 *
 *  @param mesh             The mesh to query.
 *  @param contour          The contour to query.
 *  @param face             Receives the face of the mesh contour.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetContourFace (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshContour _Nonnull                contour,
    TQ3MeshFace _Nonnull                   * _Nonnull face
);



/*!
 *  @function
 *      Q3Mesh_GetContourNumVertices
 *  @discussion
 *      Get the number of vertices in a mesh contour.
 *
 *  @param mesh             The mesh to query.
 *  @param contour          The contour to query.
 *  @param numVertices      Receives the number of vertices in the mesh contour.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetContourNumVertices (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshContour _Nonnull                contour,
    TQ3Uns32                      * _Nonnull numVertices
);



/*!
 *  @function
 *      Q3Mesh_GetCornerAttributeSet
 *  @discussion
 *      Get the attribute set of a mesh corner.
 *
 *  @param mesh             The mesh to query.
 *  @param vertex           The vertex to query.
 *  @param face             The face to query.
 *  @param attributeSet     Receives the attribute set of the mesh corner.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_GetCornerAttributeSet (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshVertex _Nonnull                 vertex,
    TQ3MeshFace _Nonnull                   face,
    TQ3AttributeSet _Nullable            * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3Mesh_SetCornerAttributeSet
 *  @discussion
 *      Set the attribute set for a mesh corner.
 *
 *  @param mesh             The mesh to update.
 *  @param vertex           The vertex to update.
 *  @param face             The face to update.
 *  @param attributeSet     The new attribute set for the mesh corner.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Mesh_SetCornerAttributeSet (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshVertex _Nonnull                 vertex,
    TQ3MeshFace _Nonnull                   face,
    TQ3AttributeSet _Nullable              attributeSet
);



/*!
 *  @function
 *      Q3Mesh_FirstMeshComponent
 *  @discussion
 *      Get the first component in a mesh.
 *
 *  @param mesh             The mesh to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first component in the mesh.
 */
Q3_EXTERN_API_C ( TQ3MeshComponent _Nonnull  )
Q3Mesh_FirstMeshComponent (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextMeshComponent
 *  @discussion
 *      Get the next component in a mesh.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next component in the mesh.
 */
Q3_EXTERN_API_C ( TQ3MeshComponent _Nonnull  )
Q3Mesh_NextMeshComponent (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstComponentVertex
 *  @discussion
 *      Get the first vertex in a mesh component.
 *
 *  @param component        The component to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first vertex in the component.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_FirstComponentVertex (
    TQ3MeshComponent _Nonnull              component,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextComponentVertex
 *  @discussion
 *      Get the next vertex in a mesh component.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next vertex in the component.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_NextComponentVertex (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstComponentEdge
 *  @discussion
 *      Get the first edge in a mesh component.
 *
 *  @param component        The component to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first edge in the component.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_FirstComponentEdge (
    TQ3MeshComponent _Nonnull              component,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextComponentEdge
 *  @discussion
 *      Get the next edge in a mesh component.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next edge in the component.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_NextComponentEdge (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstMeshVertex
 *  @discussion
 *      Get the first vertex in a mesh.
 *
 *  @param mesh             The mesh to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first vertex in the mesh.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_FirstMeshVertex (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextMeshVertex
 *  @discussion
 *      Get the next vertex in a mesh.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next vertex in the mesh.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_NextMeshVertex (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstMeshFace
 *  @discussion
 *      Get the first face in a mesh.
 *
 *  @param mesh             The mesh to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first face in the mesh.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nonnull  )
Q3Mesh_FirstMeshFace (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextMeshFace
 *  @discussion
 *      Get the next face in a mesh.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next face in the mesh.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nonnull  )
Q3Mesh_NextMeshFace (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstMeshEdge
 *  @discussion
 *      Get the first edge in a mesh.
 *
 *  @param mesh             The mesh to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first edge in the mesh.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_FirstMeshEdge (
    TQ3GeometryObject _Nonnull             mesh,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextMeshEdge
 *  @discussion
 *      Get the next edge in a mesh.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next edge in the mesh.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_NextMeshEdge (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstVertexEdge
 *  @discussion
 *      Get the first edge in a mesh vertex.
 *
 *  @param vertex           The vertex to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first edge in the vertex.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_FirstVertexEdge (
    TQ3MeshVertex _Nonnull                 vertex,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextVertexEdge
 *  @discussion
 *      Get the next edge in a mesh vertex.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next edge in the vertex.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_NextVertexEdge (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstVertexVertex
 *  @discussion
 *      Get the first vertex in a mesh vertex.
 *
 *  @param vertex           The vertex to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first vertex in the vertex.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_FirstVertexVertex (
    TQ3MeshVertex _Nonnull                 vertex,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextVertexVertex
 *  @discussion
 *      Get the next vertex in a mesh vertex.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next vertex in the vertex.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_NextVertexVertex (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstVertexFace
 *  @discussion
 *      Get the first face in a mesh vertex.
 *
 *  @param vertex           The vertex to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first face in the vertex.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nonnull  )
Q3Mesh_FirstVertexFace (
    TQ3MeshVertex _Nonnull                vertex,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextVertexFace
 *  @discussion
 *      Get the next face in a mesh vertex.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next face in the vertex.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nonnull  )
Q3Mesh_NextVertexFace (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstFaceEdge
 *  @discussion
 *      Get the first edge in a mesh face.
 *
 *  @param face             The face to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first edge in the face.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_FirstFaceEdge (
    TQ3MeshFace _Nonnull                   face,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextFaceEdge
 *  @discussion
 *      Get the next edge in a mesh face.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next edge in the face.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_NextFaceEdge (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstFaceVertex
 *  @discussion
 *      Get the first vertex in a mesh face.
 *
 *  @param face             The face to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first vertex in the face.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_FirstFaceVertex (
    TQ3MeshFace _Nonnull                  face,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextFaceVertex
 *  @discussion
 *      Get the next vertex in a mesh face.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next vertex in the face.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_NextFaceVertex (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstFaceFace
 *  @discussion
 *      Get the first face in a mesh face.
 *
 *  @param face             The face to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first face in the face.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nonnull  )
Q3Mesh_FirstFaceFace (
    TQ3MeshFace _Nonnull                  face,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextFaceFace
 *  @discussion
 *      Get the next face in a mesh face.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next face in the face.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nonnull  )
Q3Mesh_NextFaceFace (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstFaceContour
 *  @discussion
 *      Get the first contour in a mesh face.
 *
 *  @param face             The face to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first contour in the face.
 */
Q3_EXTERN_API_C ( TQ3MeshContour _Nonnull  )
Q3Mesh_FirstFaceContour (
    TQ3MeshFace _Nonnull                  face,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextFaceContour
 *  @discussion
 *      Get the next contour in a mesh face.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next contour in the face.
 */
Q3_EXTERN_API_C ( TQ3MeshContour _Nonnull  )
Q3Mesh_NextFaceContour (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstContourEdge
 *  @discussion
 *      Get the first edge in a mesh contour.
 *
 *  @param contour          The contour to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first edge in the contour.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_FirstContourEdge (
    TQ3MeshContour _Nonnull               contour,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextContourEdge
 *  @discussion
 *      Get the next edge in a mesh contour.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next edge in the contour.
 */
Q3_EXTERN_API_C ( TQ3MeshEdge _Nonnull  )
Q3Mesh_NextContourEdge (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstContourVertex
 *  @discussion
 *      Get the first vertex in a mesh contour.
 *
 *  @param contour          The contour to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first vertex in the contour.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_FirstContourVertex (
    TQ3MeshContour _Nonnull               contour,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextContourVertex
 *  @discussion
 *      Get the next vertex in a mesh contour.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next vertex in the contour.
 */
Q3_EXTERN_API_C ( TQ3MeshVertex _Nonnull  )
Q3Mesh_NextContourVertex (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_FirstContourFace
 *  @discussion
 *      Get the first face in a mesh contour.
 *
 *  @param contour          The contour to query.
 *  @param iterator         Receives the mesh iterator.
 *  @result                 The first face in the contour.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nonnull  )
Q3Mesh_FirstContourFace (
    TQ3MeshContour _Nonnull               contour,
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
 *  @function
 *      Q3Mesh_NextContourFace
 *  @discussion
 *      Get the next face in a mesh contour.
 *
 *  @param iterator         The mesh iterator.
 *  @result                 The next face in the contour.
 */
Q3_EXTERN_API_C ( TQ3MeshFace _Nonnull  )
Q3Mesh_NextContourFace (
    TQ3MeshIterator               * _Nonnull iterator
);



/*!
	@functiongroup	NURB Curve Functions
*/



/*!
 *  @function
 *      Q3NURBCurve_New
 *  @discussion
 *      Create a new NURB curve geometry object.
 *
 *  @param curveData        Data describing a NURB curve.
 *  @result                 Reference to a new NURB curve geometry object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3NURBCurve_New (
    const TQ3NURBCurveData        * _Nonnull curveData
);



/*!
 *  @function
 *      Q3NURBCurve_Submit
 *  @discussion
 *		Submits a NURB curve for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param curveData        Data describing a NURB curve.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_Submit (
    const TQ3NURBCurveData        * _Nonnull curveData,
    TQ3ViewObject _Nonnull                 view
);



/*!
 *  @function
 *      Q3NURBCurve_SetData
 *  @discussion
 *      Modify a NURB curve object by supplying a full new set of data.
 *
 *  @param curve            A NURB curve object.
 *  @param nurbCurveData    Data describing a NURB curve object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_SetData (
    TQ3GeometryObject _Nonnull            curve,
    const TQ3NURBCurveData        * _Nonnull nurbCurveData
);



/*!
 *  @function
 *      Q3NURBCurve_GetData
 *  @discussion
 *      Get the data of a NURB Curve object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3NURBCurve_EmptyData</code>.
 *
 *  @param curve            A NURB Curve object.
 *  @param nurbCurveData    Receives data describing the NURB Curve object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_GetData (
    TQ3GeometryObject _Nonnull             curve,
    TQ3NURBCurveData              * _Nonnull nurbCurveData
);



/*!
 *  @function
 *      Q3NURBCurve_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3NURBCurve_GetData</code>.
 *
 *  @param nurbCurveData    Data describing a NURB Curve, previously obtained with
 *							<code>Q3NURBCurve_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_EmptyData (
    TQ3NURBCurveData              * _Nonnull nurbCurveData
);



/*!
 *  @function
 *      Q3NURBCurve_SetControlPoint
 *  @discussion
 *      Set a control point for a NURB curve.
 *
 *  @param curve            The NURB curve to update.
 *  @param pointIndex       The index of the point to update.
 *  @param point4D          The new control point value.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_SetControlPoint (
    TQ3GeometryObject _Nonnull            curve,
    TQ3Uns32                      pointIndex,
    const TQ3RationalPoint4D      * _Nonnull point4D
);



/*!
 *  @function
 *      Q3NURBCurve_GetControlPoint
 *  @discussion
 *      Get a control point from a NURB curve.
 *
 *  @param curve            The NURB curve to query.
 *  @param pointIndex       The index of the point to query.
 *  @param point4D          Receives the control point value.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_GetControlPoint (
    TQ3GeometryObject _Nonnull            curve,
    TQ3Uns32                      pointIndex,
    TQ3RationalPoint4D            * _Nonnull point4D
);



/*!
 *  @function
 *      Q3NURBCurve_SetKnot
 *  @discussion
 *      Set a knot for a NURB curve.
 *
 *  @param curve            The NURB curve to update.
 *  @param knotIndex        The index of the knot to update.
 *  @param knotValue        The new knot value.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_SetKnot (
    TQ3GeometryObject _Nonnull             curve,
    TQ3Uns32                      knotIndex,
    float                         knotValue
);



/*!
 *  @function
 *      Q3NURBCurve_GetKnot
 *  @discussion
 *      Get a knot from a NURB curve.
 *
 *  @param curve            The NURB curve to query.
 *  @param knotIndex        The index of the knot to query.
 *  @param knotValue        Receives the knot value.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBCurve_GetKnot (
    TQ3GeometryObject _Nonnull             curve,
    TQ3Uns32                      knotIndex,
    float                         * _Nonnull knotValue
);



/*!
	@functiongroup	NURB Patch Functions
*/



/*!
 *  @function
 *      Q3NURBPatch_New
 *  @discussion
 *      Create a new NURB patch geometry object.
 *
 *  @param nurbPatchData    Data describing a NURB patch.
 *  @result                 Reference to a new NURB patch geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3NURBPatch_New (
    const TQ3NURBPatchData        * _Nonnull nurbPatchData
);



/*!
 *  @function
 *      Q3NURBPatch_Submit
 *  @discussion
 *		Submits a NURB patch for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param nurbPatchData    Data describing a NURB patch.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_Submit (
    const TQ3NURBPatchData        * _Nonnull nurbPatchData,
    TQ3ViewObject _Nonnull                view
);



/*!
 *  @function
 *      Q3NURBPatch_SetData
 *  @discussion
 *      Modify a NURB patch object by supplying a full new set of data.
 *
 *  @param nurbPatch        A NURB patch object.
 *  @param nurbPatchData    Data describing a NURB patch object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetData (
    TQ3GeometryObject _Nonnull            nurbPatch,
    const TQ3NURBPatchData        * _Nonnull nurbPatchData
);



/*!
 *  @function
 *      Q3NURBPatch_GetData
 *  @discussion
 *      Get the data of a NURB Patch object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3NURBPatch_EmptyData</code>.
 *
 *  @param nurbPatch        A NURB Patch object.
 *  @param nurbPatchData    Receives data describing the NURB Patch object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetData (
    TQ3GeometryObject _Nonnull             nurbPatch,
    TQ3NURBPatchData              * _Nonnull nurbPatchData
);



/*!
 *  @function
 *      Q3NURBPatch_SetControlPoint
 *  @discussion
 *      Set a control point for a NURB patch.
 *
 *  @param nurbPatch        The NURB patch to update.
 *  @param rowIndex         The row index of the control point.
 *  @param columnIndex      The column index of the control point.
 *  @param point4D          The new control point value.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetControlPoint (
    TQ3GeometryObject _Nonnull             nurbPatch,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    const TQ3RationalPoint4D      * _Nonnull point4D
);



/*!
 *  @function
 *      Q3NURBPatch_GetControlPoint
 *  @discussion
 *      Get a control point for a NURB patch.
 *
 *  @param nurbPatch        The NURB patch to query.
 *  @param rowIndex         The row index of the control point.
 *  @param columnIndex      The column index of the control point.
 *  @param point4D          Receives the control point value.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetControlPoint (
    TQ3GeometryObject _Nonnull             nurbPatch,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    TQ3RationalPoint4D            * _Nonnull point4D
);



/*!
 *  @function
 *      Q3NURBPatch_SetUKnot
 *  @discussion
 *      Set a U knot for a NURB patch.
 *
 *  @param nurbPatch        The NURB patch to update.
 *  @param knotIndex        The index of the knot to update.
 *  @param knotValue        The new value for the knot.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetUKnot (
    TQ3GeometryObject _Nonnull            nurbPatch,
    TQ3Uns32                      knotIndex,
    float                         knotValue
);



/*!
 *  @function
 *      Q3NURBPatch_SetVKnot
 *  @discussion
 *      Set a K knot for a NURB patch.
 *
 *  @param nurbPatch        The NURB patch to update.
 *  @param knotIndex        The index of the knot to update.
 *  @param knotValue        The new value for the knot.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_SetVKnot (
    TQ3GeometryObject _Nonnull            nurbPatch,
    TQ3Uns32                      knotIndex,
    float                         knotValue
);



/*!
 *  @function
 *      Q3NURBPatch_GetUKnot
 *  @discussion
 *      Get a U knot from a NURB patch.
 *
 *  @param nurbPatch        The NURB patch to query.
 *  @param knotIndex        The index of the knot to query.
 *  @param knotValue        Rceives the knot value.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetUKnot (
    TQ3GeometryObject _Nonnull            nurbPatch,
    TQ3Uns32                      knotIndex,
    float                         * _Nonnull knotValue
);



/*!
 *  @function
 *      Q3NURBPatch_GetVKnot
 *  @discussion
 *      Get a V knot from a NURB patch.
 *
 *  @param nurbPatch        The NURB patch to query.
 *  @param knotIndex        The index of the knot to query.
 *  @param knotValue        Rceives the knot value.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_GetVKnot (
    TQ3GeometryObject _Nonnull             nurbPatch,
    TQ3Uns32                      knotIndex,
    float                         * _Nonnull knotValue
);



/*!
 *  @function
 *      Q3NURBPatch_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3NURBPatch_GetData</code>.
 *
 *  @param nurbPatchData    Data describing a NURB Patch, previously obtained with
 *							<code>Q3NURBPatch_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3NURBPatch_EmptyData (
    TQ3NURBPatchData              * _Nonnull nurbPatchData
);



/*!
	@functiongroup	Pixmap Marker Functions
*/



/*!
 *  @function
 *      Q3PixmapMarker_New
 *  @discussion
 *      Create a new pixmap marker geometry object.
 *
 *  @param pixmapMarkerData Data describing a pixmap marker.
 *  @result                 Reference to a new pixmap marker geometry object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3PixmapMarker_New (
    const TQ3PixmapMarkerData     * _Nonnull pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_Submit
 *  @discussion
 *		Submits a pixmap marker for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param pixmapMarkerData Data describing a pixmap marker.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_Submit (
    const TQ3PixmapMarkerData     * _Nonnull pixmapMarkerData,
    TQ3ViewObject _Nonnull                 view
);



/*!
 *  @function
 *      Q3PixmapMarker_SetData
 *  @discussion
 *      Modify a pixmap marker object by supplying a full new set of data.
 *
 *  @param geometry         A pixmap marker object.
 *  @param pixmapMarkerData Data describing a pixmap marker object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetData (
    TQ3GeometryObject _Nonnull             geometry,
    const TQ3PixmapMarkerData     * _Nonnull pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_GetData
 *  @discussion
 *      Get the data of a Pixmap Marker object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3PixmapMarker_EmptyData</code>.
 *
 *  @param geometry         A Pixmap Marker object.
 *  @param pixmapMarkerData Receives data describing the Pixmap Marker object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetData (
    TQ3GeometryObject _Nonnull             geometry,
    TQ3PixmapMarkerData           * _Nonnull pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3PixmapMarker_GetData</code>.
 *
 *  @param pixmapMarkerData Data describing a Pixmap Marker, previously obtained with
 *							<code>Q3PixmapMarker_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_EmptyData (
    TQ3PixmapMarkerData           * _Nonnull pixmapMarkerData
);



/*!
 *  @function
 *      Q3PixmapMarker_GetPosition
 *  @discussion
 *      Get the position of the pixmap marker in world coordinates.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param position         Receives the position of the pixmap marker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetPosition (
    TQ3GeometryObject  _Nonnull           pixmapMarker,
    TQ3Point3D                    * _Nonnull position
);



/*!
 *  @function
 *      Q3PixmapMarker_SetPosition
 *  @discussion
 *      Change the position of a pixmap marker object.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param position         New position of the pixmap marker in world coordinates.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetPosition (
    TQ3GeometryObject _Nonnull            pixmapMarker,
    const TQ3Point3D              * _Nonnull position
);



/*!
 *  @function
 *      Q3PixmapMarker_GetXOffset
 *  @discussion
 *      Get the horizontal offset, in screen pixels, from the marker position to
 *		the upper left corner of the marker image.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param xOffset          Receives the horizontal offset of the marker in pixels.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetXOffset (
    TQ3GeometryObject _Nonnull            pixmapMarker,
    TQ3Int32                      * _Nonnull xOffset
);



/*!
 *  @function
 *      Q3PixmapMarker_SetXOffset
 *  @discussion
 *      Set the horizontal offset, in screen pixels, from the marker position to
 *		the upper left corner of the marker image.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param xOffset          The horizontal offset of the marker in pixels.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetXOffset (
    TQ3GeometryObject _Nonnull            pixmapMarker,
    TQ3Int32                      xOffset
);



/*!
 *  @function
 *      Q3PixmapMarker_GetYOffset
 *  @discussion
 *      Get the vertical offset, in screen pixels, from the marker position to
 *		the upper left corner of the marker image.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param yOffset          Receives the vertical offset of the marker in pixels.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetYOffset (
    TQ3GeometryObject _Nonnull            pixmapMarker,
    TQ3Int32                      * _Nonnull yOffset
);



/*!
 *  @function
 *      Q3PixmapMarker_SetYOffset
 *  @discussion
 *      Set the vertical offset, in screen pixels, from the marker position to
 *		the upper left corner of the marker image.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param yOffset          The vertical offset of the marker in pixels.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetYOffset (
    TQ3GeometryObject _Nonnull            pixmapMarker,
    TQ3Int32                      yOffset
);



/*!
 *  @function
 *      Q3PixmapMarker_GetPixmap
 *  @discussion
 *      Get a copy of the pixmap used within a pixmap marker, including
 *		a new reference to the image storage.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param pixmap           Receives the pixmap structure of the marker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_GetPixmap (
    TQ3GeometryObject _Nonnull            pixmapMarker,
    TQ3StoragePixmap              * _Nonnull pixmap
);



/*!
 *  @function
 *      Q3PixmapMarker_SetPixmap
 *  @discussion
 *      Change the pixmap used by a pixmap marker.  The function makes a copy of the data,
 *		including incrementing the reference count of the image storage.
 *
 *  @param pixmapMarker		A pixmap marker object.
 *  @param pixmap           New pixmap to be used by the marker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapMarker_SetPixmap (
    TQ3GeometryObject _Nonnull            pixmapMarker,
    const TQ3StoragePixmap        * _Nonnull pixmap
);



/*!
	@functiongroup	Point Functions
*/



/*!
 *  @function
 *      Q3Point_New
 *  @discussion
 *      Create a new point geometry object.
 *
 *		If you pass nullptr instead of a data pointer, you will get a point disk, at (0, 0, 0).
 *		<em>This behavior was not present in QuickDraw 3D.</em>
 *
 *  @param pointData        Data describing a point, or NULL.
 *  @result                 Reference to a new Point geometry object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable )
Q3Point_New (
    const TQ3PointData            * _Nullable pointData
);



/*!
 *  @function
 *      Q3Point_Submit
 *  @discussion
 *		Submits a point for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param pointData        Data describing a point.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point_Submit (
    const TQ3PointData         * _Nonnull pointData,
    TQ3ViewObject _Nonnull                view
);



/*!
 *  @function
 *      Q3Point_GetData
 *  @discussion
 *      Get the data of a Point object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Point_EmptyData</code>.
 *
 *  @param point            A Point object.
 *  @param pointData        Receives data describing the Point object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point_GetData (
    TQ3GeometryObject _Nonnull             point,
    TQ3PointData                  * _Nonnull pointData
);



/*!
 *  @function
 *      Q3Point_SetData
 *  @discussion
 *      Modify a point object by supplying a full new set of data.
 *
 *  @param point            A point object
 *  @param pointData        Data describing a point object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point_SetData (
    TQ3GeometryObject _Nonnull             point,
    const TQ3PointData            * _Nonnull pointData
);



/*!
 *  @function
 *      Q3Point_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Point_GetData</code>.
 *
 *  @param pointData        Data describing a Point, previously obtained with
 *							<code>Q3Point_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point_EmptyData (
    TQ3PointData                  * _Nonnull pointData
);



/*!
 *  @function
 *      Q3Point_SetPosition
 *  @discussion
 *      Change the position of a point object.
 *
 *  @param point            A point object.
 *  @param position         New position for the point.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point_SetPosition (
    TQ3GeometryObject _Nonnull            point,
    const TQ3Point3D              * _Nonnull position
);



/*!
 *  @function
 *      Q3Point_GetPosition
 *  @discussion
 *      Get the position of a point object.
 *
 *  @param point            A point object.
 *  @param position         Receives the position of the point.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Point_GetPosition (
    TQ3GeometryObject _Nonnull             point,
    TQ3Point3D                    * _Nonnull position
);



/*!
	@functiongroup	Polygon Functions
*/



/*!
 *  @function
 *      Q3Polygon_New
 *  @discussion
 *      Create a new polygon geometry object.
 *
 *  @param polygonData      Data describing a polygon.
 *  @result                 Reference to a new Polygon geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable )
Q3Polygon_New (
    const TQ3PolygonData          * _Nonnull polygonData
);



/*!
 *  @function
 *      Q3Polygon_Submit
 *  @discussion
 *		Submits a polygon for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param polygonData      Data describing a polygon.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polygon_Submit (
    const TQ3PolygonData          * _Nonnull polygonData,
    TQ3ViewObject _Nonnull                view
);



/*!
 *  @function
 *      Q3Polygon_SetData
 *  @discussion
 *      Modify a polygon object by supplying a full new set of data.
 *
 *  @param polygon          A polygon object.
 *  @param polygonData      Data describing a polygon.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polygon_SetData (
    TQ3GeometryObject _Nonnull            polygon,
    const TQ3PolygonData          * _Nonnull polygonData
);



/*!
 *  @function
 *      Q3Polygon_GetData
 *  @discussion
 *      Get the data of a Polygon object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Polygon_EmptyData</code>.
 *
 *  @param polygon          A Polygon object.
 *  @param polygonData      Receives data describing the Polygon object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polygon_GetData (
    TQ3GeometryObject _Nonnull            polygon,
    TQ3PolygonData                * _Nonnull polygonData
);



/*!
 *  @function
 *      Q3Polygon_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Polygon_GetData</code>.
 *
 *  @param polygonData      Data describing a Polygon, previously obtained with
 *							<code>Q3Polygon_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polygon_EmptyData (
    TQ3PolygonData                * _Nonnull polygonData
);



/*!
 *  @function
 *      Q3Polygon_GetVertexPosition
 *  @discussion
 *      Get the 3D position of a vertex of a polygon object.
 *
 *  @param polygon          A polygon object.
 *  @param index            A 0-based index into the array of vertices of the polygon.
 *  @param point            Receives the position of the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polygon_GetVertexPosition (
    TQ3GeometryObject _Nonnull            polygon,
    TQ3Uns32                      index,
    TQ3Point3D                    * _Nonnull point
);



/*!
 *  @function
 *      Q3Polygon_SetVertexPosition
 *  @discussion
 *      Change the 3D position of a vertex of a polygon object.
 *
 *  @param polygon          A polygon object.
 *  @param index            A 0-based index into the array of vertices of the polygon.
 *  @param point            New position of the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polygon_SetVertexPosition (
    TQ3GeometryObject _Nonnull            polygon,
    TQ3Uns32                      index,
    const TQ3Point3D              * _Nonnull point
);



/*!
 *  @function
 *      Q3Polygon_GetVertexAttributeSet
 *  @discussion
 *      Get the attribute set of a vertex in a polygon object.
 *
 *  @param polygon          A polygon object.
 *  @param index            A 0-based index into the array of vertices of the polygon.
 *  @param attributeSet     Receives a new reference to the attribute set, or nullptr.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polygon_GetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            polygon,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable     * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3Polygon_SetVertexAttributeSet
 *  @discussion
 *      Change the attribute set of a vertex in a polygon object.
 *
 *  @param polygon          A polygon object.
 *  @param index            A 0-based index into the array of vertices of the polygon.
 *  @param attributeSet     New attribute set for the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polygon_SetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            polygon,
    TQ3Uns32                      index,
    TQ3AttributeSet  _Nullable             attributeSet
);



/*!
	@functiongroup	Polyhedron Functions
*/



/*!
 *  @function
 *      Q3Polyhedron_New
 *  @discussion
 *      Create a new polyhedron geometry object.
 *
 *  @param polyhedronData   Data describing a polyhedron.
 *  @result                 Reference to a new Polyhedron geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable )
Q3Polyhedron_New (
    const TQ3PolyhedronData       * _Nonnull polyhedronData
);



/*!
 *  @function
 *      Q3Polyhedron_Submit
 *  @discussion
 *		Submits a polyhedron for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param polyhedronData   Data describing a polyhedron.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_Submit (
    const TQ3PolyhedronData       * _Nonnull polyhedronData,
    TQ3ViewObject _Nonnull                view
);



/*!
 *  @function
 *      Q3Polyhedron_SetData
 *  @discussion
 *      Modify a polyhedron object by supplying a full new set of data.
 *
 *  @param polyhedron       A polyhedron object.
 *  @param polyhedronData   Data describing a polyhedron.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetData (
    TQ3GeometryObject _Nonnull            polyhedron,
    const TQ3PolyhedronData       * _Nonnull polyhedronData
);



/*!
 *  @function
 *      Q3Polyhedron_GetData
 *  @discussion
 *      Get the data of a Polyhedron object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Polyhedron_EmptyData</code>.
 *
 *  @param polyhedron       A Polyhedron object.
 *  @param polyhedronData   Receives data describing the Polyhedron object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetData (
    TQ3GeometryObject  _Nonnull           polyhedron,
    TQ3PolyhedronData             * _Nonnull polyhedronData
);



/*!
 *  @function
 *      Q3Polyhedron_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Polyhedron_GetData</code>.
 *
 *  @param polyhedronData   Data describing a Polyhedron, previously obtained with
 *							<code>Q3Polyhedron_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_EmptyData (
    TQ3PolyhedronData             * _Nonnull polyhedronData
);



/*!
 *  @function
 *      Q3Polyhedron_SetVertexPosition
 *  @discussion
 *      Set the position of a polyhedron vertex.
 *
 *  @param polyhedron       The polyhedron to update.
 *  @param index            The index of the vertex to update.
 *  @param point            The new position for the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetVertexPosition (
    TQ3GeometryObject _Nonnull            polyhedron,
    TQ3Uns32                      index,
    const TQ3Point3D              * _Nonnull point
);



/*!
 *  @function
 *      Q3Polyhedron_GetVertexPosition
 *  @discussion
 *      Get the position of a polyhedron vertex.
 *
 *  @param polyhedron       The polyhedron to query.
 *  @param index            The index of the vertex to query.
 *  @param point            Receives the position of the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetVertexPosition (
    TQ3GeometryObject _Nonnull            polyhedron,
    TQ3Uns32                      index,
    TQ3Point3D                    * _Nonnull point
);



/*!
 *  @function
 *      Q3Polyhedron_SetVertexAttributeSet
 *  @discussion
 *      Set the attribute set of a polyhedron vertex.
 *
 *  @param polyhedron       The polyhedron to update.
 *  @param index            The index of the vertex to update.
 *  @param attributeSet     The new attribute set for the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetVertexAttributeSet (
    TQ3GeometryObject _Nonnull             polyhedron,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable              attributeSet
);



/*!
 *  @function
 *      Q3Polyhedron_GetVertexAttributeSet
 *  @discussion
 *      Get the position of a polyhedron vertex.
 *
 *  @param polyhedron       The polyhedron to query.
 *  @param index            The index of the vertex to query.
 *  @param attributeSet     Receives the attribute set of the vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetVertexAttributeSet (
    TQ3GeometryObject _Nonnull             polyhedron,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable              * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3Polyhedron_GetTriangleData
 *  @discussion
 *      Get the data of a polyhedron triangle.
 *
 *  @param polyhedron       The polyhedron to query.
 *  @param triangleIndex    The index of the triangle to query.
 *  @param triangleData     Receives the data of the triangle.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetTriangleData (
    TQ3GeometryObject _Nonnull            polyhedron,
    TQ3Uns32                      triangleIndex,
    TQ3PolyhedronTriangleData     * _Nonnull triangleData
);



/*!
 *  @function
 *      Q3Polyhedron_SetTriangleData
 *  @discussion
 *      Set the data of a polyhedron triangle.
 *
 *  @param polyhedron       The polyhedron to update.
 *  @param triangleIndex    The index of the triangle to update.
 *  @param triangleData     The new data for the triangle.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetTriangleData (
    TQ3GeometryObject _Nonnull            polyhedron,
    TQ3Uns32                      triangleIndex,
    const TQ3PolyhedronTriangleData * _Nonnull triangleData
);



/*!
 *  @function
 *      Q3Polyhedron_GetEdgeData
 *  @discussion
 *      Get the data of a polyhedron edge.
 *
 *  @param polyhedron       The polyhedron to query.
 *  @param edgeIndex        The index of the edge to query.
 *  @param edgeData         Receives the data of the edge.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_GetEdgeData (
    TQ3GeometryObject _Nonnull             polyhedron,
    TQ3Uns32                      edgeIndex,
    TQ3PolyhedronEdgeData         * _Nonnull edgeData
);



/*!
 *  @function
 *      Q3Polyhedron_SetEdgeData
 *  @discussion
 *      Set the data of a polyhedron edge.
 *
 *  @param polyhedron       The polyhedron to update.
 *  @param edgeIndex        The index of the edge to update.
 *  @param edgeData         The new data for the edge.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Polyhedron_SetEdgeData (
    TQ3GeometryObject _Nonnull            polyhedron,
    TQ3Uns32                      edgeIndex,
    const TQ3PolyhedronEdgeData   * _Nonnull edgeData
);



/*!
	@functiongroup	PolyLine Functions
*/



/*!
 *  @function
 *      Q3PolyLine_New
 *  @discussion
 *      Create a new polyline geometry object.
 *
 *  @param polylineData     Data describing a polyline.
 *  @result                 Reference to a new Polyline geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3PolyLine_New (
    const TQ3PolyLineData         * _Nonnull polylineData
);



/*!
 *  @function
 *      Q3PolyLine_Submit
 *  @discussion
 *		Submits a PolyLine for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param polyLineData     Data describing a PolyLine.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_Submit (
    const TQ3PolyLineData         * _Nonnull polyLineData,
    TQ3ViewObject _Nonnull                view
);



/*!
 *  @function
 *      Q3PolyLine_SetData
 *  @discussion
 *      Modify a PolyLine object by supplying a full new set of data.
 *
 *  @param polyLine         A PolyLine object.
 *  @param polyLineData     Data describing a PolyLine.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetData (
    TQ3GeometryObject _Nonnull             polyLine,
    const TQ3PolyLineData         * _Nonnull polyLineData
);



/*!
 *  @function
 *      Q3PolyLine_GetData
 *  @discussion
 *      Get the data of a PolyLine object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3PolyLine_EmptyData</code>.
 *
 *  @param polyLine         A PolyLine object.
 *  @param polyLineData     Receives data describing the PolyLine object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetData (
    TQ3GeometryObject _Nonnull             polyLine,
    TQ3PolyLineData               * _Nonnull polyLineData
);



/*!
 *  @function
 *      Q3PolyLine_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3PolyLine_GetData</code>.
 *
 *  @param polyLineData     Data describing a PolyLine, previously obtained with
 *							<code>Q3PolyLine_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_EmptyData (
    TQ3PolyLineData               * _Nonnull polyLineData
);



/*!
 *  @function
 *      Q3PolyLine_GetVertexPosition
 *  @discussion
 *      Get the position of a polyline vertex.
 *
 *  @param polyLine         The polyline to query.
 *  @param index            The index of the vertex to query.
 *  @param position         Receives the position of the polyline vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetVertexPosition (
    TQ3GeometryObject _Nonnull            polyLine,
    TQ3Uns32                      index,
    TQ3Point3D                    * _Nonnull position
);



/*!
 *  @function
 *      Q3PolyLine_SetVertexPosition
 *  @discussion
 *      Set the position of a polyline vertex.
 *
 *  @param polyLine         The polyline to update.
 *  @param index            The index of the vertex to update.
 *  @param position         The new position for the polyline vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetVertexPosition (
    TQ3GeometryObject _Nonnull            polyLine,
    TQ3Uns32                      index,
    const TQ3Point3D              * _Nonnull position
);



/*!
 *  @function
 *      Q3PolyLine_GetVertexAttributeSet
 *  @discussion
 *      Get the attribute set of a polyline vertex.
 *
 *  @param polyLine         The polyline to query.
 *  @param index            The index of the vertex to query.
 *  @param attributeSet     Receives the attribute set of the polyline vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            polyLine,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable              * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3PolyLine_SetVertexAttributeSet
 *  @discussion
 *      Set the attribute set of a polyline vertex.
 *
 *  @param polyLine         The polyline to update.
 *  @param index            The index of the vertex to update.
 *  @param attributeSet     The new attribute set for the polyline vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            polyLine,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable              attributeSet
);



/*!
 *  @function
 *      Q3PolyLine_GetSegmentAttributeSet
 *  @discussion
 *      Get the attribute set of a polyline segment.
 *
 *  @param polyLine         The polyline to query.
 *  @param index            The index of the segment to query.
 *  @param attributeSet     Receives the attribute set of the polyline segment.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_GetSegmentAttributeSet (
    TQ3GeometryObject _Nonnull            polyLine,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable              * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3PolyLine_SetSegmentAttributeSet
 *  @discussion
 *      Set the attribute set of a polyline segment.
 *
 *  @param polyLine         The polyline to update.
 *  @param index            The index of the segment to update.
 *  @param attributeSet     The new attribute set for the polyline segment.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PolyLine_SetSegmentAttributeSet (
    TQ3GeometryObject _Nonnull            polyLine,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable              attributeSet
);



/*!
	@functiongroup	Torus Functions
*/



/*!
 *  @function
 *      Q3Torus_New
 *  @discussion
 *      Create a new torus geometry object.
 *
 *		If you pass nullptr, you will get a default torus with orientation (1, 0, 0), major axis
 *		(0, 1, 0), minor axis (0, 0, 1), origin (0, 0, 0), and ratio 1.  <em>This behavior was
 *		not present in QuickDraw 3D.</em>
 *
 *  @param torusData        Data describing a torus, or nullptr.
 *  @result                 Reference to a new Torus geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3Torus_New (
    const TQ3TorusData            * _Nullable torusData
);



/*!
 *  @function
 *      Q3Torus_Submit
 *  @discussion
 *		Submits a torus for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param torusData        Data describing a torus.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_Submit (
    const TQ3TorusData            * _Nonnull torusData,
    TQ3ViewObject _Nonnull                 view
);



/*!
 *  @function
 *      Q3Torus_SetData
 *  @discussion
 *      Modify a torus object by supplying a full new set of data.
 *
 *  @param torus            A torus object.
 *  @param torusData        Data describing a torus.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_SetData (
    TQ3GeometryObject _Nonnull             torus,
    const TQ3TorusData            * _Nonnull torusData
);



/*!
 *  @function
 *      Q3Torus_GetData
 *  @discussion
 *      Get the data of a Torus object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Torus_EmptyData</code>.
 *
 *  @param torus            A Torus object.
 *  @param torusData        Receives data describing the Torus object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_GetData (
    TQ3GeometryObject _Nonnull            torus,
    TQ3TorusData                  * _Nonnull torusData
);



/*!
 *  @function
 *      Q3Torus_SetOrigin
 *  @discussion
 *      Change the origin of a torus object.
 *
 *  @param torus            The torus object.
 *  @param origin           The new origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_SetOrigin (
    TQ3GeometryObject _Nonnull             torus,
    const TQ3Point3D              * _Nonnull origin
);



/*!
 *  @function
 *      Q3Torus_SetOrientation
 *  @discussion
 *      Change the orientation vector of a torus object.
 *
 *  @param torus            A torus object.
 *  @param orientation      New orientation vector for the torus.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_SetOrientation (
    TQ3GeometryObject _Nonnull            torus,
    const TQ3Vector3D             * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Torus_SetMajorRadius
 *  @discussion
 *      Change the major radius vector of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param majorRadius      New major radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_SetMajorRadius (
    TQ3GeometryObject _Nonnull            torus,
    const TQ3Vector3D             * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Torus_SetMinorRadius
 *  @discussion
 *      Change the minor radius vector of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param minorRadius      New minor radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_SetMinorRadius (
    TQ3GeometryObject _Nonnull            torus,
    const TQ3Vector3D             * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Torus_SetRatio
 *  @discussion
 *      Set the ratio of a torus.
 *
 *  @param torus            The torus to update.
 *  @param ratio            The new ratio for the torus.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_SetRatio (
    TQ3GeometryObject _Nonnull            torus,
    float                         ratio
);



/*!
 *  @function
 *      Q3Torus_GetOrigin
 *  @discussion
 *      Get the origin of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param origin           Receives the origin.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_GetOrigin (
    TQ3GeometryObject _Nonnull            torus,
    TQ3Point3D                    * _Nonnull origin
);



/*!
 *  @function
 *      Q3Torus_GetOrientation
 *  @discussion
 *      Get the orientation vector of a torus.
 *
 *  @param torus            A torus object.
 *  @param orientation      Receives the orientation vector of the torus.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_GetOrientation (
    TQ3GeometryObject _Nonnull            torus,
    TQ3Vector3D                   * _Nonnull orientation
);



/*!
 *  @function
 *      Q3Torus_GetMajorRadius
 *  @discussion
 *      Get the major radius vector of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param majorRadius      Receives the major radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_GetMajorRadius (
    TQ3GeometryObject _Nonnull            torus,
    TQ3Vector3D                   * _Nonnull majorRadius
);



/*!
 *  @function
 *      Q3Torus_GetMinorRadius
 *  @discussion
 *      Get the minor radius vector of a Torus object.
 *
 *  @param torus            The Torus object.
 *  @param minorRadius      Receives the minor radius vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_GetMinorRadius (
    TQ3GeometryObject _Nonnull            torus,
    TQ3Vector3D                   * _Nonnull minorRadius
);



/*!
 *  @function
 *      Q3Torus_GetRatio
 *  @discussion
 *      Get the ratio of a torus.
 *
 *  @param torus            The torus to query.
 *  @param ratio            Receives the ratio of the torus.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_GetRatio (
    TQ3GeometryObject _Nonnull            torus,
    float                         * _Nonnull ratio
);



/*!
 *  @function
 *      Q3Torus_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Torus_GetData</code>.
 *
 *  @param torusData        Data describing a Torus, previously obtained with
 *							<code>Q3Torus_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Torus_EmptyData (
    TQ3TorusData                  * _Nonnull torusData
);



/*!
	@functiongroup	Triangle Functions
*/



/*!
 *  @function
 *      Q3Triangle_New
 *  @discussion
 *      Create a new triangle geometry object.
 *
 *  @param triangleData     Data describing a triangle.
 *  @result                 Reference to a new Triangle geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable )
Q3Triangle_New (
    const TQ3TriangleData         * _Nonnull triangleData
);



/*!
 *  @function
 *      Q3Triangle_Submit
 *  @discussion
 *		Submits a triangle for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param triangleData     Data describing a triangle.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Triangle_Submit (
    const TQ3TriangleData         * _Nonnull triangleData,
    TQ3ViewObject _Nonnull                view
);



/*!
 *  @function
 *      Q3Triangle_SetData
 *  @discussion
 *      Modify a triangle object by supplying a full new set of data.
 *
 *  @param triangle         A triangle object.
 *  @param triangleData     Data describing a triangle.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Triangle_SetData (
    TQ3GeometryObject _Nonnull            triangle,
    const TQ3TriangleData         * _Nonnull triangleData
);



/*!
 *  @function
 *      Q3Triangle_GetData
 *  @discussion
 *      Get the data of a Triangle object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3Triangle_EmptyData</code>.
 *
 *  @param triangle         A Triangle object.
 *  @param triangleData     Receives data describing the Triangle object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Triangle_GetData (
    TQ3GeometryObject _Nonnull            triangle,
    TQ3TriangleData               * _Nonnull triangleData
);



/*!
 *  @function
 *      Q3Triangle_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3Triangle_GetData</code>.
 *
 *  @param triangleData     Data describing a Triangle, previously obtained with
 *							<code>Q3Triangle_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Triangle_EmptyData (
    TQ3TriangleData               * _Nonnull triangleData
);



/*!
 *  @function
 *      Q3Triangle_GetVertexPosition
 *  @discussion
 *      Get the position of a triangle vertex.
 *
 *  @param triangle         The triangle to query.
 *  @param index            The index of the vertex to query.
 *  @param point            Receives the position of the triangle vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Triangle_GetVertexPosition (
    TQ3GeometryObject _Nonnull            triangle,
    TQ3Uns32                      index,
    TQ3Point3D                    * _Nonnull point
);



/*!
 *  @function
 *      Q3Triangle_SetVertexPosition
 *  @discussion
 *      Set the position of a triangle vertex.
 *
 *  @param triangle         The triangle to update.
 *  @param index            The index of the vertex to update.
 *  @param point            The new position for the triangle vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Triangle_SetVertexPosition (
    TQ3GeometryObject _Nonnull            triangle,
    TQ3Uns32                      index,
    const TQ3Point3D              * _Nonnull point
);



/*!
 *  @function
 *      Q3Triangle_GetVertexAttributeSet
 *  @discussion
 *      Get the attribute set of a triangle vertex.
 *
 *  @param triangle         The triangle to query.
 *  @param index            The index of the vertex to query.
 *  @param attributeSet     Receives the attribute set of the triangle vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Triangle_GetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            triangle,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable              * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3Triangle_SetVertexAttributeSet
 *  @discussion
 *      Set the attribute set of a triangle vertex.
 *
 *  @param triangle         The triangle to update.
 *  @param index            The index of the vertex to update.
 *  @param attributeSet     The new attribute set for the triangle vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Triangle_SetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            triangle,
    TQ3Uns32                      index,
    TQ3AttributeSet _Nullable              attributeSet
);



/*!
	@functiongroup	TriGrid Functions
*/



/*!
 *  @function
 *      Q3TriGrid_New
 *  @discussion
 *      Create a new TriGrid geometry object.
 *
 *  @param triGridData      Data describing a TriGrid.
 *  @result                 Reference to a new TriGrid geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable  )
Q3TriGrid_New (
    const TQ3TriGridData          * _Nonnull triGridData
);



/*!
 *  @function
 *      Q3TriGrid_Submit
 *  @discussion
 *		Submits a TriGrid for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param triGridData      Data describing a TriGrid.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_Submit (
    const TQ3TriGridData          * _Nonnull triGridData,
    TQ3ViewObject _Nonnull                view
);



/*!
 *  @function
 *      Q3TriGrid_SetData
 *  @discussion
 *      Modify a TriGrid object by supplying a full new set of data.
 *
 *  @param triGrid          A TriGrid object.
 *  @param triGridData      Data describing a TriGrid.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetData (
    TQ3GeometryObject _Nonnull            triGrid,
    const TQ3TriGridData          * _Nonnull triGridData
);



/*!
 *  @function
 *      Q3TriGrid_GetData
 *  @discussion
 *      Get the data of a TriGrid object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3TriGrid_EmptyData</code>.
 *
 *  @param triGrid          A TriGrid object.
 *  @param triGridData      Receives data describing the TriGrid object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetData (
    TQ3GeometryObject _Nonnull            triGrid,
    TQ3TriGridData                * _Nonnull triGridData
);



/*!
 *  @function
 *      Q3TriGrid_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3TriGrid_GetData</code>.
 *
 *  @param triGridData      Data describing a TriGrid, previously obtained with
 *							<code>Q3TriGrid_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_EmptyData (
    TQ3TriGridData                * _Nonnull triGridData
);



/*!
 *  @function
 *      Q3TriGrid_GetVertexPosition
 *  @discussion
 *      Get the position of a trigrid vertex.
 *
 *  @param triGrid          The trigrid to query.
 *  @param rowIndex         The row index of the vertex to query.
 *  @param columnIndex      The column index of the vertex to query.
 *  @param position         Receives the position of the trigrid vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetVertexPosition (
    TQ3GeometryObject _Nonnull            triGrid,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    TQ3Point3D                    * _Nonnull position
);



/*!
 *  @function
 *      Q3TriGrid_SetVertexPosition
 *  @discussion
 *      Set the position of a trigrid vertex.
 *
 *  @param triGrid          The trigrid to update.
 *  @param rowIndex         The row index of the vertex to update.
 *  @param columnIndex      The column index of the vertex to update.
 *  @param position         The new position for the trigrid vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetVertexPosition (
    TQ3GeometryObject _Nonnull            triGrid,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    const TQ3Point3D              * _Nonnull position
);



/*!
 *  @function
 *      Q3TriGrid_GetVertexAttributeSet
 *  @discussion
 *      Get the attribute set of a trigrid vertex.
 *
 *  @param triGrid          The trigrid to query.
 *  @param rowIndex         The row index of the vertex to query.
 *  @param columnIndex      The column index of the vertex to query.
 *  @param attributeSet     Receives the attribute set of the trigrid vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            triGrid,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    TQ3AttributeSet _Nullable              * _Nonnull attributeSet
);



/*!
 *  @function
 *      Q3TriGrid_SetVertexAttributeSet
 *  @discussion
 *      Set the attribute set of a trigrid vertex.
 *
 *  @param triGrid          The trigrid to update.
 *  @param rowIndex         The row index of the vertex to update.
 *  @param columnIndex      The column index of the vertex to update.
 *  @param attributeSet     The new attribute set for the trigrid vertex.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetVertexAttributeSet (
    TQ3GeometryObject _Nonnull            triGrid,
    TQ3Uns32                      rowIndex,
    TQ3Uns32                      columnIndex,
    TQ3AttributeSet _Nullable              attributeSet
);



/*!
 *  @function
 *      Q3TriGrid_GetFacetAttributeSet
 *  @discussion
 *      Get the attribute set of a trigrid face.
 *
 *  @param triGrid              The trigrid to query.
 *  @param faceIndex            The index of the face to query.
 *  @param facetAttributeSet    Receives the attribute set of the trigrid face.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_GetFacetAttributeSet (
    TQ3GeometryObject _Nonnull             triGrid,
    TQ3Uns32                      faceIndex,
    TQ3AttributeSet _Nullable              * _Nonnull facetAttributeSet
);



/*!
 *  @function
 *      Q3TriGrid_SetFacetAttributeSet
 *  @discussion
 *      Set the attribute set of a trigrid face.
 *
 *  @param triGrid              The trigrid to update.
 *  @param faceIndex            The index of the face to update.
 *  @param facetAttributeSet    The new attribute set for the trigrid face.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriGrid_SetFacetAttributeSet (
    TQ3GeometryObject _Nonnull            triGrid,
    TQ3Uns32                      faceIndex,
    TQ3AttributeSet  _Nullable             facetAttributeSet
);



/*!
	@functiongroup	TriMesh Functions
*/



/*!
 *  @function
 *      Q3TriMesh_New
 *  @discussion
 *      Create a new TriMesh geometry object.
 *
 *		If you wish, you can set the <code>isEmpty</code> flag in the <code>bBox</code> field,
 *		and Quesa will compute the bounding box.
 *		
 *		Edge and face indices will be checked, possibly resulting in an index out
 *		of bounds error or warning.  Also, if Quesa was compiled with the flag
 *		QUESA_NORMALIZE_NORMALS turned on (as it is by default), face and vertex
 *		normal vectors will be normalized.
 *
 *  @param triMeshData      Data describing a TriMesh.
 *  @result                 Reference to a new TriMesh geometry object, or nullptr on failure.
 */
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable )
Q3TriMesh_New (
    const TQ3TriMeshData          * _Nonnull triMeshData
);



/*!
	@function	Q3TriMesh_New_NoCopy
	@abstract	Create a new TriMesh geometry object without any allocation or
				copying.
	@discussion	This is a version of <code>Q3TriMesh_New</code> for use in cases
				where TriMeshes are being created at runtime and performance is
				critical.  It assumes that each of the 6 array pointers within
				the <code>TQ3TriMeshData</code> structure, and each array
				pointer within the <code>TQ3TriMeshAttributeData</code>
				structures, is either nullptr or has been allocated with one of the
				Quesa memory allocation functions.  Ownership of these pointers
				is transferred to the TriMesh object.
				
				Unlike <code>Q3TriMesh_New</code>, this function does not
				validate edge and face indices.  If Quesa was compiled with the
				flag <code>QUESA_NORMALIZE_NORMALS</code> turned on (as it is by
				default), face and vertex normal vectors will be normalized.
				
				If you wish, you can set the <code>isEmpty</code> flag in the
				<code>bBox</code> field, and Quesa will compute the bounding box.
	
	@param triMeshData      Data describing a TriMesh.
	@result                 Reference to a new TriMesh geometry object, or nullptr on failure.
*/
Q3_EXTERN_API_C ( TQ3GeometryObject _Nullable )
Q3TriMesh_New_NoCopy (
    const TQ3TriMeshData          * _Nonnull triMeshData
);



/*!
 *  @function
 *      Q3TriMesh_Submit
 *  @discussion
 *		Submits a TriMesh for drawing, picking, bounding, or writing in immediate mode.
 *
 *		This function should only be called in a submitting loop.
 *
 *  @param triMeshData      Data describing a TriMesh.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriMesh_Submit (
    const TQ3TriMeshData          * _Nonnull triMeshData,
    TQ3ViewObject _Nonnull                view
);



/*!
 *  @function
 *      Q3TriMesh_SetData
 *  @discussion
 *      Modify a TriMesh object by supplying a full new set of data.
 *
 *		If you wish, you can set the <code>isEmpty</code> flag in the <code>bBox</code> field,
 *		and Quesa will compute the bounding box.
 *
 *  @param triMesh          A TriMesh object.
 *  @param triMeshData      Data describing a TriMesh.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriMesh_SetData (
    TQ3GeometryObject _Nonnull            triMesh,
    const TQ3TriMeshData          * _Nonnull triMeshData
);



/*!
 *  @function
 *      Q3TriMesh_GetData
 *  @discussion
 *      Get the data of a TriMesh object.
 *
 *      This function may allocate memory, which should be freed using
 *		<code>Q3TriMesh_EmptyData</code>.
 *
 *  @param triMesh          A TriMesh object.
 *  @param triMeshData      Receives data describing the TriMesh object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriMesh_GetData (
    TQ3GeometryObject _Nonnull            triMesh,
    TQ3TriMeshData                * _Nonnull triMeshData
);



/*!
 *  @function
 *      Q3TriMesh_EmptyData
 *  @discussion
 *      Release memory allocated by <code>Q3TriMesh_GetData</code>.
 *
 *  @param triMeshData      Data describing a TriMesh, previously obtained with
 *							<code>Q3TriMesh_GetData</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TriMesh_EmptyData (
    TQ3TriMeshData                * _Nonnull triMeshData
);



/*!
 *  @function
 *      Q3TriMesh_LockData
 *  @discussion
 *      Lock a Trimesh for direct access.
 *
 *      Returns a pointer to the internal TQ3TriMeshData for a TriMesh,
 *      allowing direct access without the need to copy TriMesh data out
 *      of and back in to Quesa.
 *
 *      The readOnly flag should be used to indicate if the application
 *      needs to make changes to the TriMesh data, or if the pointer
 *      should be considered const.
 *
 *      When the application no longer needs access to the TriMesh data,
 *      it must unlock the TriMesh with Q3TriMesh_UnlockData. Changes to
 *      the TriMesh data may not be relayed to renderers until the TriMesh
 *      has been unlocked.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param triMesh          The TriMesh to lock.
 *  @param readOnly         Indicates if the returned data is read-only.
 *  @param triMeshData      Receives a pointer to the TQ3TriMeshData for
 *                          the TriMesh.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3TriMesh_LockData (
    TQ3GeometryObject _Nonnull            triMesh,
    TQ3Boolean                    readOnly,
    TQ3TriMeshData                * _Nonnull * _Nonnull triMeshData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3TriMesh_UnlockData
 *  @discussion
 *      Unlocks a TriMesh previously locked with Q3TriMesh_LockData.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param triMesh          The TriMesh to unlock.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3TriMesh_UnlockData (
    TQ3GeometryObject _Nonnull            triMesh
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *		Q3TriMesh_OptimizeData
 *	@abstract
 *		Modify TriMesh data, if needed, for efficient use by the
 *		interactive renderer.
 *	
 *	@discussion
 *		This operation modifies TriMesh data so that:
 *				<ol>
 *					<li>Face normals exist.</li>
 *					<li>Vertex normals exist.</li>
 *					<li>If Face colors exist, then vertex colors exist.</li>
 *				</ol>
 *		If face normals do not already exist, they will be computed
 *		by cross products of edges.  The orientation will be assumed
 *		to be counterclockwise.
 *				
 *		If vertex normals do not already exist, they will be derived
 *		from face normals.  When a vertex belongs to faces with
 *		different normals, the vertex will be duplicated.
 *				
 *		If a color attribute (diffuse, transparent, or specular) exists
 *		on faces but not vertices, it will be converted to a vertex
 *		attribute, duplicating vertices when needed.
 *				
 *		If no optimization is needed, outDidChange will return kQ3False
 *		and outData will be cleared to zero.  If optimization was
 *		performed, indicated by outDidChange being kQ3True, then you
 *		are responsible for calling Q3TriMesh_EmptyData on the outData
 *		structure when you are done with it.
 *
 *		The time it takes to perform the optimization depends on the numbers
 *		of vertices, faces, edges, and attributes in the TriMesh.  However,
 *		determining whether optimization is <em>needed</em> is quick, as it
 *		depends only on the number of attribute types.
 *	
 *      <em>This function is not available in QD3D.</em>
 *
 *	@param		inData			TriMesh data.
 *	@param		outData			Receives new TriMesh data, if outDidChange is true.
 *	@param		outDidChange	Receives a flag indicating whether new data
 *								was created.
 *	@result		Success or failure of the operation.
*/
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C( TQ3Status )
Q3TriMesh_OptimizeData( const TQ3TriMeshData* _Nonnull inData,
								TQ3TriMeshData* _Nonnull outData,
								TQ3Boolean* _Nonnull outDidChange
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *		Q3TriMesh_Optimize
 *	@abstract
 *		Modify a TriMesh, if needed, for efficient use by the interactive renderer.
 *	
 *	@discussion
 *		See discussion of Q3TriMesh_OptimizeData for the optimizations
 *		that are performed.  If no optimizations are needed, nullptr is returned.
 *	
 *      <em>This function is not available in QD3D.</em>
 *
 *	@param		inTriMesh		A TriMesh geometry.
 *	@result		A TriMesh or nullptr.
*/
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C( TQ3GeometryObject _Nullable )
Q3TriMesh_Optimize(
	TQ3GeometryObject _Nonnull inTriMesh
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
	@function		Q3TriMesh_MakeTriangleStrip
	@abstract		Compute a triangle strip.
	@discussion		Although this function does not take a TriMesh as a
					parameter, it is grouped with TriMesh functions because
					Quesa's OpenGL renderer currently only uses triangle strips
					with TriMeshes.
					
					Also see the triangle strip element functions in
					QuesaCustomElements.h.
					
					When you are finished with the data returned in the
					outStrip parameter, free it using Q3Memory_Free. 
					
 					<em>This function is not available in QD3D.</em>
 	@param	inNumTriangles	Number of triangles.
 	@param	inTriangles		Point indices for the triangles.  The length of
 							this array should be 3 * inNumTriangles.
 	@param	outStripLength	Receives number of indices in outStrip.
 	@param	outStrip		Receives pointer to array of point indices in the
 							strip.  You are responsible for freeing this
 							memory with Q3Memory_Free.
 	@result	Success or failure of the operation.
*/
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C( TQ3Status )
Q3TriMesh_MakeTriangleStrip(
	TQ3Uns32 inNumTriangles,
	const TQ3Uns32* _Nonnull inTriangles,
	TQ3Uns32* _Nonnull outStripLength,
	TQ3Uns32* _Nonnull * _Nonnull outStrip
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS


/*!
	@function	Q3TriMesh_GetNakedGeometry
	@abstract	Get a reference to the unattributed geometry owned by a TriMesh.
	@discussion	Using this function and its partner <code>Q3TriMesh_SetNakedGeometry</code>,
				you can have two TriMesh objects with the same geometry but
				different attribute sets.  This can be helpful in saving
				memory.  That is, you can make two TriMeshes share geometry by saying:
				
				<blockquote><pre><code>
				TQ3Object secondTriMesh = Q3Object_Duplicate( firstTriMesh );
				TQ3Object firstNaked = Q3TriMesh_GetNakedGeometry( firstTriMesh );
				Q3TriMesh_SetNakedGeometry( secondTriMesh, firstNaked );
				Q3Object_Dispose( firstNaked );
				</code></pre></blockquote>
				
				Sharing of naked geometry works in a copy-on-write way.  That is, if two
				TriMeshes share the naked geometry, but then you modify one of the TriMeshes
				using <code>Q3TriMesh_SetData</code>, <code>Q3TriMesh_Optimize</code>, or
				<code>Q3TriMesh_LockData</code> with a read-write lock, then the TriMeshes will
				no longer share naked geometry.
				
				A naked geometry of a TriMesh is a different type of object than a TriMesh.
				You can't do anything with it but share it, dispose it, duplicate it, or pass it
				as the second parameter of <code>Q3TriMesh_SetNakedGeometry</code>.
				
	@param		inGeom		A TriMesh object.
	@result		A new reference to the unattributed geometry owned by the
				given TriMesh.
*/
Q3_EXTERN_API_C( TQ3GeometryObject _Nonnull )
Q3TriMesh_GetNakedGeometry( TQ3GeometryObject _Nonnull inGeom );



/*!
	@function	Q3TriMesh_SetNakedGeometry
	@abstract	Change the unattributed geometry owned by a TriMesh.
	@discussion	Using this function and its partner <code>Q3TriMesh_GetNakedGeometry</code>,
				you can have two TriMesh objects with the same geometry but
				different attribute sets.  This can be helpful in saving
				memory.  See the discussion of <code>Q3TriMesh_GetNakedGeometry</code>.
	@param		inTriMesh		A TriMesh object to modify.
	@param		inNaked			An internal geometry previously obtained using
								Q3TriMesh_GetNakedGeometry.
*/
Q3_EXTERN_API_C( void )
Q3TriMesh_SetNakedGeometry( TQ3GeometryObject _Nonnull inTriMesh,
							TQ3GeometryObject _Nonnull inNaked );

// Work around a HeaderDoc bug
/*!
	@functiongroup
*/



//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


