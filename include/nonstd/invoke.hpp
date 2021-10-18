//
// Copyright (c) 2017 Martin Moene
//
// https://github.com/martinmoene/invoke-lite
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef NONSTD_INVOKE_LITE_HPP
#define NONSTD_INVOKE_LITE_HPP

#define invoke_lite_MAJOR  0
#define invoke_lite_MINOR  0
#define invoke_lite_PATCH  0

#define invoke_lite_VERSION  invoke_STRINGIFY(invoke_lite_MAJOR) "." invoke_STRINGIFY(invoke_lite_MINOR) "." invoke_STRINGIFY(invoke_lite_PATCH)

#define invoke_STRINGIFY(  x )  invoke_STRINGIFY_( x )
#define invoke_STRINGIFY_( x )  #x

// invoke-lite configuration:

#define invoke_invoke_DEFAULT  0
#define invoke_invoke_NONSTD   1
#define invoke_invoke_STD      2

// tweak header support:

#ifdef __has_include
# if __has_include(<nonstd/invoke.tweak.hpp>)
#  include <nonstd/invoke.tweak.hpp>
# endif
#define invoke_HAVE_TWEAK_HEADER  1
#else
#define invoke_HAVE_TWEAK_HEADER  0
//# pragma message("invoke.hpp: Note: Tweak header not supported.")
#endif

// invoke selection and configuration:

#ifndef  invoke_CONFIG_SELECT_INVOKE
# define invoke_CONFIG_SELECT_INVOKE  ( invoke_HAVE_STD_INVOKE ? invoke_invoke_STD : invoke_invoke_NONSTD )
#endif

#ifndef  invoke_USE_ALTERNATE_IMPL
# define invoke_USE_ALTERNATE_IMPL  0
#endif

// Control presence of exception handling (try and auto discover):

#ifndef invoke_CONFIG_NO_EXCEPTIONS
# if defined(_MSC_VER)
#  include <cstddef>    // for _HAS_EXCEPTIONS
# endif
# if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || (_HAS_EXCEPTIONS)
#  define invoke_CONFIG_NO_EXCEPTIONS  0
# else
#  define invoke_CONFIG_NO_EXCEPTIONS  1
# endif
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   invoke_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define invoke_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define invoke_CPLUSPLUS  __cplusplus
# endif
#endif

#define invoke_CPP98_OR_GREATER  ( invoke_CPLUSPLUS >= 199711L )
#define invoke_CPP11_OR_GREATER  ( invoke_CPLUSPLUS >= 201103L )
#define invoke_CPP14_OR_GREATER  ( invoke_CPLUSPLUS >= 201402L )
#define invoke_CPP17_OR_GREATER  ( invoke_CPLUSPLUS >= 201703L )
#define invoke_CPP20_OR_GREATER  ( invoke_CPLUSPLUS >= 202000L )

// Use C++17 std::invoke if available and requested:

#if invoke_CPP17_OR_GREATER && defined(__has_include )
# if __has_include( <invoke> )
#  define invoke_HAVE_STD_INVOKE  1
# else
#  define invoke_HAVE_STD_INVOKE  0
# endif
#else
# define  invoke_HAVE_STD_INVOKE  0
#endif

#define  invoke_USES_STD_INVOKE  ( (invoke_CONFIG_SELECT_INVOKE == invoke_invoke_STD) || ((invoke_CONFIG_SELECT_INVOKE == invoke_invoke_DEFAULT) && invoke_HAVE_STD_INVOKE) )

//
// Use standard C++17 version:
//

#if invoke_USES_STD_INVOKE

#pragma message ("*** Using std::invoke")

#include <functional>

namespace nonstd {
    using std::invoke;
}

#elif invoke_CPP11_OR_GREATER
#pragma message ("*** Using nonstd::invoke - C++11")

//
// C++11, code taken from http://en.cppreference.com/w/cpp/types/result_of:
//

#include <type_traits>
#include <utility>

#if invoke_USE_ALTERNATE_IMPL   // see issue #1
# include <functional>
#endif

// Compiler versions:
//
// MSVC++  6.0  _MSC_VER == 1200  invoke_COMPILER_MSVC_VERSION ==  60  (Visual Studio 6.0)
// MSVC++  7.0  _MSC_VER == 1300  invoke_COMPILER_MSVC_VERSION ==  70  (Visual Studio .NET 2002)
// MSVC++  7.1  _MSC_VER == 1310  invoke_COMPILER_MSVC_VERSION ==  71  (Visual Studio .NET 2003)
// MSVC++  8.0  _MSC_VER == 1400  invoke_COMPILER_MSVC_VERSION ==  80  (Visual Studio 2005)
// MSVC++  9.0  _MSC_VER == 1500  invoke_COMPILER_MSVC_VERSION ==  90  (Visual Studio 2008)
// MSVC++ 10.0  _MSC_VER == 1600  invoke_COMPILER_MSVC_VERSION == 100  (Visual Studio 2010)
// MSVC++ 11.0  _MSC_VER == 1700  invoke_COMPILER_MSVC_VERSION == 110  (Visual Studio 2012)
// MSVC++ 12.0  _MSC_VER == 1800  invoke_COMPILER_MSVC_VERSION == 120  (Visual Studio 2013)
// MSVC++ 14.0  _MSC_VER == 1900  invoke_COMPILER_MSVC_VERSION == 140  (Visual Studio 2015)
// MSVC++ 14.1  _MSC_VER >= 1910  invoke_COMPILER_MSVC_VERSION == 141  (Visual Studio 2017)
// MSVC++ 14.2  _MSC_VER >= 1920  invoke_COMPILER_MSVC_VERSION == 142  (Visual Studio 2019)

#if defined(_MSC_VER ) && !defined(__clang__)
# define invoke_COMPILER_MSVC_VER      (_MSC_VER )
# define invoke_COMPILER_MSVC_VERSION  (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900 ) ) )
#else
# define invoke_COMPILER_MSVC_VER      0
# define invoke_COMPILER_MSVC_VERSION  0
#endif

#define invoke_COMPILER_VERSION( major, minor, patch )  ( 10 * ( 10 * (major) + (minor) ) + (patch) )

#if defined(__clang__)
# define invoke_COMPILER_CLANG_VERSION  invoke_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
# define invoke_COMPILER_CLANG_VERSION  0
#endif

#if defined(__GNUC__) && !defined(__clang__)
# define invoke_COMPILER_GNUC_VERSION  invoke_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
# define invoke_COMPILER_GNUC_VERSION  0
#endif

// half-open range [lo..hi):
//#define invoke_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )

// Presence of language and library features:

#define invoke_HAVE( feature )  ( invoke_HAVE_##feature )

#ifdef _HAS_CPP0X
# define invoke_HAS_CPP0X  _HAS_CPP0X
#else
# define invoke_HAS_CPP0X  0
#endif

#define invoke_CPP11_90   (invoke_CPP11_OR_GREATER || invoke_COMPILER_MSVC_VER >= 1500)
#define invoke_CPP11_100  (invoke_CPP11_OR_GREATER || invoke_COMPILER_MSVC_VER >= 1600)
#define invoke_CPP11_120  (invoke_CPP11_OR_GREATER || invoke_COMPILER_MSVC_VER >= 1800)
#define invoke_CPP11_140  (invoke_CPP11_OR_GREATER || invoke_COMPILER_MSVC_VER >= 1900)

#define invoke_CPP14_000  (invoke_CPP14_OR_GREATER)
#define invoke_CPP17_000  (invoke_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define invoke_HAVE_CONSTEXPR_11           invoke_CPP11_140
#define invoke_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG \
                                        invoke_CPP11_120
#define invoke_HAVE_INITIALIZER_LIST       invoke_CPP11_120
#define invoke_HAVE_NOEXCEPT               invoke_CPP11_140
#define invoke_HAVE_NULLPTR                invoke_CPP11_100
#define invoke_HAVE_TYPE_TRAITS            invoke_CPP11_90
#define invoke_HAVE_STATIC_ASSERT          invoke_CPP11_100
#define invoke_HAVE_ADD_CONST              invoke_CPP11_90
#define invoke_HAVE_OVERRIDE               invoke_CPP11_90
#define invoke_HAVE_REMOVE_REFERENCE       invoke_CPP11_90

#define invoke_HAVE_TR1_ADD_CONST          (!! invoke_COMPILER_GNUC_VERSION )
#define invoke_HAVE_TR1_REMOVE_REFERENCE   (!! invoke_COMPILER_GNUC_VERSION )
#define invoke_HAVE_TR1_TYPE_TRAITS        (!! invoke_COMPILER_GNUC_VERSION )

// Presence of C++11 language features:

#define invoke_HAVE_CONSTEXPR_11           invoke_CPP11_140
#define invoke_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG \
                                        invoke_CPP11_120
#define invoke_HAVE_INITIALIZER_LIST       invoke_CPP11_120
#define invoke_HAVE_NOEXCEPT               invoke_CPP11_140
#define invoke_HAVE_NULLPTR                invoke_CPP11_100
#define invoke_HAVE_TYPE_TRAITS            invoke_CPP11_90
#define invoke_HAVE_STATIC_ASSERT          invoke_CPP11_100
#define invoke_HAVE_ADD_CONST              invoke_CPP11_90
#define invoke_HAVE_OVERRIDE               invoke_CPP11_90
#define invoke_HAVE_REMOVE_REFERENCE       invoke_CPP11_90

#define invoke_HAVE_TR1_ADD_CONST          (!! invoke_COMPILER_GNUC_VERSION )
#define invoke_HAVE_TR1_REMOVE_REFERENCE   (!! invoke_COMPILER_GNUC_VERSION )
#define invoke_HAVE_TR1_TYPE_TRAITS        (!! invoke_COMPILER_GNUC_VERSION )

// Presence of C++14 language features:

#define invoke_HAVE_CONSTEXPR_14           invoke_CPP14_000

// Presence of C++17 language features:

#define invoke_HAVE_NODISCARD              invoke_CPP17_000

// Presence of C++ language features:

#if invoke_HAVE_CONSTEXPR_11
# define invoke_constexpr constexpr
#else
# define invoke_constexpr /*constexpr*/
#endif

#if invoke_HAVE_CONSTEXPR_14
# define invoke_constexpr14 constexpr
#else
# define invoke_constexpr14 /*constexpr*/
#endif

#if invoke_HAVE_NOEXCEPT
# define invoke_noexcept noexcept
#else
# define invoke_noexcept /*noexcept*/
#endif

#if invoke_HAVE_NULLPTR
# define invoke_nullptr nullptr
#else
# define invoke_nullptr NULL
#endif

#if invoke_HAVE_NODISCARD
# define invoke_nodiscard [[nodiscard]]
#else
# define invoke_nodiscard /*[[nodiscard]]*/
#endif

#if invoke_HAVE_OVERRIDE
# define invoke_override override
#else
# define invoke_override /*override*/
#endif

// additional includes:

#if invoke_CONFIG_NO_EXCEPTIONS
# include <cassert>
#else
# include <typeinfo>
#endif

#if ! invoke_HAVE_NULLPTR
# include <cstddef>
#endif

#if invoke_HAVE_INITIALIZER_LIST
# include <initializer_list>
#endif

#if invoke_HAVE_TYPE_TRAITS
# include <type_traits>
#elif invoke_HAVE_TR1_TYPE_TRAITS
# include <tr1/type_traits>
#endif

namespace nonstd {
namespace detail {

#if invoke_USE_ALTERNATE_IMPL   // see issue #1

#pragma message ("*** Using alternate version")

// C++11 implementation contributed by Peter Featherstone, @pfeatherstone

template< typename F, typename ... Args >
auto INVOKE( F&& fn, Args&& ... args )
-> typename std::enable_if<
    std::is_member_pointer<typename std::decay<F>::type>::value,
    decltype(std::mem_fn(fn)( std::forward<Args>(args)...) )>::type
{
    return std::mem_fn(fn)( std::forward<Args>(args)...);
}

template< typename F, typename ... Args >
auto INVOKE( F&& fn, Args&& ... args )
-> typename std::enable_if<
    ! std::is_member_pointer<typename std::decay<F>::type>::value,
    decltype(std::forward<F>(fn)( std::forward<Args>(args)...) )>::type
{
    return std::forward<F>(fn)(std::forward<Args>(args)...);
}

#else // invoke_USE_ALTERNATE_IMPL

template< typename T > struct is_reference_wrapper : std::false_type {};
template< typename U > struct is_reference_wrapper<std::reference_wrapper<U>> : std::true_type {};

template <typename Base, typename T, typename Derived, typename... Args>
auto INVOKE( T Base::*pmf, Derived && ref, Args &&... args )
    -> typename std::enable_if<
        std::is_function<T>::value
        && std::is_base_of<Base, typename std::decay<Derived>::type>::value
        , decltype((std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...))>::type
{
    return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
}

template< typename Base, typename T, typename RefWrap, typename... Args >
auto INVOKE( T Base::*pmf, RefWrap && ref, Args &&... args )
    -> typename std::enable_if<
        std::is_function<T>::value
        && is_reference_wrapper<typename std::decay<RefWrap>::type>::value
        , decltype((ref.get().*pmf)(std::forward<Args>(args)...))>::type
{
    return (ref.get().*pmf)(std::forward<Args>(args)...);
}

template< typename Base, typename T, typename Ptr, typename... Args>
auto INVOKE( T Base::*pmf, Ptr && ptr, Args &&... args )
    -> typename std::enable_if<
        std::is_function<T>::value
        && !is_reference_wrapper<typename std::decay<Ptr>::type>::value
        && !std::is_base_of<Base, typename std::decay<Ptr>::type>::value
        , decltype(((*std::forward<Ptr>(ptr)).*pmf)(std::forward<Args>(args)...))>::type
{
    return ((*std::forward<Ptr>(ptr)).*pmf)( std::forward<Args>( args )...);
}

template< typename Base, typename T, typename Derived>
auto INVOKE( T Base::*pmd, Derived && ref )
    -> typename std::enable_if<
        !std::is_function<T>::value
        && std::is_base_of<Base, typename std::decay<Derived>::type>::value
        , decltype(std::forward<Derived>(ref).*pmd)>::type
{
    return std::forward<Derived>(ref).*pmd;
}

template< typename Base, typename T, typename RefWrap>
auto INVOKE( T Base::*pmd, RefWrap && ref )
    -> typename std::enable_if<
        !std::is_function<T>::value
        && is_reference_wrapper<typename std::decay<RefWrap>::type>::value
        , decltype(ref.get().*pmd)>::type
{
    return std::forward<RefWrap>(ref.get()).*pmd;
}

template< typename Base, typename T, typename Ptr>
auto INVOKE( T Base::*pmd, Ptr && ptr )
    -> typename std::enable_if<
        !std::is_function<T>::value
        && !is_reference_wrapper<typename std::decay<Ptr>::type>::value
        && !std::is_base_of<Base, typename std::decay<Ptr>::type>::value
        , decltype((*std::forward<Ptr>(ptr)).*pmd)>::type
{
    return (*std::forward<Ptr>(ptr)).*pmd;
}

template< typename F, typename... Args>
auto INVOKE( F && f, Args &&... args )
 -> typename std::enable_if<
    !std::is_member_pointer<typename std::decay<F>::type>::value
    , decltype(std::forward<F>(f)(std::forward<Args>(args)...))>::type
{
    return std::forward<F>( f )( std::forward<Args>( args )...);
}

#endif // invoke_USE_ALTERNATE_IMPL

} // namespace detail

// conforming C++14 implementation (is also a valid C++11 implementation):

namespace detail {

template< typename AlwaysVoid, typename, typename...>
struct invoke_result {};

template< typename F, typename... Args >
struct invoke_result< decltype( void(detail::INVOKE(std::declval<F>(), std::declval<Args>()...)) ), F, Args...>
{
    using type = decltype( detail::INVOKE(std::declval<F>(), std::declval<Args>()...) );
};

//template< typename Fn, typename... ArgTypes >
//struct is_nothrow_invocable : std::false_type{};;

} // namespace detail

template< typename > struct result_of;

template< typename F, typename... ArgTypes >
struct result_of< F(ArgTypes...) > : detail::invoke_result< void, F, ArgTypes...> {};

template< typename F, typename... ArgTypes >
struct invoke_result : detail::invoke_result< void, F, ArgTypes...> {};

template< typename F, typename... Args >
typename invoke_result< F, Args...>::type invoke( F && f, Args &&... args )
// noexcept( detail::is_nothrow_invocable<F, Args...>::value )
{
    return detail::INVOKE( std::forward<F>( f ), std::forward<Args>( args )...);
}

} // namespace nonstd

#else // not C++17, not C++11

#pragma message ("*** Using nonstd::invoke - C++98")

//
// C++98, code based on proposal n1454.
//
// N1454. A uniform method for computing function object return types (revision 1),
// by Douglas Gregor, http://wg21.link/n1454

namespace nonstd {
namespace std98 {

// type traits:

template< typename T, T v >
struct integral_constant
{
   typedef integral_constant< T, v > type;
   typedef T value_type;
   static const T value = v;
};

typedef integral_constant< bool, true  > true_type;
typedef integral_constant< bool, false > false_type;

//template< typename T > struct remove_reference     { typedef T type; };
//template< typename T > struct remove_reference<T&> { typedef T type; };
////template< typename T > struct remove_reference<T&&> {typedef T type;};
//
//template< typename T > struct add_lvalue_reference     { typedef T& type; };
//template< typename T > struct add_lvalue_reference<T&> { typedef T& type; };

template< typename T > struct remove_const          { typedef T type; };
template< typename T > struct remove_const<const T> { typedef T type; };

template< typename T > struct remove_volatile             { typedef T type; };
template< typename T > struct remove_volatile<volatile T> { typedef T type; };

template< typename T > struct remove_cv { typedef typename remove_volatile<typename remove_const<T>::type>::type type; };

template< typename T > struct is_const          : false_type {};
template< typename T > struct is_const<const T> : true_type {};

template< typename T > struct is_pointer_helper     : false_type {};
template< typename T > struct is_pointer_helper<T*> : true_type {};
template< typename T > struct is_pointer : is_pointer_helper<typename remove_cv<T>::type> {};

template< typename T >              struct is_member_pointer_helper : false_type {};
template< typename T , typename U > struct is_member_pointer_helper<T U::*> : true_type {};
template< typename T >              struct is_member_pointer : is_member_pointer_helper<typename remove_cv<T>::type> {};

template< bool B, typename T = void > struct enable_if {};
template<         typename T        > struct enable_if< true, T > { typedef T type; };

// result type deduction:

namespace detail {

template< typename F >
struct forwardN;

} // namespace detail

template< typename F >
struct result_of;

template< typename F >
struct result_of<F()> { typedef typename detail::forwardN<F>::template result<F>::type type; };

template< typename F, typename T1 >
struct result_of<F(T1)> { typedef typename detail::forwardN<F>::template result<T1>::type type; };

template< typename F, typename T1, typename T2 >
struct result_of<F(T1,T2)> { typedef typename detail::forwardN<F>::template result<T1,T2>::type type; };

namespace detail {

template< typename F >
struct forwardN
{
    F f;

    template< typename T> struct result { typedef T type; };
    template< typename R> struct result<R (*)() > { typedef R type; };
//    template< typename R, typename C > struct result< R (C::*)() > { typedef R type; };

    template< typename T, typename T1 > struct result<       T(T1) > { typedef typename result_of<       F( T1 ) >::type type; };
    template< typename T, typename T1 > struct result< const T(T1) > { typedef typename result_of< const F( T1 ) >::type type; };

    template< typename T, typename T1, typename T2 > struct result<       T(T1, T2) > { typedef typename result_of<       F( T1, T2 ) >::type type; };
    template< typename T, typename T1, typename T2 > struct result< const T(T1, T2) > { typedef typename result_of< const F( T1, T2 ) >::type type; };

    template< typename T1> typename result<       forwardN(T1) >::type operator()( T1 & t1 )       { return f( t1 ); }
    template< typename T1> typename result< const forwardN(T1) >::type operator()( T1 & t1 ) const { return f( t1 ); }

    template< typename T1, typename T2> typename result<       forwardN(T1, T2) >::type operator()( T1 & t1, T2 & t2 )       { return f( t1, t2 ); }
    template< typename T1, typename T2> typename result< const forwardN(T1, T2) >::type operator()( T1 & t1, T2 & t2 ) const { return f( t1, t2 ); }
};

} // namespace detail

// limited set of invoke():
// - 0..2 arguments
// - no mix of const and non-const reference arguments
// - ...

template< typename F>
typename result_of<F()>::type
invoke( F f ) { return f(); }

template< typename F, typename T1 >
typename enable_if< !is_member_pointer<F>::value, typename result_of<F(T1)>::type >::type
invoke( F f, T1 const & t1 ) { return f( t1 ); }

template< typename F, typename T1, typename T2, int >
typename enable_if< !is_member_pointer<F>::value, typename result_of<F(T1,T2)>::type >::type
invoke( F f, T1 const & t1, T2 const & t2 ) { return f( t1, t2 ); }

template< typename Base, typename T, typename Obj>
typename enable_if< !is_pointer<Obj>::value && !is_const<Obj>::value, T& >::type
invoke( T Base::* pmd, Obj & ref ) { return ref.*pmd; }

template< typename Base, typename T, typename Obj>
typename enable_if< !is_pointer<Obj>::value && is_const<Obj>::value, T >::type
invoke( T const Base::* pmd, Obj & ref ) { return ref.*pmd; }

template< typename Base, typename T, typename Obj>
typename enable_if< !is_const<Obj>::value, T& >::type
invoke( T Base::* pmd, Obj * ptr ) { return (*ptr).*pmd; }

template< typename Base, typename T, typename Obj>
typename enable_if< is_const<Obj>::value, T >::type
invoke( T const Base::*pmd, Obj * ptr ) { return (*ptr).*pmd; }

template< typename R, typename Base, typename Obj> R invoke( R (Base::*pmf)() const, Obj const & ref )  { return (ref.*pmf)(); }
template< typename R, typename Base, typename Obj> R invoke( R (Base::*pmf)()      , Obj       & ref )  { return (ref.*pmf)(); }

template< typename R, typename Base, typename Obj> R invoke( R (Base::*pmf)() const, Obj       * ptr )  { return ((*ptr).*pmf)(); }
template< typename R, typename Base, typename Obj> R invoke( R (Base::*pmf)()      , Obj       * ptr )  { return ((*ptr).*pmf)(); }

template< typename R, typename Base, typename Obj, typename T1 > R invoke( R (Base::*pmf)(T1) const, Obj const & ref, T1 const & t1 ) { return (ref.*pmf)( t1 ); }
template< typename R, typename Base, typename Obj, typename T1 > R invoke( R (Base::*pmf)(T1)      , Obj       & ref, T1 const & t1 ) { return (ref.*pmf)( t1 ); }
template< typename R, typename Base, typename Obj, typename A1
                                                 , typename T1 > R invoke( R (Base::*pmf)(A1)      , Obj       & ref, T1       & t1 ) { return (ref.*pmf)( t1 ); }

template< typename R, typename Base, typename Obj, typename T1 > R invoke( R (Base::*pmf)(T1) const, Obj const * ptr, T1 const & t1 ) { return ((*ptr).*pmf)( t1 ); }
template< typename R, typename Base, typename Obj, typename T1 > R invoke( R (Base::*pmf)(T1)      , Obj       * ptr, T1 const & t1 ) { return ((*ptr).*pmf)( t1 ); }
template< typename R, typename Base, typename Obj, typename A1
                                                 , typename T1 > R invoke( R (Base::*pmf)(A1)      , Obj       * ptr, T1       & t1 ) { return ((*ptr).*pmf)( t1 ); }

template< typename R, typename Base, typename Obj, typename T1, typename T2 > R invoke( R (Base::*pmf)(T1,T2) const, Obj const & ref, T1 const & t1 , T2 const & t2 ) { return (ref.*pmf)( t1, t2 ); }
template< typename R, typename Base, typename Obj, typename T1, typename T2 > R invoke( R (Base::*pmf)(T1,T2)      , Obj       & ref, T1 const & t1 , T2 const & t2 ) { return (ref.*pmf)( t1, t2 ); }
template< typename R, typename Base, typename Obj, typename A1, typename A2
                                                 , typename T1, typename T2 > R invoke( R (Base::*pmf)(A1,A2)      , Obj       & ref, T2       & t1 , T2       & t2 ) { return (ref.*pmf)( t1, t2 ); }

template< typename R, typename Base, typename Obj, typename T1, typename T2 > R invoke( R (Base::*pmf)(T1,T2) const, Obj const * ptr, T1 const & t1 , T2 const & t2 ) { return ((*ptr).*pmf)( t1, t2 ); }
template< typename R, typename Base, typename Obj, typename T1, typename T2 > R invoke( R (Base::*pmf)(T1,T2)      , Obj       * ptr, T1 const & t1 , T2 const & t2 ) { return ((*ptr).*pmf)( t1, t2 ); }
template< typename R, typename Base, typename Obj, typename A1, typename A2
                                                 , typename T1, typename T2 > R invoke( R (Base::*pmf)(A1,A2)      , Obj       * ptr, T2       & t1 , T2       & t2 ) { return ((*ptr).*pmf)( t1, t2 ); }

} // namespace std98

using namespace std98;

} // namespace nonstd

#endif // invoke_CPP17_OR_GREATER, invoke_CPP11_OR_GREATER

#endif // NONSTD_INVOKE_LITE_HPP
