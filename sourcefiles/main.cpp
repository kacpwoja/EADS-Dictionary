#include <iostream>
#include "../templates/Dictionary.h"
#include <exception>

int main()
{
	{//constructors
		Dictionary<int, int> test;
		if( !test.empty() )
			std::cerr << "Default constructor error: tree not empty\n";
		if( test.height() )
			std::cerr << "Default constructor error: tree higher than 0\n";
		
		test.insert( 1, 1 );
		Dictionary<int, int> test2( test );

		if( test2.empty() )
			std::cerr << "Copy constructor error: copied tree is empty\n";
		if( test2.height() != 1 )
			std::cerr << "Copy constructor error: copied tree is higher than expected\n";
	}

	{//insert
		Dictionary<int, int> test;
		test.insert( 5, 5 );

		if( test.empty() )
			std::cerr << "Insert error: tree still empty\n";
		if( test.height() != 1 )
			std::cerr << "Insert error: height other than expected\n";

		test.insert( 3, 3 );
		if( test.height() != 2 )
			std::cerr << "Insert error: height other than expected\n";

		try
		{
			test.insert( 3, 3 );
			std::cerr << "Insert error: exception expected\n";
		}
		catch( Dictionary<int, int>::DictionaryError e )
		{}

	}

	{//operator =
		Dictionary<int, int> test;
		for( int i = 1; i < 11; i++ )
			test.insert( i * 2, i * 2 );

		Dictionary<int, int> test2;
		test2 = test;

		if( test2.height() != test.height() )
			std::cerr << "Operator= error: height not matching\n";
	}

	{//operator==
		Dictionary<int, int> test;
		for( int i = 1; i < 11; i++ )
			test.insert( i * 2, i * 2 );

		Dictionary<int, int> test2;
		if( test2 == test )
			std::cerr << "Operator == error: returned true on different trees\n";

		test2 = test;
		if( test2 != test )
			std::cerr << "Operator != error: returned true on copied trees\n";
	}

	{//get
		Dictionary<int, int> test;
		for( int i = 1; i < 11; i++ )
			test.insert( i * 2, i * 2 );

		int var;
		try
		{
			var = test.get( 4 );
		}
		catch( Dictionary<int, int>::DictionaryError e )
		{
			std::cerr << "Get error: threw exception when element exists\n";
		}

		try
		{
			var = test.get( 3 );
			std::cerr << "Get error: didn't throw exception when element doesn't exist\n";
		}
		catch( Dictionary<int, int>::DictionaryError e )
		{}
	}

	{//remove
		Dictionary<int, int> test;
		for( int i = 1; i < 11; i++ )
			test.insert( i * 2, i * 2 );

		try
		{
			test.remove( 4 );
			int var;
			try
			{
				var = test.get( 4 );
				std::cerr << "Remove error: element that was to be removed exists\n";
			}
			catch( Dictionary<int, int>::DictionaryError e )
			{}
		}
		catch( Dictionary<int, int>::DictionaryError e )
		{
			std::cerr << "Remove error: threw exception when element exists\n";
		}

		try
		{
			test.remove( 3 );
			std::cerr << "Remove error: didn't throw an exception when element doesn't exist\n";
		}
		catch( Dictionary<int, int>::DictionaryError e )
		{}
	}

	{//clear
		Dictionary<int, int> test;
		for( int i = 1; i < 11; i++ )
			test.insert( i * 2, i * 2 );

		test.clear();
		if( !test.empty() )
			std::cerr << "Clear error: tree not empty\n";
	}

	{//balancing
		Dictionary<int, int> test;
		for( int i = 1; i < 50; i+= 2 )
			test.insert( i, i );

		test.remove( 23 );
		test.remove( 19 );
		test.remove( 17 );

		std::cerr << "Testing balancing: displaying tree.\n";
		test.display();
	}

	char wait;
	std::cin >> wait;
	return 0;
}