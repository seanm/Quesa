/*  NAME:
        E3Main.cpp

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include <new>

#include "E3Prefix.h"
#include "E3Version.h"
#include "E3View.h"
#include "E3Camera.h"
#include "E3Geometry.h"
#include "E3DrawContext.h"
#include "E3Renderer.h"
#include "E3Group.h"
#include "E3Set.h"
#include "E3Light.h"
#include "E3Style.h"
#include "E3String.h"
#include "E3Transform.h"
#include "E3Main.h"
#include "E3Memory.h"
#include "E3Storage.h"
#include "E3Pick.h"
#include "E3IO.h"
#include "E3Shader.h"
#include "E3Texture.h"
#include "E3CustomElements.h"
#include "E3IOFileFormat.h"
#include "E3StackCrawl.h"


#if QUESA_OS_MACINTOSH
	#include <libkern/OSAtomic.h>
#endif

#include <cstring>
#include <map>
#include <set>
#include <utility>


//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define	kPropertyHashTableSize					32




//=============================================================================
//      Global Variables
//-----------------------------------------------------------------------------

#if QUESA_OS_MACINTOSH
	extern volatile int32_t gObjectCount;
	volatile int32_t	gObjectCount = 0;
#elif QUESA_OS_WIN32
	extern volatile LONG gObjectCount;
	volatile LONG		gObjectCount = 0;
#endif



//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
namespace
{
	
struct PropIterData
{
	TQ3Object			object;
	TQ3PropertyIterator userIterator;
	void*				userData;
};

typedef std::set< TQ3Object* > WeakRefSet;

typedef std::map< TQ3Object, WeakRefSet > ObToWeakRefs;

}



//=============================================================================
//      Internal variables
//-----------------------------------------------------------------------------

static ObToWeakRefs* sObToWeakRefs = nullptr;


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      E3ShapeInfo::E3ShapeInfo : Constructor for class info of root class.
//-----------------------------------------------------------------------------

E3ShapeInfo::E3ShapeInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3SharedInfo ( newClassMetaHandler, newParent )
	{
	// Fill in the method data of the class
	
	// There are (currently) no new methods in shape class
	}


//=============================================================================
//      e3shape_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3shape_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3ShapeInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3shape_metahandler : Shape metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shape_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = nullptr;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeNewObjectClass:
			theMethod = (TQ3XFunctionPointer) e3shape_new_class_info;
			break;

		}
	
	return(theMethod);
}





//=============================================================================
//      E3SharedInfo::E3SharedInfo : Constructor for class info of root class.
//-----------------------------------------------------------------------------
#pragma mark -
E3SharedInfo::E3SharedInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3Root ( newClassMetaHandler, newParent )
	{
	// Fill in the method data of the class
	
//	disposeMethod = (TQ3XObjectDisposeMethod) Find_Method ( kQ3XMethodTypeObjectDispose , kQ3True ) ;
//	newMethod = (TQ3XObjectNewMethod) Find_Method ( kQ3XMethodTypeObjectNew , kQ3False ) ; // N.B. False, not inherited
//	deleteMethod = (TQ3XObjectDeleteMethod) Find_Method ( kQ3XMethodTypeObjectDelete , kQ3False ) ; // N.B. False, not inherited
//	duplicateMethod = (TQ3XObjectDuplicateMethod) Find_Method ( kQ3XMethodTypeObjectDuplicate , kQ3False ) ; // N.B. False, not inherited
	}


//=============================================================================
//      e3shared_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3shared_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3SharedInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3shared_new : Shared new method.
//-----------------------------------------------------------------------------
TQ3Status
e3shared_new ( TQ3Object inObject, void *privateData, const void *paramData )
{
#pragma unused(privateData)
#pragma unused(paramData)
	E3Shared* theObject = (E3Shared*) inObject;


	// Initialise our instance data
	theObject->sharedData.refCount  = 1 ;
	theObject->sharedData.editIndex = 1 ;

#if Q3_DEBUG
	theObject->sharedData.logRefs = kQ3False;
#endif
	
	return kQ3Success ;
}





//=============================================================================
//      E3Shared_Dispose : Shared dispose method.
//-----------------------------------------------------------------------------
void
E3Shared_Dispose ( TQ3Object inObject )
	{
	// Find the instance data
	if ( inObject == nullptr )
		return ;



	// Decrement the reference count
	E3Shared* theObject = (E3Shared*) inObject;
	Q3_ASSERT(theObject->sharedData.refCount >= 1);
	theObject->sharedData.refCount--;

#if Q3_DEBUG
	if (theObject->IsLoggingRefs())
	{
		Q3_MESSAGE_FMT("Ref count of %p reduced to %d", theObject,
			(int) theObject->sharedData.refCount );
	}
#endif


	// If the reference count falls to 0, dispose of the object
	if ( theObject->sharedData.refCount == 0 )
		theObject->DestroyInstance () ;
	}





//=============================================================================
//      E3Shared_AddReference : Increment reference count.
//-----------------------------------------------------------------------------
void E3Shared_AddReference( E3Shared* theObject )
{
	if ( theObject == nullptr )
		return ;

	theObject->sharedData.refCount++;
#if Q3_DEBUG
	if (theObject->sharedData.refCount < 2)
	{
		Q3_MESSAGE_FMT("E3Shared::GetReference has refCount %d.",
			(int)theObject->sharedData.refCount );
		Q3_MESSAGE_FMT("Class of messed up object was %s.",
			theObject->GetClass()->GetName() );
	}
#endif
	Q3_ASSERT(theObject->sharedData.refCount >= 2);
#if Q3_DEBUG
	if (theObject->IsLoggingRefs())
	{
		Q3_MESSAGE_FMT("Ref count of %p increased to %d", theObject,
			(int) theObject->sharedData.refCount );
	}
#endif
}





//=============================================================================
//      e3shared_duplicate : Shared duplicate method.
//-----------------------------------------------------------------------------
TQ3Status
e3shared_duplicate(TQ3Object fromObject,     const void *fromPrivateData,
						 TQ3Object toObject, const void *toPrivateData)
{
	E3Shared		*instanceData = (E3Shared *) toObject ;
	E3Shared		*fromInstanceData = (E3Shared *) fromObject ;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	instanceData->sharedData.refCount  = 1;
	instanceData->sharedData.editIndex = E3Integer_Abs( fromInstanceData->sharedData.editIndex );

#if Q3_DEBUG
	instanceData->sharedData.logRefs = kQ3False;
#endif

	return(kQ3Success);
}





//=============================================================================
//      e3shared_write : Default write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3shared_write ( TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData )
	{
	// Submit the object
	return E3FileFormat_Method_SubmitObject ( theView, theObject, objectType, objectData ) ;
	}





//=============================================================================
//      e3shared_metahandler : Shared metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3shared_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = nullptr;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeNewObjectClass:
			theMethod = (TQ3XFunctionPointer) e3shared_new_class_info;
			break;

		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3shared_new;
			break;

		case kQ3XMethodTypeObjectDispose:
			theMethod = (TQ3XFunctionPointer) E3Shared_Dispose;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3shared_duplicate;
			break;

		case kQ3XMethodTypeObjectSubmitWrite:
			theMethod = (TQ3XFunctionPointer) e3shared_write;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      E3Root::E3Root : Constructor for class info of root class.
//-----------------------------------------------------------------------------
#pragma mark -

E3Root::E3Root	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
	: E3ClassInfo ( newClassMetaHandler , newParent ) ,
	// Fill in the method data of the class
	disposeMethod		( (TQ3XObjectDisposeMethod)		Find_Method ( kQ3XMethodTypeObjectDispose , kQ3True ) ) ,
	deleteMethod		( (TQ3XObjectDeleteMethod)		Find_Method ( kQ3XMethodTypeObjectDelete , kQ3False ) ) , // N.B. False, not inherited
	duplicateMethod		( (TQ3XObjectDuplicateMethod)	Find_Method ( kQ3XMethodTypeObjectDuplicate , kQ3False ) ) , // N.B. False, not inherited
	newMethod			( (TQ3XObjectNewMethod)			Find_Method ( kQ3XMethodTypeObjectNew , kQ3False ) ) , // N.B. False, not inherited
	submitRenderMethod	( (TQ3XObjectSubmitMethod)		Find_Method ( kQ3XMethodTypeObjectSubmitRender , kQ3True ) ) ,
	submitPickMethod	( (TQ3XObjectSubmitMethod)		Find_Method ( kQ3XMethodTypeObjectSubmitPick , kQ3True ) ) ,
	submitBoundsMethod	( (TQ3XObjectSubmitMethod)		Find_Method ( kQ3XMethodTypeObjectSubmitBounds , kQ3True ) ) ,
	submitWriteMethod	( (TQ3XObjectSubmitMethod)		Find_Method ( kQ3XMethodTypeObjectSubmitWrite , kQ3True ) )
	{
	if ( disposeMethod == nullptr )
		SetAbstract () ;
	}


//=============================================================================
//      e3root_new_class_info : Method to construct a class info record for the root class.
// This is necessary because applications can register their own object classes based on ours
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3root_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
	{
	return new ( std::nothrow ) E3Root ( newClassMetaHandler, newParent ) ;
	}


//=============================================================================
//      propertyTable_disposeItems : Free one data block in a property table.
//-----------------------------------------------------------------------------
static TQ3Status
propertyTable_disposeOne( E3HashTablePtr srcTable,
							TQ3ObjectType theKey,
							void *theItem,
							void* userData )
{
#pragma unused( srcTable, theKey, userData )
	Q3Memory_Free( &theItem );
	return kQ3Success;
}


//=============================================================================
//      propertyTable_disposeItems : Free the data in a property table.
//-----------------------------------------------------------------------------
static void
propertyTable_disposeItems( E3HashTablePtr ioTable )
{
	if (ioTable != nullptr)
	{
		E3HashTable_Iterate( ioTable, propertyTable_disposeOne, nullptr );
	}
}


//=============================================================================
//      propertyTable_duplicateOne : Copy one item between object property tables.
//-----------------------------------------------------------------------------
static TQ3Status
propertyTable_duplicateOne( E3HashTablePtr srcTable,
							TQ3ObjectType theKey,
							void *theItem,
							void* userData )
{
	TQ3Status status = kQ3Success;
	E3HashTablePtr dstTable = (E3HashTablePtr) userData;
	TQ3Uns32 dataSize;
	Q3Memory_Copy( theItem, &dataSize, sizeof(dataSize) );
	void* newItem = Q3Memory_Allocate( static_cast<TQ3Uns32>(dataSize + sizeof(dataSize)) );
	if (newItem == nullptr)
	{
		status = kQ3Failure;
	}
	else
	{
		Q3Memory_Copy( theItem, newItem, static_cast<TQ3Uns32>(dataSize + sizeof(dataSize)) );
		E3HashTable_Add( dstTable, theKey, newItem );
	}
	return status;
}


//=============================================================================
//      propertyTable_duplicate : Duplicate an object property table.
//-----------------------------------------------------------------------------
static E3HashTablePtr
propertyTable_duplicate( E3HashTablePtr inSrcTable )
{
	TQ3Uns32	tableSize = E3HashTable_GetTableSize( inSrcTable );
	E3HashTablePtr	dstTable = E3HashTable_Create( tableSize );
	if (dstTable != nullptr)
	{
		if (kQ3Failure == E3HashTable_Iterate( inSrcTable,
			propertyTable_duplicateOne, dstTable ))
		{
			propertyTable_disposeItems( dstTable );
			E3HashTable_Destroy( &dstTable );
		}
	}
	return dstTable;
}





//=============================================================================
//     propertyIterateFunc: Hash table callback that calls a user callback.
//-----------------------------------------------------------------------------
static TQ3Status
propertyIterateFunc( E3HashTablePtr theTable,
					TQ3ObjectType theKey,
					void *theItem,
					void *userData )
{
#pragma unused( theItem )
	PropIterData* iterData = (PropIterData*) userData;
	
	TQ3Status status = (*iterData->userIterator)( iterData->object,
		theKey, iterData->userData );
	
	return status;
}





//=============================================================================
//      e3root_new : Root object new method.
//-----------------------------------------------------------------------------
TQ3Status
e3root_new( TQ3Object theObject, void *privateData, const void *paramData )
{
#pragma unused( paramData )
#pragma unused( privateData )


#if Q3_DEBUG
	E3GlobalsPtr	theGlobals = E3Globals_Get();
	static TQ3Boolean	sIsMakingListHead = kQ3False;
	
	if (sIsMakingListHead == kQ3True)
	{
		theObject->next = theObject;
		theObject->prev = theObject;
		theObject->stackCrawl = nullptr;
	}
	else
	{
		// initialize instance data
		if (theGlobals->isLeakChecking == kQ3True)
		{
			// make sure the list has a header
			if (theGlobals->listHead == nullptr)
			{
				sIsMakingListHead = kQ3True;	// prevent infinite recursion
				theGlobals->listHead = E3ClassTree::CreateInstance ( kQ3ObjectTypeRoot,
					kQ3False, nullptr ) ;
				sIsMakingListHead = kQ3False;
				Q3_REQUIRE_OR_RESULT( theGlobals->listHead != nullptr, kQ3Failure );
			}
			Q3_ASSERT( theGlobals->listHead->GetClass ()->GetType () == kQ3ObjectTypeRoot ) ;
			
			// insert the new node between the list header and last normal node
			theObject->next = theGlobals->listHead;
			theObject->prev = PREVLINK( theGlobals->listHead );
			NEXTLINK( PREVLINK( theGlobals->listHead ) ) = theObject;
			PREVLINK( theGlobals->listHead ) = theObject;
			
			// Record a stack crawl if possible
			theObject->stackCrawl = E3StackCrawl_New();
		}
		else
		{
			theObject->next = nullptr;
			theObject->prev = nullptr;
			theObject->stackCrawl = nullptr;
		}
	}
#endif
	
	theObject->theSet = nullptr;
	theObject->propertyTable = nullptr;
	
	// Update the global object count.
#if QUESA_OS_MACINTOSH
	OSAtomicIncrement32( &gObjectCount );
#elif QUESA_OS_WIN32
	InterlockedIncrement( &gObjectCount );
#endif
	
	return kQ3Success;
}





//=============================================================================
//      e3root_duplicate : Root object duplicate method.
//-----------------------------------------------------------------------------
TQ3Status
e3root_duplicate(TQ3Object fromObject,     const void *fromPrivateData,
						 TQ3Object toObject, const void* inDstData)
{
#pragma unused( fromObject )
	TQ3Status	q3status;
	void *toPrivateData = (void*) inDstData;
	
	q3status = e3root_new( toObject, toPrivateData, nullptr );
	
	if (q3status == kQ3Success)
	{
		OpaqueTQ3Object*	fromInstanceData = (OpaqueTQ3Object*) fromPrivateData;
		OpaqueTQ3Object*	toInstanceData = (OpaqueTQ3Object*) toPrivateData;
		
		
		if (fromInstanceData->theSet != nullptr)
		{
			toInstanceData->theSet = Q3Object_Duplicate( fromInstanceData->theSet );
			if (toInstanceData->theSet == nullptr)
			{
				q3status = kQ3Failure;
			}
		}
		
		
		if ( (q3status == kQ3Success) && (fromInstanceData->propertyTable != nullptr) )
		{
			toInstanceData->propertyTable = propertyTable_duplicate(
				fromInstanceData->propertyTable );
			
			if (toInstanceData->propertyTable == nullptr)
			{
				q3status = kQ3Failure;
			}
		}
	}
	
	return q3status;
}





//=============================================================================
//      e3root_delete : Root delete method.
//-----------------------------------------------------------------------------
void
e3root_delete( TQ3Object theObject, void *privateData )
{
	OpaqueTQ3Object	*instanceData = (OpaqueTQ3Object *) privateData;

	Q3_ASSERT(privateData == theObject);
//	Q3_ASSERT(privateData == theObject->FindLeafInstanceData () ) ;
	
	Q3Object_CleanDispose( &instanceData->theSet );
	if (instanceData->propertyTable != nullptr)
	{
		propertyTable_disposeItems( instanceData->propertyTable );
		E3HashTable_Destroy( &instanceData->propertyTable );
	}

	
	// Update the global object count.
#if QUESA_OS_MACINTOSH
	OSAtomicDecrement32( &gObjectCount );
#elif QUESA_OS_WIN32
	InterlockedDecrement( &gObjectCount );
#endif


#if Q3_DEBUG
	if ( instanceData->prev != nullptr )
	{
		NEXTLINK( instanceData->prev ) = instanceData->next;
	}
	if ( instanceData->next != nullptr )
	{
		PREVLINK( instanceData->next ) = instanceData->prev;
	}

	instanceData->prev = nullptr;
	instanceData->next = nullptr;
	
	E3StackCrawl_Dispose( instanceData->stackCrawl );
#endif
}





//=============================================================================
//      e3root_dispose : Root object dispose method.
//-----------------------------------------------------------------------------
static void
e3root_dispose(TQ3Object theObject)
	{
	// Dispose of the object
	theObject->DestroyInstance () ;
	}





//=============================================================================
//      e3root_metahandler : Root object metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3root_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = nullptr;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectClassVersion:
			theMethod = (TQ3XFunctionPointer) kQ3PackedVersion;
			break;

		case kQ3XMethodTypeNewObjectClass:
			theMethod = (TQ3XFunctionPointer) e3root_new_class_info;
			break;

		case kQ3XMethodTypeObjectDispose:
			theMethod = (TQ3XFunctionPointer) e3root_dispose;
			break;
		
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3root_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3root_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3root_duplicate;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3main_registercoreclasses : Register the core object classes.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3main_registercoreclasses(void)
	{
	// Register the classes
	TQ3Status qd3dStatus = E3ClassTree::RegisterClass (
											kQ3ObjectTypeQuesa,
											OpaqueTQ3Object::eClassType,
											kQ3ClassNameRoot,
											e3root_metahandler,
											sizeof ( OpaqueTQ3Object ),
											sizeof ( OpaqueTQ3Object ),
											0
											);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_WITH_MEMBER (		kQ3ClassNameShared,
												e3shared_metahandler,
												E3Shared,
												sharedData ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (		kQ3ClassNameShape,
												e3shape_metahandler,
												E3Shape ) ;

	return qd3dStatus ;
	}




//=============================================================================
//      e3main_unregistercoreclasses : Unregister the core object classes.
//-----------------------------------------------------------------------------
static TQ3Status
e3main_unregistercoreclasses(void)
	{
	// Unregister the classes. Unregistering the root class will
	// unregister everything else anchored underneath it.
	return E3ClassTree::UnregisterClass ( kQ3ObjectTypeRoot, kQ3True ) ;
	}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Initialize : Initialise Quesa.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Initialize(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Status		qd3dStatus = kQ3Success;


	// If we've not initialised Quesa yet, do so now
	if (!theGlobals->systemInitialised)
		{
		// Initialise the platform
		qd3dStatus = E3System_Initialise();



		// Initialise Quesa
		if (qd3dStatus == kQ3Success)
			qd3dStatus = e3main_registercoreclasses();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Memory_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3String_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Transform_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Group_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Set_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Light_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Style_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3View_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3DrawContext_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Camera::RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Geometry_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Shader_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Texture_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Renderer_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Storage_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3File_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3Pick_RegisterClass();

		if (qd3dStatus == kQ3Success)
			qd3dStatus = E3CustomElements_RegisterClass();
		


		// Load our plug-ins
		if (qd3dStatus == kQ3Success)
			E3System_LoadPlugins();



		// Set our flag
		if (qd3dStatus == kQ3Success)
			theGlobals->systemInitialised = kQ3True;
		}
	
	
	// Or post a notice
	else
		E3ErrorManager_PostNotice(kQ3NoticeSystemAlreadyInitialized);



	// If all went well, increment the reference count
	if (qd3dStatus == kQ3Success)
		theGlobals->systemRefCount++;

	return(qd3dStatus);
}





//=============================================================================
//      E3Exit : Terminate Quesa.
//-----------------------------------------------------------------------------
TQ3Status
E3Exit(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();



	// Make sure we've been initialised
	if (!theGlobals->systemInitialised)
		return(kQ3Failure);



	// Decrement the instance count		
	Q3_ASSERT(theGlobals->systemRefCount > 0);
	theGlobals->systemRefCount--;



	// If this was the last instance, terminate Quesa
	if (theGlobals->systemRefCount == 0)
		{
		// Dump some stats
#if QUESA_DUMP_STATS_ON_EXIT
		E3ClassTree::Dump () ;
#endif

#if QUESA_ALLOW_QD3D_EXTENSIONS && Q3_DEBUG
		if ( Q3Memory_IsRecording() && (Q3Memory_CountRecords() > 0) )
		{
			E3ErrorManager_PostError( kQ3ErrorMemoryLeak, kQ3False );
			Q3Memory_DumpRecording( "Quesa-leaks.txt", "Q3Exit" );
			Q3Memory_ForgetRecording();
		}
#endif



		// Reset leak-checking globals to initial state
#if Q3_DEBUG
		Q3Object_CleanDispose(&theGlobals->listHead );
		theGlobals->isLeakChecking = kQ3False;
#endif



		// Unload our plug-ins
		E3System_UnloadPlugins();



		// Terminate Quesa
		E3CustomElements_UnregisterClass();
		E3Pick_UnregisterClass();
		E3File_UnregisterClass();
		E3Storage_UnregisterClass();
		E3Renderer_UnregisterClass();
		E3Texture_UnregisterClass();
		E3Shader_UnregisterClass();
		E3Geometry_UnregisterClass();
		E3Camera::UnregisterClass();
		E3DrawContext_UnregisterClass();
		E3View_UnregisterClass();
		E3Style_UnregisterClass();
		E3Light_UnregisterClass();
		E3Set_UnregisterClass();
		E3Group_UnregisterClass();
		E3Transform_UnregisterClass();
		E3String_UnregisterClass();
		E3Memory_UnregisterClass();
		e3main_unregistercoreclasses();
		E3ClassTree::Destroy () ;



		// Terminate the platform
		E3System_Terminate();



		// Set our flag
		theGlobals->systemInitialised = kQ3False;
		}

	return(kQ3Success);
}





//=============================================================================
//      E3IsInitialized : Test to see if Quesa has been initialised.
//-----------------------------------------------------------------------------
TQ3Boolean
E3IsInitialized(void)
{	E3GlobalsPtr	theGlobals = E3Globals_Get();



	// Return as we've initialised Quesa
	return(theGlobals->systemInitialised);
}





//=============================================================================
//      E3GetVersion : Return the build version.
//-----------------------------------------------------------------------------
//		Note :	kQ3MajorVersion and kQ3MinorVersion are each in BCD format.
//				For example, 12 is represented as 0x12.
//
//				May be called outside of a Q3Initialize/Q3Exit block.
//-----------------------------------------------------------------------------
TQ3Status
E3GetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{


	// Return the build version
	*majorRevision = 10 * (kQ3MajorVersion >> 4) + (kQ3MajorVersion & 0x0f);
	#if (kQ3MinorVersion & 0x0f) != 0
		*minorRevision = 10 * (kQ3MinorVersion >> 4) + (kQ3MinorVersion & 0x0f);
	#else  // single-digit minor version
		*minorRevision = (kQ3MinorVersion >> 4);
	#endif

	return(kQ3Success);
}





//=============================================================================
//      E3GetReleaseVersion : Return the build version in 'vers' format.
//-----------------------------------------------------------------------------
//		Note :	We return the version number in the format of the first four
//				bytes of a 'vers' resource. For example, "1.23a56" is
//				represented as 0x01234056.
//
//				For more information, see the description of the 'vers'
//				resource in "Inside Macintosh: Macintosh Toolbox Essential",
//				p. 7-69.
//
//				May be called outside of a Q3Initialize/Q3Exit block.
//-----------------------------------------------------------------------------
TQ3Status
E3GetReleaseVersion(TQ3Uns32 *releaseRevision)
{


	// Return the release version
	*releaseRevision = kQ3PackedVersion;

	return(kQ3Success);
}





//=============================================================================
//      E3ObjectHierarchy_GetTypeFromString : Find the type for a class.
//-----------------------------------------------------------------------------
//		Note : Given a class name, returns the type of the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3ObjectHierarchy_GetTypeFromString(const TQ3ObjectClassNameString objectClassString, TQ3ObjectType *objectClassType)
	{
	// Initialise a return value
	*objectClassType = kQ3ObjectTypeInvalid ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassString ) ;
	if ( theClass == nullptr )
		return kQ3Failure ;



	// Return the type
	*objectClassType = theClass->GetType () ;
	return kQ3Success ;
	}





//=============================================================================
//      E3ObjectHierarchy_GetStringFromType : Find the name for a class.
//-----------------------------------------------------------------------------
//		Note : Given a class type, returns the name of the class.
//-----------------------------------------------------------------------------
TQ3Status
E3ObjectHierarchy_GetStringFromType(TQ3ObjectType objectClassType, TQ3ObjectClassNameString objectClassString)
	{
	// Initialise a return value
	strcpy ( objectClassString, "" ) ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassType ) ;
	if ( theClass == nullptr )
		return kQ3Failure ;



	// Return the name
	strcpy ( objectClassString, theClass->GetName () ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3ObjectHierarchy_IsTypeRegistered : Test if a class is registered.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ObjectHierarchy_IsTypeRegistered(TQ3ObjectType objectClassType)
	{
	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassType ) ;

	return theClass != nullptr ? kQ3True : kQ3False ;
	}





//=============================================================================
//      E3ObjectHierarchy_IsNameRegistered : Test if a class is registered.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ObjectHierarchy_IsNameRegistered(const char *objectClassName)
	{
	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassName ) ;

	return theClass != nullptr ? kQ3True : kQ3False ;
	}





//=============================================================================
//      E3ObjectHierarchy_GetSubClassData : Get the sub-classes of a class.
//-----------------------------------------------------------------------------
//		Note :	Fills in subClassData with the number and class types of all of
//				the subclasses immediately below the specified class.
//-----------------------------------------------------------------------------
TQ3Status
E3ObjectHierarchy_GetSubClassData(TQ3ObjectType objectClassType, TQ3SubClassData *subClassData)
	{
	// Initialise a return value
	subClassData->numClasses = 0;
	subClassData->classTypes = nullptr;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectClassType ) ;
	if ( theClass == nullptr )
		return kQ3Failure ;



	// Allocate the array for the child classes
	Q3Memory_Clear(subClassData, sizeof(TQ3SubClassData));

	TQ3Uns32 numChildren = theClass->GetNumChildren () ;
	if (numChildren != 0)
		{
		subClassData->classTypes = (TQ3ObjectType *) Q3Memory_Allocate(static_cast<TQ3Uns32>(sizeof(TQ3ObjectType) * numChildren));
		if (subClassData->classTypes == nullptr)
			return(kQ3Failure);

		subClassData->numClasses = numChildren;
		}



	// Collect the child classes	
	for ( TQ3Uns32 n = 0 ; n < numChildren ; ++n )
		{
		// Get the child
		E3ClassInfoPtr theChild = theClass->GetChild ( n ) ;
		Q3_ASSERT_VALID_PTR(theChild);
		
		
		// Grab the type
		subClassData->classTypes [ n ] = theChild->GetType () ;
		}

	return kQ3Success ;
	}





//=============================================================================
//      E3ObjectHierarchy_EmptySubClassData : Dispose of the sub-class data.
//-----------------------------------------------------------------------------
TQ3Status
E3ObjectHierarchy_EmptySubClassData(TQ3SubClassData *subClassData)
{


	// Dispose of the data
	Q3Memory_Free(&subClassData->classTypes);



	// Reset the structure
	subClassData->numClasses = 0;
	subClassData->classTypes = nullptr;

	return(kQ3Success);
}





//=============================================================================
//      E3Object_Dispose : Dispose of an object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
OpaqueTQ3Object::Dispose ( void )
{

	// Dispose of the object
	( (E3Root*) GetClass () )->disposeMethod( this );

	return kQ3Success ;
}





//=============================================================================
//      E3Object_CleanDispose : Dispose of an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_CleanDispose(TQ3Object *theObject)
{	TQ3Status		qd3dStatus;



	// If we have an object, dispose of it and clear the pointer
	if (theObject != nullptr && *theObject != nullptr)
		{
		qd3dStatus = (*theObject)->Dispose();
		*theObject = nullptr;
		}
	else
		qd3dStatus = kQ3Success;

	return(qd3dStatus);
}





//=============================================================================
//      E3Object_GetWeakReference : Record an object reference so that it can
//									be made zero when the object is deleted.
//-----------------------------------------------------------------------------
void	E3Object_GetWeakReference( TQ3Object* theRefAddress )
{
	if (sObToWeakRefs == nullptr)
	{
		sObToWeakRefs = new ObToWeakRefs;
	}
	
	//Q3_MESSAGE_FMT("+ weak ref %p -> %p", theRefAddress, *theRefAddress );
	(*sObToWeakRefs)[ *theRefAddress ].insert( theRefAddress );
}





//=============================================================================
//      E3Object_ReleaseWeakReference : Forget a zeroing weak reference.
//-----------------------------------------------------------------------------
void	E3Object_ReleaseWeakReference( TQ3Object* theRefAddress )
{
	if (sObToWeakRefs != nullptr)
	{
		//Q3_MESSAGE_FMT("- weak ref %p -> %p", theRefAddress, *theRefAddress );
		(*sObToWeakRefs)[ *theRefAddress ].erase( theRefAddress );
	}
}






//=============================================================================
//      E3Object_ZeroWeakReferences : Zero weak references to an object that
//										has been deleted.
//-----------------------------------------------------------------------------
void	E3Object_ZeroWeakReferences( TQ3Object deletedObject )
{
	if (sObToWeakRefs != nullptr)
	{
		ObToWeakRefs::iterator found = sObToWeakRefs->find( deletedObject );
		if (found != sObToWeakRefs->end())
		{
			//Q3_MESSAGE_FMT("----");
			WeakRefSet& weakRefs( found->second );
			WeakRefSet::iterator endIt = weakRefs.end();
			for (WeakRefSet::iterator i = weakRefs.begin(); i != endIt; ++i)
			{
				TQ3Object* theRefAddr( *i );
				//Q3_MESSAGE_FMT("zeroing reference %p to dead object %p",
				//	theRefAddr, deletedObject );
				*theRefAddr = nullptr;
			}
			sObToWeakRefs->erase( found );
			//Q3_MESSAGE_FMT("----");
		}
	}
}





//=============================================================================
//      E3Object_Duplicate : Duplicate an object.
//-----------------------------------------------------------------------------
//		Note :	Draw context and view objects can not be duplicated, since
//				these refer to resources created by the application that belong
//				to the window system.
//
//				If the new object is a shared object, its reference count is
//				set to 1.
//-----------------------------------------------------------------------------
TQ3Object
E3Object_Duplicate(TQ3Object theObject)
	{
	// Reject draw context and view objects
	//
	// Note - the current Quesa implementation does not support
	// duplicating file storage objects, as opposed to memory storage objects.
	//
	// When this is functionality is added, the kQ3SharedTypeStorage
	// test should be removed.
	//
	// If this causes your app problems, please contact Jose Cruanyes
	// or James Walker to discuss a fix.
	if ( E3View_IsOfMyClass ( theObject ) // Can't access E3View here as it isn't in a header file (yet) 
	|| Q3_OBJECT_IS_CLASS ( theObject, E3DrawContext )
	|| ( Q3_OBJECT_IS_CLASS ( theObject, E3Storage ) &&
		! Q3_OBJECT_IS_CLASS ( theObject, E3MemoryStorage ) &&
		! Q3_OBJECT_IS_CLASS ( theObject, E3PathStorage ) ) )
		{
		E3ErrorManager_PostError(kQ3ErrorInvalidObjectType, kQ3False);
		return(nullptr);
		}



	// Duplicate the object
	return theObject->DuplicateInstance () ;
	}





//=============================================================================
//      E3Object_Submit : Submit an object to a view.
//-----------------------------------------------------------------------------
TQ3Status
E3Object_Submit(TQ3Object theObject, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object
	qd3dStatus = E3View_SubmitRetained(theView, theObject);
	return(qd3dStatus);
}





//=============================================================================
//      E3Object_IsDrawable : Determine if an object is drawable.
//-----------------------------------------------------------------------------
//		Note :	There is no is-drawable method as such, instead we interpret
//				the 'address' of the method as a TQ3Boolean.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Object_IsDrawable(TQ3Object theObject)
	{
	// Get the 'method'
	TQ3Boolean isDrawable = (TQ3Boolean) ( nullptr != theObject->GetClass ()->GetMethod (
									kQ3XMethodTypeObjectIsDrawable ) ) ;

	return isDrawable ;
	}





//=============================================================================
//      E3Object_IsWritable : Determine if an object is writeable.
//-----------------------------------------------------------------------------
//		Note : We return as the object has a kQ3XMethodTypeObjectWrite method.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Object_IsWritable(TQ3Object theObject, TQ3FileObject theFile)
	{
#pragma unused(theFile)



	// Get the method
	TQ3XObjectWriteMethod writeMethod = (TQ3XObjectWriteMethod) theObject->GetClass ()->GetMethod ( kQ3XMethodTypeObjectWrite ) ;



	// Return as the method exists or not
	return (TQ3Boolean) ( writeMethod != nullptr ) ;
	}





//=============================================================================
//      E3Object_GetType : Get the fundamental type of an object.
//-----------------------------------------------------------------------------
//		Note :	To determine the fundamental type, we walk up the object's
//				parents until we come to the level below the root object.
//
//				NB - we can not filter the result, since some undocumented QD3D
//				objects are registered off the root object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Object_GetType(TQ3Object theObject)
	{
	// Get the type of the object below the root object
	return theObject->GetObjectType ( kQ3ObjectTypeRoot ) ;
	}





//=============================================================================
//      E3Object_IsType : Is an object an instance of a particular type?
//-----------------------------------------------------------------------------
TQ3Boolean
E3Object_IsType(TQ3Object theObject, TQ3ObjectType theType)
	{
	// Return as the object is an instance of the type
	return theObject->GetClass ()->IsType ( theType ) ;
	}





//=============================================================================
//      E3Object_AddElement : Add an element to an object
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::AddElement ( TQ3ElementType theType, const void *theData )
	{
	// Translate public type to internal type for set elements
	if ( theType == kQ3ElementTypeSet )
		theType = kQ3ObjectTypeSetElement;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return ( (E3Set*) this )->Add ( theType, theData ) ;

		
	// otherwise use the set within the instance data
	if ( theSet == nullptr )
		{
		theSet = E3Set_New () ;
		
		if ( theSet == nullptr )
			return kQ3Failure ;
		}
	
	TQ3Status qd3dStatus = ( (E3Set*) theSet )->Add ( theType, theData ) ;
	
	
	if ( ( qd3dStatus != kQ3Failure ) && Q3_OBJECT_IS_CLASS ( this, E3Shared ) )
		( (E3Shared*) this )->Edited () ;
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3Object_GetElement : Get element data from an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::GetElement ( TQ3ElementType theType, void *theData )
	{
	// Translate public type to internal type for set elements
	if (theType == kQ3ElementTypeSet)
		theType = kQ3ObjectTypeSetElement;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return ( (E3Set*) this )->Get ( theType, theData ) ;
	
	// otherwise use the set within the instance data
	
	if ( theSet == nullptr )
		return kQ3Failure ;
	
	return ( (E3Set*) theSet )->Get ( theType, theData ) ;
	}





//=============================================================================
//      E3Object_ContainsElement : Test whether an object contains an element.
//-----------------------------------------------------------------------------
TQ3Boolean
OpaqueTQ3Object::ContainsElement ( TQ3ElementType theType )
	{
	// Translate public type to internal type for set elements
	if ( theType == kQ3ElementTypeSet )
		theType = kQ3ObjectTypeSetElement ;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return Q3Set_Contains ( (TQ3SetObject) this, theType ) ;
	
	// otherwise use the set within the instance data		
		
	if ( theSet != nullptr )
		return Q3Set_Contains ( theSet, theType ) ;
		
		
	
	return kQ3False ;
	}





//=============================================================================
//      E3Object_GetNextElementType : Get the next element type in an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::GetNextElementType ( TQ3ElementType *theType )
	{
	TQ3Status qd3dStatus = kQ3Failure ;
	
	
	
	// Translate public type to internal type for set elements
	if ( *theType == kQ3ElementTypeSet )
		*theType = kQ3ObjectTypeSetElement ;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		qd3dStatus = Q3Set_GetNextElementType ( (TQ3SetObject) this, theType ) ;
	else	// otherwise use the set within the instance data
		{
		if ( theSet == nullptr )
			{
			*theType = kQ3ElementTypeNone ;
			return kQ3Success ;
			}
		
		qd3dStatus = Q3Set_GetNextElementType ( theSet, theType ) ;
		}



	if ( *theType == kQ3ObjectTypeSetElement )
		*theType = kQ3ElementTypeSet ;
	
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3Object_EmptyElements : Remove all elements from an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::EmptyElements ( void )
	{
	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return Q3Set_Empty ( (TQ3SetObject) this ) ;
	
	// otherwise use the set within the instance data
	if ( theSet == nullptr )
		return kQ3Success ;
	
	TQ3Status qd3dStatus = Q3Set_Empty ( theSet ) ;
	
	
	if ( ( qd3dStatus != kQ3Failure ) && Q3_OBJECT_IS_CLASS ( this, E3Shared ) )
		( (E3Shared*) this )->Edited () ;
	
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3Object_ClearElement : Remove a specific type of element from an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::ClearElement ( TQ3ElementType theType )
	{
	// Translate public type to internal type for set elements
	if (theType == kQ3ElementTypeSet)
		theType = kQ3ObjectTypeSetElement;



	// If we've actually been passed a set, use it directly
	if ( Q3_OBJECT_IS_CLASS ( this, E3Set ) )
		return Q3Set_Clear ( (TQ3SetObject) this, theType ) ;
	
	// otherwise use the set within the instance data
	if ( theSet == nullptr )
		return kQ3Success ;
		
	TQ3Status qd3dStatus = Q3Set_Clear ( theSet, theType ) ;
		
		
	if ( ( qd3dStatus != kQ3Failure ) && Q3_OBJECT_IS_CLASS ( this, E3Shared ) )
		( (E3Shared*) this )->Edited () ;
	
	
	return qd3dStatus ;
	}





//=============================================================================
//      E3Object_GetSet : Get the set of an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::GetSet ( TQ3SetObject *set )
	{
	E3Shared_Acquire ( set, theSet ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      OpaqueTQ3Object::SetSet : Set the set of an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::SetSet ( TQ3SetObject set )
	{
	E3Shared_Replace ( &theSet, set ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//     OpaqueTQ3Object::SetProperty : Set a property of an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::SetProperty( TQ3ObjectType inPropType,
								TQ3Uns32 inDataSize,
								const void* inData )
{
	TQ3Status didset = kQ3Failure;
	
	if (propertyTable == nullptr)
	{
		propertyTable = E3HashTable_Create( kPropertyHashTableSize );
	}
	
	if (propertyTable != nullptr)
	{
		(void) RemoveProperty( inPropType );
		
		char* itemBuf = (char*) Q3Memory_Allocate( static_cast<TQ3Uns32>(inDataSize + sizeof(TQ3Uns32)) );
		if (itemBuf != nullptr)
		{
			Q3Memory_Copy( &inDataSize, itemBuf, sizeof(TQ3Uns32) );
			Q3Memory_Copy( inData, itemBuf + sizeof(TQ3Uns32), inDataSize );
			
			didset = E3HashTable_Add( propertyTable, inPropType, itemBuf );
			
			if (didset == kQ3Failure)
			{
				Q3Memory_Free( &itemBuf );
			}
			else
			{
				if (E3Shared_IsOfMyClass( this ))
				{
					E3Shared_Edited( this );
				}
			}
		}
	}
	
	return didset;
}





//=============================================================================
//     OpaqueTQ3Object::GetProperty : Get a property of an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::GetProperty( TQ3ObjectType inPropType,
								TQ3Uns32 inBufferSize,
								TQ3Uns32* outDataSize,
								void* outDataBuffer ) const
{
	TQ3Status found = kQ3Failure;
	
	if (propertyTable != nullptr)
	{
		void* itemAddr = E3HashTable_Find( propertyTable, inPropType );
		if (itemAddr != nullptr)
		{
			found = kQ3Success;
			
			TQ3Uns32 dataSize = *(TQ3Uns32*) itemAddr;
			
			if (outDataBuffer != nullptr)
			{
				dataSize = E3Num_Min( dataSize, inBufferSize );
				
				if (dataSize > 0)
				{
					memcpy( outDataBuffer, ((char*) itemAddr) + sizeof(dataSize),
						dataSize );
				}
			}

			if (outDataSize != nullptr)
			{
				*outDataSize = dataSize;
			}
		}
	}
	
	return found;
}





//=============================================================================
//     OpaqueTQ3Object::GetPropertyAddress : Get pointer to property data of an object.
//-----------------------------------------------------------------------------
const void*
OpaqueTQ3Object::GetPropertyAddress( TQ3ObjectType inPropType ) const
{
	const void* theAddr = nullptr;
	
	if (propertyTable != nullptr)
	{
		const char* itemAddr = (const char*) E3HashTable_Find( propertyTable,
			inPropType );
		
		if (itemAddr != nullptr)
		{
			theAddr = itemAddr + sizeof(TQ3Uns32);
		}
	}
	
	return theAddr;
}





//=============================================================================
//     OpaqueTQ3Object::RemoveProperty : Remove a property of an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::RemoveProperty( TQ3ObjectType inPropType )
{
	TQ3Status found = kQ3Failure;
	
	if (propertyTable != nullptr)
	{
		void* itemAddr = E3HashTable_Find( propertyTable, inPropType );
		if (itemAddr != nullptr)
		{
			Q3Memory_Free( &itemAddr );
			E3HashTable_Remove( propertyTable, inPropType );
			found = kQ3Success;

			if (E3Shared_IsOfMyClass( this ))
			{
				E3Shared_Edited( this );
			}
		}
	}
	return found;
}





//=============================================================================
//     OpaqueTQ3Object::IterateProperties : Iterate over properties of an object.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::IterateProperties( TQ3PropertyIterator userIterator,
									void* userData )
{
	TQ3Status status = kQ3Success;
	
	if (propertyTable != nullptr)
	{
		PropIterData iterData =
		{
			this,
			userIterator,
			userData
		};
		status = E3HashTable_Iterate( propertyTable, propertyIterateFunc,
			&iterData );
	}
	return status;
}


#pragma mark -
//=============================================================================
//      E3Shared_IsOfMyClass : Check if object pointer is valid and of type shared
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3ObjectTypeShared )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3Shared_IsOfMyClass ( TQ3Object object )
	{
	if ( object == nullptr )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Shared ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Shared_GetType : Get the type of a shared object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Shared_GetType(TQ3SharedObject sharedObject)
	{
	// Return the type
	return sharedObject->GetObjectType ( kQ3ObjectTypeShared ) ;
	}





//=============================================================================
//      E3Shared_GetReference : Get a reference to a shared object.
//-----------------------------------------------------------------------------
//		Note :	We increase the reference count of the shared object by 1.
//
//				Note that other Quesa code assumes acquiring a new reference
//				to an object can not fail, provided the object is in fact a
//				shared object.
//
//				If acquiring a new reference could ever fail, routines like
//				E3Shared_Replace will need to be updated.
//-----------------------------------------------------------------------------
E3Shared*
E3Shared::GetReference ( void )
{
	// Increment the reference count and return the object. Note that we
	// return the object passed in: this is OK since we're not declared
	// to return a different object.
	E3Shared_AddReference( this );

	return this ;
}





//=============================================================================
//      E3Shared_IsReferenced : Is a reference shared?
//-----------------------------------------------------------------------------
//		Note :	We return kQ3True if there is more than one reference to the
//				object, and kQ3False if there is one reference to the object.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Shared::IsReferenced ( void )
	{
	// Return as the reference count is greater than 1
	return ( (TQ3Boolean) ( sharedData.refCount > 1 ) ) ;
	}





//=============================================================================
//      E3Shared_GetReferenceCount : Return the reference count.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Shared::GetReferenceCount ( void )
	{
	// Return the reference count
	return sharedData.refCount ;
	}





//=============================================================================
//      E3Shared_GetEditIndex : Return the edit index of a shared object.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Shared::GetEditIndex ( void )
	{
	// Return the edit index
	return E3Integer_Abs( sharedData.editIndex );
	}





//=============================================================================
//      E3Shared::SetEditIndex : Set the edit index of a shared object.
//-----------------------------------------------------------------------------
void
E3Shared::SetEditIndex( TQ3Uns32 inIndex )
{
	sharedData.editIndex = inIndex;
}


#if Q3_DEBUG
TQ3Boolean	E3Shared::IsLoggingRefs() const
{
	return sharedData.logRefs;
}

void		E3Shared::SetLoggingRefs( TQ3Boolean inLog )
{
	sharedData.logRefs = inLog;
}
#endif


//=============================================================================
//      E3Shared_Edited : Increase the edit index of an object.
//-----------------------------------------------------------------------------
TQ3Status
E3Shared::Edited ( void )
{
	if (sharedData.editIndex >= 0)
	{
		// Increment the edit index
		++sharedData.editIndex ;
	}
	
	return kQ3Success ;
}





//=============================================================================
//      SetEditIndexLocked : Set or clear a lock on the edit index
//-----------------------------------------------------------------------------
void
E3Shared::SetEditIndexLocked( TQ3Boolean inIsLocked )
{
	if (inIsLocked)
	{
		sharedData.editIndex = - E3Integer_Abs( sharedData.editIndex );
	}
	else	// unlock
	{
		sharedData.editIndex = E3Integer_Abs( sharedData.editIndex );
	}
}





//=============================================================================
//      IsEditIndexLocked : Test whether the edit index is locked
//-----------------------------------------------------------------------------
TQ3Boolean
E3Shared::IsEditIndexLocked() const
{
	return (sharedData.editIndex < 0) ? kQ3True : kQ3False;
}





//=============================================================================
//      E3Shape_IsOfMyClass : Check if object pointer is valid and of type shape
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3SharedTypeShape )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Boolean
E3Shape_IsOfMyClass ( TQ3Object object )
	{
	if ( object == nullptr )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Shape ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Shape_GetType : Get the type of a shape object.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Shape_GetType(TQ3ShapeObject theShape)
	{
	// Return the type
	return theShape->GetObjectType ( kQ3SharedTypeShape ) ;
	}





//=============================================================================
//      E3Shape_GetSet : Get the set for a shape.
//-----------------------------------------------------------------------------
TQ3Status
E3Shape_GetSet(TQ3ShapeObject theShape, TQ3SetObject *theSet)
	{
	*theSet = nullptr ;
	return theShape->GetElement ( kQ3ElementTypeSet, theSet ) ;
	}





//=============================================================================
//      E3Shape_SetSet : Set the set for a shape.
//-----------------------------------------------------------------------------
TQ3Status
E3Shape_SetSet(TQ3ShapeObject theShape, TQ3SetObject theSet)
	{
	if ( theSet == nullptr )
		return theShape->ClearElement ( kQ3ElementTypeSet ) ;
	
	return theShape->AddElement ( kQ3ElementTypeSet, &theSet ) ;
	}





//=============================================================================
//      E3Shape_SubmitElements : Submit custom elements of a shape.
//-----------------------------------------------------------------------------
//		This will be called by E3FFW_3DMF_TraverseObject to submit
//		any custom elements that might be attached to the shape.
//-----------------------------------------------------------------------------
TQ3Status
OpaqueTQ3Object::SubmitElements ( TQ3ViewObject inView )
	{
	// Use the shape's set, if any.
	if ( theSet == nullptr )
		return kQ3Success ;
	
	
	return ( (E3Set*) theSet )->SubmitElements ( inView ) ;
	}





//=============================================================================
//      E3Bitmap_Empty : Dispose of the memory allocated for a bitmap.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Bitmap_Empty(TQ3Bitmap *theBitmap)
{


	// Dispose of the bitmap's image
	Q3Memory_Free(&theBitmap->image);

	return(kQ3Success);
}





//=============================================================================
//      E3Bitmap_GetImageSize : Get the amount of memory required for a bitmap.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Bitmap_GetImageSize(TQ3Uns32 theWidth, TQ3Uns32 theHeight)
{	TQ3Uns32		imageSize;



	// Bitmaps are 1 bit deep, so every 8 pixels requires 1 byte
	imageSize  = ((theWidth - 1) / 8) + 1;
	imageSize *= theHeight;

	return(imageSize);
}





//=============================================================================
//      E3Bitmap_GetBit : Get a bit from a bitmap.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Bitmap_GetBit(const TQ3Bitmap *theBitmap, TQ3Uns32 x, TQ3Uns32 y)
{	TQ3Uns8			*bytePtr, theByte, bitShift;
	TQ3Boolean		theState;



	// Locate the byte we need
	bytePtr  = theBitmap->image + (y * theBitmap->rowBytes);
	bytePtr += (x / 8);
	theByte  = *bytePtr;



	// Locate the bit we need
	bitShift = (TQ3Uns8) (x % 8);
	if (theBitmap->bitOrder == kQ3EndianBig)
		theState = ((theByte >> (7 - bitShift)) & 0x01) != 0 ? kQ3True : kQ3False;
	else
		theState = ((theByte >>  (   bitShift)) & 0x01) != 0 ? kQ3True : kQ3False;

	return(theState);
}





//=============================================================================
//      E3Bitmap_SetBit : Set a bit within a bitmap.
//-----------------------------------------------------------------------------
TQ3Status
E3Bitmap_SetBit(TQ3Bitmap *theBitmap, TQ3Uns32 x, TQ3Uns32 y, TQ3Boolean theState)
{	TQ3Uns8		*bytePtr, theByte, bitShift, byteMask;



	// Locate the byte we need
	bytePtr  = theBitmap->image + (y * theBitmap->rowBytes);
	bytePtr += (x / 8);



	// Prepare the mask
	bitShift = (TQ3Uns8) (x % 8);

	if (theBitmap->bitOrder == kQ3EndianBig)
		byteMask = (1 << (7 - bitShift));
	else
		byteMask = (1 << (    bitShift));



	// Update the byte
	theByte = *bytePtr;
	
	if (theState)
		theByte |=  byteMask;
	else
		theByte &= ~byteMask;
	
	*bytePtr = theByte;

	return(kQ3Success);
}






