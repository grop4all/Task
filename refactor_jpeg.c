#include "refactor_jpeg.h"


struct my_error_mgr {
	struct jpeg_error_mgr pub;	/* "public" fields */

	jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

void my_error_exit (j_common_ptr cinfo)
{
	/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
	my_error_ptr myerr = (my_error_ptr) cinfo->err;

	/* Always display the message. */
	/* We could postpone this until after returning, if we chose. */
	(*cinfo->err->output_message) (cinfo);

	/* Return control to the setjmp point */
	longjmp(myerr->setjmp_buffer, 1);
}


int _read_JPEG_file (char * filename)
{

	struct jpeg_decompress_struct cinfo;
	
	struct my_error_mgr jerr;
	/* More stuff */
	FILE * infile;		/* source file */
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */


	if ((infile = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		return 0;
	}


	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;

	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
		return 0;
	}
 
	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, infile);

	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	#ifdef TEST
	printf("%i\n",cinfo.output_width);
	printf("%i\n",cinfo.output_height);
	#endif
	
	row_stride = cinfo.output_width * cinfo.output_components;
	
	buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	while (cinfo.output_scanline < cinfo.output_height) {
	 // get the pointer to the row:
		jpeg_read_scanlines(&cinfo, buffer ,1);
		unsigned char* pixel_row = (unsigned char*)(buffer[0]);
	// iterate over the pixels:
	#ifdef TEST
		for(int i = 0; i < cinfo.output_width; i++)
		{
			int red = pixel_row[i];
			int green = pixel_row[i + 1];
			int blue = pixel_row[i + 2];
			printf("%i %i %i\n", red, green, blue);
		}
	#endif
	}

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	fclose(infile);


	return 1;
}




