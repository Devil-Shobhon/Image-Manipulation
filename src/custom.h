#ifndef CUSTOM_H
#define CUSTOM_H

#include <iup.h>
#include <im.h>
#include <im_image.h>

typedef struct {
    char *currentImageFile;
    imImage *currentImage;
    imImage *undoImage;
    Ihandle *imageWidget;
} AppState;

extern AppState state;

// Core Utility & GUI Prototypes
void setupGui(void);
void updateUIImage(Ihandle *self);
int isAppStateValid(void);
void freeState(void);
void printState(void);

// Callback Prototypes
int open_clb(Ihandle *self);
int saveAs_clb(Ihandle *self);
int ext_clb(Ihandle *self);

int grayScale_clb(Ihandle *self);
int Inversion_clb(Ihandle *self);
int horizontalFlip_clb(Ihandle *self);
int verticalFlip_clb(Ihandle *self);
int rotate90_clb(Ihandle *self);
int blur_clb(Ihandle *self);
int sharpen_clb(Ihandle *self); 
int undo_clb(Ihandle *self);

int brightness_clb(Ihandle *self);
int crop_clb(Ihandle *self);

// Core Processing Algorithms
void apply_grayscale(imImage *img);
void apply_inversion(imImage *img);
void apply_horizontal_flip(imImage *img);
void apply_vertical_flip(imImage *img);
imImage* apply_rotate90(imImage *img);
imImage* apply_blur(imImage *img);
imImage* apply_sharpen(imImage *img);
void apply_brightness(imImage *img, int adjustment);
imImage* apply_crop(imImage *img, int startX, int startY, int cropW, int cropH);

#endif