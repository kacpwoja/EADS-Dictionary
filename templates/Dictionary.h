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
		
		Node(Key nK, Info nI): key( nK ), info ( nI ),
							   right( nullptr ), left( nullptr ), balance( 0 ) {};
	};
	
	Node* root;

	Node* copy( Node* src ); //
	void display( std::ostream& os, Node* node, int indent = 0 ) const noexcept; //
	void clear( Node*& node ); //
	bool compare( Node* rhs, Node* lhs ) const noexcept; //
	bool insert( Node*& dest, const Key& nKey, const Info& nInfo ); //
	void lRotate( Node*& dest ); //
	void rRotate( Node*& dest ); //
	bool remove( const Key& elem, Node*& dest );
	int height( Node* dest ) const noexcept;
public:
	class DictionaryError final: std::invalid_argument
	{
	public:
		using std::invalid_argument::invalid_argument;
		using std::invalid_argument::what;
	};

	Dictionary(): root( nullptr ) {}; //
	Dictionary( const Dictionary& src ): root( nullptr ) { *this = src; }; //
	~Dictionary() { clear(); }; //
	Dictionary& operator=( const Dictionary& rhs ); //
	
	void insert( const Key& nKey, const Info& nInfo ) { insert( root, nKey, nInfo ); }; //
	void remove( const Key& elem ) { remove( elem, root ); }; //
	
	Info get( const Key& elem ) const; //
	
	void clear() { clear( root ); }; //
	bool empty() const noexcept { return root == nullptr; };
	int height() const noexcept { return height( root ); };
	
	void display( std::ostream& os = std::cout ) const noexcept { display( os, root ); }; //
	
	bool operator==( const Dictionary& rhs ) const noexcept { return compare( root, rhs.root ); }; //
	bool operator!=( const Dictionary& rhs ) const noexcept { return !( *this == rhs ); }; //
};

template<typename Key, typename Info>
void Dictionary<Key, Info>::clear( Node*& node )
{
	if( node == nullptr )
		return;
	
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
	if( dest == nullptr )
	{
		dest = new Node( nKey, nInfo );
		dest->balance = 0;
		return true;
	}
	
	if( dest->key == nKey )
	{
		throw DictionaryError( "Error adding an element: Key already exists!" );
	}
	
	if( dest->key < nKey )
	{
		if ( !insert( dest->right, nKey, nInfo ) )
			return false;
		if ( dest->balance == -1 )
		{
			dest->balance = 0;
			return false;
		}
		if ( dest->balance == 1 )
		{
			Node* temp = dest->right;
			if( temp->balance == 0 )
			{
				throw DictionaryError( "Unexpected state of Dictionary encountered" );
			}
			if( temp->balance == 1 )
			{
				dest->balance = 0;
				temp->balance = 0;
				lRotate( dest );
				return false;
			}
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
				rRotate( temp );
				dest->right = temp;
				lRotate( dest );
				return false;
			}
		}
		if ( dest->balance == 0 )
		{
			dest->balance = 1;
			return true;
		}
	}
	
	if( dest->key > nKey )
	{
		if ( !insert( dest->left, nKey, nInfo ) )
			return false;
		if ( dest->balance == 1 )
		{
			dest->balance = 0;
			return false;
		}
		if ( dest->balance == -1 )
		{
			Node* temp = dest->left;
			if( temp->balance == 0 )
			{
				throw DictionaryError( "Unexpected state of Dictionary encountered" );
			}
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
				lRotate( temp );
				dest->left = temp;
				rRotate( root );
				return false;
			}
			if( temp->balance == -1 )
			{
				dest->balance = 0;
				temp->balance = 0;
				rRotate( dest );
				return false;
			}
		}
		if ( dest->balance == 0 )
		{
			dest->balance = -1;
			return true;
		}
	}
}

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
bool Dictionary<Key, Info>::remove( const Key& elem, Node*& dest )
{
	if( dest == nullptr )
		throw DictionaryError( "Error while removing: key doesn't exist!" );
	if( elem > dest->key )
	{
		//balance
		if( remove( elem, dest->right ) )
		{
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
			if( dest->balance == -1 )
			{
				Node* temp = dest->left;
				if( temp->balance == 0 )
				{
					temp->balance = 1;
					rRotate( dest );
					return false;
				}
				if( temp->balance == -1 )
				{
					dest->balance = 0;
					temp->balance = 0;
					lRotate( dest );
					return true;
				}
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
					rRotate( temp );
					dest->left = temp;
					lRotate( dest );
					return true;
				}
			}
		}
		return false;
	}
	if( elem < dest->key )
	{
		//balance
		if( remove( elem, dest->left ) )
		{
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
			if( dest->balance == 1 )
			{
				Node* temp = dest->right;
				if( temp->balance == 0 )
				{
					temp->balance = 1;
					lRotate( dest );
					return false;
				}
				if( temp->balance == 1 )
				{
					dest->balance = 0;
					temp->balance = 0;
					rRotate( dest );
					return true;
				}
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
					lRotate( temp );
					dest->left = temp;
					rRotate( dest );
					return true;
				}
			}
		}
		return false;
	}

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

	return 1 + height( dest->right ) + height( dest->left );
}
