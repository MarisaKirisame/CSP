#ifndef TEST_HPP
#define TEST_HPP
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test.hpp>
using namespace CSP;
BOOST_AUTO_TEST_CASE( CSP_test )
{
    {
        enum var { F, T, U, W, R, O, C1, C2, C3 };
        std::vector< std::map< var, size_t > > result;
        std::list< size_t > digits { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        std::list< size_t > carry { 0, 1 };
        std::list< local_constraint< var, size_t > > con;
        con.push_back(
            make_local_constraint< var, size_t >(
                std::vector< var >( { O, R, C1 } ),
                [&]( const std::vector< std::reference_wrapper< const size_t > > & ass )
                { return ass[0] * 2 == ass[1] + ass[2] * 10; } ) );
        con.push_back(
            make_local_constraint< var, size_t >(
                std::vector< var >( { O, R } ),
                [&]( const std::vector< std::reference_wrapper< const size_t > > & ass )
                { return ( ass[0] * 2 - ass[1] ) % 10 == 0; } ) );
        con.push_back(
            make_local_constraint< var, size_t >(
                std::vector< var >( { C1, W, U, C2 } ),
                [&]( const std::vector< std::reference_wrapper< const size_t > > & ass )
                { return ass[0] + ass[1] * 2 == ass[2] + 10 * ass[3]; } ) );
        con.push_back(
            make_local_constraint< var, size_t >(
                std::vector< var >( { C1, W, U } ),
                [&]( const std::vector< std::reference_wrapper< const size_t > > & ass )
                { return ( ass[0] + ass[1] * 2 - ass[2] ) % 10 == 0; } ) );
        con.push_back(
            make_local_constraint< var, size_t >(
                std::vector< var >( { C2, T, O, C3 } ),
                [&]( const std::vector< std::reference_wrapper< const size_t > > & ass )
                { return ass[0] + ass[1] * 2 == ass[2] + 10 * ass[3]; } ) );
        con.push_back(
            make_local_constraint< var, size_t >(
                std::vector< var >( { C2, T, O } ),
                [&]( const std::vector< std::reference_wrapper< const size_t > > & ass )
                { return ( ass[0] + ass[1] * 2 - ass[2] ) % 10 == 0; } ) );
        con.push_back(
            make_local_constraint< var, size_t >(
                std::vector< var >( { C3, F } ),
                [&]( const std::vector< std::reference_wrapper< const size_t > > & ass )
                { return ass[0] == ass[1]; } ) );
        con.push_back(
            make_local_constraint< var, size_t >(
                std::vector< var >( { F } ),
                [&]( const std::vector< std::reference_wrapper< const size_t > > & ass )
                { return ass[0] != 0; } ) );
        for ( var i : { F, T, U, W, R, O } )
        {
            for ( var j : { F, T, U, W, R, O } )
            {
                if ( i > j )
                {
                    con.push_back(
                        make_local_constraint< var, size_t >(
                            std::vector< var >( { i, j } ),
                            [&]( const std::vector< std::reference_wrapper< const size_t > > & ass )
                            { return ass[0] != ass[1]; } ) );
                }
            }
        }
        backtracking_search(
            std::map< var, std::list< size_t > >(
            {
                {F, digits},
                {O, digits},
                {U, digits},
                {R, digits},
                {T, digits},
                {W, digits},
                {C1, carry},
                {C2, carry},
                {C3, carry}
            } ),
            4,
            con,
            std::back_inserter( result ) );
        BOOST_CHECK_EQUAL( result.size( ), 7 );
        for ( const std::map< var, size_t > & ass : result )
        {
            for ( const auto & c : con )
            { BOOST_CHECK( c( ass ) ); }
        }
    }
    {
        enum var { A = 0, B = 1, C = 2, D = 3 };
        std::vector< std::map< size_t, var > > result;
        std::list< var > answers { A, B, C, D };
        std::list< local_constraint< size_t, var > > con;
        auto make_equal =
            [&]( var v, size_t s, size_t l, size_t r )
            {
                con.push_back(
                    make_local_constraint< size_t, var >(
                    { s, l, r },
                    [=]( const std::vector< std::reference_wrapper< const var > > & vec )
                    { return vec[0] != v || vec[1] == vec[2]; } ) );
            };
        auto make_constrain_2 =
            [&]( var a, var b )
            {
                con.push_back( make_local_constraint< size_t, var >
                (
                    { 2, 5 },
                    [=]( const std::vector< std::reference_wrapper< const var > > & vec )
                    { return vec[0] != a || vec[1] == b; }
                ) );
            };
            make_constrain_2( A, C );
            make_constrain_2( B, D );
            make_constrain_2( C, A );
            make_constrain_2( D, B );
        auto make_constrain_3 =
            [&]( var v, size_t x )
            {
                con.push_back( make_local_constraint< size_t, var >
                (
                    { 3, 6, 2, 4 },
                    [=]( const std::vector< std::reference_wrapper< const var > > & vec )
                    {
                        return vec[0] != v ||
                                ( vec[x] != vec[1] && vec[x] != vec[2] && vec[x] != vec[3] );
                    }
                ) );
            };
        make_constrain_3( A, 0 );
        make_constrain_3( B, 1 );
        make_constrain_3( C, 2 );
        make_constrain_3( D, 3 );
        make_equal( A, 5, 5, 8 );
        make_equal( B, 5, 5, 4 );
        make_equal( C, 5, 5, 9 );
        make_equal( D, 5, 5, 7 );
        make_equal( A, 4, 1, 5 );
        make_equal( B, 4, 2, 7 );
        make_equal( C, 4, 1, 9 );
        make_equal( D, 4, 6, 10 );
        make_equal( A, 6, 2, 4 );
        make_equal( B, 6, 1, 6 );
        make_equal( C, 6, 3, 10 );
        make_equal( D, 6, 5, 9 );
        make_equal( A, 6, 2, 8 );
        make_equal( B, 6, 1, 8 );
        make_equal( C, 6, 3, 8 );
        make_equal( D, 6, 5, 8 );
        auto make_min =
            [&]( var answer, var min_count )
            {
                con.push_back( make_local_constraint< size_t, var >
                (
                    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
                    [=]( const std::vector< std::reference_wrapper< const var > > & vec )
                    {
                        size_t
                            A_count = std::count( vec.begin( ), vec.end( ), A ),
                            B_count = std::count( vec.begin( ), vec.end( ), B ),
                            C_count = std::count( vec.begin( ), vec.end( ), C ),
                            D_count = std::count( vec.begin( ), vec.end( ), D );
                        return vec[6] != answer ||
                                ( ( min_count == A ?
                                        A_count :
                                        ( min_count == B ?
                                            B_count :
                                            ( min_count == C ?
                                                C_count : D_count ) ) ) ==
                                std::min( std::min( A_count, B_count ), std::min( C_count, D_count ) ) );
                    }
                ) );
            };
        auto make_minmax =
            [&]( var answer, size_t difference )
            {
                con.push_back(
                    make_local_constraint< size_t, var > (
                        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
                        [=]( const std::vector< std::reference_wrapper< const var > > & vec )
                        {
                            size_t
                                A_count = std::count( vec.begin( ), vec.end( ), A ),
                                B_count = std::count( vec.begin( ), vec.end( ), B ),
                                C_count = std::count( vec.begin( ), vec.end( ), C ),
                                D_count = std::count( vec.begin( ), vec.end( ), D );
                            return vec[9] != answer ||
                                ( ( std::max( std::max( A_count, B_count ), std::max( C_count, D_count ) ) -
                                  std::min( std::min( A_count, B_count ), std::min( C_count, D_count ) ) ) ==
                                difference );
                        }
                ) );
            };
        auto non_adjacent =
            []( var a, var b )
            {
                switch ( a )
                {
                    case A:
                        return b == D || b == C;
                    case B:
                        return b == D;
                    case C:
                        return b == A;
                    case D:
                        return b == A || b == B;
                }
                throw std::invalid_argument( "unknown enum value." );
            };
        auto make_constrain_8 =
            [&]( var answer, size_t question )
            {
                con.push_back( make_local_constraint< size_t, var >
                (
                    { 8, 1, question },
                    [=]( const std::vector< std::reference_wrapper< const var > > & vec )
                    { return vec[0] != answer || non_adjacent( vec[1], vec[2] ); }
                ) );
            };
        auto make_constrain_9 =
            [&]( var answer, size_t question )
            {
                con.push_back( make_local_constraint< size_t, var >
                (
                    { 9, 1, 6, question, 5 },
                    [=]( const std::vector< std::reference_wrapper< const var > > & vec )
                    { return vec[0] != answer || ( ( vec[1] == vec[2] ) != ( vec[3] == vec[4] ) ); }
                ) );
            };
        make_constrain_8( A, 7 );
        make_constrain_8( B, 5 );
        make_constrain_8( C, 2 );
        make_constrain_8( D, 10 );
        make_constrain_9( A, 6 );
        make_constrain_9( B, 10 );
        make_constrain_9( C, 2 );
        make_constrain_9( D, 9 );
        make_min( A, C );
        make_min( B, B );
        make_min( C, A );
        make_min( D, D );
        make_minmax( A, 3 );
        make_minmax( B, 2 );
        make_minmax( C, 4 );
        make_minmax( D, 1 );
        backtracking_search(
            std::map< size_t, std::list< var > >(
            {
                {1, answers},
                {2, answers},
                {3, answers},
                {4, answers},
                {5, answers},
                {6, answers},
                {7, answers},
                {8, answers},
                {9, answers},
                {10, answers}
            } ),
            10,
            con,
            std::back_inserter( result ) );
        for ( const std::map< size_t, var > & ass : result )
        {
            for ( const auto & p : con )
            { BOOST_CHECK( p( ass ) ); }
        }
        BOOST_CHECK( result.size( ) == 1 );
    }
}

#endif // TEST_HPP
