#include <vector>
#include <memory>

namespace ft {


template <class T, class Allocator = std::allocator<T> >
class vector{
	public:

	typedef T 									value_type;
	typedef Allocator							allocator_type;
	typedef typename allocator_type::reference	reference;
	vector(){
		
	}
};

}