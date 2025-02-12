/*! @header QuesaExtension.h
        Declares the Quesa extension interface.
        
	@ignore	_Nullable
	@ignore _Nonnull
	@ignore	_Null_unspecified
 */
/*  NAME:
        QuesaExtension.h

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
#ifndef QUESA_EXTENSION_HDR
#define QUESA_EXTENSION_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"
#include "QuesaErrors.h"

// Disable QD3D header
#if defined(__QD3DEXTENSION__)
#error
#endif

#define __QD3DEXTENSION__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Mac file types
enum {
    kQ3XExtensionMacCreatorType                 = Q3_OBJECT_TYPE('Q', '3', 'X', 'T'),
    kQ3XExtensionMacFileType                    = Q3_OBJECT_TYPE('s', 'h', 'l', 'b')
};


/*!
 *	@enum	 	Class method types
 *	@abstract	Codes identifying methods of Quesa objects.
 *	@discussion	These values should be considered to be of type TQ3XMethodType.
 *
 *	@constant	kQ3XMethodTypeObjectClassVersion
 *						This is the type of a pseudo-method, really just a value of
 *						type TQ3XObjectClassVersion rather than a function pointer.
 *	@constant	kQ3XMethodTypeObjectClassRegister
 *						Currently unused by Quesa.
 *	@constant	kQ3XMethodTypeObjectClassReplace
 *						Currently unused by Quesa.
 *	@constant	kQ3XMethodTypeObjectClassUnregister
 *						Currently unused by Quesa.
 *	@constant	kQ3XMethodTypeObjectNew
 *						Method of type TQ3XObjectNewMethod that is called when
 *						an object is created.
 *	@constant	kQ3XMethodTypeObjectDelete
 *						Method of type TQ3XObjectDeleteMethod that is called
 *						when an object is deleted.
 *	@constant	kQ3XMethodTypeObjectDuplicate
 *						Method of type TQ3XObjectDuplicateMethod that is called
 *						when an object is duplicated.
 *	@constant	kQ3XMethodTypeViewSubmitRetainedRender
 *						Method of type TQ3XViewSubmitRetainedMethod belonging to a
 *						View object that is called when a retained object is
 *						submitted to the View for rendering.
 *	@constant	kQ3XMethodTypeViewSubmitRetainedPick
 *						Method of type TQ3XViewSubmitRetainedMethod belonging to a
 *						View object that is called when a retained object is
 *						submitted to the View for picking.
 *	@constant	kQ3XMethodTypeViewSubmitRetainedBound
 *						Method of type TQ3XViewSubmitRetainedMethod belonging to a
 *						View object that is called when a retained object is
 *						submitted to the View for bounding.
 *	@constant	kQ3XMethodTypeViewSubmitRetainedWrite
 *						Method of type TQ3XViewSubmitRetainedMethod belonging to a
 *						View object that is called when a retained object is
 *						submitted to the View for writing.
 *	@constant	kQ3XMethodTypeViewSubmitImmediateRender
 *						Method of type TQ3XViewSubmitImmediateMethod belonging to a
 *						View object that is called when an immediate object is
 *						submitted to the View for rendering.
 *	@constant	kQ3XMethodTypeViewSubmitImmediatePick
 *						Method of type TQ3XViewSubmitImmediateMethod belonging to a
 *						View object that is called when an immediate object is
 *						submitted to the View for picking.
 *	@constant	kQ3XMethodTypeViewSubmitImmediateBound
 *						Method of type TQ3XViewSubmitImmediateMethod belonging to a
 *						View object that is called when an immediate object is
 *						submitted to the View for bounding.
 *	@constant	kQ3XMethodTypeViewSubmitImmediateWrite
 *						Method of type TQ3XViewSubmitImmediateMethod belonging to a
 *						View object that is called when an immediate object is
 *						submitted to the View for writing.
*/
enum {
    kQ3XMethodTypeObjectClassVersion            = Q3_METHOD_TYPE('v', 'r', 's', 'n'),
    kQ3XMethodTypeObjectClassRegister           = Q3_METHOD_TYPE('r', 'g', 's', 't'),
    kQ3XMethodTypeObjectClassReplace            = Q3_METHOD_TYPE('r', 'g', 'r', 'p'),
    kQ3XMethodTypeObjectClassUnregister         = Q3_METHOD_TYPE('u', 'n', 'r', 'g'),
    kQ3XMethodTypeObjectNew                     = Q3_METHOD_TYPE('n', 'e', 'w', 'o'),
    kQ3XMethodTypeObjectDelete                  = Q3_METHOD_TYPE('d', 'l', 't', 'e'),
    kQ3XMethodTypeObjectDuplicate               = Q3_METHOD_TYPE('d', 'u', 'p', 'l'),
    kQ3XMethodTypeViewSubmitRetainedRender      = Q3_METHOD_TYPE('s', 'v', 'r', 'r'),
    kQ3XMethodTypeViewSubmitRetainedPick        = Q3_METHOD_TYPE('s', 'v', 'r', 'p'),
    kQ3XMethodTypeViewSubmitRetainedBound       = Q3_METHOD_TYPE('s', 'v', 'r', 'b'),
    kQ3XMethodTypeViewSubmitRetainedWrite       = Q3_METHOD_TYPE('s', 'v', 'r', 'w'),
    kQ3XMethodTypeViewSubmitImmediateRender     = Q3_METHOD_TYPE('s', 'v', 'i', 'r'),
    kQ3XMethodTypeViewSubmitImmediatePick       = Q3_METHOD_TYPE('s', 'v', 'i', 'p'),
    kQ3XMethodTypeViewSubmitImmediateBound      = Q3_METHOD_TYPE('s', 'v', 'i', 'b'),
    kQ3XMethodTypeViewSubmitImmediateWrite      = Q3_METHOD_TYPE('s', 'v', 'i', 'w')
};





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Version type
/*!
 *	@typedef
 *		TQ3XObjectClassVersion
 *	@abstract
 *		Pseudo-method that returns the version of a class.
*/
typedef TQ3Uns32 TQ3XObjectClassVersion;


// Class methods
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XObjectClassRegisterMethod)(
                            TQ3XObjectClass _Nonnull     objectClass,
                            void                * _Null_unspecified classPrivate);

typedef Q3_CALLBACK_API_C(void,                TQ3XObjectClassReplaceMethod)(
                            TQ3XObjectClass _Nonnull     oldObjectClass,
                            void                * _Null_unspecified oldClassPrivate,
                            TQ3XObjectClass _Nonnull     newObjectClass,
                            void                * _Null_unspecified newClassPrivate);

typedef Q3_CALLBACK_API_C(void,                TQ3XObjectClassUnregisterMethod)(
                            TQ3XObjectClass _Nonnull     objectClass,
                            void                * _Null_unspecified classPrivate);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XObjectNewMethod)(
                            TQ3Object _Nonnull          object,
                            void                * _Null_unspecified privateData,
                            const void          * _Null_unspecified parameters);

typedef Q3_CALLBACK_API_C(void,                TQ3XObjectDeleteMethod)(
                            TQ3Object _Nonnull           object,
                            void                * _Null_unspecified privateData);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XObjectDuplicateMethod)(
                            TQ3Object _Nonnull           fromObject,
                            const void          * _Null_unspecified fromPrivateData,
                            TQ3Object _Nonnull           toObject,
                            const void          * _Null_unspecified toPrivateData);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XSharedLibraryRegister)(
                            void);


/*!
 *	@typedef
 *		TQ3XViewSubmitRetainedMethod
 *	@abstract
 *		Method type that a view object uses when a retained object is submitted.
 *	@param	theView			A view object.
 *	@param	theObject		An object.
 *	@result                 Success or failure of the operation.
*/
typedef Q3_CALLBACK_API_C(TQ3Status,	TQ3XViewSubmitRetainedMethod)(
							TQ3ViewObject _Nonnull		theView,
							TQ3Object _Nonnull			theObject);

/*!
 *	@typedef
 *		TQ3XViewSubmitImmediateMethod
 *	@abstract
 *		Method type that a view object uses when an immediate object is submitted.
 *	@param	theView			A view object.
 *	@param	objectType		An object type.
 *	@param	objectData		The data of the object.
 *	@result                 Success or failure of the operation.
*/
typedef Q3_CALLBACK_API_C(TQ3Status,	TQ3XViewSubmitImmediateMethod)(
							TQ3ViewObject _Nonnull		theView,
							TQ3ObjectType		objectType,
							const void			* _Nullable objectData);


/*!
 *  @typedef
 *      TQ3XSharedLibraryInfo
 *  @discussion
 *      Parameter data for Q3XSharedLibrary_Register.
 *
 *      The sharedLibrary field is often used to hold a platform-specific
 *      shared library reference. It has no meaning for Quesa, and is used
 *      as an identifier to match up calls to Q3XSharedLibrary_Unregister
 *      with the appropriate call to Q3XSharedLibrary_Register.
 *
 *  @field registerFunction    Shared library registration method.
 *  @field sharedLibrary       Platform-specific shared library reference.
 */
typedef struct TQ3XSharedLibraryInfo {
    TQ3XSharedLibraryRegister _Nonnull       registerFunction;
    TQ3Uns32                        sharedLibrary;
} TQ3XSharedLibraryInfo;





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Version packing/unpacking
#define Q3_OBJECT_CLASS_VERSION(_major, _minor)         \
        (TQ3Uns32) (((_major) << 16) | (_minor))

#define Q3_OBJECT_GET_MAJOR_VERSION(_version)           \
        (TQ3Uns32) ((_version) >> 16)

#define Q3_OBJECT_GET_MINOR_VERSION(_version)           \
        (TQ3Uns32) ((_version) & 0x0000FFFF)





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3XObjectHierarchy_RegisterClass
 *  @discussion
 *      Register a new object class in the class tree.
 *
 *      Quesa does not support the virtualMetaHandler or methodsSize parameters,
 *      and these should be set to nullptr and 0 respectively.
 *
 *      Virtual methods are inherited from the parent class metahandler in the
 *      Quesa class tree, and the QD3D docs are unclear on the purpose of
 *      methodsSize.
 *
 *  @param parentType            The object type of the parent the new class should
 *                               be derived from. To derive new classes from the
 *                               root TQ3Object class, pass in kQ3ObjectTypeInvalid.
 *  @param objectType            Receives the new object type for the new class.
 *                               This value is used as the class identifier in binary
 *                               3DMF files, and is returned by the _GetType and
 *                               Q3Object_GetLeafType class. It may also be passed to
 *                               Q3Object_IsType to identify objects of this class.
 *  @param objectName            A C string which acts as a unique textual identifier
 *                               for the new class. This value is used as the class
 *                               identifier in text 3DMF files.
 *  @param metaHandler           The metahandler for the class.
 *  @param virtualMetaHandler    The virtual metahandler for the class (should be nullptr).
 *  @param methodsSize           The size of the private class data (shoudld be 0).
 *  @param instanceSize          The size of the instance data for the class, if any.
 *  @result                      The new class reference.
 */
Q3_EXTERN_API_C ( TQ3XObjectClass _Nonnull  )
Q3XObjectHierarchy_RegisterClass (
    TQ3ObjectType                 parentType,
    TQ3ObjectType * _Nonnull      objectType,
    const char * _Nonnull         objectName,
    TQ3XMetaHandler _Nonnull      metaHandler,
    TQ3XMetaHandler _Nullable     virtualMetaHandler,
    TQ3Uns32                      methodsSize,
    TQ3Uns32                      instanceSize
);



/*!
 *  @function
 *      Q3XObjectHierarchy_UnregisterClass
 *  @discussion
 *      Unregister a object class from the class tree.
 *
 *      Only classes registered by Q3XObjectHierarchy_RegisterClass should
 *      be unregistered by this routine.
 *
 *  @param objectClass      The class to unregister.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XObjectHierarchy_UnregisterClass (
    TQ3XObjectClass _Nonnull               objectClass
);



/*!
 *  @function
 *      Q3XObjectClass_GetMethod
 *  @discussion
 *      Get a method of a class.
 *
 *  @param objectClass      The class to query.
 *  @param methodType       The method type to retrieve.
 *  @result                 The function pointer for the method.
 */
Q3_EXTERN_API_C ( TQ3XFunctionPointer _Nullable )
Q3XObjectClass_GetMethod (
    TQ3XObjectClass _Nonnull               objectClass,
    TQ3XMethodType                methodType
);



/*!
 *  @function
 *      Q3XObjectHierarchy_NewObject
 *  @discussion
 *      Instantiate a class.
 *
 *  @param objectClass      The class to instantiate.
 *  @param parameters       The parameter data for the NewMethod method.
 *  @result                 A new instance of the class.
 */
Q3_EXTERN_API_C ( TQ3Object _Nonnull  )
Q3XObjectHierarchy_NewObject (
    TQ3XObjectClass _Nonnull               objectClass,
    void * _Null_unspecified               parameters
);



/*!
 *  @function
 *      Q3XObjectClass_GetLeafType
 *  @discussion
 *      Get the leaf type of a class.
 *
 *      The leaf type is the type returned by Q3XObjectHierarchy_RegisterClass
 *      when the class was registered.
 *
 *  @param objectClass      The class to query.
 *  @result                 The leaf type of the class.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3XObjectClass_GetLeafType (
    TQ3XObjectClass _Nonnull               objectClass
);



/*!
 *  @function
 *      Q3XObjectHierarchy_GetClassVersion
 *  @discussion
 *      Get the version of a class.
 *
 *      The class version is returned by the class ClassVersion method.
 *
 *  @param objectClassType  The class to query.
 *  @param version          Receives the version of the class.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XObjectHierarchy_GetClassVersion (
    TQ3ObjectType                 objectClassType,
    TQ3XObjectClassVersion * _Nonnull version
);



/*!
 *  @function
 *      Q3XObjectClass_GetType
 *  @discussion
 *      Get the type of a class.
 *
 *  @param objectClass      The class to query.
 *  @param theType          Receives the type of the class.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XObjectClass_GetType (
    TQ3XObjectClass _Nonnull               objectClass,
    TQ3ObjectType * _Nonnull               theType
);



/*!
 *  @function
 *      Q3XObjectHierarchy_FindClassByType
 *  @discussion
 *      Locate a class within the class tree.
 *
 *  @param theType          The class type.
 *  @result                 The class registered under theType, or nullptr.
 */
Q3_EXTERN_API_C ( TQ3XObjectClass _Nullable  )
Q3XObjectHierarchy_FindClassByType (
    TQ3ObjectType                 theType
);



/*!
 *  @function
 *      Q3XObjectClass_GetPrivate
 *  @discussion
 *      Get the private instance data of an object.
 *
 *      If no instance data space was reserved for the object, a nullptr
 *      pointer will be returned.
 *
 *  @param objectClass      The class of the object.
 *  @param targetObject     The object to query.
 *  @result                 A pointer to the private instance data.
 */
Q3_EXTERN_API_C ( void * _Nullable )
Q3XObjectClass_GetPrivate (
    TQ3XObjectClass _Nonnull               objectClass,
    TQ3Object _Nonnull                     targetObject
);



/*!
 *  @function
 *      Q3XObject_GetClass
 *  @discussion
 *      Get the class of an object.
 *
 *  @param object           The object to query.
 *  @result                 The class of the object.
 */
Q3_EXTERN_API_C ( TQ3XObjectClass _Nonnull  )
Q3XObject_GetClass (
    TQ3Object _Nonnull                     object
);



/*!
 *  @function
 *      Q3XSharedLibrary_Register
 *  @discussion
 *      Register a shared library with Quesa.
 *
 *      Can be called from within a shared library entry point to register
 *      a callback with Quesa. The callback will be invoked by Q3Initialize
 *      during initialisation, to allow it to register custom classes with
 *      the Quesa class tree.
 *
 *  @param sharedLibraryInfo    The shared library registration state.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XSharedLibrary_Register (
    TQ3XSharedLibraryInfo         * _Nonnull sharedLibraryInfo
);



/*!
 *  @function
 *      Q3XSharedLibrary_Unregister
 *  @discussion
 *      Unregister a shared library with Quesa.
 *
 *      The sharedLibrary parameter should match the sharedLibrary field
 *      within the TQ3XSharedLibraryInfo that was passed to
 *      Q3XSharedLibrary_Register.
 *
 *  @param sharedLibrary    The shared library reference.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XSharedLibrary_Unregister (
    TQ3Uns32                      sharedLibrary
);



/*!
 *  @function
 *      Q3XError_Post
 *  @discussion
 *      Post an error to the error manager.
 *
 *      Although intended only for use by extensions in QD3D, this call
 *      is also available to general application clients in Quesa.
 *
 *  @param error            The error to post.
 */
Q3_EXTERN_API_C ( void  )
Q3XError_Post (
    TQ3Error                      error
);



/*!
 *  @function
 *      Q3XWarning_Post
 *  @discussion
 *      Post a warning to the error manager.
 *
 *      Although intended only for use by extensions in QD3D, this call
 *      is also available to general application clients in Quesa.
 *
 *  @param warning          The warning to post.
 */
Q3_EXTERN_API_C ( void  )
Q3XWarning_Post (
    TQ3Warning                    warning
);



/*!
 *  @function
 *      Q3XNotice_Post
 *  @discussion
 *      Post a notice to the error manager.
 *
 *      Although intended only for use by extensions in QD3D, this call
 *      is also available to general application clients in Quesa.
 *
 *  @param notice           The notice to post.
 */
Q3_EXTERN_API_C ( void  )
Q3XNotice_Post (
    TQ3Notice                     notice
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


