#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iup.h>
#include <im.h>
#include <im_image.h>
#include <iupim.h>

#include "../include/custom.h"

// --- Image Processing Core Algorithms ---

void apply_grayscale(imImage *img)
{
    int w = img->width, h = img->height;
    unsigned char *r = img->data[0], *g = img->data[1], *b = img->data[2];

    for (int i = 0; i < w * h; i++)
    {
        unsigned char gray = (unsigned char)(0.299 * r[i] + 0.587 * g[i] + 0.114 * b[i]);
        r[i] = g[i] = b[i] = gray;
    }
}

void apply_inversion(imImage *img)
{
    int w = img->width, h = img->height;
    unsigned char *r = img->data[0], *g = img->data[1], *b = img->data[2];

    for (int i = 0; i < w * h; i++)
    {
        r[i] = 255 - r[i];
        g[i] = 255 - g[i];
        b[i] = 255 - b[i];
    }
}

void apply_horizontal_flip(imImage *img)
{
    int w = img->width, h = img->height;
    unsigned char *r = img->data[0], *g = img->data[1], *b = img->data[2];

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w / 2; j++)
        {
            int idx1 = i * w + j;
            int idx2 = i * w + (w - 1 - j);

            unsigned char temp = r[idx1]; r[idx1] = r[idx2]; r[idx2] = temp;
            temp = g[idx1]; g[idx1] = g[idx2]; g[idx2] = temp;
            temp = b[idx1]; b[idx1] = b[idx2]; b[idx2] = temp;
        }
    }
}

void apply_vertical_flip(imImage *img)
{
    int w = img->width, h = img->height;
    unsigned char *r = img->data[0], *g = img->data[1], *b = img->data[2];

    for (int i = 0; i < h / 2; i++)
    {
        for (int j = 0; j < w; j++)
        {
            int idx1 = i * w + j;
            int idx2 = (h - 1 - i) * w + j;

            unsigned char temp = r[idx1]; r[idx1] = r[idx2]; r[idx2] = temp;
            temp = g[idx1]; g[idx1] = g[idx2]; g[idx2] = temp;
            temp = b[idx1]; b[idx1] = b[idx2]; b[idx2] = temp;
        }
    }
}

imImage* apply_rotate90(imImage *img)
{
    int w = img->width, h = img->height;
    unsigned char *r = img->data[0], *g = img->data[1], *b = img->data[2];

    imImage *newImage = imImageCreate(h, w, img->color_space, img->data_type);
    unsigned char *n_r = newImage->data[0], *n_g = newImage->data[1], *n_b = newImage->data[2];

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            int src_index = i * w + j;
            int dst_index = (w - 1 - j) * h + i;

            n_r[dst_index] = r[src_index];
            n_g[dst_index] = g[src_index];
            n_b[dst_index] = b[src_index];
        }
    }
    return newImage;
}

imImage* apply_blur(imImage *img)
{
    int w = img->width, h = img->height;
    unsigned char *r = img->data[0], *g = img->data[1], *b = img->data[2];

    imImage *newImage = imImageCreate(w, h, img->color_space, img->data_type);
    unsigned char *n_r = newImage->data[0], *n_g = newImage->data[1], *n_b = newImage->data[2];

    int dx[] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            unsigned int sum_r = 0, sum_g = 0, sum_b = 0, divider = 0;
            for (int k = 0; k < 9; k++)
            {
                int neighbor_row = i + dx[k], neighbor_col = j + dy[k];
                if (neighbor_row >= 0 && neighbor_row < h && neighbor_col >= 0 && neighbor_col < w)
                {
                    int src = neighbor_row * w + neighbor_col;
                    sum_r += r[src]; sum_g += g[src]; sum_b += b[src];
                    divider++;
                }
            }
            int dst = i * w + j;
            n_r[dst] = (unsigned char)(sum_r / divider);
            n_g[dst] = (unsigned char)(sum_g / divider);
            n_b[dst] = (unsigned char)(sum_b / divider);
        }
    }
    return newImage;
}

// Bonus Feature: 3x3 Sharpen Kernel Convolution
imImage* apply_sharpen(imImage *img)
{
    int w = img->width, h = img->height;
    unsigned char *r = img->data[0], *g = img->data[1], *b = img->data[2];

    imImage *newImage = imImageCreate(w, h, img->color_space, img->data_type);
    unsigned char *n_r = newImage->data[0], *n_g = newImage->data[1], *n_b = newImage->data[2];

    int kernel[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            int sum_r = 0, sum_g = 0, sum_b = 0;

            for (int ki = -1; ki <= 1; ki++)
            {
                for (int kj = -1; kj <= 1; kj++)
                {
                    int nr = i + ki, nc = j + kj;
                    if (nr < 0) nr = 0; if (nr >= h) nr = h - 1;
                    if (nc < 0) nc = 0; if (nc >= w) nc = w - 1;

                    int src = nr * w + nc;
                    int weight = kernel[ki + 1][kj + 1];

                    sum_r += r[src] * weight;
                    sum_g += g[src] * weight;
                    sum_b += b[src] * weight;
                }
            }

            if (sum_r > 255) sum_r = 255; if (sum_r < 0) sum_r = 0;
            if (sum_g > 255) sum_g = 255; if (sum_g < 0) sum_g = 0;
            if (sum_b > 255) sum_b = 255; if (sum_b < 0) sum_b = 0;

            int dst = i * w + j;
            n_r[dst] = (unsigned char)sum_r;
            n_g[dst] = (unsigned char)sum_g;
            n_b[dst] = (unsigned char)sum_b;
        }
    }
    return newImage;
}

void apply_brightness(imImage *img, int adjustment)
{
    int w = img->width, h = img->height;
    unsigned char *r = img->data[0], *g = img->data[1], *b = img->data[2];

    for (int i = 0; i < w * h; i++)
    {
        int _r = r[i] + adjustment, _g = g[i] + adjustment, _b = b[i] + adjustment;
        if (_r > 255) _r = 255; if (_r < 0) _r = 0;
        if (_g > 255) _g = 255; if (_g < 0) _g = 0;
        if (_b > 255) _b = 255; if (_b < 0) _b = 0;

        r[i] = (unsigned char)_r; g[i] = (unsigned char)_g; b[i] = (unsigned char)_b;
    }
}

imImage* apply_crop(imImage *img, int startX, int startY, int cropW, int cropH)
{
    int w = img->width;
    imImage *croppedImage = imImageCreate(cropW, cropH, img->color_space, img->data_type);

    unsigned char *src_r = img->data[0], *src_g = img->data[1], *src_b = img->data[2];
    unsigned char *dst_r = croppedImage->data[0], *dst_g = croppedImage->data[1], *dst_b = croppedImage->data[2];

    for (int i = 0; i < cropH; i++)
    {
        for (int j = 0; j < cropW; j++)
        {
            int src_idx = ((startY + i) * w) + (startX + j);
            int dst_idx = (i * cropW) + j;

            dst_r[dst_idx] = src_r[src_idx];
            dst_g[dst_idx] = src_g[src_idx];
            dst_b[dst_idx] = src_b[src_idx];
        }
    }
    return croppedImage;
}

// --- IUP Callbacks (Invoking Algorithms) ---

int grayScale_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;
    if (state.undoImage) imImageDestroy(state.undoImage);
    state.undoImage = imImageDuplicate(state.currentImage);

    apply_grayscale(state.currentImage);
    updateUIImage(self);
    return IUP_DEFAULT;
}

int Inversion_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;
    if (state.undoImage) imImageDestroy(state.undoImage);
    state.undoImage = imImageDuplicate(state.currentImage);

    apply_inversion(state.currentImage);
    updateUIImage(self);
    return IUP_DEFAULT;
}

int horizontalFlip_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;
    if (state.undoImage) imImageDestroy(state.undoImage);
    state.undoImage = imImageDuplicate(state.currentImage);

    apply_horizontal_flip(state.currentImage);
    updateUIImage(self);
    return IUP_DEFAULT;
}

int verticalFlip_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;
    if (state.undoImage) imImageDestroy(state.undoImage);
    state.undoImage = imImageDuplicate(state.currentImage);

    apply_vertical_flip(state.currentImage);
    updateUIImage(self);
    return IUP_DEFAULT;
}

int rotate90_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;
    if (state.undoImage) imImageDestroy(state.undoImage);
    state.undoImage = imImageDuplicate(state.currentImage);

    imImage *rotated = apply_rotate90(state.currentImage);
    imImageDestroy(state.currentImage);
    state.currentImage = rotated;

    updateUIImage(self);
    return IUP_DEFAULT;
}

int blur_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;
    if (state.undoImage) imImageDestroy(state.undoImage);
    state.undoImage = imImageDuplicate(state.currentImage);

    imImage *blurred = apply_blur(state.currentImage);
    imImageDestroy(state.currentImage);
    state.currentImage = blurred;

    updateUIImage(self);
    return IUP_DEFAULT;
}

int sharpen_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;
    if (state.undoImage) imImageDestroy(state.undoImage);
    state.undoImage = imImageDuplicate(state.currentImage);

    imImage *sharpened = apply_sharpen(state.currentImage);
    imImageDestroy(state.currentImage);
    state.currentImage = sharpened;

    updateUIImage(self);
    return IUP_DEFAULT;
}

int brightness_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;
    Ihandle *inputBox = IupGetAttributeHandle(self, "MY_INPUT_TXT");
    char *str = IupGetAttribute(inputBox, "VALUE");
    if (!str || strlen(str) == 0) return IUP_DEFAULT;

    int adjustment = atoi(str);
    if (adjustment < -255 || adjustment > 255) return IUP_DEFAULT;

    if (state.undoImage) imImageDestroy(state.undoImage);
    state.undoImage = imImageDuplicate(state.currentImage);

    apply_brightness(state.currentImage, adjustment);
    updateUIImage(self);
    return IUP_DEFAULT;
}

int crop_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;

    int startX = atoi(IupGetAttribute((Ihandle*)IupGetAttributeHandle(self, "CROP_X"), "VALUE"));
    int startY = atoi(IupGetAttribute((Ihandle*)IupGetAttributeHandle(self, "CROP_Y"), "VALUE"));
    int cropW = atoi(IupGetAttribute((Ihandle*)IupGetAttributeHandle(self, "CROP_W"), "VALUE"));
    int cropH = atoi(IupGetAttribute((Ihandle*)IupGetAttributeHandle(self, "CROP_H"), "VALUE"));

    int w = state.currentImage->width, h = state.currentImage->height;
    if (cropW <= 0 || cropH <= 0 || startX >= w || startY >= h || startX < 0 || startY < 0) return IUP_DEFAULT;

    if (startX + cropW > w) cropW = w - startX;
    if (startY + cropH > h) cropH = h - startY;

    if (state.undoImage) imImageDestroy(state.undoImage);
    state.undoImage = imImageDuplicate(state.currentImage);

    imImage *cropped = apply_crop(state.currentImage, startX, startY, cropW, cropH);
    imImageDestroy(state.currentImage);
    state.currentImage = cropped;

    updateUIImage(self);
    return IUP_DEFAULT;
}

int undo_clb(Ihandle *self)
{
    if (!isAppStateValid()) return IUP_CLOSE;
    if (state.undoImage == NULL) return IUP_DEFAULT;

    imImage *temp = imImageDuplicate(state.undoImage);
    imImageDestroy(state.currentImage);
    imImageDestroy(state.undoImage);
    state.undoImage = NULL;

    state.currentImage = temp;
    updateUIImage(self);
    return IUP_DEFAULT;
}