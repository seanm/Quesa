/*! @header QuesaCamera.h
        Declares the Quesa camera objects.
        
	@ignore	_Nullable
	@ignore _Nonnull
	@ignore	_Null_unspecified
 */
/*  NAME:
        QuesaCamera.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

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
#ifndef QUESA_CAMERA_HDR
#define QUESA_CAMERA_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DCAMERA__)
#error
#endif

#define __QD3DCAMERA__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------

/*!
	@enum		TQ3FisheyeCroppingFormat
	@abstract	Type of cropping for a fisheye camera
	@constant	kQ3FisheyeCroppingFormatCircular
						The fisheye rendering fills a circular area that fits tightly
						in the smaller view dimension and is centered in the larger
						view dimension.
	@constant	kQ3FisheyeCroppingFormatCroppedCircle
						The circular area of fisheye rendering is tight against the
						left and right edges of the view, and is cropped on the top
						and bottom.
	@constant	kQ3FisheyeCroppingFormatFullFrame
						The fisheye rendering is cropped so that the view rectangle
						is totally filled.
*/
typedef enum TQ3FisheyeCroppingFormat
{
	kQ3FisheyeCroppingFormatCircular = 0,
	kQ3FisheyeCroppingFormatCroppedCircle,
	kQ3FisheyeCroppingFormatFullFrame,
	kQ3FisheyeCroppingFormatSize32 = 0x7FFFFFFF
} TQ3FisheyeCroppingFormat;

/*!
	@enum		TQ3FisheyeMappingFunction
	@abstract	Type of fisheye mapping function.  See
				<a href="https://en.wikipedia.org/wiki/Fisheye_lens#Mapping_function">
				Wikipedia on Fisheye Lenses</a>.
	@discussion	The mapping function relates the angle of an incoming light ray to the distance r
				from the center of the sensor to the image of the ray, taking into account the
				focal length f.
	@constant	kQ3FisheyeMappingFunctionOrthographic		r = f sin( angle ) <br/>
						Since this function is increasing only up to an angle of pi/2, this kind of fisheye lens
						cannot have an angle of view greater than pi (180 degrees).
	@constant	kQ3FisheyeMappingFunctionStereographic		r = 2 * f * tan( angle/2 )
	@constant	kQ3FisheyeMappingFunctionEquidistant		r = f * angle
	@constant	kQ3FisheyeMappingFunctionEquisolidAngle		r = 2 * f * sin( angle/2 )
*/
typedef enum TQ3FisheyeMappingFunction
{
	kQ3FisheyeMappingFunctionOrthographic = 0,
	kQ3FisheyeMappingFunctionStereographic,
	kQ3FisheyeMappingFunctionEquidistant,
	kQ3FisheyeMappingFunctionEquisolidAngle,
	kQ3FisheyeMappingFunctionSize32 = 0x7FFFFFFF
} TQ3FisheyeMappingFunction;

/*!
 *  @struct
 *      TQ3CameraPlacement
 *  @discussion
 *      Describes the location and orientation of a camera. All points are
 *      in world-space coordinates.

 *  @field cameraLocation   The location of the camera.
 *  @field pointOfInterest  The point at which the camera is aimed.
 *  @field upVector         The up vector for the camera. This vector must be
 *                          normalised and perpendicular to the viewing direction
 *                          of the camera. This vector is transformed to the y
 *                          axis of the viewing plane.
 */
typedef struct TQ3CameraPlacement {
    TQ3Point3D                                  cameraLocation;
    TQ3Point3D                                  pointOfInterest;
    TQ3Vector3D                                 upVector;
} TQ3CameraPlacement;


/*!
 *  @struct
 *      TQ3CameraRange
 *  @abstract
 *      Describes the hither and yon clipping planes of a camera.
 *  @discussion
 *		Objects closer than the hither distance or farther than the yon distance
 *		will be clipped, i.e., invisible, so you usually want hither to be closer
 *		than any object and yon to be farther than any object.
 *
 *		The hither and yon values, together with the bit depth of your depth buffer,
 *		determine how small a difference in depths can be resolved.  It is more
 *		important to make hither as large as possible than to make yon as small
 *		as possible.

 *  @field hither           The distance from the camera to the near clipping plane.
 *                          This value must always be greater than 0.
 *  @field yon              The distance from the camera to the far clipping plane.
 *                          This value must always be greater than the hither field,
 *                          and is allowed to be INFINITY.
 */
typedef struct TQ3CameraRange {
    float                                       hither;
    float                                       yon;
} TQ3CameraRange;


/*!
 *  @struct
 *      TQ3CameraViewPort
 *  @discussion
 *      Describes the viewport for a camera.
 *
 *      The camera viewport is the rectangular area of the view plane which is
 *      mapped to the rendered area of the current draw context. The default
 *      mapping is a square of size 2.0x2.0, with the top left corner anchored
 *      at {-1.0, 1.0}.
 *
 *      By adjusting the viewport, it is possible to control which area of the
 *      camera's view is rendered (e.g., to divide an image into a series of
 *      horizontal strips for printing).
 *
 *  @field origin           The origin for the viewport.
 *  @field width            The width of the viewport.
 *  @field height           The width of the viewport.
 */
typedef struct TQ3CameraViewPort {
    TQ3Point2D                                  origin;
    float                                       width;
    float                                       height;
} TQ3CameraViewPort;


/*!
 *  @struct
 *      TQ3CameraData
 *  @discussion
 *      Describes the common state for a camera.
 *
 *      The common camera state includes its position and orientation within
 *      the world (placement), the near and far clipping planes (range), and
 *      the current viewport.
 *      
 *		The placement fully determines the world to view transformation, while the
 *		range and viewport affect the view to frustum transformation.
 *
 *  @field placement        The position and orientation of the camera.
 *  @field range            The near and far clipping planes of the camera.
 *  @field viewPort         The viewport for the camera.
 */
typedef struct TQ3CameraData {
    TQ3CameraPlacement                          placement;
    TQ3CameraRange                              range;
    TQ3CameraViewPort                           viewPort;
} TQ3CameraData;


/*!
 *  @struct
 *      TQ3OrthographicCameraData
 *  @discussion
 *      Describes the state for an orthographic camera.
 *
 *      An orthographic camera is defined by four view planes, which form a
 *      box aligned with the camera view direction. These planes are defined
 *      by distances relative to the coordinate system formed by the camera
 *      location and its view direction.
 *
 *  @field cameraData       The common state for the camera.
 *  @field left             The left side of the view volume.
 *  @field top              The top side of the view volume.
 *  @field right            The right side of the view volume.
 *  @field bottom           The bottom side of the view volume.
 */
typedef struct TQ3OrthographicCameraData {
    TQ3CameraData                               cameraData;
    float                                       left;
    float                                       top;
    float                                       right;
    float                                       bottom;
} TQ3OrthographicCameraData;


/*!
 *  @struct
 *      TQ3ViewPlaneCameraData
 *  @discussion
 *      Describes the state for a view plane camera.
 *
 *      A view plane camera is a perspective camera which allows the specification
 *      of an off-center viewing frustum.
 *
 *      The frustum is formed by following the camera view direction for a given
 *      distance, then taking the specified rectangle on that plane. The frustum
 *      extends from the camera position through the four edges of this rectangle.
 *
 *  @field cameraData                The common state for the camera.
 *  @field viewPlane                 The distance from the camera to the view plane.
 *  @field halfWidthAtViewPlane      The half-width of the rectangle on the view plane.
 *  @field halfHeightAtViewPlane     The half-height of the rectangle on the view plane.
 *  @field centerXOnViewPlane        The x-center of the rectangle on the view plane.
 *  @field centerYOnViewPlane        The y-center of the rectangle on the view plane.
 */
typedef struct TQ3ViewPlaneCameraData {
    TQ3CameraData                               cameraData;
    float                                       viewPlane;
    float                                       halfWidthAtViewPlane;
    float                                       halfHeightAtViewPlane;
    float                                       centerXOnViewPlane;
    float                                       centerYOnViewPlane;
} TQ3ViewPlaneCameraData;


/*!
 *  @struct
 *      TQ3ViewAngleAspectCameraData
 *  @discussion
 *      Describes the state for a traditional perspective camera.
 *
 *      A view angle aspect camera is a perspective camera defined by a field of
 *      view angle and an aspect ratio.
 *
 *		The field of view angle must be a positive angle in radians.  If the
 *		aspect ratio is greater than 1.0, the field of view represents the
 *		vertical range of visibility, and if the aspect ratio is less than 1.0,
 *		the field of view is horizontal.  In other words, the field of view is
 *		the smaller of the two angles.
 *
 *  @field cameraData       The common state for the camera.
 *  @field fov              The field of view of the camera, in radians.
 *  @field aspectRatioXToY  The horizontal-to-vertical aspect ratio of the camera.
 */
typedef struct TQ3ViewAngleAspectCameraData {
    TQ3CameraData                               cameraData;
    float                                       fov;
    float                                       aspectRatioXToY;
} TQ3ViewAngleAspectCameraData;



/*!
	@struct			TQ3FisheyeCameraData
	@abstract		State for a fisheye camera.
	@field			cameraData		The common state for the camera.
	@field			sensorSize			The dimensions of the sensor or film in mm.
									The aspect ratio should be the same as the
									view.
	@field			focalLength		The focal length of the lens in mm.
	@field			mappingFunction	The mapping function.
	@field			croppingFormat		The cropping format.
*/
typedef struct TQ3FisheyeCameraData
{
    TQ3CameraData                  cameraData;
	TQ3Vector2D                    sensorSize;
	float                          focalLength;
	TQ3FisheyeMappingFunction      mappingFunction;
	TQ3FisheyeCroppingFormat       croppingFormat;
} TQ3FisheyeCameraData;




//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Camera_GetType
 *  @discussion
 *      Get the type of a camera.
 *
 *      Returns kQ3CameraTypeOrthographic, kQ3CameraTypeViewAngleAspect, or
 *      kQ3CameraTypeViewPlane. Returns kQ3ObjectTypeInvalid if the camera type
 *      is unknown.
 *
 *  @param camera           The camera to query.
 *  @result                 The type of the camera object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Camera_GetType (
    TQ3CameraObject _Nonnull               camera
);



/*!
 *  @function
 *      Q3Camera_SetData
 *  @discussion
 *      Set the common state for a camera.
 *
 *  @param camera           The camera to update.
 *  @param cameraData       The new common state for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_SetData (
    TQ3CameraObject _Nonnull             camera,
    const TQ3CameraData * _Nonnull		 cameraData
);



/*!
 *  @function
 *      Q3Camera_GetData
 *  @discussion
 *      Get the common state of a camera.
 *
 *  @param camera           The camera to query.
 *  @param cameraData       Receives the common state of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetData (
    TQ3CameraObject _Nonnull             camera,
    TQ3CameraData * _Nonnull			 cameraData
);



/*!
 *  @function
 *      Q3Camera_SetPlacement
 *  @discussion
 *      Set the placement for a camera.
 *
 *  @param camera           The camera to update.
 *  @param placement        The new placement for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_SetPlacement (
    TQ3CameraObject _Nonnull               camera,
    const TQ3CameraPlacement * _Nonnull    placement
);



/*!
 *  @function
 *      Q3Camera_GetPlacement
 *  @discussion
 *      Get the placement of a camera.
 *
 *  @param camera           The camera to query.
 *  @param placement        Receives the placement of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetPlacement (
    TQ3CameraObject _Nonnull              camera,
    TQ3CameraPlacement * _Nonnull         placement
);



/*!
 *  @function
 *      Q3Camera_SetRange
 *  @discussion
 *      Set the range for a camera.
 *
 *  @param camera           The camera to update.
 *  @param range            The new range for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_SetRange (
    TQ3CameraObject _Nonnull               camera,
    const TQ3CameraRange * _Nonnull        range
);



/*!
 *  @function
 *      Q3Camera_GetRange
 *  @discussion
 *      Get the range of a camera.
 *
 *  @param camera           The camera to query.
 *  @param range            Receives the range of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetRange (
    TQ3CameraObject _Nonnull               camera,
    TQ3CameraRange * _Nonnull              range
);



/*!
 *  @function
 *      Q3Camera_SetViewPort
 *  @discussion
 *      Set the view port for a camera.
 *
 *  @param camera           The camera to update.
 *  @param viewPort         The new view port for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_SetViewPort (
    TQ3CameraObject _Nonnull               camera,
    const TQ3CameraViewPort * _Nonnull     viewPort
);



/*!
 *  @function
 *      Q3Camera_GetViewPort
 *  @discussion
 *      Get the view port of a camera.
 *
 *  @param camera           The camera to query.
 *  @param viewPort         Receives the view port of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetViewPort (
    TQ3CameraObject _Nonnull               camera,
    TQ3CameraViewPort * _Nonnull           viewPort
);



/*!
 *  @function
 *      Q3Camera_GetWorldToView
 *  @discussion
 *      Get the world-to-view matrix of a camera.
 *
 *      The world-to-view matrix transforms world coordinates to a coordinate
 *      system relative to the camera. The origin of this coordinate system is
 *      the camera location, with the camera view pointing down the -z axis
 *      and the camera up vector placed along the +y axis.
 *
 *  @param camera           The camera to query.
 *  @param worldToView      Receives the world-to-view matrix of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetWorldToView (
    TQ3CameraObject _Nonnull               camera,
    TQ3Matrix4x4 * _Nonnull                worldToView
);



/*!
 *  @function
 *      Q3Camera_GetWorldToFrustum
 *  @discussion
 *      Get the world-to-frustum matrix of a camera.
 *
 *      The world-to-frustum matrix transforms world coordinates to the viewing
 *      frustum coordinate system. It is equivalent to multiplying the matrices
 *      returned by Q3Camera_GetWorldToView and Q3Camera_GetViewToFrustum.
 *		See the documentation of Q3View_GetWorldToFrustumMatrixState for more
 *		information.
  *
 *      If the camera uses a nonlinear projection (fisheye or all-seeing cameras) then
 *      the view to frustum transformation cannot be expressed by a matrix, and this
 *      function returns kQ3Failure.
*
 *  @param camera           The camera to query.
 *  @param worldToFrustum   Receives the world-to-frustum matrix of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetWorldToFrustum (
    TQ3CameraObject _Nonnull               camera,
    TQ3Matrix4x4 * _Nonnull                worldToFrustum
);



/*!
 *  @function
 *      Q3Camera_GetViewToFrustum
 *  @discussion
 *      Get the view-to-frustum matrix of a camera.
 *
 *      The view-to-frustum matrix transforms the camera coordinate system
 *      (as returned by Q3Camera_GetWorldToView) to the viewing frustum
 *      coordinate system.
 *
 *      The frustum coordinate system ranges from 0.0 (near) to -1.0 (far) in z, and from
 *      -1.0 to +1.0 in both x and y.
 *
 *      If the camera uses a nonlinear projection (fisheye or all-seeing cameras) then
 *      the view to frustum transformation cannot be expressed by a matrix, and this
 *      function returns kQ3Failure.
 *
 *  @param camera           The camera to query.
 *  @param viewToFrustum    Receives the view-to-frustum matrix of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Camera_GetViewToFrustum (
    TQ3CameraObject _Nonnull               camera,
    TQ3Matrix4x4 * _Nonnull                viewToFrustum
);


/*!
	@function	Q3Camera_TransformViewToFrustum
	@abstract	Transform a point from view coordinates to frustum coordinates.
	@discussion	Whereas Q3Camera_GetViewToFrustum does not work for certain kinds
				of cameras, this function should work for all cameras.
	@param		camera		The camera.
	@param		viewPt		A point in view coordinates.
	@param		frustumPt	Receives a point in frustum coordinates.
*/
Q3_EXTERN_API_C( void )
Q3Camera_TransformViewToFrustum( TQ3CameraObject _Nonnull camera,
								const TQ3Point3D* _Nonnull viewPt,
								TQ3Point3D* _Nonnull frustumPt );


/*!
	@function	Q3Camera_TransformFrustumToView
	@abstract	Transform a point from frustum coordinates to view coordinates.
	@discussion	Whereas Q3Camera_GetViewToFrustum does not work for certain kinds
				of cameras, this function should work for all cameras.
	@param		camera		The camera.
	@param		frustumPt	A point in frustum coordinates.
	@param		viewPt		Receives a point in view coordinates.
*/
Q3_EXTERN_API_C( void )
Q3Camera_TransformFrustumToView( TQ3CameraObject _Nonnull camera,
								const TQ3Point3D* _Nonnull frustumPt,
								TQ3Point3D* _Nonnull viewPt );

/*!
 *  @function
 *      Q3Camera_IsBoundingBoxVisible
 *  @abstract
 *      Test a bounding box for visibility.
 *
 *  @discussion
 *      The bounding box (assumed to be in world coordinates) is tested for
 *		intersection with the view frustum of the camera.
 *
 *		This function is similar to Q3View_IsBoundingBoxVisible, but since it
 *		does not use local coordinates, it does not need to be used within a
 *		submitting loop.
 *
 *  @param camera           The view to check the bounding box against.
 *  @param bbox             The world bounding box to test.
 *  @result                 True or false as the bounding box is visible.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Camera_IsBoundingBoxVisible (
    TQ3CameraObject _Nonnull               camera,
    const TQ3BoundingBox * _Nonnull        bbox
);



/*!
	@functiongroup Orthographic
*/


/*!
 *  @function
 *      Q3OrthographicCamera_New
 *  @discussion
 *      Create a new orthographic camera object.
 *
 *  @param orthographicData The data for the camera object.
 *  @result                 The new camera object.
 */
Q3_EXTERN_API_C ( TQ3CameraObject _Nonnull  )
Q3OrthographicCamera_New (
    const TQ3OrthographicCameraData * _Nonnull orthographicData
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetData
 *  @discussion
 *      Get the data for an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param cameraData       Receives the data of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetData (
    TQ3CameraObject _Nonnull               camera,
    TQ3OrthographicCameraData * _Nonnull   cameraData
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetData
 *  @discussion
 *      Set the data for an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param cameraData       The new data for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetData (
    TQ3CameraObject _Nonnull                   camera,
    const TQ3OrthographicCameraData * _Nonnull cameraData
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetLeft
 *  @discussion
 *      Set the left side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param left             The new left side for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetLeft (
    TQ3CameraObject _Nonnull              camera,
    float                                 left
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetLeft
 *  @discussion
 *      Get the left side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param left             Receives the left side of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetLeft (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       left
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetTop
 *  @discussion
 *      Set the top side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param top              The new top side for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetTop (
    TQ3CameraObject _Nonnull               camera,
    float                                  top
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetTop
 *  @discussion
 *      Get the top side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param top              Receives the top side of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetTop (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       top
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetRight
 *  @discussion
 *      Set the right side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param right            The new right side for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetRight (
    TQ3CameraObject _Nonnull               camera,
    float                                  right
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetRight
 *  @discussion
 *      Get the right side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param right            Receives the right side of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetRight (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       right
);



/*!
 *  @function
 *      Q3OrthographicCamera_SetBottom
 *  @discussion
 *      Set the bottom side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to update.
 *  @param bottom           The new bottom side for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_SetBottom (
    TQ3CameraObject _Nonnull               camera,
    float                                  bottom
);



/*!
 *  @function
 *      Q3OrthographicCamera_GetBottom
 *  @discussion
 *      Get the bottom side of the viewing frustum of an orthographic camera.
 *
 *  @param camera           The camera to query.
 *  @param bottom           Receives the bottom side of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrthographicCamera_GetBottom (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       bottom
);


/*!
	@functiongroup View Plane
*/

/*!
 *  @function
 *      Q3ViewPlaneCamera_New
 *  @discussion
 *      Create a new view plane camera object.
 *
 *  @param cameraData       The data for the camera object.
 *  @result                 The new camera object.
 */
Q3_EXTERN_API_C ( TQ3CameraObject _Nonnull  )
Q3ViewPlaneCamera_New (
    const TQ3ViewPlaneCameraData  * _Nonnull cameraData
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetData
 *  @discussion
 *      Get the data for a view plane camera.
 *
 *  @param camera           The camera to query.
 *  @param cameraData       Receives the data of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetData (
    TQ3CameraObject _Nonnull               camera,
    TQ3ViewPlaneCameraData * _Nonnull      cameraData
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetData
 *  @discussion
 *      Set the data for a view plane camera.
 *
 *  @param camera           The camera to update.
 *  @param cameraData       The new data for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetData (
    TQ3CameraObject _Nonnull                camera,
    const TQ3ViewPlaneCameraData * _Nonnull cameraData
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetViewPlane
 *  @discussion
 *      Set the view plane distance for a view plane camera.
 *
 *      The view plane distance is the distance along the camera view vector
 *      from the camera location.
 *
 *  @param camera           The camera to update.
 *  @param viewPlane        The new view plane distance for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetViewPlane (
    TQ3CameraObject _Nonnull               camera,
    float                                  viewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetViewPlane
 *  @discussion
 *      Get the view plane distance of a view plane camera.
 *
 *      The view plane distance is the distance along the camera view vector
 *      from the camera location.
 *
 *  @param camera           The camera to query.
 *  @param viewPlane        Receives the view plane distance of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetViewPlane (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       viewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetHalfWidth
 *  @discussion
 *      Set the half-width of the view rectangle of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle,
 *      whose width is twice the specified half-width.
 *
 *  @param camera                  The camera to update.
 *  @param halfWidthAtViewPlane    The new half-width of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetHalfWidth (
    TQ3CameraObject _Nonnull               camera,
    float                                  halfWidthAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetHalfWidth
 *  @discussion
 *      Get the half-width of the view rectangle of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle,
 *      whose width is twice the returned half-width.
 *
 *  @param camera                  The camera to query.
 *  @param halfWidthAtViewPlane    Receives the half-width of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetHalfWidth (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       halfWidthAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetHalfHeight
 *  @discussion
 *      Set the half-height of the view rectangle of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle,
 *      whose height is twice the specified half-height.
 *
 *  @param camera                  The camera to update.
 *  @param halfHeightAtViewPlane   The new half-height of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetHalfHeight (
    TQ3CameraObject _Nonnull               camera,
    float                                  halfHeightAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetHalfHeight
 *  @discussion
 *      Get the half-height of the view rectangle of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle,
 *      whose height is twice the returned half-height.
 *
 *  @param camera                  The camera to query.
 *  @param halfHeightAtViewPlane   Receives the half-height of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetHalfHeight (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       halfHeightAtViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetCenterX
 *  @discussion
 *      Set the x coordinate of the view rectangle center of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle, whose
 *      origin in x is at the specified coordinate.
 *
 *  @param camera                  The camera to update.
 *  @param centerXOnViewPlane      The new x coordinate for the center of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetCenterX (
    TQ3CameraObject _Nonnull               camera,
    float                                  centerXOnViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetCenterX
 *  @discussion
 *      Get the x coordinate of the view rectangle center of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle, whose
 *      origin in x is at the returned coordinate.
 *
 *  @param camera                  The camera to query.
 *  @param centerXOnViewPlane      Receives the x coordinate of the center of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetCenterX (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       centerXOnViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_SetCenterY
 *  @discussion
 *      Set the y coordinate of the view rectangle center of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle, whose
 *      origin in y is at the specified coordinate.
 *
 *  @param camera                  The camera to update.
 *  @param centerYOnViewPlane      The new y coordinate for the center of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_SetCenterY (
    TQ3CameraObject _Nonnull               camera,
    float                                  centerYOnViewPlane
);



/*!
 *  @function
 *      Q3ViewPlaneCamera_GetCenterY
 *  @discussion
 *      Get the y coordinate of the view rectangle center of a view plane camera.
 *
 *      The area of the view plane which will be rendered is a rectangle, whose
 *      origin in y is at the returned coordinate.
 *
 *  @param camera                  The camera to query.
 *  @param centerYOnViewPlane      Receives the y coordinate of the center of the view rectangle.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewPlaneCamera_GetCenterY (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       centerYOnViewPlane
);


/*!
	@functiongroup All Seeing
*/


/*!
 *  @function
 *      Q3AllSeeingCamera_New
 *  @discussion
 *      Create a new all-seeing camera object.
 *
 *  	This kind of camera sees in all directions, using equirectangular coordinates.
 *		
 *		Rendering with this camera will experience some inaccuracy, due to the fact that
 *		OpenGL works by linearly interpolating across triangles, while the equirectangular
 *		projection is very much nonlinear.  Naturally, the greater the angular diameter of a
 *		triangle as seen from the camera, the greater the inaccuracy.  Subdividing a
 *		triangulation can reduce the nonlinearity artifacts.
 *		
 *		There will also be more artifacts around the "poles" of the map (much as one sees
 *		in cartography), which correspond to the up and down directions of the camera.
 *
 *  @param cameraData       The data for the camera object.
 *  @result                 The new camera object.
 */
Q3_EXTERN_API_C ( TQ3CameraObject _Nonnull  )
Q3AllSeeingCamera_New(
    const TQ3CameraData * _Nonnull cameraData
);

/*!
	@functiongroup Fisheye
*/

/*!
	@function	Q3FisheyeCamera_New
	@abstract	Create a fisheye camera.
	@discussion	Rendering with this camera will experience some inaccuracy, due to the fact that
				OpenGL works by linearly interpolating across triangles, while the fisheye
				projection is very much nonlinear.  Naturally, the greater the angular diameter of a
				triangle as seen from the camera, the greater the inaccuracy.  Subdividing a
				triangulation can reduce the nonlinearity artifacts.
	@param cameraData       The data for the camera object.
	@result		The new camera object.
*/
Q3_EXTERN_API_C ( TQ3CameraObject _Nonnull  )
Q3FisheyeCamera_New(
    const TQ3FisheyeCameraData * _Nonnull cameraData
);



/*!
	@function	Q3FisheyeCamera_GetData
	@abstract	Get the data for a fisheye camera.
	@param		camera		The camera.
	@param		data		Receives the camera data.
	@result		Success or failure of the operation.
*/
Q3_EXTERN_API_C ( TQ3Status  )
Q3FisheyeCamera_GetData( TQ3CameraObject _Nonnull camera,
						TQ3FisheyeCameraData* _Nonnull data );



/*!
	@function	Q3FisheyeCamera_SetData
	@abstract	Set the data for a fisheye camera.
	@param		camera		The camera.
	@param		cameraData	The new data.
	@result		Success or failure of the operation.
*/
Q3_EXTERN_API_C ( TQ3Status  )
Q3FisheyeCamera_SetData( TQ3CameraObject _Nonnull camera,
						const TQ3FisheyeCameraData * _Nonnull cameraData );


/*!
	@function	Q3FisheyeCamera_CalcAngleOfView
	@abstract	Compute the angle of view of a fisheye camera from other parameters.
	@discussion	In the full frame cropping format, the maximum angle of view is achieved only
				on the diagonals, whereas in the cropped circle format the maximum angle of view
				is achieved horizontally.
	@param			inSensorSize			The dimensions of the sensor or film in mm.
	@param			inMappingFunc			The mapping function.
	@param			inCropping				The cropping format.
	@param			inFocalLength			The focal length in mm.
	@result			The maximum angle of view in radians.
*/
Q3_EXTERN_API_C( float )
Q3FisheyeCamera_CalcAngleOfView( const TQ3Vector2D* _Nonnull inSensorSize,
								TQ3FisheyeMappingFunction inMappingFunc,
								TQ3FisheyeCroppingFormat inCropping,
								float inFocalLength );


/*!
	@function	Q3FisheyeCamera_CalcFocalLength
	@abstract	Compute the focal length of a fisheye camera from other parameters.
	@param			inSensorSize			The dimensions of the sensor or film in mm.
	@param			inMappingFunc			The mapping function.
	@param			inCropping				The cropping format.
	@param			inAngleOfView			The maximum angle of view in radians.
	@result			The focal length in mm.
*/
Q3_EXTERN_API_C( float )
Q3FisheyeCamera_CalcFocalLength( const TQ3Vector2D* _Nonnull inSensorSize,
								TQ3FisheyeMappingFunction inMappingFunc,
								TQ3FisheyeCroppingFormat inCropping,
								float inAngleOfView );


/*!
	@functiongroup View Angle
*/


/*!
 *  @function
 *      Q3ViewAngleAspectCamera_New
 *  @discussion
 *      Create a new view angle aspect camera object.
 *
 *  @param cameraData       The data for the camera object.
 *  @result                 The new camera object.
 */
Q3_EXTERN_API_C ( TQ3CameraObject _Nonnull  )
Q3ViewAngleAspectCamera_New (
    const TQ3ViewAngleAspectCameraData * _Nonnull cameraData
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_SetData
 *  @discussion
 *      Set the data for a view angle aspect camera.
 *
 *  @param camera           The camera to update.
 *  @param cameraData       The new data for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetData (
    TQ3CameraObject _Nonnull                      camera,
    const TQ3ViewAngleAspectCameraData * _Nonnull cameraData
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_GetData
 *  @discussion
 *      Get the data for a view angle aspect camera.
 *
 *  @param camera           The camera to query.
 *  @param cameraData       Receives the data of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetData (
    TQ3CameraObject _Nonnull                 camera,
    TQ3ViewAngleAspectCameraData  * _Nonnull cameraData
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_SetFOV
 *  @discussion
 *      Set the field of view for a view angle aspect camera.
 *
 *      The field of view is specified in radians.
 *
 *  @param camera           The camera to update.
 *  @param fov              The new field of view for the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetFOV (
    TQ3CameraObject _Nonnull               camera,
    float                                  fov
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_GetFOV
 *  @discussion
 *      Get the field of view of a view angle aspect camera.
 *
 *      The field of view is specified in radians.
 *
 *  @param camera           The camera to query.
 *  @param fov              Receives the field of view of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetFOV (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       fov
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_SetAspectRatio
 *  @discussion
 *      Set the aspect ratio for a view angle aspect camera.
 *
 *      If the aspect ratio is greater than 1.0, the field of view of the
 *      camera is vertical. If it is less than 1.0, the field of view is
 *      horizontal.
 *
 *  @param camera           The camera to update.
 *  @param aspectRatioXToY  The new horizontal-to-vertical aspect ratio of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_SetAspectRatio (
    TQ3CameraObject _Nonnull               camera,
    float                                  aspectRatioXToY
);



/*!
 *  @function
 *      Q3ViewAngleAspectCamera_GetAspectRatio
 *  @discussion
 *      Get the aspect ratio of a view angle aspect camera.
 *
 *      If the aspect ratio is greater than 1.0, the field of view of the
 *      camera is vertical. If it is less than 1.0, the field of view is
 *      horizontal.
 *
 *  @param camera           The camera to query.
 *  @param aspectRatioXToY  Receives the horizontal-to-vertical aspect ratio of the camera.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ViewAngleAspectCamera_GetAspectRatio (
    TQ3CameraObject _Nonnull               camera,
    float * _Nonnull                       aspectRatioXToY
);



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


