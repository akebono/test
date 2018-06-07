#include <libpng16/png.h>
struct texture{
	int width;
	int height;
	unsigned char *data;
};
struct texture read_texture(char *filename){
	png_structp png_ptr;
	png_infop info_ptr;
	int bit_depth,color_type,i,j;
	unsigned int twidth,theight,rowbytes;

	struct texture tex;
	unsigned char **data;
	FILE *fp;
	fp=fopen(filename,"rb");

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	info_ptr = png_create_info_struct(png_ptr);

	png_init_io(png_ptr,fp);
	png_read_info(png_ptr,info_ptr);
	png_get_IHDR(png_ptr,info_ptr,&twidth,&theight,&bit_depth,&color_type,0,0,0);

	rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	data=malloc(theight*sizeof(char*));
	for(i=0;i<theight;i++){
		data[i]=malloc(rowbytes);
	}

	png_read_image(png_ptr,data); //causes segfault
	tex.width=twidth;
	tex.height=theight;
	tex.data=malloc(rowbytes*theight);
	for(j=0;j<theight;j++){
		for(i=0;i<rowbytes;i++){
			tex.data[i+j*rowbytes]=data[theight-j-1][i];
		}
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, 0);
	fclose(fp);
	return tex;
}
