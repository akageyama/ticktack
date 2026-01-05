#ifndef __TICKTACK_H
#define __TICKTACK_H

void __tick( const char * );
void __tack( const char * );
void __ticktack_print( void );

#define __tick_ (__tick( __func__ ))
#define __tack_ (__tack( __func__ ))
#define __toe_  (__ticktack_print())

#endif
