#ifndef IC_BASE_LIST
#define IC_BASE_LIST 1
#include <functional>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <fstream>
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
			 typename KeyEqu = std::equal_to<KeyValue>,
			 typename ValueCmp = std::equal_to<Value> >
	class sklist
	{  
	public:		
		sklist (int limitlevel = DEFAULT_MAXSIZE, double pro = DEFAULT_PROBILITY) :
			M_limitlevel(limitlevel), M_pro(pro)
		{
			M_maxlevel = 0;	
			M_header = make_node(limitlevel, 0, 0);
			M_keycmp = KeyCmp();
			M_valuecmp = ValueCmp();
		}
		virtual ~sklist ()
		{
			//TODO:free
			M_NodePointer x = M_header;
			M_NodePointer tmp = x->forward[0];
			while (tmp != nullptr) 
			{
				delete x;	
				x = tmp;
				tmp = tmp->forward[0];
			};		
		}
	//Data member
	private:
		typedef  sklist_node<KeyValue,Value>	M_Node;
		typedef  sklist_node<KeyValue,Value>*	M_NodePointer;
		typedef	 Value*							M_ValuePoiner;
		int										M_limitlevel;
		double									M_pro;
		M_NodePointer							M_header;
		KeyCmp									M_keycmp;
		KeyEqu									M_keyequ;
		ValueCmp								M_valuecmp;
		int										M_maxlevel;	
	//private member function
	private:
		/****
		* make a node with level and key *
		****/
		inline M_NodePointer
		make_node(int level, KeyValue key, Value value)	
		{
			M_NodePointer res	= new M_Node;
			res->key			= key;
			res->value			= value;
			res->forward		= new M_NodePointer [level+1];
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
			while ( random_pro() < M_pro && level < M_limitlevel ) 
			{
				level++;	
			}
			return level;
		}
	
		/****
		* Search by key *
		****/
		inline M_NodePointer
		search( KeyValue key ) const
		{
			M_NodePointer x = M_header;
			for( int i = M_maxlevel; i >=0 ; --i )
			{
				while ( x->forward[i] != nullptr && M_keycmp(x->forward[i]->key,key)) 
				{
					x = x->forward[i];	
				}
			}
			x = x->forward[0];
			if ( x != nullptr && M_valuecmp(x->key,key) ) 
			{
				return x;	
			}
			else 
			{
				return nullptr;	
			}
		}

	public:
		void print(std::ostream& os = std::cout)
		{
			M_NodePointer x = M_header;
			while ( x->forward[0]!=nullptr ) 
			{
				os << "(" << x->forward[0]->key << " , "
					<< x->forward[0]->value << ")";
				x = x->forward[0];
			}
			os << std::endl;
		}
		/****
		* support random access *
		****/
		inline const Value 
		operator[](KeyValue key) const
		{
			M_NodePointer res = search(key);	
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
			M_NodePointer x = M_header;
			M_NodePointer update[M_limitlevel+1];
			memset( update, 0, M_limitlevel+1 );
			for(int i = M_maxlevel; i >=0 ; --i)	
			{
				while (x->forward[i] != nullptr && M_keycmp(x->forward[i]->key, key)) 
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
			
			if ( x!=nullptr && M_keyequ(x->key, key) ) 
			{
				x->value = NewValue;	
			}
			else 
			{
				int level = random_level();
				if ( level > M_maxlevel ) 
				{
					for(int i = M_maxlevel+1; i <= level; ++i)	
					{
						update[i] = M_header;	
					}
					// update maxlevel
					M_maxlevel = level;
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
			M_NodePointer update[M_limitlevel+1];
			M_NodePointer x = M_header;
			for(int i = M_maxlevel; i>= 0; --i)
			{
				while ( x->forward[i] != nullptr && M_keycmp(x->forward[i]->key,key)) 
				{
					x = x->forward[i];	
				}
				update[i] = x;
			}
			x = x->forward[0];
			if ( x != nullptr && M_keyequ(x->key,key) ) 
			{
				for(int i = 0; i <= M_maxlevel; ++i)	
				{
					if( update[i]->forward[i] != x) break;
					update[i]->forward[i] = x->forward[i];
				}
				delete x;

				//update maxlevel
				while (M_maxlevel>0 &&  M_header->forward[M_maxlevel]==nullptr ) 
				{
					M_maxlevel--;	
				}
			}
		}
	};
}
#endif /* ifndef IC_BASE_LIST */
