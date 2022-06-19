#ifndef HIERARCHY_H
#define HIERARCHY_H

template < class Type >
class mwHierarchy
{
public:
    mwHierarchy();
    ~mwHierarchy();

    void                    SetOwner( Type* owner );
    Type*                   Owner();

    void                    ParentTo( mwHierarchy& node );
    void                    MakeSiblingAfter( mwHierarchy& node );

    bool                    ParentedBy( const mwHierarchy& node ) const;
    
    void                    RemoveFromParent();
    void                    RemoveFromHierarchy();

    Type*                   GetParent() const;
    Type*                   GetChild() const;               // First child node type
    Type*                   GetSibling() const;             // Next sibling node type
    Type*                   GetPriorSibling() const;        // Prev sibling node type
    Type*                   GetNext() const;
    Type*                   GetNextLeaf() const;
private:
    mwHierarchy*            mParent;
    mwHierarchy*            mSibling;
    mwHierarchy*            mChild;

    Type*                   mOwner;

    mwHierarchy<Type>*      GetPriorSiblingNode() const;
};

// ============
//
// mwHierarchy constructor
//
// ============

template < class Type >
mwHierarchy<Type>::mwHierarchy()
{
    mParent     = NULL;
    mSibling    = NULL;
    mChild      = NULL;

    mOwner      = NULL;
}

// ============
//
// mwHierarchy destructor
//
// ============

template < class Type >
mwHierarchy<Type>::~mwHierarchy()
{
    RemoveFromHierarchy();
}

// ============
//
// Sets current node Owner
//
// ============

template < class Type >
void mwHierarchy<Type>::SetOwner( Type* owner )
{
    mOwner = owner;
}

// ============
//
// Returns current owner pointer
//
// ============

template < class Type >
Type* mwHierarchy<Type>::Owner()
{
    return mOwner;
}

// ============
//
// Set current node's parent node
//
// ============

template < class Type >
void mwHierarchy<Type>::ParentTo( mwHierarchy& node )
{
    mParent     = &node;
    mSibling    = node.mChild;
    node.mChild = this;
}

// ============
//
// Set current node's is the next sibling
//
// ============

template < class Type >
void mwHierarchy<Type>::MakeSiblingAfter( mwHierarchy& node )
{
    RemoveFromParent();

    mParent         = node.mParent;
    mSibling        = node.mSibling;
    node.mSibling   = this;
}

// ============
//
// Check current node is the inherited node from specified node
//
// ============

template < class Type >
bool mwHierarchy<Type>::ParentedBy( const mwHierarchy& node ) const
{
    if ( mParent == &node )
    {
        return true;
    }
    else if ( mParent )
    {
        return mParent->ParentedBy( node );
    }

    return false;
}

// ============
//
// Remove all children node
//
// ============

template < class Type >
void mwHierarchy<Type>::RemoveFromParent()
{
    mwHierarchy<Type>* prev;

    if ( mParent )
    {
        prev = GetPriorSiblingNode();

        if ( prev )
        {
            prev->mSibling = mSibling;
        }
        else
        {
            mParent->mChild = mSibling;
        }
    }

    mParent     = NULL;
    mSibling    = NULL;
}

// ============
//
// Remove current node from inheritance tree
//
// ============

template < class Type >
void mwHierarchy<Type>::RemoveFromHierarchy()
{
    mwHierarchy<Type>* node;
    mwHierarchy<Type>* parentNode;

    parentNode = mParent;
    RemoveFromParent();

    if ( parentNode )
    {
        while ( mChild )
        {
            node = mChild;
            node->RemoveFromParent();
            node->ParentTo( *parentNode );
        }
    }
    else
    {
        while ( mChild )
        {
            mChild->RemoveFromParent();
        }
    }
}

// ============
//
// Parent node type
//
// ============

template < class Type >
Type* mwHierarchy<Type>::GetParent() const
{
    if ( mParent )
    {
        return mParent->mOwner;
    }

    return NULL;
}

// ============
//
// Returns first child owner
//
// ============

template < class Type >
Type* mwHierarchy<Type>::GetChild() const
{
    if ( mChild )
    {
        return mChild->mOwner;
    }

    return NULL;
}

// ============
//
// Next sibling owner
//
// ============

template < class Type >
Type* mwHierarchy<Type>::GetSibling() const
{
    //if ( mChild )
    if ( mSibling )
    {
        return mSibling->mOwner;
    }

    return NULL;
}

// ============
//
// Prior sibling node
//
// ============

template < class Type >
mwHierarchy<Type>* mwHierarchy<Type>::GetPriorSiblingNode() const
{
    if ( mParent == NULL || mParent->mChild == this )
    {
        return NULL;
    }

    mwHierarchy<Type>*  prev;
    mwHierarchy<Type>*  node;

    node = mParent->mChild;
    prev = NULL;

    while ( ( node != this ) && ( node != NULL ) )
    {
        prev = node;
        node = node->mSibling;
    }

    if ( node != this )
    {
        assert( false );
        return NULL;
    }

    return prev;
}

// ============
//
// Prior sibling node owner
//
// ============

template < class Type >
Type* mwHierarchy<Type>::GetPriorSibling() const
{
    mwHierarchy<Type>*  prev;

    prev = GetPriorSiblingNode();

    if ( prev )
    {
        return prev->mOwner;
    }

    return NULL;
}

// ============
//
// Return next node owner in whole inheritance tree
//
// ============

template < class Type >
Type* mwHierarchy<Type>::GetNext() const
{
    const mwHierarchy<Type>* node;

    if ( mChild )
    {
        return mChild->mOwner;
    }
    else
    {
        node = this;
        while ( node && node->mSibling == NULL )
        {
            node = node->mParent;
        }

        if ( node )
        {
            return node->mSibling->mOwner;
        }
        else
        {
            return NULL;
        }
    }

}

// ============
//
// 
//
// ============

template < class Type >
Type* mwHierarchy<Type>::GetNextLeaf() const
{
    assert( 0 );
}

#endif // HIERARCHY_H
