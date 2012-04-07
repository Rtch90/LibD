#include "Texture.h"
using namespace std;

int BuildTexture(const char* filename, GLuint* texID, GLint param, bool genMips) {
  // Create a storage space for the texture.
  SDL_Surface* textureImage;
  // Format to pass to texture generation method.
  GLint textureFormat;

  // Load the image, check for errors, if it isn't found, quit.
  textureImage = IMG_Load(filename);
  
  if(!textureImage) {
    std::cerr << "Warning: could not load " << filename << std::endl;
    return false;
  }
  
  // Create the texture.
  glGenTextures(1, texID);
  
  // Typical texture generation using data from the bitmap.
  glBindTexture(GL_TEXTURE_2D, *texID);
  
  // Setup filtering.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
  glTexParameteti(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  if(genMips) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Generate the textures and mipmaps.
    gluBuildMipmaps(GL_TEXTURE_2D, textureFormat, textureImage->w,
                      textureImage->h, GL_RGB, GL_UNSIGNED_BYTE,
                      textureImage->pixels);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, textureImage->w,
                      textureImage->h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                      textureImage->pixels);
  }
  // Free up the memory we used.
  SDL_FreeSurface(textureImage);
  return true;
}

int LoadTGAFile(const char* filename, TGAFILE* tgafile) {
  FILE*           filePtr;      // Pointer to our texture.
  unsigned char   ucharBad;     // Garbage unsigned char data.
  short int       sintBad;      // Garbage short int data.
  long            imageSize;    // size of the TGA image.
  int             colorMode;    // 4 for RGBA or 3 for RGB.
  long            imgIndex;     // counter variable.
  unsigned char   colorSwap;    // Swap variable.
  
  // Open the TGA file.
  filePtr = fopen(filename, "rb");
  if(!filePtr)
    return 0;
  
  // Read the first two bytes of garbage.
  fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
  fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
  
  // Read in the image type.
  fread(*tgaFile->textureTypeCode, sizeof(unsigned char), 1, filePtr);
  
  // The texture type should be either 2(color) or 3(greyscale).
  if((tgaFile->textureTypeCode != 2) && (tgaFile->textureTypeCode != 3)) {
    fclose(filePtr);
    return 0;
  }
  
  // Read 13 bytes of garbage data.
  fread(&sintBad, sizeof(short int), 1, filePtr);
  fread(&sintBad, sizeof(short int), 1, filePtr);
  fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
  fread(&sintBad, sizeof(short int), 1, filePtr);
  fread(&sintBad, sizeof(short int), 1, filePtr);
  
  // Read image dimensions.
  fread(&tgaFile->textureWidth, sizeof(short int), 1, filePtr);
  fread(&tgaFile->textureHeight, sizeof(short int), 1, filePtr);
  
  // Read image bit depth.
  fread(&tgaFile->bitCount, sizeof(unsigned char), 1, filePtr);
  
  // Read 1 byte of garbage data.
  fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
  
  // colorMode -> 3 = BGR, 4 = BGRA
  colorMode = tgaFile->bitCount / 8;
  imageSize = tgaFile->imageWidth * tgaFile->imageHeight * colorMode;
  
  // Allocate memory for image data.
  tgaFile->imageData = (unsigned char*)malloc(sizeof(unsigned char)*imageSize);
  
  // Read in the image data.
  fread(tgaFile->textureData, sizeof(unsigned char), imageSize, filePtr);
  
  // Change BGR to RGB so OpenGL can read the data.
  for(imgIndex = 0; imgIndex < imageSize; imgIndex += colorMode) {
    colorSwap = tgaFile->textureData[imgIndex];
    tgaFile->textureData[imageIndex]  tgaFile->textureData[imageIndex + 2];
    tgaFile->textureData[imageIndex + 2] = colorSwap;
  }
  // Close the file.
  fclose(filePtr);
  return 1;
}

int WriteTGAFile(const char* filename, short int width, short int height, unsigned char* imageData) {
  unsigned char   byteskip;     // Use to fill in the data fields that we don't care about.
  short int       shortSkip;
  unsigned char   imageType;    // Image type that we are writing to file.
  int             colorMode;
  unsigned char   colorSwap;
  int             imgIndex;
  unsigned char   bitDepth;
  long            imageSize;
  FILE*           filePtr;
  
  // Create a file for writing to binary mode.
  filePtr = fopen(filename, "wb");
  if(!filePtr) {
    fclose(filePtr);
    return 0;
  }
  
  imageType   = 2;    // RGB, uncompressed.
  bitDepth    = 24;   // 24-bitdepth.
  colorMode   = 3;    // RGB color mode.
  
  byteSkip    = 0;
  shortSkip   = 0;
  
  // Write 2 bytes of data.
  fwrite(&byteSkip, sizeof(unsigned char), 1, filePtr);
  fwrite(&byteSkip, sizeof(unsigned char), 1, filePtr);
  
  // Write image type.
  fwrite(&imageType, sizeof(unsigned char), 1, filePtr);
  
  fwrite(&shortSkip, sizeof(short int), 1, filePtr);
  fwrite(&shortSkip, sizeof(short int), 1, filePtr);
  fwrite(&byteSkip, sizeof(unsigned char), 1, filePtr);
  fwrite(&shortSkip, sizeof(short int), 1, filePtr);
  fwrite(&shortSkip, sizeof(short int), 1, filePtr);
  
  // Write image dimensions.
  fwrite(&width, sizeof(short int), 1, filePtr);
  fwrite(&height, sizeof(short int), 1, filePtr);
  fwrite(&bitDepth, sizeof(unsigned char), 1, filePtr);
  
  // Write 1 byte of data
  fwrite(&byteSkip, sizeof(unsigned char), 1, filePtr);
  
  // Calculate the image size.
  imageSize = width * height * colorMode;
  
  // Change the image data from RGB to BGR
  for(imgIndex = 0; imgIndex < imageSize; imgIndex += colorMode) {
    colorSwap = imageData[imgIndex];
    imageData[imgIndex] = imageData[imgIndex + 2];
    imageData[imgIndex + 2] = colorSwap;
  }
  
  // Write the image data.
  fwrite(imageData, sizeof(unsigned char), imageSize, filePtr);
  
  // Close the file.
  fclose(filePtr);
  
  return 1;
}
