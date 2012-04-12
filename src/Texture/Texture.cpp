#include "../System/Debug.h"
#include "Texture.h"
#include <iostream>
using namespace std;

#ifndef GL_BGR_EXT
#define GL_BGR_EXT 0x80E0
#endif

#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT 0x80E1
#endif

ResourceManager<Texture> textureManager;

static GLuint boundTexture = 0;

static bool IsBGR(SDL_Surface* surf) {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
  return (surf->format->Rmask & 0x00FF0000) &&
         (surf->format->Bmask & 0x000000FF);
#else
  return (surf->format->Bmask & 0xFF000000) &&
         (surf->format->Rmask & 0x0000FF00);
#endif
}

int BuildTexture(const char* filename, GLuint* texID, GLint param, bool genMips) {
  // Create a storage space for the texture.
  SDL_Surface* textureImage;
  // Format to pass to texture generation method.
  GLint format;
  GLint internalFormat;

  // Load the image, check for errors, if it isn't found, quit.
  textureImage = IMG_Load(filename);
  
  if(!textureImage) {
     Debug::logger->message("Warning: could not load %s", filename);
    return false;
  }

  switch(textureImage->format->BitsPerPixel) {
  case 8:
    internalFormat = format = GL_ALPHA;
    break;
  case 16:
    internalFormat = format = GL_LUMINANCE_ALPHA;
    break;
  case 24:
    internalFormat = GL_RGB;
    if (IsBGR(textureImage))
      format = GL_BGR_EXT;
    else
      format = GL_RGB;
    break;
  case 32:
    internalFormat = GL_RGBA;
    if (IsBGR(textureImage))
      format = GL_BGR_EXT;
    else
      format = GL_RGBA;
    break;
  default:
    format = internalFormat = GL_NONE;
  }

  if(internalFormat == GL_NONE || format == GL_NONE) {
    Debug::logger->message("Warning: invalid texture format for %s", filename);
    SDL_FreeSurface(textureImage);
    return false;
  }
  
  // Create the texture.
  glGenTextures(1, texID);
  
  // Typical texture generation using data from the bitmap.
  BindTexture(*texID);
  
  // Setup filtering.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  if(genMips) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Generate the textures and mipmaps.
    gluBuild2DMipmaps(GL_TEXTURE_2D, internalFormat, textureImage->w,
                      textureImage->h, format, GL_UNSIGNED_BYTE,
                      textureImage->pixels);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, textureImage->w,
                      textureImage->h, 0, format, GL_UNSIGNED_BYTE,
                      textureImage->pixels);
  }
  // Free up the memory we used.
  SDL_FreeSurface(textureImage);
  return true;
}

int LoadTGAFile(const char* filename, TGAFILE* tgaFile) {
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
  fread(&tgaFile->textureTypeCode, sizeof(unsigned char), 1, filePtr);
  
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
  imageSize = tgaFile->textureWidth * tgaFile->textureHeight * colorMode;
  
  // Allocate memory for image data.
  tgaFile->textureData = (unsigned char*)malloc(sizeof(unsigned char)*imageSize);
  
  // Read in the image data.
  fread(tgaFile->textureData, sizeof(unsigned char), imageSize, filePtr);
  
  // Change BGR to RGB so OpenGL can read the data.
  for(imgIndex = 0; imgIndex < imageSize; imgIndex += colorMode) {
    colorSwap = tgaFile->textureData[imgIndex];
    tgaFile->textureData[imgIndex] = tgaFile->textureData[imgIndex + 2];
    tgaFile->textureData[imgIndex + 2] = colorSwap;
  }
  // Close the file.
  fclose(filePtr);
  return 1;
}

int WriteTGAFile(const char* filename, short int width, short int height, unsigned char* imageData) {
  unsigned char   byteSkip;     // Use to fill in the data fields that we don't care about.
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

void BindTexture(GLuint texID) {
  if(boundTexture != texID){
    glBindTexture(GL_TEXTURE_2D, texID);
    boundTexture = texID;
  }
}

Texture::Texture() {
  texID = 0;
  width = 0;
  height = 0;
}

Texture::~Texture() {
  if(texID != 0) {
    glDeleteTextures(1, &texID);
    texID = 0;
  }
}

bool Texture::Load(const std::string& filename) {
  if(BuildTexture(filename.c_str(), &texID, GL_CLAMP, false)) {
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    return true;
  }
  return false;
}
