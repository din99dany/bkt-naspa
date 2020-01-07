#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdio>
double InputFunction( double x ) 
{
    return x*x + 2 * x + 1;
}


class Cromozon
{
    public:
    
    Cromozon()
    {   
        
    }

    Cromozon( std::vector<int> encode )
    {   
        mEncode = encode;
        CalculateFromEncode( encode );
    }

    void GenerateRandom()
    {
        for ( int i = 0; i < mLenght; ++i )
        {   
            mEncode.push_back( rand() % 2 );
        }
        CalculateFromEncode( mEncode );
    }

    void Mutate()
    {
        double u = Cromozon::GetUniformVariable();
        if ( u < Cromozon::mMutationChance )
        {
            int place = Cromozon::GetRandomPlace();
            mEncode[ place ] = !mEncode[ place ];
            CalculateFromEncode( mEncode );
            std::cout<< id << " has mutated at gene " << place << "\n";
        }

    }

    void CalculateFromEncode( std::vector<int> encode )
    {
        int base10 = 0;

        for ( int i = 0; i < mLenght; ++i )
        {   
            base10 += encode[ i ] * pow( 2, i );
        }

        mX = mLimits.first + 
            (mLimits.second - mLimits.first) * base10 /
            ( pow( 2, mLenght ) - 1 );

        mScor = InputFunction( mX );
    }

    static int GetRandomPlace() { return rand() % mLenght; }

    std::pair<Cromozon, Cromozon > Mix( Cromozon toMix )
    {
        int rupere = Cromozon::GetRandomPlace();
        
        Cromozon c1 = *this;
        Cromozon c2 = toMix;

        std::cout<< "Mixing " << c1.id << " with " << c2.id << "\n";

        for ( int i = 0; i < rupere; ++i ) 
        {
            std::swap( c1.mEncode[ i ], c2.mEncode[ i ] );
        }

        return { c1, c2 };
    }

    void Print()
    {
        std::cout<<"i:" << id << " ";
        for ( int i = 0; i < mEncode.size(); ++i )
            std::cout<<mEncode[ i ];

        std::cout << " x:" << mX << " f(x):" << mScor << "\n";
    }

    void SetId( int x ) { id=x; }
    int GetId() { return id; }

    double GetScore()
    {
        return mScor;
    }

    static double GetUniformVariable()
    {
        std::random_device rd;  
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis( 0, 1 );
        return dis(gen);
    }
    
    static int mLenght;
    static int mPrecision;
    static double mMutationChance;
    static double mCrossoverChance;
    static std::pair<double, double> mLimits;
    
    private :

    std::vector<int> mEncode;
    double mScor;
    double mX;
    int id;
};

std::pair<double, double> Cromozon::mLimits = { 0,0 };
int Cromozon::mLenght = 0;
int Cromozon::mPrecision = 0;
double Cromozon::mMutationChance = 0;
double Cromozon::mCrossoverChance = 0;

int FindInterval( double x, std::vector<double> intervals );
std::vector< Cromozon > SelectNewPopulation( std::vector<Cromozon> V );
std::vector< Cromozon > CrossOver( std::vector<Cromozon> V );
std::vector< Cromozon > MutatePopulation( std::vector<Cromozon> V );
void PrintReorderPopulation( std::vector<Cromozon>& V );

void ReadData()
{
    double a, b, mate, corss, prec;

    std::cin >> a >> b >> mate >> corss >> prec;

    Cromozon::mLimits = {a,b};
    Cromozon::mLenght = floor(log2((b-a)*pow( 10, prec ) ));
    Cromozon::mPrecision = prec;
    Cromozon::mMutationChance = mate;
    Cromozon::mCrossoverChance = corss;
    std::cout<<Cromozon::mCrossoverChance << "\n";
}

int main()
{
    srand( time(NULL));
    freopen("date.out","w",stdout);
    freopen("date.in","r",stdin);

    int noOfGenerations, populationSize;

    std::cin >> noOfGenerations >> populationSize;
    ReadData();

    std::vector< Cromozon > V;
    std::vector< double > maxims;
    for ( int i = 0; i < populationSize; ++i )
    {
        Cromozon toAdd;
        V.push_back(toAdd);
        V[ i ].GenerateRandom();
    }
    
    while ( noOfGenerations-- )
    {   
        PrintReorderPopulation(V);
        auto newPopulation = SelectNewPopulation( V );
        auto mixPopulation = CrossOver( newPopulation );
        V = MutatePopulation( mixPopulation );
        
        double maxx = V[ 0 ].GetScore();
        for ( auto cromo : V )
        {
            maxx = std::max(maxx, cromo.GetScore());
        }
        maxims.push_back(maxx);
    }
    PrintReorderPopulation(V);

    for ( auto item : maxims )
    {
        std::cout << item << " ";
    }
}

std::vector< Cromozon > SelectNewPopulation( std::vector<Cromozon> V )
{
    double allScore = 0;
    for ( auto cromo : V )
        allScore += cromo.GetScore();
    
    std::vector< double > intervals;
    intervals.push_back( 0 );

    for ( int i = 0; i < V.size(); ++i )
    {   
        intervals.push_back( intervals.back() + V[ i ].GetScore() / allScore );
        std::cout << "[" << intervals[ i ] <<"," << intervals.back() <<"]";
        if ( i % 4 == 3 )
            std::cout<<"\n";
    }
    
    if ( V.size() % 4 != 0 )
    std::cout<<"\n";

    std::vector<Cromozon> res;
    for ( int i = 0; i < V.size(); ++i )
    {
        double u = Cromozon::GetUniformVariable();
        std::cout<< i << ": " << V[ i ].GetScore() / allScore << "\n";
        int index = FindInterval( u, intervals );
        res.push_back( V[ index ] );
    }

    return res;

}

std::vector< Cromozon > CrossOver( std::vector<Cromozon> V )
{
    std::vector< int > toMixIndex;
    std::vector< Cromozon > rest;

    std::cout <<"those were selected for mutation : ";
    for ( int i = 0; i < V.size(); ++i )
    {
        if ( Cromozon::GetUniformVariable() < Cromozon::mCrossoverChance )
        {
            toMixIndex.push_back( i );
            std::cout << V[ i ].GetId() << " ";
        }
        else
        {
            rest.push_back( V[ i ] );
        }
    }
    std::cout<<"\n";

    int i;
    for ( i = 0; i < toMixIndex.size() - 1; i += 2 )
    {   
        auto newPair = V[ toMixIndex[ i ] ].Mix( V[ toMixIndex[ i + 1 ] ] );
        rest.push_back( newPair.first );
        rest.push_back( newPair.second );
    }

    if ( i != toMixIndex.size() )
    {
        rest.push_back( V[ toMixIndex.back() ] );
    }

    return rest;

}

std::vector< Cromozon > MutatePopulation( std::vector<Cromozon> V )
{   
    for ( int i = 0; i < V.size(); ++i )
    {
        V[ i ].Mutate();
    }
    return V;
}

int FindInterval( double x, std::vector<double> intervals )
{
    int st = 0;
    int dr = intervals.size() - 2;   
    while ( st <= dr )
    {   
        int mid = ( st + dr ) / 2;
        if ( mid + 1 < intervals.size() && intervals[ mid ] <= x && x < intervals[ mid + 1 ] )
        {
            return mid;
        }
        else if ( intervals[ mid ] <= x )
        {
            st = mid + 1;
        }
        else
        {
            dr = mid - 1;
        }
    }
    return 0;
}

void PrintReorderPopulation( std::vector<Cromozon>& V )
{   
    std::cout << "------------------------------\n";

    for ( int i = 0; i < V.size(); ++i )
    {
        V[ i ].SetId( i );
        V[ i ].Print();
    }
}