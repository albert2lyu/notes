#include <iostream>
#include <vector>

#include <boost/shared_ptr.hpp>

class Foo {
    int bar_;
public:
    explicit Foo( int bar ) :
        bar_( bar ) {}

    int bar() const { return bar_; }
};

Foo *factory( int bar ) {
    Foo *result = new Foo( bar );

    return result;
}

// typedef to make the name shorter. It gets cumbersome
// otherwise.
typedef std::vector<boost::shared_ptr<Foo> > FooList;

int main() {
    FooList foos;

    for ( int i = 0; i < 10; i++ )
        foos.push_back( boost::shared_ptr<Foo>( factory( i + 1 ) ) );

    FooList::const_iterator iter = foos.begin();
    for ( ; iter != foos.end(); iter++ )
        std::cout << (*iter)->bar() << std::endl;
}