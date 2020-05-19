// /////////////////////////////////////////////////////////////////////// 
//
//  OpenCL 1.1 code.
//
// ////////////////////////////////////////////////////////////////////// 
// =====================================================================
//
// zero the buffer histogram only. 
//
// local size : 8   ( but every threads access 8 elements )
// global size: 32  ( 256 / ( local_size ) = 32  )
// 
// =====================================================================
kernel void  oclftr_buff2d_hist_256_zero (
        global uint  *hist  // actually, it is 256 elements. 
    )                       
{
    // -----------------------------------------------------------------    
    //  [ 8 * 8 ][ 8 * 8 ][ 8 * 8 ][ 8 * 8 ] -- [] workgroup filled elements  
    //  
    // -----------------------------------------------------------------
    int4 slot_ofv  = int4( get_global_id(0), get_global_id(0), get_global_id(0), get_global_id(0)) / get_local_size(0) * 64 +     // which wkg 
                     int4( get_local_id(0),  get_local_id(0),  get_local_id(0),  get_local_id(0) ) * 8                            // which slot
               
    int4 prev = slot_idx + int4( 0, 1, 2, 3 ); // previous 4 elements position
    int4 suff = slot_idx + int4( 4, 5, 6, 7 );

    hist[ prev.x ] = 0; hist[ prev.y ] = 0; hist[ prev.z ] = 0; hist[ prev.w ] = 0; 
    hist[ suff.x ] = 0; hist[ suff.y ] = 0; hist[ suff.z ] = 0; hist[ suff.w ] = 0;
}


// =====================================================================
//
// calculate the buffer histogram ( uchar version )
//
// local  size: ( 8, 8 ) 
// global size: ( dim1, dim2 ) / ( 2, 2 ) 
//
// =====================================================================
kernel  void  oclftr_buff2d_hist_uchar (
       global uint        *hist,    // the source of histgoram, need 256 * 4 bytes. and need to be cleared zero before use. 
       global const uchar *src,     // buffer source
       int dim1, int dim2           // actual size
       )
{
    // ----------------------------------------------------------------
    // each thread access 4 elements. like below : 
    //    0  1  
    //    2  3
    // a 512 * 512 buffer should setup the global size as ( 512, 512 ) / 2 ==> ( 256, 256 )
    //
    // ----------------------------------------------------------------
    int2 tmp     = int2( get_global_id(0) + get_global_offset(0), get_global_id(1) + get_global_offset(1)) / 
                   int2( get_local_size(0), get_local_size(1)) * 16 +
                   int2( get_local_id(0),   get_local_id(1)) * 2;   // e.g  global_id is 255,  255 / 8 * 16 + 7 * 2 = 510

    int4 data_ofv   = int4( tmp.x,  tmp.x + 1,  tmp.x,      tmp.x + 1 ) + 
                      int4( tmp.y,  tmp.y,      tmp.y + 1,  tmp.y + 1 ) * dim1;  

    bool is_pos_ok0 = ( tmp.x     < dim1 ) | ( tmp.y     < dim2 );   
    bool is_pos_ok1 = ( tmp.x + 1 < dim1 ) | ( tmp.y     < dim2 );
    bool is_pos_ok2 = ( tmp.x     < dim1 ) | ( tmp.y + 1 < dim2 );
    bool is_pos_ok3 = ( tmp.x + 1 < dim1 ) | ( tmp.y + 1 < dim2 );

    // ----------------------------------------------------------------
    // declare the local resource
    // ----------------------------------------------------------------
    local uint loc_hist[ 256 ];  // at least used 1KB local memory ...

    tmp.x = ( get_local_id(0) + get_local_id(1) * get_local_size(0)) * 4;
    int4 loc_idx = int4( tmp.x, tmp.x, tmp.x, tmp.x ) + int4( 0, 1, 2, 3 );

    loc_hist[ loc_idx.x ] = 0; // in [  0,   63 ]
    loc_hist[ loc_idx.y ] = 0; // in [  64, 127 ]
    loc_hist[ loc_idx.z ] = 0; // in [ 128, 191 ]
    loc_hist[ loc_idx.w ] = 0; // in [ 192, 255 ]

    barrier( CLK_LOCAL_MEM_FENCE ); // here flush local memory to each other threads, all threads must execute it
                                    // here wait all threads completed it.    

    // ----------------------------------------------------------------
    // read the global data by global index
    // ----------------------------------------------------------------
    atomic_add( & loc_hist[ select( loc_idx.x, uint( src[ data_ofv.x ] ), uchar( is_pos_ok0 )) ], select( 0, 1, uchar( is_pos_ok0 )) );
    atomic_add( & loc_hist[ select( loc_idx.y, uint( src[ data_ofv.y ] ), uchar( is_pos_ok1 )) ], select( 0, 1, uchar( is_pos_ok1 )) );
    atomic_add( & loc_hist[ select( loc_idx.z, uint( src[ data_ofv.z ] ), uchar( is_pos_ok2 )) ], select( 0, 1, uchar( is_pos_ok2 )) );
    atomic_add( & loc_hist[ select( loc_idx.w, uint( src[ data_ofv.w ] ), uchar( is_pos_ok3 )) ], select( 0, 1, uchar( is_pos_ok3 )) );
   
    barrier( CLK_LOCAL_MEM_FENCE );

    // ----------------------------------------------------------------
    // write to the global result memory
    // ----------------------------------------------------------------
    atomic_add( & hist[ loc_idx.x ], loc_hist[ loc_idx.x ] );
    atomic_add( & hist[ loc_idx.y ], loc_hist[ loc_idx.y ] );
    atomic_add( & hist[ loc_idx.z ], loc_hist[ loc_idx.z ] );
    atomic_add( & hist[ loc_idx.w ], loc_hist[ loc_idx.w ] );  
} 


