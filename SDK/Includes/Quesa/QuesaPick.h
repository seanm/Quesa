/*! @header QuesaPick.h
        Declares the Quesa pick objects.
          
	@ignore	_Nullable
	@ignore _Nonnull
	@ignore	_Null_unspecified
*/
/*  NAME:
        QuesaPick.h

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
#ifndef QUESA_PICK_HDR
#define QUESA_PICK_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QuesaStyle.h"
#include "QuesaGeometry.h"

// Disable QD3D header
#if defined(__QD3DPICK__)
#error
#endif

#define __QD3DPICK__





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
 *  @enum
 *      TQ3PickDetailMasks
 *  @discussion
 *      Bitfield indicating the type of data to be returned by picks.
 *
 *  @constant kQ3PickDetailNone                      No pick details are required.
 *  @constant kQ3PickDetailMaskPickID                The picking ID of the picked object.
 *													 Data type: TQ3Uns32.
 *  @constant kQ3PickDetailMaskPath                  The path through the model hierarchy to the picked object.
 *													 Data type: TQ3HitPath.
 *  @constant kQ3PickDetailMaskObject                The picked object.
 *													 Data type: TQ3SharedObject (a new reference).
 *  @constant kQ3PickDetailMaskLocalToWorldMatrix    The local-to-world matrix for the picked object.
 *													 Data type: TQ3Matrix4x4.
 *  @constant kQ3PickDetailMaskXYZ                   The picked location in world coordinates.
 *													 Data type: TQ3Point3D.
 *  @constant kQ3PickDetailMaskDistance              The distance between the picked location and the pick origin.
 *													 Data type: float.
 *  @constant kQ3PickDetailMaskNormal                The surface normal at the picked location.
 *													 Data type: TQ3Vector3D.
 *  @constant kQ3PickDetailMaskShapePart             The shape part of the picked object.
 *													 Only relevant to Mesh objects.
 *													 Data type: TQ3ShapePartObject (a new reference).
 *  @constant kQ3PickDetailMaskPickPart              The picked object, edge, or vertex.
 *													 Only relevant to Mesh objects.
 *													 Data type: TQ3PickParts.
 *  @constant kQ3PickDetailMaskUV                    The UV coordinate of the picked location.
 *													 Data type: TQ3Param2D.
 *	@constant kQ3PickDetailMaskTriMeshFace			 The The 0-based index of the TriMesh face that
 *													 was hit.  Not in QuickDraw 3D.
 *													 Data type: TQ3Uns32.
 *	@constant kQ3PickDetailMaskBarycentric			 The barycentric coordinates of a picked point within
 *													 a triangle.  Not available for points, lines etc.
 *													 Not in QuickDraw 3D.
 *													 Data type: TQ3Param3D.
 */
typedef enum TQ3PickDetailMasks {
    kQ3PickDetailNone                           = 0,
    kQ3PickDetailMaskPickID                     = (1 << 0),
    kQ3PickDetailMaskPath                       = (1 << 1),
    kQ3PickDetailMaskObject                     = (1 << 2),
    kQ3PickDetailMaskLocalToWorldMatrix         = (1 << 3),
    kQ3PickDetailMaskXYZ                        = (1 << 4),
    kQ3PickDetailMaskDistance                   = (1 << 5),
    kQ3PickDetailMaskNormal                     = (1 << 6),
    kQ3PickDetailMaskShapePart                  = (1 << 7),
    kQ3PickDetailMaskPickPart                   = (1 << 8),
    kQ3PickDetailMaskUV                         = (1 << 9),
    kQ3PickDetailMaskTriMeshFace                = (1 << 10),
    kQ3PickDetailMaskBarycentric                = (1 << 11),
    kQ3PickDetailSize32                         = 0x7FFFFFFF
} TQ3PickDetailMasks;


/*!
 *  @enum
 *      TQ3PickSort
 *  @discussion
 *      The order in which pick results should be sorted.
 *
 *  @constant kQ3PickSortNone           The pick hit list should not be sorted.
 *  @constant kQ3PickSortNearToFar      The pick hit list should be sorted from near to far.
 *  @constant kQ3PickSortFarToNear      The pick hit list should be sorted from far to near.
 */
typedef enum TQ3PickSort {
    kQ3PickSortNone                             = 0,
    kQ3PickSortNearToFar                        = 1,
    kQ3PickSortFarToNear                        = 2,
    kQ3PickSortSize32                           = 0x7FFFFFFF
} TQ3PickSort;


// Hit request
/*!
 *	@defined	kQ3ReturnAllHits
 *	@discussion
 *		This value can be placed in the <code>numHitsToReturn</code> field of the
 *		<code>TQ3PickData</code> structure to indicate that we want information
 *		about all hits.
*/
#define kQ3ReturnAllHits                        0





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Pick detail
/*!
 *	@typedef	TQ3PickDetail
 *	@discussion
 *		A 32-bit integer used as a set of flags.  See <code>TQ3PickDetailMasks</code>
 *		for enumerated constants that can be combined in this data type.
*/
typedef TQ3Uns32 TQ3PickDetail;


/*!
 *  @struct
 *      TQ3PickData
 *  @discussion
 *      Describes the common state for a pick object.
 *
 *  @field sort             The type of sorting, if any, to performed on the results.
 *  @field mask             The type of pick information to be returned.
 *  @field numHitsToReturn  The number of hits to return. Set to <code>kQ3ReturnAllHits</code>
 *                          to retrieve all hits.
 */
typedef struct TQ3PickData {
    TQ3PickSort                                 sort;
    TQ3PickDetail                               mask;
    TQ3Uns32                                    numHitsToReturn;
} TQ3PickData;


/*!
 *  @struct
 *      TQ3WindowPointPickData
 *  @discussion
 *      Describes the state for a window-point pick object.
 *
 *  @field data             The common state for the pick.
 *  @field point            The pick point in local window coordinates.  Note that on Win32,
 *							this means relative to the HWND which contains the view.
 *  @field vertexTolerance  The vertex tolerance.  Only relevant to picking Point objects.
 *  @field edgeTolerance    The edge tolerance.  Only relevant to picking one-dimensional
 *							objects such as Lines and PolyLines.
 */
typedef struct TQ3WindowPointPickData {
    TQ3PickData                                 data;
    TQ3Point2D                                  point;
    float                                       vertexTolerance;
    float                                       edgeTolerance;
} TQ3WindowPointPickData;


/*!
 *  @struct
 *      TQ3WindowRectPickData
 *  @discussion
 *      Describes the state for a window-rect pick object.
 *
 *  @field data             The common state for the pick.
 *  @field rect             The pick rect in local window coordinates.
 */
typedef struct TQ3WindowRectPickData {
    TQ3PickData                                 data;
    TQ3Area                                     rect;
} TQ3WindowRectPickData;


/*!
 *  @struct
 *      TQ3WorldRayPickData
 *  @discussion
 *      Describes the state for a world-ray pick object.
 *
 *  @field data             The common state for the pick.
 *  @field ray              The pick ray in world coordinates.  The direction
 *							must be normalized.
 *  @field vertexTolerance  The vertex tolerance.  Only relevant to picking Point objects.
 *  @field edgeTolerance    The edge tolerance.  Only relevant to picking one-dimensional
 *							objects such as Lines and PolyLines.
 */
typedef struct TQ3WorldRayPickData {
    TQ3PickData                                 data;
    TQ3Ray3D                                    ray;
    float                                       vertexTolerance;
    float                                       edgeTolerance;
} TQ3WorldRayPickData;


/*!
 *  @struct
 *      TQ3HitPath
 *  @discussion
 *      Hit path data.
 *
 *		Returned by Q3Pick_GetPickDetailData for the kQ3PickDetailMaskPath
 *      pick selector. Disposed of by Q3HitPath_EmptyData.
 *
 *      rootGroup holds the top level group which encloses the picked
 *      object. positions holds an array for each nested group within
 *      the submit sequence, indicating the position that was submitted
 *      at each level. This array contains depth values.
 *
 *      E.g., assuming 'object' is the picked object, then submitting:
 *
 *          group1(object)
 *
 *      would produce a rootGroup field of group 1, a depth field of 1,
 *      and positions[0] would contain the position of object within
 *      group1.
 *
 *      Submitting:
 *
 *          group1(group2(object))
 *
 *      would produce a rootGroup field of group1, a depth field of 2,
 *      positions[0] would hold the position of group2 within group1,
 *      and positions[1] would hold the position of object within group2.
 *
 *
 *      Note that the contents of the position array are only valid if
 *      the submitted groups are unchanged since they were submitted
 *      for picking.
 *
 *      If these groups have had items added or removed since they
 *      were submitted, the positions array will no longer be valid.
 *
 *  @field rootGroup        Top level group which was submitted.
 *  @field depth            Number of valid entries within positions.
 *  @field positions        Array of group positions leading to the picked object.
 */
typedef struct TQ3HitPath {
    TQ3GroupObject _Nullable                             rootGroup;
    TQ3Uns32                                   			 depth;
    TQ3GroupPosition _Nullable                           * _Nullable positions;
} TQ3HitPath;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

/*!
	@functiongroup	General Picking
*/


/*!
 *  @function
 *      Q3Pick_GetType
 *  @discussion
 *      Get the type of a pick object.
 *
 *      Returns kQ3ObjectTypeInvalid if the pick type is unknown.
 *
 *  @param pick             The pick to query.
 *  @result                 The type of the pick object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Pick_GetType (
    TQ3PickObject _Nonnull                pick
);



/*!
 *  @function
 *      Q3Pick_GetData
 *  @discussion
 *      Get the common state of a pick object.
 *
 *  @param pick             The pick object to query.
 *  @param data             Receives the common state of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetData (
    TQ3PickObject _Nonnull                pick,
    TQ3PickData                   * _Nonnull data
);



/*!
 *  @function
 *      Q3Pick_SetData
 *  @discussion
 *      Set the common state of a pick object.
 *
 *  @param pick             The pick object to update.
 *  @param data             The new common state for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_SetData (
    TQ3PickObject _Nonnull                pick,
    const TQ3PickData             * _Nonnull data
);



/*!
 *  @function
 *      Q3Pick_GetVertexTolerance
 *  @discussion
 *      Get the vertex tolerance of a pick object.
 *
 *  @param pick             The pick object to query.
 *  @param vertexTolerance  Receives the vertex tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetVertexTolerance (
    TQ3PickObject _Nonnull                pick,
    float                         * _Nonnull vertexTolerance
);



/*!
 *  @function
 *      Q3Pick_GetEdgeTolerance
 *  @discussion
 *      Get the edge tolerance of a pick object.
 *
 *  @param pick             The pick object to query.
 *  @param edgeTolerance    Receives the edge tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetEdgeTolerance (
    TQ3PickObject _Nonnull                pick,
    float                         * _Nonnull edgeTolerance
);



/*!
 *  @function
 *      Q3Pick_GetFaceTolerance
 *  @discussion
 *      Get the face tolerance of a pick object.
 *
 *  @param pick             The pick object to query.
 *  @param faceTolerance    Receives the face tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetFaceTolerance (
    TQ3PickObject _Nonnull                pick,
    float                         * _Nonnull faceTolerance
);



/*!
 *  @function
 *      Q3Pick_SetVertexTolerance
 *  @discussion
 *      Set the vertex tolerance of a pick object.
 *
 *		The distance between a picking ray and a line is measured in world space
 *		when using a world ray pick, and measured in 2D window space when using
 *		a window point pick.  Tolerances are ignored when using a window rect
 *		pick.
 *
 *  @param pick             The pick object to update.
 *  @param vertexTolerance  The new vertex tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_SetVertexTolerance (
    TQ3PickObject _Nonnull                pick,
    float                         vertexTolerance
);



/*!
 *  @function
 *      Q3Pick_SetEdgeTolerance
 *  @discussion
 *      Set the edge tolerance of a pick object.
 *
 *		The distance between a picking ray and a face is measured in world space
 *		when using a world ray pick, and measured in 2D window space when using
 *		a window point pick.  Tolerances are ignored when using a window rect
 *		pick.
 *
 *  @param pick             The pick object to update.
 *  @param edgeTolerance    The new edge tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_SetEdgeTolerance (
    TQ3PickObject _Nonnull                pick,
    float                         edgeTolerance
);



/*!
 *  @function
 *      Q3Pick_SetFaceTolerance
 *  @discussion
 *      Set the face tolerance of a pick object.
 *
 *		The distance between a picking ray and a face is measured in world space
 *		when using a world ray pick, and measured in 2D window space when using
 *		a window point pick.  Tolerances are ignored when using a window rect
 *		pick.
 *
 *  @param pick             The pick object to update.
 *  @param faceTolerance    The new face tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_SetFaceTolerance (
    TQ3PickObject _Nonnull                pick,
    float                         faceTolerance
);



/*!
 *  @function
 *      Q3Pick_GetNumHits
 *  @discussion
 *      Get the number of hits of a pick object.
 *
 *  @param pick             The pick object to query.
 *  @param numHits          Receives the number of hits of a pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetNumHits (
    TQ3PickObject _Nonnull                pick,
    TQ3Uns32                      * _Nonnull numHits
);



/*!
 *  @function
 *      Q3Pick_EmptyHitList
 *  @discussion
 *      Empty the hit list of a pick object.
 *
 *  @param pick             The pick object to update.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_EmptyHitList (
    TQ3PickObject _Nonnull                pick
);



/*!
 *  @function
 *      Q3Pick_GetPickDetailValidMask
 *  @discussion
 *      Get the available data mask for a pick result.
 *
 *      Pick results are indexed from 0, and the number of available
 *      pick results is returned by Q3Pick_GetNumHits.
 *
 *  @param pick                   The pick object to query.
 *  @param index                  The index of the hit to query.
 *  @param pickDetailValidMask    Receives the available data mask for the hit.
 *  @result                       Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetPickDetailValidMask (
    TQ3PickObject _Nonnull                pick,
    TQ3Uns32                      index,
    TQ3PickDetail                 * _Nonnull pickDetailValidMask
);



/*!
 *  @function
 *      Q3Pick_GetPickDetailData
 *  @discussion
 *      Get the data for a pick result.
 *
 *      When the kQ3PickDetailMaskPath data is requested, it must be disposed
 *      of by the caller using Q3HitPath_EmptyData.
 *
 *      Pick results are indexed from 0, and the number of available
 *      pick results is returned by Q3Pick_GetNumHits.
 *
 *  @param pick             The pick object to query.
 *  @param index            The index of the hit to query.
 *  @param pickDetailValue  The type of data requested from the hit.
 *  @param detailData       Receives the data from the hit.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetPickDetailData (
    TQ3PickObject _Nonnull                pick,
    TQ3Uns32                      index,
    TQ3PickDetail                 pickDetailValue,
    void                          * _Nonnull detailData
);



/*!
 *  @function
 *      Q3HitPath_EmptyData
 *  @discussion
 *      Release the memory allocated by a previous call to Q3Pick_GetPickDetailData.
 *
 *  @param hitPath          The hit-path data to release.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3HitPath_EmptyData (
    TQ3HitPath                    * _Nonnull hitPath
);


/*!
	@functiongroup	Window Point Picking
*/

/*!
 *  @function
 *      Q3WindowPointPick_New
 *  @discussion
 *      Create a new window-point pick object.
 *
 *  @param data             The data for the pick object.
 *  @result                 The new pick object.
 */
Q3_EXTERN_API_C ( TQ3PickObject _Nonnull )
Q3WindowPointPick_New (
    const TQ3WindowPointPickData  * _Nonnull data
);



/*!
 *  @function
 *      Q3WindowPointPick_GetPoint
 *  @discussion
 *      Get the pick point of a window-point pick object.
 *
 *  @param pick             The pick object to query.
 *  @param point            Receives the pick point of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_GetPoint (
    TQ3PickObject _Nonnull                pick,
    TQ3Point2D                    * _Nonnull point
);



/*!
 *  @function
 *      Q3WindowPointPick_SetPoint
 *  @discussion
 *      Set the pick point of a window-point pick object.
 *
 *  @param pick             The pick object to update.
 *  @param point            The new pick point for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_SetPoint (
    TQ3PickObject _Nonnull                pick,
    const TQ3Point2D              * _Nonnull point
);



/*!
 *  @function
 *      Q3WindowPointPick_GetData
 *  @discussion
 *      Get the data of a window-point pick object.
 *
 *  @param pick             The pick object to query.
 *  @param data             Receives the data of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_GetData (
    TQ3PickObject _Nonnull                pick,
    TQ3WindowPointPickData        * _Nonnull data
);



/*!
 *  @function
 *      Q3WindowPointPick_SetData
 *  @discussion
 *      Set the data for a window-point pick object.
 *
 *  @param pick             The pick object to update.
 *  @param data             The new data for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_SetData (
    TQ3PickObject _Nonnull                pick,
    const TQ3WindowPointPickData  * _Nonnull data
);


/*!
	@functiongroup	Window Rectangle Picking
*/

/*!
 *  @function
 *      Q3WindowRectPick_New
 *  @discussion
 *      Create a new window-rect pick object.
 *
 *  @param data             The data for the pick object.
 *  @result                 The new pick object.
 */
Q3_EXTERN_API_C ( TQ3PickObject _Nonnull )
Q3WindowRectPick_New (
    const TQ3WindowRectPickData   * _Nonnull data
);



/*!
 *  @function
 *      Q3WindowRectPick_GetRect
 *  @discussion
 *      Get the pick rect of a window-rect pick object.
 *
 *  @param pick             The pick object to query.
 *  @param rect             Receives the pick rect of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_GetRect (
    TQ3PickObject _Nonnull                pick,
    TQ3Area                       * _Nonnull rect
);



/*!
 *  @function
 *      Q3WindowRectPick_SetRect
 *  @discussion
 *      Set the pick rect of a window-rect pick object.
 *
 *  @param pick             The pick object to update.
 *  @param rect             The new pick rect of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_SetRect (
    TQ3PickObject _Nonnull                pick,
    const TQ3Area                 * _Nonnull rect
);



/*!
 *  @function
 *      Q3WindowRectPick_GetData
 *  @discussion
 *      Get the data of a window-rect pick object.
 *
 *  @param pick             The pick object to query.
 *  @param data             Receives the data of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_GetData (
    TQ3PickObject _Nonnull                pick,
    TQ3WindowRectPickData         * _Nonnull data
);



/*!
 *  @function
 *      Q3WindowRectPick_SetData
 *  @discussion
 *      Set the data of a window-rect pick object.
 *
 *  @param pick             The pick object to update.
 *  @param data             The new data for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_SetData (
    TQ3PickObject _Nonnull                pick,
    const TQ3WindowRectPickData   * _Nonnull data
);


/*!
	@functiongroup	World Ray Picking
*/

/*!
 *  @function
 *      Q3WorldRayPick_New
 *  @discussion
 *      Create a new world-ray pick object.
 *
 *  @param data             The data for the pick object.
 *  @result                 The new pick object.
 */
Q3_EXTERN_API_C ( TQ3PickObject _Nonnull )
Q3WorldRayPick_New (
    const TQ3WorldRayPickData     * _Nonnull data
);



/*!
 *  @function
 *      Q3WorldRayPick_GetRay
 *  @discussion
 *      Get the pick ray of a world-ray pick object.
 *
 *  @param pick             The pick object to query.
 *  @param ray              Receives the ray of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_GetRay (
    TQ3PickObject _Nonnull                pick,
    TQ3Ray3D                      * _Nonnull ray
);



/*!
 *  @function
 *      Q3WorldRayPick_SetRay
 *  @discussion
 *      Set the pick ray of a world-ray pick object.
 *
 *  @param pick             The pick object to update.
 *  @param ray              The new ray for the pick object.  The direction must
 *							be normalized.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_SetRay (
    TQ3PickObject _Nonnull                pick,
    const TQ3Ray3D                * _Nonnull ray
);



/*!
 *  @function
 *      Q3WorldRayPick_GetData
 *  @discussion
 *      Get the data of a world-ray pick object.
 *
 *  @param pick             The pick object to query.
 *  @param data             Receives the data for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_GetData (
    TQ3PickObject _Nonnull                pick,
    TQ3WorldRayPickData           * _Nonnull data
);



/*!
 *  @function
 *      Q3WorldRayPick_SetData
 *  @discussion
 *      Set the data of a world-ray pick object.
 *
 *  @param pick             The pick object to update.
 *  @param data             The new data for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_SetData (
    TQ3PickObject _Nonnull                pick,
    const TQ3WorldRayPickData     * _Nonnull data
);

/*!
	@functiongroup	Object Parts
*/


/*!
 *  @function
 *      Q3ShapePart_GetType
 *  @discussion
 *      Get the type of a shape part object.
 *
 *  @param shapePartObject  The shape part to query.
 *  @result                 The type of the shape part.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3ShapePart_GetType (
    TQ3ShapePartObject _Nonnull           shapePartObject
);



/*!
 *  @function
 *      Q3MeshPart_GetType
 *  @discussion
 *      Get the type of a mesh part object.
 *
 *      Returns kQ3MeshPartTypeMeshFacePart, kQ3MeshPartTypeMeshEdgePart,
 *      or kQ3MeshPartTypeMeshVertexPart. If the type can not be determined,
 *      returns kQ3ObjectTypeInvalid.
 *
 *  @param meshPartObject   The mesh part to query.
 *  @result                 The type of the mesh part.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3MeshPart_GetType (
    TQ3MeshPartObject _Nonnull            meshPartObject
);



/*!
 *  @function
 *      Q3ShapePart_GetShape
 *  @discussion
 *      Get the shape object that contains a shape part object.
 *
 *  @param shapePartObject  The shape part to query.
 *  @param shapeObject      Receives the shape object that contains shapePartObject.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ShapePart_GetShape (
    TQ3ShapePartObject _Nonnull           shapePartObject,
    TQ3ShapeObject _Nonnull               * _Nonnull shapeObject
);



/*!
 *  @function
 *      Q3MeshPart_GetComponent
 *  @discussion
 *      Get the mesh component that contains a mesh part object.
 *
 *  @param meshPartObject   The mesh part to query.
 *  @param component        Receives the mesh component that contains meshPartObject.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MeshPart_GetComponent (
    TQ3MeshPartObject _Nonnull            meshPartObject,
    TQ3MeshComponent _Nonnull             * _Nonnull component
);



/*!
 *  @function
 *      Q3MeshFacePart_GetFace
 *  @discussion
 *      Get the mesh face that corresponds to a mesh face part.
 *
 *  @param meshFacePartObject    The mesh face to query.
 *  @param face                  Receives the mesh face that corresponds to meshFacePartObject.
 *  @result                      Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MeshFacePart_GetFace (
    TQ3MeshFacePartObject _Nonnull        meshFacePartObject,
    TQ3MeshFace _Nonnull                  * _Nonnull face
);



/*!
 *  @function
 *      Q3MeshEdgePart_GetEdge
 *  @discussion
 *      Get the mesh edge that corresponds to a mesh edge part.
 *
 *  @param meshEdgePartObject    The mesh edge to query.
 *  @param edge                  Receives the mesh edge that corresponds to meshEdgePartObject.
 *  @result                      Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MeshEdgePart_GetEdge (
    TQ3MeshEdgePartObject _Nonnull        meshEdgePartObject,
    TQ3MeshEdge _Nonnull                  * _Nonnull edge
);



/*!
 *  @function
 *      Q3MeshVertexPart_GetVertex
 *  @discussion
 *      Get the mesh vertex that corresponds to a mesh vertex part.
 *
 *  @param meshVertexPartObject    The mesh vertex to query.
 *  @param vertex                  Receives the mesh vertex that corresponds to meshVertedPartObject.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MeshVertexPart_GetVertex (
    TQ3MeshVertexPartObject _Nonnull      meshVertexPartObject,
    TQ3MeshVertex _Nonnull                * _Nonnull vertex
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


