#include <stdio.h>
#include <stdlib.h>

#include <iup.h>
#include <im.h>
#include <im_image.h>
#include <iupim.h>

#include "custom.h"  // <--- "../include/custom.h" বাদ দিয়ে "custom.h" দাও

void setupGui(void)
{
    Ihandle *window, *vboxMain;

    // File Action Buttons
    Ihandle *btn_open = IupButton("Open Image", NULL);
    Ihandle *btn_saveAs = IupButton("Save As", NULL);
    Ihandle *btn_exit = IupButton("Exit", NULL);

    IupSetAttribute(btn_open, "RASTERSIZE", "130x30");
    IupSetAttribute(btn_saveAs, "RASTERSIZE", "130x30");
    IupSetAttribute(btn_exit, "RASTERSIZE", "130x30");

    Ihandle *vboxFile = IupVbox(btn_open, btn_saveAs, btn_exit, NULL);
    IupSetAttribute(vboxFile, "GAP", "6");
    IupSetAttribute(vboxFile, "MARGIN", "8x8");
    Ihandle *frameFile = IupFrame(vboxFile);
    IupSetAttribute(frameFile, "TITLE", " File Operations ");

    // Feature Buttons
    Ihandle *btn_grayScale = IupButton("Grayscale", NULL);
    Ihandle *btn_Inversion = IupButton("Inversion", NULL);
    Ihandle *btn_horizontalFlip = IupButton("Horizontal Flip", NULL);
    Ihandle *btn_verticalFlip = IupButton("Vertical Flip", NULL);
    Ihandle *btn_rotate90 = IupButton("Rotate 90°", NULL);
    Ihandle *btn_blur = IupButton("Blur", NULL);
    Ihandle *btn_sharpen = IupButton("Sharpen", NULL);
    Ihandle *btn_undo = IupButton("Undo", NULL);

    IupSetAttribute(btn_grayScale, "RASTERSIZE", "130x26");
    IupSetAttribute(btn_Inversion, "RASTERSIZE", "130x26");
    IupSetAttribute(btn_horizontalFlip, "RASTERSIZE", "130x26");
    IupSetAttribute(btn_verticalFlip, "RASTERSIZE", "130x26");
    IupSetAttribute(btn_rotate90, "RASTERSIZE", "130x26");
    IupSetAttribute(btn_blur, "RASTERSIZE", "130x26");
    IupSetAttribute(btn_sharpen, "RASTERSIZE", "130x26");
    IupSetAttribute(btn_undo, "RASTERSIZE", "130x26");

    // Callbacks Assignment
    IupSetCallback(btn_open, "ACTION", (Icallback)open_clb);
    IupSetCallback(btn_saveAs, "ACTION", (Icallback)saveAs_clb);
    IupSetCallback(btn_exit, "ACTION", (Icallback)ext_clb);

    IupSetCallback(btn_grayScale, "ACTION", (Icallback)grayScale_clb);
    IupSetCallback(btn_Inversion, "ACTION", (Icallback)Inversion_clb);
    IupSetCallback(btn_horizontalFlip, "ACTION", (Icallback)horizontalFlip_clb);
    IupSetCallback(btn_verticalFlip, "ACTION", (Icallback)verticalFlip_clb);
    IupSetCallback(btn_rotate90, "ACTION", (Icallback)rotate90_clb);
    IupSetCallback(btn_blur, "ACTION", (Icallback)blur_clb);
    IupSetCallback(btn_sharpen, "ACTION", (Icallback)sharpen_clb);
    IupSetCallback(btn_undo, "ACTION", (Icallback)undo_clb);

    Ihandle *vboxFilters = IupVbox(
        btn_grayScale, btn_Inversion, btn_horizontalFlip,
        btn_verticalFlip, btn_rotate90, btn_blur, btn_sharpen,
        IupLabel(""), btn_undo, NULL
    );
    IupSetAttribute(vboxFilters, "GAP", "4");
    IupSetAttribute(vboxFilters, "MARGIN", "8x8");
    Ihandle *frameFilters = IupFrame(vboxFilters);
    IupSetAttribute(frameFilters, "TITLE", " Tools & Filters ");

    Ihandle *vboxLeftSidebar = IupVbox(frameFile, frameFilters, NULL);
    IupSetAttribute(vboxLeftSidebar, "GAP", "10");

    // Brightness Controls
    Ihandle *brightnessInput = IupText(NULL);
    Ihandle *brightnessBtn = IupButton("Apply", NULL);
    IupSetAttribute(brightnessInput, "MASK", IUP_MASK_INT);
    IupSetAttribute(brightnessInput, "VISIBLECOLUMNS", "4");
    IupSetAttributeHandle(brightnessBtn, "MY_INPUT_TXT", brightnessInput);
    IupSetCallback(brightnessBtn, "ACTION", (Icallback)brightness_clb);

    Ihandle *hboxBrightness = IupHbox(IupLabel("Level (-255..255): "), brightnessInput, brightnessBtn, NULL);
    IupSetAttribute(hboxBrightness, "ALIGNMENT", "ACENTER");
    Ihandle *frameBrightness = IupFrame(hboxBrightness);
    IupSetAttribute(frameBrightness, "TITLE", " Brightness ");

    // Crop Controls
    Ihandle *cropXInput = IupText(NULL); Ihandle *cropYInput = IupText(NULL);
    Ihandle *cropWInput = IupText(NULL); Ihandle *cropHInput = IupText(NULL);
    IupSetAttribute(cropXInput, "MASK", IUP_MASK_UINT); IupSetAttribute(cropXInput, "VISIBLECOLUMNS", "3");
    IupSetAttribute(cropYInput, "MASK", IUP_MASK_UINT); IupSetAttribute(cropYInput, "VISIBLECOLUMNS", "3");
    IupSetAttribute(cropWInput, "MASK", IUP_MASK_UINT); IupSetAttribute(cropWInput, "VISIBLECOLUMNS", "3");
    IupSetAttribute(cropHInput, "MASK", IUP_MASK_UINT); IupSetAttribute(cropHInput, "VISIBLECOLUMNS", "3");

    Ihandle *cropBtn = IupButton("Crop", NULL);
    IupSetCallback(cropBtn, "ACTION", (Icallback)crop_clb);
    IupSetAttributeHandle(cropBtn, "CROP_X", cropXInput); IupSetAttributeHandle(cropBtn, "CROP_Y", cropYInput);
    IupSetAttributeHandle(cropBtn, "CROP_W", cropWInput); IupSetAttributeHandle(cropBtn, "CROP_H", cropHInput);

    Ihandle *hboxCrop = IupHbox(
        IupLabel("X:"), cropXInput, IupLabel("Y:"), cropYInput,
        IupLabel("W:"), cropWInput, IupLabel("H:"), cropHInput, cropBtn, NULL
    );
    IupSetAttribute(hboxCrop, "ALIGNMENT", "ACENTER");
    Ihandle *frameCrop = IupFrame(hboxCrop);
    IupSetAttribute(frameCrop, "TITLE", " Crop Area ");

    Ihandle *hboxTopControls = IupHbox(frameBrightness, frameCrop, NULL);
    IupSetAttribute(hboxTopControls, "GAP", "8");

    // Preview Display Canvas
    state.imageWidget = IupLabel(NULL);
    IupSetAttribute(state.imageWidget, "IMAGE", "DUMMY_INIT_NAME");
    IupSetAttribute(state.imageWidget, "TITLE", NULL);
    IupSetAttribute(state.imageWidget, "EXPAND", "YES");
    IupSetAttribute(state.imageWidget, "ALIGNMENT", "ACENTER:ACENTER");

    Ihandle *frameImage = IupFrame(IupVbox(state.imageWidget, NULL));
    IupSetAttribute(frameImage, "TITLE", " Preview ");
    IupSetAttribute(frameImage, "EXPAND", "YES");

    Ihandle *vboxCenterDisplay = IupVbox(hboxTopControls, frameImage, NULL);
    IupSetAttribute(vboxCenterDisplay, "GAP", "8");
    IupSetAttribute(vboxCenterDisplay, "EXPAND", "YES");

    Ihandle *hboxMainLayout = IupHbox(vboxLeftSidebar, vboxCenterDisplay, NULL);
    IupSetAttribute(hboxMainLayout, "GAP", "12");
    IupSetAttribute(hboxMainLayout, "MARGIN", "10x10");

    vboxMain = IupVbox(hboxMainLayout, NULL);

    window = IupDialog(vboxMain);
    IupSetAttribute(window, "TITLE", "Image Manipulation App by Shobhon");
    IupSetAttribute(window, "SIZE", "720x500");
    IupSetAttribute(window, "MINSIZE", "650x450");

    IupShowXY(window, IUP_CENTER, IUP_CENTER);
    IupMainLoop();
}