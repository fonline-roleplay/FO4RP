#ifndef __UTIL_H__
#define __UTIL_H__


#define CSTR_EXT_MAX_LEN ( 32 ) // bytes

extern int   cstr_ends_with( const char* src, const char* ends );
extern int   cstr_start_with( const char* src, const char* starts );
extern int 	 cstr_len( const char* src );
extern char* cstr_copy_heap( const char* src );
extern void  cstr_path_get_ext( const char* src, char* out, unsigned int maxSize );

#endif //__UTIL_H__