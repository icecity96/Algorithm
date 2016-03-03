#ifndef IC_BASE_LIST
#define IC_BASE_LIST 1
#include <functional>
#include <cstdlib>
#include <iostream>
#include <cstring>
namespace IceCity{
	const double DEFAULT_PROBILITY = 1/4;
	const int	 DEFAULT_MAXSIZE   = 16;
	//node base
	template <typename KeyValue,typename Value>
	struct sklist_node 
	{
		KeyValue key;
		Value value;
		sklist_node<KeyValue,Value>** forward;
	};
	
	/****
	* custom exception *
	****/
	class NotFoundException : public std::exception 
	{
		public:
			virtual const char* what() const throw()
			{
				return "Can't find a value associate with this key";
			}
	}exp_notfound; 

	template <typename KeyValue,typename Value,
			 typename KeyCmp = std::less<KeyValue> , 
			 typename ValueCmp = std::equal_to<Value> >
	class sklist
	{
	public:		
		sklist (int limitlevel = DEFAULT_MAXSIZE, double pro = DEFAULT_PROBILITY) :
			_M_limitlevel(limitlevel), _M_pro(pro)
		{
			_M_maxlevel = 0;	
			_M_header = make_node(limitlevel, 0, 0);
			_M_keycmp = KeyCmp();
			_M_valuecmp = ValueCmp();
		}
		virtual ~sklist ()
		{
			//TODO:free
			_M_NodePointer x = _M_header;
			_M_NodePointer tmp = x->forward[0];
			while (tmp != nullptr) 
			{
				delete x;	
				x = tmp;
				tmp = tmp->forward[0];
			};		
		}
	//Data member
	private:
		typedef  sklist_node<KeyValue,Value>	_M_Node;
		typedef  sklist_node<KeyValue,Value>*	_M_NodePointer;
		typedef	 Value*							_M_ValuePoiner;
		int										_M_limitlevel;
		double									_M_pro;
		_M_NodePointer							_M_header;
		KeyCmp									_M_keycmp;
		ValueCmp								_M_valuecmp;
		int										_M_maxlevel;	
	//private member function
	private:
		/****
		* make a node with level and key *
		****/
		inline _M_NodePointer
		make_node(int level, KeyValue key, Value value)	
		{
			_M_NodePointer res		= new _M_Node;
			res->key			= key;
			res->value			= value;
			res->forward		= new _M_NodePointer [level+1];
			return res;
		}

		/****
		* creat an rand number [0.0,1.0) *
		****/
		inline double 
		random_pro()
		{
			return static_cast<double>(rand())/RAND_MAX;
		}

		/****
		* get the level for a node *
		****/
		int 
		random_level()
		{
			int level = 0;
			while ( random_pro() < _M_pro && level < _M_limitlevel ) 
			{
				level++;	
			}
			return level;
		}
	
		/****
		* Search by key *
		****/
		inline _M_NodePointer
		search( KeyValue key ) const
		{
			_M_NodePointer x = _M_header;
			for( int i = _M_maxlevel; i >=0 ; --i )
			{
				while ( x->forward[i] != nullptr && _M_keycmp(x->forward[i]->key,key)) 
				{
					x = x->forward[i];	
				}
			}
			x = x->forward[0];
			if ( x != nullptr && _M_valuecmp(x->key,key) ) 
			{
				return x;	
			}
			else 
			{
				return nullptr;	
			}
		}

	public:
		void print()
		{
			_M_NodePointer x = _M_header;
			while (x != nullptr) 
			{
				std::cout << x->forward[0]->value << std::endl;	
				x = x->forward[0];
			}
		}
		/****
		* support random access *
		****/
		inline const Value 
		operator[](KeyValue key) const
		{
			_M_NodePointer res = search(key);	
			if (res == nullptr) 
			{
				throw exp_notfound;	
			}
			else
			{
				return res->value;
			}
		}

		/****
		* insert an node in skiplist *
		****/
		void 
		insert(KeyValue key, Value NewValue)
		{
			_M_NodePointer x = _M_header;
			_M_NodePointer update[_M_limitlevel+1];
			memset( update, 0, _M_limitlevel+1 );
			for(int i = _M_maxlevel; i >=0 ; --i)	
			{
				while (x->forward[i] != nullptr && _M_keycmp(x->forward[i]->key, key)) 
				{
					x = x->forward[i];	
				}
				update[i] = x;
			}
			x = x->forward[0];
			/****
			* if x->key equal to search key update it
			* else insert it*
			****/
			
			if ( _M_keycmp(x->key, key) ) 
			{
				x->value = NewValue;	
			}
			else 
			{
				int level = random_level();
				if ( level > _M_maxlevel ) 
				{
					for(int i = _M_maxlevel+1; i <= level; ++i)	
					{
						update[i] = _M_header;	
					}
					// update maxlevel
					_M_maxlevel = level;
				}
				x = make_node( level, key, NewValue );
				for( int i = 0; i <= level; ++i )
				{
					x->forward[i] = update[i]->forward[i];	
					update[i]->forward[i] = x;
				}
			}
		}	

		void 
		delete_key(KeyValue key)
		{
			_M_NodePointer update[_M_limitlevel+1];
			_M_NodePointer x = _M_header;
			for(int i = _M_maxlevel; i>= 0; --i)
			{
				while ( x->forward[i] != nullptr && _M_keycmp(x->forward[i]->key,key)) 
				{
					x = x->forward[i];	
				}
				update[i] = x;
			}
			x = x->forward[0];
			if ( x != nullptr && _M_keycmp(x->key,key) ) 
			{
				for(int i = 0; i <= _M_maxlevel; ++i)	
				{
					if( update[i]->forward[i] != x) break;
					update[i]->forward[i] = x->forward[i];
				}
				delete x;

				//update maxlevel
				while (_M_maxlevel>0 &&  _M_header->forward[_M_maxlevel]==nullptr ) 
				{
					_M_maxlevel--;	
				}
			}
		}
	};
}
#endif /* ifndef IC_BASE_LIST */
