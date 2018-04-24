/********************
**Kacper Wojakowski**
***kwojakow 293064***
********************/
#pragma once

#include <iostream>
#include <iomanip>
#include <stdexcept>

template<typename Key, typename Info>
class Dictionary
{
	struct Node
	{
		Key key;
		Info info;
		Node* right;
		Node* left;
		short int balance;
		
		Node(Key nK, Info nI): key( nK ), info ( nI ), right( nullptr ), left( nullptr ), balance( 0 ) {};
	};
	
	Node* root;

	/**************************************************
	*			  	  Private methods				  *
	**************************************************/
	//Recursive called by public
	Node* copy( Node* src ); 
	void display( std::ostream& os, Node* node, int indent = 0 ) const noexcept; 
	void clear( Node*& node ); 
	bool compare( Node* rhs, Node* lhs ) const noexcept; 
	bool insert( Node*& dest, const Key& nKey, const Info& nInfo ); //true means the subtree grew
	bool remove( const Key& elem, Node*& dest ); //true means the subtree shrinked
	int height( Node* dest ) const noexcept;

	//Rotations
	void lRotate( Node*& dest ); 
	void rRotate( Node*& dest ); 
	void rlRotate( Node*& dest );
	void lrRotate( Node*& dest );
public:
	/**************************************************
	*			  	  Exception class				  *
	**************************************************/
	class DictionaryError final: std::invalid_argument
	{
	public:
		using std::invalid_argument::invalid_argument;
		using std::invalid_argument::what;
	};

	/**************************************************
	*  Constructors, destructor, copy and assignment  *
	**************************************************/
	Dictionary(): root( nullptr ) {};
	Dictionary( const Dictionary& src ): root( nullptr ) { *this = src; };
	~Dictionary() { clear(); };
	Dictionary& operator=( const Dictionary& rhs );
	
	/**************************************************
	*				Insertion and removal		  	  *
	**************************************************/

	//Inserts an element
	//Parameters:	Key and Info to be inserted
	//Returns:		void, can throw DictionaryError
	void insert( const Key& nKey, const Info& nInfo ) { insert( root, nKey, nInfo ); };

	//Removes an element
	//Parameters:	Key to remove
	//Returns:		void, can throw DictionaryError
	void remove( const Key& elem ) { remove( elem, root ); }; 
	
	/**************************************************
	*						 Get					  *
	**************************************************/

	//Retrieves an element
	//Parameters:	Key to retrieve
	//Returns:		Info of given key, can throw DictionaryError
	Info get( const Key& elem ) const; //
	
	/**************************************************
	*				 General methods				  *
	**************************************************/

	//Clears the tree
	//Parameters:	none
	//Returns:		void
	void clear() { clear( root ); }; 

	//Checks if tree is empty
	//Parameters:	none
	//Returns:		true if empty, false otherwise
	bool empty() const noexcept { return root == nullptr; };

	//Calculates the height of the tree
	//Parameters:	none
	//Returns:		The height of the tree
	int height() const noexcept { return height( root ); };
	
	/**************************************************
	*		Display method (for checking balance)	  *
	**************************************************/

	//Displays the tree (works properly if keys are less than 8 characters)
	//Parameters:	Stream to display to (std::cout by default)
	//Returns:		void
	void display( std::ostream& os = std::cout ) const noexcept { display( os, root ); }; 
	
	/**************************************************
	*				 Comparison operators			  *
	**************************************************/

	//Checks if two trees are identical
	//Parameters:	Tree to compare to
	//Returns:		True if identical, false otherwise
	bool operator==( const Dictionary& rhs ) const noexcept { return compare( root, rhs.root ); }; 

	//Checks if two trees are different
	//Parameters:	Tree to compare to
	//Returns:		True if different, false otherwise
	bool operator!=( const Dictionary& rhs ) const noexcept { return !( *this == rhs ); }; 
};

template<typename Key, typename Info>
void Dictionary<Key, Info>::clear( Node*& node )
{
	if( node == nullptr )
		return;
	//Postorder cleaning
	clear( node->left );
	clear( node->right );
	delete node;
	node = nullptr;
}

template<typename Key, typename Info>
void Dictionary<Key, Info>::display( std::ostream& os, Node* node, int indent ) const noexcept
{
	if( node == nullptr )
		return;
	
	display( os, node->right, indent + 8 );
	os << std::setw( indent ) << " " << node->key << std::endl;
	display( os, node->left, indent + 8 );
}

template<typename Key, typename Info>
typename Dictionary<Key, Info>::Node* Dictionary<Key, Info>::copy( Node* src )
{
	Node* temp = nullptr;
	
	if( src != nullptr )
	{
		temp = new Node( src->key, src->info );
		temp->balance = src->balance;
		temp->left = copy( src->left );
		temp->right = copy( src->right );
	}

	return temp;
}

template<typename Key, typename Info>
Dictionary<Key, Info>& Dictionary<Key, Info>::operator=( const Dictionary& rhs )
{
	if( this == &rhs )
		return *this;
	
	clear();
	root = copy( rhs.root );
	return *this;
}

template<typename Key, typename Info>
Info Dictionary<Key, Info>::get( const Key& elem ) const
{
	Node* current = root;
	while( current != nullptr )
	{
		if( current->key == elem )
			return current->info;
		if( current->key < elem )
			current = current->right;
		else if( current->key > elem )
			current = current->left;
	}
	throw DictionaryError( "Error while accessing element: Key does not exist!" );
}

template<typename Key, typename Info>
bool Dictionary<Key, Info>::compare( Node* rhs, Node* lhs ) const noexcept
{
	if( rhs == lhs )
		return true;
	if( rhs == nullptr )
		return false;
	if( lhs == nullptr )
		return false;
	
	bool that = ( ( rhs->key == lhs->key ) && ( rhs->info == lhs->info ) );
	
	return that && compare( rhs->right, lhs->right ) && compare( rhs->left, lhs->left );
}

template<typename Key, typename Info>
bool Dictionary<Key, Info>::insert( Node*& dest, const Key & nKey, const Info & nInfo )
{
	//Adding a new element
	if( dest == nullptr )
	{
		dest = new Node( nKey, nInfo );
		dest->balance = 0;
		return true;
	}
	//Throwing an exception if it exists
	if( dest->key == nKey )
	{
		throw DictionaryError( "Error adding an element: Key already exists!" );
	}
	//Adding a bigger element
	if( dest->key < nKey )
	{
		/************************
		*		BALANCING		*
		************************/
		//Right subtree didn't grow, so this tree does not either
		if ( !insert( dest->right, nKey, nInfo ) )
			return false;
		//Simple cases: tree not unbalanced to the right
		if ( dest->balance == -1 )
		{
			dest->balance = 0;
			return false;
		}
		if( dest->balance == 0 )
		{
			dest->balance = 1;
			return true;
		}
		//Balancing/Rotating the tree, because it's unbalanced to the right
		if ( dest->balance == 1 )
		{
			Node* temp = dest->right;
			//Case should not be possible
			if( temp->balance == 0 )
			{
				throw DictionaryError( "Unexpected state of Dictionary encountered" );
			}
			//Single Rotation
			if( temp->balance == 1 )
			{
				dest->balance = 0;
				temp->balance = 0;
				lRotate( dest );
				return false;
			}
			//Double Rotation
			if( temp->balance == -1 )
			{
				Node* temp2 = temp->left;
				if( temp2->balance == -1 )
				{
					dest->balance = 0;
					temp->balance = 1;
				}
				else if( temp2->balance == 0 )
				{
					dest->balance = 0;
					temp->balance = 0;
				}
				else if( temp2->balance == 1 )
				{
					dest->balance = -1;
					temp->balance = 0;
				}
				temp2->balance = 0;
				rlRotate( dest );
				return false;
			}
		}
	}//end of bigger element
	
	//Adding a smaller element
	if( dest->key > nKey )
	{
		/************************
		*		BALANCING		*
		************************/
		//Left subtree didn't grow, so this tree does not either
		if ( !insert( dest->left, nKey, nInfo ) )
			return false;
		//Simple cases: tree not unbalanced to the left
		if ( dest->balance == 1 )
		{
			dest->balance = 0;
			return false;
		}
		if( dest->balance == 0 )
		{
			dest->balance = -1;
			return true;
		}
		//Balancing/Rotating the tree, because it's unbalanced to the left
		if ( dest->balance == -1 )
		{
			Node* temp = dest->left;
			//Case should not be possible
			if( temp->balance == 0 )
			{
				throw DictionaryError( "Unexpected state of Dictionary encountered" );
			}
			//Single rotation
			if( temp->balance == -1 )
			{
				dest->balance = 0;
				temp->balance = 0;
				rRotate( dest );
				return false;
			}
			//Double rotation
			if( temp->balance == 1 )
			{
				Node* temp2 = temp->right;
				if( temp2->balance == -1 )
				{
					dest->balance = 1;
					temp->balance = 0;
				}
				else if( temp2->balance == 0 )
				{
					dest->balance = 0;
					temp->balance = 0;
				}
				else if( temp2->balance == 1 )
				{
					dest->balance = 0;
					temp->balance = -1;
				}
				temp2->balance = 0;
				lrRotate( dest );
				return false;
			}
		}
	}//end of smaller element
}//end of insert

template<typename Key, typename Info>
void Dictionary<Key, Info>::lRotate( Node*& dest )
{
	if( dest == nullptr )
		throw DictionaryError( "Unexpected state of Dictionary encountered" );

	Node* temp = dest->right;
	if( temp == nullptr )
		throw DictionaryError( "Unexpected state of Dictionary encountered" );

	dest->right = temp->left;
	temp->left = dest;
	dest = temp;
}

template<typename Key, typename Info>
void Dictionary<Key, Info>::rRotate( Node*& dest )
{
	if( dest == nullptr )
		throw DictionaryError( "Unexpected state of Dictionary encountered" );

	Node* temp = dest->left;
	if( temp == nullptr )
		throw DictionaryError( "Unexpected state of Dictionary encountered" );

	dest->left = temp->right;
	temp->right = dest;
	dest = temp;
}

template<typename Key, typename Info>
void Dictionary<Key, Info>::rlRotate( Node *& dest )
{
	Node* temp = dest->right;
	rRotate( temp );
	dest->right = temp;
	lRotate( dest );
}

template<typename Key, typename Info>
void Dictionary<Key, Info>::lrRotate( Node *& dest )
{
	Node* temp = dest->left;
	lRotate( temp );
	dest->left = temp;
	rRotate( root );
}

template<typename Key, typename Info>
bool Dictionary<Key, Info>::remove( const Key& elem, Node*& dest )
{
	//Key not found
	if( dest == nullptr )
		throw DictionaryError( "Error while removing: key doesn't exist!" );
	//Key is bigger
	if( elem > dest->key )
	{
		/************************
		*		BALANCING		*
		************************/
		if( remove( elem, dest->right ) )
		{
			//Simple cases
			if( dest->balance == 0 )
			{
				dest->balance = -1;
				return false;
			}
			if( dest->balance == 1 )
			{
				dest->balance = 0;
				return true;
			}
			//Tree needs rotation
			if( dest->balance == -1 )
			{
				Node* temp = dest->left;
				//Single right
				if( temp->balance == 0 )
				{
					temp->balance = 1;
					rRotate( dest );
					return false;
				}
				//Single left
				if( temp->balance == -1 )
				{
					dest->balance = 0;
					temp->balance = 0;
					lRotate( dest );
					return true;
				}
				//Double
				if( temp->balance == 1 )
				{
					Node* temp2 = temp->right;
					if( temp2->balance == 0 )
					{
						temp->balance = 0;
					}
					else if( temp2->balance == 1 )
					{
						temp2->balance = 0;
						temp->balance = -1;
					}
					else if( temp->balance == -1 )
					{
						temp->balance = 0;
						dest->balance = 0;
					}
					rlRotate( dest );
					return true;
				}
			}
		}
		//Subtree didn't shrink, so this didn't either
		return false;
	}//end of bigger element

	//Key is smaller
	if( elem < dest->key )
	{
		/************************
		*		BALANCING		*
		************************/
		if( remove( elem, dest->left ) )
		{
			//Simple cases
			if( dest->balance == 0 )
			{
				dest->balance = 1;
				return false;
			}
			if( dest->balance == -1 )
			{
				dest->balance = 0;
				return true;
			}
			//Needs balancing
			if( dest->balance == 1 )
			{
				Node* temp = dest->right;
				//Single left
				if( temp->balance == 0 )
				{
					temp->balance = 1;
					lRotate( dest );
					return false;
				}
				//Single right
				if( temp->balance == 1 )
				{
					dest->balance = 0;
					temp->balance = 0;
					rRotate( dest );
					return true;
				}
				//Double
				if( temp->balance == -1 )
				{
					Node* temp2 = temp->left;
					if( temp2->balance == 0 )
					{
						temp->balance = 0;
					}
					else if( temp2->balance == 1 )
					{
						dest->balance = 0;
						temp->balance = 0;
						temp2->balance = -1;
					}
					else if( temp->balance == -1 )
					{
						temp->balance = 1;
						temp2->balance = 0;
					}
					lrRotate( dest );
					return true;
				}
			}
		}
		//Subtree didn't shrink, so this does not either
		return false;
	}//end of smaller

	//Deleting elements with at least one empty subtree
	if( dest->right == nullptr )
	{
		Node *temp = dest;
		dest = dest->left;
		delete temp;
		return true;
	}
	if( dest->left == nullptr )
	{
		Node* temp = dest;
		dest = dest->right;
		delete temp;
		return true;
	}

	//Deleting an element with two subtrees
	Node* curr = dest->left;
	Node* prev = nullptr;
	while( curr->right != nullptr )
	{
		prev = curr;
		curr = curr->right;
	}
	dest->key = curr->key;
	dest->info = curr->info;

	if( prev == nullptr )
		dest->left = curr->left;
	else
		prev->right = curr->left;
	delete curr;
	return true;
}

template<typename Key, typename Info>
int Dictionary<Key, Info>::height( Node * dest ) const noexcept
{
	if( dest == nullptr )
		return 0;

	int r = height( dest->right );
	int l = height( dest->left );

	return 1 + ( r > l ? r : l );
}
