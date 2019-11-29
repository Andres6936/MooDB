#include <iostream>
#include <vector>
#include <string>

class Database;

class Table
{

private:

    static bool StringVectorHas( const std::vector <std::string> &vec, const std::string &substr )
    {
        for ( const std::string &i : vec )
        {
            if ( i.find( substr ) != std::string::npos )
            {
                return true;
            }
        }

        return false;
    }

public:

    std::vector <std::string> Columns; // A horizonal vector containing the number of columns and their names
    std::vector <std::vector <std::string> > Data; // A vertical vector (list) of horizontal vectors containing string data

    Database *parentDB = nullptr;
    std::string Name;

    void RefreshFile( );

    void Insert( std::vector <std::string> data )
    {
        if ( data.size( ) != Columns.size( ))
        {
            throw "Inserted data does not match columns";
        }
        if ( StringVectorHas( data, "{" ) || StringVectorHas( data, "}" ) || StringVectorHas( data, "" ))
        {
            throw "Inserted data contains a forbidden character";
        }
        Data.push_back( data );
    }

    void Print( )
    {
        std::vector <int> ColumnOffsets;

        for ( int i = 0; i < Columns.size( ); i++ )
        {
            ColumnOffsets.push_back( -1 );
        }

        for (std::vector <std::string> & i : Data)
        {
            for ( int j = 0; j < i.size( ); j++ )
            {
                if (( i[ j ].length( ) - Columns[ j ].length( ) > ColumnOffsets[ j ] ||
                      ColumnOffsets[ j ] == -1 ) && i[ j ].length( ) > Columns[ j ].length( ))
                {
                    ColumnOffsets[ j ] = i[ j ].length( ) - Columns[ j ].length( );
                }
            } // we are looping horizontally
        } // we are looping vertically

        std::cout << std::endl << Name << ":" << std::endl;

        std::string Headers;
        std::string Seperator;

        Headers += "| ";
        for ( int i = 0; i < Columns.size( ); i++ )
        {
            Headers += Columns[ i ];
            if ( ColumnOffsets[ i ] != -1 )
            {
                for ( int j = 0; j < ColumnOffsets[ i ]; j++ )
                {
                    Headers += " ";
                }
            }
            Headers += " | ";
        }

        Seperator += "+";
        for ( int i = 0; i < Headers.length( ) - 3; i++ )
        {
            if ( Headers.at( i + 1 ) == '|' )
            {
                Seperator += "+";
            }
            else
            {
                Seperator += "-";
            }
        }

        Seperator += "+";

        std::cout << Seperator << "\n" << Headers << "\n" << Seperator;

        for (std::vector <std::string> & i : Data)
        {
            std::cout << "\n| ";
            for ( int j = 0; j < i.size( ); j++ )
            {
                std::cout << i[ j ];

                if ( ColumnOffsets[ j ] != -1 )
                {
                    if ( i[ j ].length( ) < ( ColumnOffsets[ j ] + Columns[ j ].length( )))
                    {
                        for ( int p = 0;
                              p < ( ColumnOffsets[ j ] + Columns[ j ].length( )) - i[ j ].length( ); p++ )
                        {
                            std::cout << " ";
                        }
                    }
                }
                else
                {
                    for ( int p = 0; p < Columns[ j ].length( ) - i[ j ].length( ); p++ )
                    {
                        std::cout << " ";
                    }
                }

                std::cout << " | ";
            } // we are looping horizontally
        } // we are looping vertically

        std::cout << std::endl << Seperator << std::endl;
    }

    Table( std::string tableName, std::vector <std::string> columns )
    {
        Name = tableName;
        Columns = columns;
    }

    std::vector <std::string> GetRow( const int Index )
    {
        if ( Index > Data.size( ) || Index < 0 )
        {
            throw "Row at index " + std::to_string( Index ) + " is out of bounds";
        }

        return Data[ Index ];
    }

    std::vector <std::vector <std::string> >
    GetRowsWithValue( const std::string &nColumn, const std::string &ColumnValue )
    {
        std::vector <std::vector <std::string> > ret;

        std::vector <std::string> vColumn = GetColumn( nColumn );

        for ( int i = 0; i < vColumn.size( ); i++ )
        {
            if ( vColumn[ i ] == ColumnValue )
            {
                ret.push_back( GetRow( i ));
            }
        }

        return ret;
    } // will return a vector of vectors of strings where that row's column value equals the 'ColumnValue'

    std::vector <std::string> GetColumn( const std::string &ColumnName )
    {
        std::vector <std::string> ret;

        int ColumnIndex = -1;

        for ( int i = 0; i < Columns.size( ); i++ )
        {
            if ( Columns[ i ] == ColumnName )
            {
                ColumnIndex = i;
                break;
            }
        }

        if ( ColumnIndex == -1 )
        {
            throw "Could not find column '" + ColumnName + "'";
        }

        for ( std::vector <std::string> &i : Data )
        {
            ret.push_back( i[ ColumnIndex ] );
        }

        return ret;
    }
};