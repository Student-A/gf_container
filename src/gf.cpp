#include <boost/multi_array.hpp>
#include <boost/timer/timer.hpp>
#include <boost/multi_array.hpp>
#include <iostream>
#include <complex>
#include <gf.h>

using namespace std; 

const int N = 400; 

typedef complex<double> dcomplex; 

int main()
{
   cout << " Starting main " << endl; 

   // Enum that contains names of gf indeces
   enum class MIXED{ w, W };

   // Double-valued 2-dimensional gf
   using mygf_t = gf< double, 2 >;

   // Create object with one fermionic and one bosonic frequency
   mygf_t my_gf( boost::extents[ffreq(N)][bfreq(N)] ); 

   // Initialize values with a lambda function
   my_gf.init( []( const mygf_t::idx_t& idx )->double{ return idx(0) + N * idx(1); } ); 

   // Fill a vector with all possible indeces
   std::vector< mygf_t::idx_t > idx_lst; 
   my_gf.fill_idx_lst( idx_lst ); 

   // Consider the element x,y
   int x = 300;
   int y = 300; 
   mygf_t::idx_t idx( { x, y } ); 

   // Access index object with names
   cout << " idx(w) " << idx( MIXED::w ) << endl; 
   cout << " idx(W) " << idx( MIXED::W ) << endl; 

   // Acess element 
   cout << " Direct access of gf[x][y] " << my_gf[x][y] << endl; 
   cout << " Acess with idx_t object " << my_gf( idx ) << endl; 
   cout << " Acess with corresponding pos1d " << my_gf( my_gf.get_pos_1d( idx ) ) << endl; 

   // Output idx_t type object and check get_pos_1d and get_idx functions
   cout << " idx " << idx << endl; 
   cout << " idx " << my_gf.get_idx( my_gf.get_pos_1d( idx ) ) << endl; 

   // Test sum over all container elements in two different ways
   double val = 0.0; 

   // .. with direct access
   {
      boost::timer::auto_cpu_timer t;
      for( int w = -N; w < N; w++ )
	 for( int W = -N; W < N + 1; W++ )
	 {
	    val += my_gf[w][W]; 
	 }
   }

   cout << " val " << val << endl; 
   val = 0.0; 

   // .. with idx_t access
   {
      boost::timer::auto_cpu_timer t;
      for( auto idx : idx_lst )
	 {
	    val += my_gf( idx ); 
	 }
   }

   cout << " val " << val << endl; 

   // -- Access with idx_t of similar gf_t
   using mygf_other_t = gf< int, 2 >;
   mygf_other_t my_other_gf( boost::extents[ffreq(N+1)][bfreq(N+1)] ); 
   mygf_other_t::idx_t other_idx( { 0, 0} ); 
   mygf_t::idx_t cloned_idx( other_idx ); 
   my_gf ( other_idx ); 

   //std::max_element( my_gf.begin(), my_gf.end() ); 
   cout << " Check symmation " << my_gf[x][y] << endl; 
   cout << " Direct access of gf[x][y] " << my_gf[x][y] << endl; 
   my_gf += my_gf; 
   cout << " After gf+gf " << my_gf[x][y] << endl; 

   // Example for two-particle vertex
   const int POS_FREQ_COUNT_VERT = 10; 
   const int PATCH_COUNT = 4; 
   const int QN_COUNT = 2; 

   enum class I2P{ w1_in, w2_in, w1_out, k1_in, k2_in, k1_out, s1_in, s2_in, s1_out, s2_out }; 
   class gf_2p_t : public gf< dcomplex, 10 >              ///< Container type for two-particle correlation functions
   {
      public:
	 gf_2p_t():
	    gf< dcomplex, 10 >( boost::extents[ffreq(POS_FREQ_COUNT_VERT)][ffreq(POS_FREQ_COUNT_VERT)][ffreq(POS_FREQ_COUNT_VERT)]
		  [PATCH_COUNT][PATCH_COUNT][PATCH_COUNT]
		  [QN_COUNT][QN_COUNT][QN_COUNT][QN_COUNT] )
      {}
   }; 
   using idx_2p_t = gf_2p_t::idx_t;  

}
