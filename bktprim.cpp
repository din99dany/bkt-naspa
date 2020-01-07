#include <iostream>
#include <vector>

std::vector<int> res;
std::vector< int > Primes;
void Decimate( int n, int backtrash=0 );

int main()
{
    int n;
    std::cin >> n;

    Decimate(n);
    return 0;
}

bool Prime( int x )
{   
    if ( x <= 1 )
    {
        return false;
    }

    for ( int i = 2; i * i <= x; ++i )
    {
        if ( x % i == 0 )
        {
            return false;
        }
    }

    return true;
}

void Decimate( int n, int backtrash )
{
    static bool primeGen = true;

    if ( primeGen )
    {   
        primeGen = false;
        for ( int i = 2; i <= n; ++i )
        {
            if ( Prime( i ) ) 
            {
                Primes.push_back( i );
            }
        }
        res.push_back(0);
    }

    if ( backtrash < n )
    {
        for ( auto item : Primes )
        {   
            if ( item < res.back() )
                continue;

            res.push_back(item);
            Decimate( n, backtrash + item );
            res.pop_back();
        }

    }
    else if ( backtrash == n )
    {
        for( auto item : res ) 
        {   
            if ( item != 0 )
            {
                std :: cout << item << " ";
            }
        }
        std::cout << "\n";
    }

}