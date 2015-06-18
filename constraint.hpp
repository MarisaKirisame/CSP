#ifndef CONSTRAINT_HPP
#define CONSTRAINT_HPP
namespace CSP
{
    template< typename VARIABLE_ID_T, typename VARIABLE_T >
    struct local_constraint
    {
        std::vector< VARIABLE_ID_T > related_var;
        std::function< bool( std::vector< std::reference_wrapper< const VARIABLE_T > > ) > predicate;
        local_constraint(
            const std::vector< VARIABLE_ID_T > & v,
            const std::function< bool( std::vector< std::reference_wrapper< const VARIABLE_T > > ) > & f ) :
            related_var( v ), predicate( f ) { }
        bool operator ( )( const std::map< VARIABLE_ID_T, VARIABLE_T > & partial_assignment ) const
        {
            std::vector< std::reference_wrapper< const VARIABLE_T > > arg;
            arg.reserve( related_var.size( ) );
            for ( const VARIABLE_ID_T & i : related_var )
            {
                auto it = partial_assignment.find( i );
                if ( it == partial_assignment.end( ) ) { return true; }
                arg.push_back( it->second );
            }
            return predicate( arg );
        }
    };
    template< typename VARIABLE_ID_T, typename VARIABLE_T >
    local_constraint< VARIABLE_ID_T, VARIABLE_T > make_local_constraint(
        const std::vector< VARIABLE_ID_T > & v,
        const std::function< bool( std::vector< std::reference_wrapper< const VARIABLE_T > > ) > & f )
    { return local_constraint< VARIABLE_ID_T, VARIABLE_T >( v, f ); }
}
#endif // CONSTRAINT_HPP
