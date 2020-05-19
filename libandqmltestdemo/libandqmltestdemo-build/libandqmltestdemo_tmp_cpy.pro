CPY_SRC_FILE += \
    D:/demos/libandqmltestdemo/libandqmltestdemo/common/libandqmltestdemo_common.pri \
    D:/demos/libandqmltestdemo/libandqmltestdemo/libandqmltestdemo.pri \
    D:/demos/libandqmltestdemo/libandqmltestdemo/libandqmltestdemo_update.txt \
 
CPY_TGT_PATH += \
    D:/demos/libandqmltestdemo/libandqmltestdemo/mkspecs/../../libandqmltestdemo-build/libandqmltestdemo/common \
    D:/demos/libandqmltestdemo/libandqmltestdemo/mkspecs/../../libandqmltestdemo-build/libandqmltestdemo/ \
    D:/demos/libandqmltestdemo/libandqmltestdemo/mkspecs/../../libandqmltestdemo-build/libandqmltestdemo/ \
 
CPY_TGT_FILE += \
    D:/demos/libandqmltestdemo/libandqmltestdemo/mkspecs/../../libandqmltestdemo-build/libandqmltestdemo/common/libandqmltestdemo_common.pri  \
    D:/demos/libandqmltestdemo/libandqmltestdemo/mkspecs/../../libandqmltestdemo-build/libandqmltestdemo//libandqmltestdemo.pri  \
    D:/demos/libandqmltestdemo/libandqmltestdemo/mkspecs/../../libandqmltestdemo-build/libandqmltestdemo//libandqmltestdemo_update.txt  \
 
for ( tgt_path, CPY_TGT_PATH ) {
    ! exists( $$tgt_path ) : mkpath( $$tgt_path ) 
}
tgt_cntr = 
for ( src_file, CPY_SRC_FILE ) { 
    tgt_file = $$member( CPY_TGT_FILE, $$size( tgt_cntr )) 
    tgt_cntr += 1 
    exists( $$src_file ) { 
       message( copy $$src_file ) 
       win32 {
           cpy_tgt_file = $$replace( tgt_file, /, \\ )
           cpy_src_file = $$replace( src_file, /, \\ )
           system( "copy $${cpy_src_file} $${cpy_tgt_file} /y ")
       } else {
           system( "\cp -rf $${src_file} $${tgt_file}" )
       }
   }
}
