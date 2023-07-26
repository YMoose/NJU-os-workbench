 #ifndef data_conv_h
#define data_conv_h

#define ptr_uchar(p) (*(unsigned char*)(p)) 

#define ptr_le_u32(p) (((unsigned int)ptr_uchar(p))|(((unsigned int)ptr_uchar(p+1))<<8)|(((unsigned int)ptr_uchar(p+2))<<16)|(((unsigned int)ptr_uchar(p+3))<<24))

#define ptr_le_u24(p) (((unsigned int)ptr_uchar(p))|(((unsigned int)ptr_uchar(p+1))<<8)|(((unsigned int)ptr_uchar(p+2))<<16))

#endif /* data_conv_h */