#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>

void my_error_exit(j_common_ptr cinfo);
unsigned char** read_JPEG_file(char * filename);
