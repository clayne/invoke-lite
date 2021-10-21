#include "invoke-main.t.hpp"

namespace {



int f42()
{
    return 42;
}

int f42_nef() invoke_noexcept_op( false )
{
    return 42;
}

int arg( int a  )
{
    return a;
}

int add( int a, int b )
{
    return a + b;
}

struct holder
{
    int v;

    holder( int v_)
        : v(v_)
    {}

    int val() const
    {
        return v;
    }

    int add( int a ) const
    {
        return v + a;
    }

    int add_constref( int const & a ) const
    {
        return v + a;
    }

    int add_nonconst( int a )
    {
        return v + a;
    }

    void change_arg( int & a )
    {
        a = v;
    }
};

struct functor
{
    int operator()() const
    {
        return 42;
    }

    int operator()(int i) const
    {
        return i;
    }
};

//
// invoke():
//

CASE("invoke: a free function, no arguments")
{
    EXPECT( nonstd::invoke( f42 ) == 42 );
}

CASE("invoke: a free function, no arguments, noexcept(false) (C++11)")
{
    EXPECT( nonstd::invoke( f42_nef ) == 42 );
}

CASE("invoke: a free function, 1 argument")
{
    EXPECT( nonstd::invoke( arg, 7 ) == 7 );
}

CASE("invoke: a free function, 2 arguments")
{
    EXPECT( nonstd::invoke( add, 1, 2 ) == 3 );
}

CASE("invoke: a member function, no arguments")
{
    const holder v(42);

    EXPECT( nonstd::invoke( &holder::val, v ) == 42 );
}

CASE("invoke: a member function, 1 argument")
{
    const holder v(1);

    EXPECT( nonstd::invoke( &holder::add, v, 2 ) == 3 );
}

CASE("invoke: a member function, 1 const ref argument")
{
    const holder v(1);

    EXPECT( nonstd::invoke( &holder::add_constref, v, 2 ) == 3 );
}

CASE("invoke: a non-const member function, 1 argument")
{
    /*const*/ holder v(1);

    EXPECT( nonstd::invoke( &holder::add_nonconst, v, 2 ) == 3 );
}

CASE("invoke: a member function, changing its reference argument")
{
    int arg = 7;
    holder v(42);

    nonstd::invoke( &holder::change_arg, v, arg );

    EXPECT( arg == 42 );
}

CASE("invoke: (access) a data member - via const object ref")
{
    const holder h(42);

    EXPECT( nonstd::invoke( &holder::v, h ) == 42 );
}

CASE("invoke: (access) a data member - via const object ptr")
{
    const holder h(42);

    EXPECT( nonstd::invoke( &holder::v, &h ) == 42 );
}

CASE("invoke: (access) a data member - via non-const object ref")
{
    holder h(42);

    EXPECT( nonstd::invoke( &holder::v, h ) == 42 );
}

CASE("invoke: (access) a data member - via non-const object ptr")
{
    holder h(42);

    EXPECT( nonstd::invoke( &holder::v, &h ) == 42 );
}

CASE("invoke: change an invoked (accessed) data member - via non-const object ref")
{
    holder h(42);

    nonstd::invoke( &holder::v, h ) = 7;

    EXPECT( h.v == 7 );
}

CASE("invoke: change an invoked (accessed) data member - via non-const object ptr")
{
    holder h(42);

    nonstd::invoke( &holder::v,  &h ) = 7;

    EXPECT( h.v == 7 );
}

CASE("invoke: a function object, no arguments")
{
    EXPECT( nonstd::invoke( functor() ) == 42 );
}

CASE("invoke: a function object, 1 argument")
{
    EXPECT( nonstd::invoke( functor(), 42 ) == 42 );
}

CASE("invoke: a lambda, no arguments")
{
    EXPECT( nonstd::invoke( [](){ return 42; } ) == 42 );
}

CASE("invoke: a lambda, 1 argument")
{
    EXPECT( nonstd::invoke( [](int a){ return a; }, 42 ) == 42 );
}

//
// apply():
//

//
// Other:
//

CASE("invoke: App" "[.invoke][.app]")
{
    std::cout << "App: ...\n";
}

CASE( "tweak header: Reads tweak header if supported " "[tweak]" )
{
#if invoke_HAVE_TWEAK_HEADER
    EXPECT( invoke_TWEAK_VALUE == 42 );
#else
    EXPECT( !!"Tweak header is not available (invoke_HAVE_TWEAK_HEADER: 0)." );
#endif
}

} // anonymous namespace
