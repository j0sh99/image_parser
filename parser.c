#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <jpeglib.h>

// code source: https://www.tspi.at/2020/03/20/libjpegexample.html#the-other-way-storing-raw-images-into-jpeg
struct imgRawImage {
	unsigned int numComponents;
	unsigned long int width, height;

	unsigned char* lpData;
};

int storeJpegImageFile(struct imgRawImage* lpImage, char* lpFilename) {
	struct jpeg_compress_struct info;
	struct jpeg_error_mgr err;

	unsigned char* lpRowBuffer[1];

	FILE* fHandle;

	fHandle = fopen(lpFilename, "wb");
	if(fHandle == NULL) {
		#ifdef DEBUG
			fprintf(stderr, "%s:%u Failed to open output file %s\n", __FILE__, __LINE__, lpFilename);
		#endif
		return 1;
	}

	info.err = jpeg_std_error(&err);
	jpeg_create_compress(&info);

	jpeg_stdio_dest(&info, fHandle);

	info.image_width = lpImage->width;
	info.image_height = lpImage->height;
	info.input_components = 3;
	info.in_color_space = JCS_RGB;

	jpeg_set_defaults(&info);
	jpeg_set_quality(&info, 100, TRUE);

	jpeg_start_compress(&info, TRUE);

	/* Write every scanline ... */
	while(info.next_scanline < info.image_height) {
		lpRowBuffer[0] = &(lpImage->lpData[info.next_scanline * (lpImage->width * 3)]);
		jpeg_write_scanlines(&info, lpRowBuffer, 1);
	}

	jpeg_finish_compress(&info);
	fclose(fHandle);

	jpeg_destroy_compress(&info);
	return 0;
}

void read_ciff(FILE* file){

    // read bytes
    char magic[4];
    if (fread(magic, 4, 1, file) != 1) return;

    // check magic bytes
    if (memcmp("CIFF", magic, sizeof(magic)) != 0)
        return;

    uint64_t header_size;
    if (fread(&header_size, 8, 1, file) != 1) return;
    header_size = le64toh(header_size);

    // check header_size
    if (header_size < 24)
        return;

    uint64_t content_size;
    if (fread(&content_size, 8, 1, file) != 1) return;
    content_size = le64toh(content_size);

    uint64_t width;
    if (fread(&width, 8, 1, file) != 1) return;
    width = le64toh(width);

    uint64_t height;
    if (fread(&height, 8, 1, file) != 1) return;
    height = le64toh(height);

    // check size
    //if ( content_size != (width*height)*3 )
    //    return;

    char* caption_and_tags = calloc(header_size - 36, 1);
    if (fread(caption_and_tags, 1, header_size - 36, file) != header_size - 36) return;

    // read pixels
    //int* pixels = calloc(content_size, 1);

    unsigned char pixels[width][height][3];
    if (fread(pixels, 3, width*height, file) != width*height) return;

    struct imgRawImage jpegpic;

    jpegpic.width = width;
    jpegpic.height = height;
    jpegpic.numComponents = 3;

    jpegpic.lpData = (unsigned char*) pixels;

    char* pic = "kimenet.jpg";
    storeJpegImageFile(&jpegpic, pic);

}

int main(int argc, char* argv[]) {

    //FILE * ciff, caff;
    // parse terminal input

    if (strcmp(argv[1], "-ciff") == 0 && argv[2] != NULL){
        printf("ciff file\n");
    }
    if (strcmp(argv[1], "-caff") == 0 && argv[2] != NULL){
        printf("caff file\n");
    }
    //--ciff <filename>
    char* filename = argv[2];
    FILE *file;

    file = fopen(filename, "rb");
    if (file == NULL) {
        //printf("Failed to open the file.\n");
        return -1;
    }

    read_ciff(file);

    /*

    // read bytes
    char magic[4];
    fread(magic, 4, 1, file);

    uint64_t header_size;
    fread(&header_size, 8, 1, file);
    header_size = be64toh(header_size);

    uint64_t content_size;
    fread(&content_size, 8, 1, file);
    content_size = be64toh(content_size);

    uint64_t width;
    fread(&width, 8, 1, file);
    width = be64toh(width);

    uint64_t height;
    fread(&height, 8, 1, file);
    height = be64toh(height);

    char* caption_and_tags = calloc(header_size - 36, 1);
    fread(caption_and_tags, 1, header_size - 36, file);

    */
    
    return 0;
}
