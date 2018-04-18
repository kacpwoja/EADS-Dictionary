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
	
	void copy( Node* src, Node* dest ); //
	void displayPR( std::ostream& os, Node* node, int indent = 0 ) const noexcept; //
	void clearPR( Node* node ); //
	bool compare( Node* rhs, Node* lhs ) const noexcept; //
	void add( Node* dest, const Key& nKey, const Info& nInfo, bool growth );
public:
	Dictionary(): root( nullptr ) {}; //
	Dictionary( const Dictionary& src ): root( nullptr ) { return *this = src; }; //
	~Dictionary() { clear(); }; //
	Dictionary& operator=( const Dictionary& rhs ); //
	
	void insert( const Key& nKey, const Info& nInfo ) { add( root, nKey, nInfo, false ); };
	void remove( const Key& elem );
	
	Info get( const Key& elem ) const; //
	
	void clear() { clearPR( root ); }; //
	
	void display( std::ostream& os = std::cout ) const noexcept { displayPR( os, root, 0 ); }; //
	
	bool operator==( const Dictionary& rhs ) const noexcept { return compare( root, rhs.root ); }; //
	bool operator!=( const Dictionary& rhs ) const noexcept { return !( *this == rhs ); }; //
};

template<typename Key, typename Info>
void Dictionary::clearPR( Node* node )
{
	if( node == nullptr )
		return;
	
	clearPR( node->left );
	clearPR( node->right );
	delete node;
	node = nullptr;
}

template<typename Key, typename Info>
void Dictionary::displayPR( std::ostream& os, Node* node, int indent )
{
	if( node == nullptr )
		return;
	
	displayPR( os, node->right, indent + 8 );
	os << std::setw( indent ) << " " << node->key << std::endl;
	displayPR( os, node->left, indent + 8 );
}

template<typename Key, typename Info>
void Dictionary::copy( Node* src, Node* dest )
{
	if( src == nullptr )
		return;
		
	dest = new Node( src->key, src->info );
	copy( src->left, dest->left );
	copy( src->right, dest->right );
}

template<typename Key, typename Info>
Dictionary& Dictionary::operator=( const Dictionary& rhs )
{
	if( this == &rhs )
		return *this;
		
	clear();
	copy( rhs.root, root );
}

template<typename Key, typename Info>
Info Dictionary::get( const Key& elem )
{
	Node* current = root;
	while( current != nullptr )
	{
		if( current->key == elem )
			return info;
		if( current->key < elem )
			current = current->right;
		else if( current->key > elem )
			current = current->left;
	}
	throw std::invalid_argument( "Key does not exist!" );
}

template<typename Key, typename Info>
bool Dictionary::compare( Node* rhs, Node* lhs )
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
void Dictionary::add( Node* dest, const Key& nKey, const Info nInfo, bool growth )
{
	if( dest == nullptr )
	{
		dest = new Node( nKey, nInfo );
		growth = true;
		return;
	}
	
	if( dest->key == nKey )
	{
		throw std::invalid_argument( "Key already exists!" );
	}
	
	if( dest->key < nKey )
	{
		add( dest->right, nKey, nInfo, growth );
		if ( != growth )
			return;
		if ( dest->balance == -1 )
		{
			dest->balance = 0;
			growth = false;
			return;
		}
		if ( dest->balance == 1 )
		{
			//TODO: BALANCING, DUŻO
			Node *temp = dest->right;
			if( temp->balance == 0 )
			{
				growth = true;
				return;
			}
			if( temp->balance == 1 )
			{
				dest->balance = -1;
				temp->balance = -1;
				dest->right = temp->left;
				temp->left = dest;
				dest = temp;
				growth = true;
				return;
			}
			if( temp->balance == -1 )
			{
				
			}
		}
		if ( dest->balance == 0 )
		{
			dest->balance = 1;
			growth = true;
			return;
		}
	}
	
	if( dest->key > nKey )
	{
		add( dest->left, nKey, nInfo, growth );
		if ( != growth )
			return;
		if ( dest->balance == 1 )
		{
			dest->balance = 0;
			growth = false;
			return;
		}
		if ( dest->balance == -1 )
		{
			//TODO: BALANCING, DUŻO
			Node *temp = dest->right;
			if( temp->balance == 0 )
			{
				
			}
			if( temp->balance == 1 )
			{
				
			}
			if( temp->balance == -1 )
			{
				
			}
		}
		if ( dest->balance == 0 )
		{
			dest->balance = -1;
			growth = true;
			return;
		}
	}
}
