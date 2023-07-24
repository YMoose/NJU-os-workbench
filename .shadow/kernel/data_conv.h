#ifndef data_conv_h
#define data_conv_h

#define ptr_char(p) (*(char*)(p)) 

#define ptr_le_u32(p) (((unsigned int)ptr_char(p+3))|(((unsigned int)ptr_char(p+2))<<8)|(((unsigned int)ptr_char(p+1))<<16)|(((unsigned int)ptr_char(p))<<24))

#endif /* data_conv_h */