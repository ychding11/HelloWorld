#!/usr/bin/python

import cv2
import sys
import time
import numpy as np
from random import randint
from matplotlib import pyplot as plt

# These global varibles will be overrided by user input.
# Use these global varibles to avoid modiy lots of legcy
# code.
img_sample = np.zeros((100,100,3), np.uint8);
img        = np.zeros((100,100,3), np.uint8);
PatchSize    = 0
OverlapWidth = 0
InitialThresConstant = 0.0
sample_width  = 0
sample_height = 0

# Output synthesised image size is hard coded.
img_height = 250
img_width  = 200

#---------------------------------------------------------------------------------------#
#|                      Best Fit Patch and related functions                           |#
#---------------------------------------------------------------------------------------#
def OverlapErrorVertical( imgPx, samplePx ):
    iLeft,jLeft   = imgPx
    iRight,jRight = samplePx
    OverlapErr = 0.0
    diff = np.zeros((3))
    for i in range( PatchSize ):
        for j in range( OverlapWidth ):
            diff[0] =  int(img[i + iLeft, j+ jLeft][0]) - int(img_sample[i + iRight, j + jRight][0])
            diff[1] =  int(img[i + iLeft, j+ jLeft][1]) - int(img_sample[i + iRight, j + jRight][1])
            diff[2] =  int(img[i + iLeft, j+ jLeft][2]) - int(img_sample[i + iRight, j + jRight][2])
            OverlapErr += (diff[0]**2 + diff[1]**2 + diff[2]**2)**0.5
    return OverlapErr

def OverlapErrorHorizntl( leftPx, rightPx ):
    iLeft,jLeft   = leftPx
    iRight,jRight = rightPx
    OverlapErr = 0.0
    diff = np.zeros((3))
    for i in range( OverlapWidth ):
        for j in range( PatchSize ):
            diff[0] =  int(img[i + iLeft, j+ jLeft][0]) - int(img_sample[i + iRight, j + jRight][0])
            diff[1] =  int(img[i + iLeft, j+ jLeft][1]) - int(img_sample[i + iRight, j + jRight][1])
            diff[2] =  int(img[i + iLeft, j+ jLeft][2]) - int(img_sample[i + iRight, j + jRight][2])
            OverlapErr += (diff[0]**2 + diff[1]**2 + diff[2]**2)**0.5
    return OverlapErr

#Display candidate patches in image.
def displayCandidate(px, py):
    tempImage = img_sample.copy();
    tempPt1   = (px, py)
    tempPt2   = (px + PatchSize, py+ PatchSize)
    print "- Candidate Patch Location: ", tempPt1, tempPt2
    cv2.rectangle(tempImage, tempPt1, tempPt2, (255, 0, 0)) #draw selected patch
    cv2.imshow('Selected Patch',tempImage)
    cv2.waitKey(1)

#Search in Sample image for candidate patches whose threshold value of overlap
#eara should meet certern value.
#param [px] patches in target image.
def GetBestPatches( px, ThresholdOverlapError ):#Will get called in GrowImage
    PixelList = [] #empty candidate list
    #check for top layer
    if px[0] == 0:
        for i in range(sample_height - PatchSize):
            for j in range(OverlapWidth, sample_width - PatchSize ):
                #iterate all possible patches in sample image. Only vertical overlap exist.
                error = OverlapErrorVertical( (px[0], px[1] - OverlapWidth), (i, j - OverlapWidth)  )
                #print "- Test Patch (%d, %d), Vertical overlap error=%f" %(i, j,error)
                if error  < ThresholdOverlapError:
                    PixelList.append((i,j))
                    displayCandidate(i,j)
                elif error < ThresholdOverlapError/2:
                    displayCandidate(i,j)
                    return [(i,j)]
                    
    #check for leftmost layer
    elif px[1] == 0:
        for i in range(OverlapWidth, sample_height - PatchSize ):
            for j in range(sample_width - PatchSize):
                error = OverlapErrorHorizntl( (px[0] - OverlapWidth, px[1]), (i - OverlapWidth, j)  )
                #print "- Test Patch (%d, %d), Horizntl overlap error=%f" %(i, j,error)
                if error  < ThresholdOverlapError:
                    PixelList.append((i,j))
                    displayCandidate(i,j)
                elif error < ThresholdOverlapError/2:
                    displayCandidate(i,j)
                    return [(i,j)]
    #for pixel placed inside 
    else:
        for i in range(OverlapWidth, sample_height - PatchSize):
            for j in range(OverlapWidth, sample_width - PatchSize):
                error_Vertical   = OverlapErrorVertical( (px[0], px[1] - OverlapWidth), (i,j - OverlapWidth)  )
                error_Horizntl   = OverlapErrorHorizntl( (px[0] - OverlapWidth, px[1]), (i - OverlapWidth,j) )
                #print "- Test Patch (%d, %d), Vertical error=%f, Horizntl error=%f" %(i, j,error_Vertical, error_Horizntl)
                if error_Vertical  < ThresholdOverlapError and error_Horizntl < ThresholdOverlapError:
                    displayCandidate(i,j)
                    PixelList.append((i,j))
                elif error_Vertical < ThresholdOverlapError/2 and error_Horizntl < ThresholdOverlapError/2:
                    displayCandidate(i,j)
                    return [(i,j)]
    return PixelList

#-----------------------------------------------------------------------------------------------#
#|                              Quilting and related Functions                                 |#
#-----------------------------------------------------------------------------------------------#

def SSD_Error( offset, imgPx, samplePx ):
    err_r = int(img[imgPx[0] + offset[0], imgPx[1] + offset[1]][0]) - int(img_sample[samplePx[0] + offset[0], samplePx[1] + offset[1]][0])
    err_g = int(img[imgPx[0] + offset[0], imgPx[1] + offset[1]][1]) - int(img_sample[samplePx[0] + offset[0], samplePx[1] + offset[1]][1])
    err_b = int(img[imgPx[0] + offset[0], imgPx[1] + offset[1]][2]) - int(img_sample[samplePx[0] + offset[0], samplePx[1] + offset[1]][2])
    return (err_r**2 + err_g**2 + err_b**2) / 3.0

#---------------------------------------------------------------#
#|                  Calculating Cost                           |#
#---------------------------------------------------------------#

def GetCostVertical(imgPx, samplePx):
    Cost = np.zeros((PatchSize, OverlapWidth))
    for j in range(OverlapWidth):
        for i in range(PatchSize):
            if i == PatchSize - 1:
                Cost[i,j] = SSD_Error((i ,j - OverlapWidth), imgPx, samplePx)
            else:
                if j == 0 :
                    Cost[i,j] = SSD_Error((i , j - OverlapWidth), imgPx, samplePx) + min( SSD_Error((i + 1, j - OverlapWidth), imgPx, samplePx),SSD_Error((i + 1,j + 1 - OverlapWidth), imgPx, samplePx) )
                elif j == OverlapWidth - 1:
                    Cost[i,j] = SSD_Error((i, j - OverlapWidth), imgPx, samplePx) + min( SSD_Error((i + 1, j - OverlapWidth), imgPx, samplePx), SSD_Error((i + 1, j - 1 - OverlapWidth), imgPx, samplePx) )
                else:
                    Cost[i,j] = SSD_Error((i, j -OverlapWidth), imgPx, samplePx) + min(SSD_Error((i + 1, j - OverlapWidth), imgPx, samplePx),SSD_Error((i + 1, j + 1 - OverlapWidth), imgPx, samplePx),SSD_Error((i + 1, j - 1 - OverlapWidth), imgPx, samplePx))
    return Cost

def GetCostHorizntl(imgPx, samplePx):
    Cost = np.zeros((OverlapWidth, PatchSize))
    for i in range( OverlapWidth ):
        for j in range( PatchSize ):
            if j == PatchSize - 1:
                Cost[i,j] = SSD_Error((i - OverlapWidth, j), imgPx, samplePx)
            elif i == 0:
                Cost[i,j] = SSD_Error((i - OverlapWidth, j), imgPx, samplePx) + min(SSD_Error((i - OverlapWidth, j + 1), imgPx, samplePx), SSD_Error((i + 1 - OverlapWidth, j + 1), imgPx, samplePx))
            elif i == OverlapWidth - 1:
                Cost[i,j] = SSD_Error((i - OverlapWidth, j), imgPx, samplePx) + min(SSD_Error((i - OverlapWidth, j + 1), imgPx, samplePx), SSD_Error((i - 1 - OverlapWidth, j + 1), imgPx, samplePx))
            else:
                Cost[i,j] = SSD_Error((i - OverlapWidth, j), imgPx, samplePx) + min(SSD_Error((i - OverlapWidth, j + 1), imgPx, samplePx), SSD_Error((i + 1 - OverlapWidth, j + 1), imgPx, samplePx), SSD_Error((i - 1 - OverlapWidth, j + 1), imgPx, samplePx))
    return Cost

#---------------------------------------------------------------#
#|                  Finding Minimum Cost Path                  |#
#---------------------------------------------------------------#

def FindMinCostPathVertical(Cost):
    Boundary = np.zeros((PatchSize),np.int)
    ParentMatrix = np.zeros((PatchSize, OverlapWidth),np.int)
    for i in range(1, PatchSize):
        for j in range(OverlapWidth):
            if j == 0:
                ParentMatrix[i,j] = j if Cost[i-1,j] < Cost[i-1,j+1] else j+1
            elif j == OverlapWidth - 1:
                ParentMatrix[i,j] = j if Cost[i-1,j] < Cost[i-1,j-1] else j-1
            else:
                curr_min = j if Cost[i-1,j] < Cost[i-1,j-1] else j-1
                ParentMatrix[i,j] = curr_min if Cost[i-1,curr_min] < Cost[i-1,j+1] else j+1
            Cost[i,j] += Cost[i-1, ParentMatrix[i,j]]
    minIndex = 0
    for j in range(1,OverlapWidth):
        minIndex = minIndex if Cost[PatchSize - 1, minIndex] < Cost[PatchSize - 1, j] else j
    Boundary[PatchSize-1] = minIndex
    for i in range(PatchSize - 1,0,-1):
        Boundary[i - 1] = ParentMatrix[i,Boundary[i]]
    return Boundary

def FindMinCostPathHorizntl(Cost):
    Boundary = np.zeros(( PatchSize),np.int)
    ParentMatrix = np.zeros((OverlapWidth, PatchSize),np.int)
    for j in range(1, PatchSize):
        for i in range(OverlapWidth):
            if i == 0:
                ParentMatrix[i,j] = i if Cost[i,j-1] < Cost[i+1,j-1] else i + 1
            elif i == OverlapWidth - 1:
                ParentMatrix[i,j] = i if Cost[i,j-1] < Cost[i-1,j-1] else i - 1
            else:
                curr_min = i if Cost[i,j-1] < Cost[i-1,j-1] else i - 1
                ParentMatrix[i,j] = curr_min if Cost[curr_min,j-1] < Cost[i-1,j-1] else i + 1
            Cost[i,j] += Cost[ParentMatrix[i,j], j-1]
    minIndex = 0
    for i in range(1,OverlapWidth):
        minIndex = minIndex if Cost[minIndex, PatchSize - 1] < Cost[i, PatchSize - 1] else i
    Boundary[PatchSize-1] = minIndex
    for j in range(PatchSize - 1,0,-1):
        Boundary[j - 1] = ParentMatrix[Boundary[j],j]
    return Boundary

#---------------------------------------------------------------#
#|                      Quilting                               |#
#---------------------------------------------------------------#

def QuiltVertical(Boundary, imgPx, samplePx):
    for i in range(PatchSize):
        for j in range(Boundary[i], 0, -1):
            img[imgPx[0] + i, imgPx[1] - j] = img_sample[ samplePx[0] + i, samplePx[1] - j ]
def QuiltHorizntl(Boundary, imgPx, samplePx):
    for j in range(PatchSize):
        for i in range(Boundary[j], 0, -1):
            img[imgPx[0] - i, imgPx[1] + j] = img_sample[samplePx[0] - i, samplePx[1] + j]

def QuiltPatches( imgPx, samplePx ):
    #check for top layer
    if imgPx[0] == 0:
        Cost = GetCostVertical(imgPx, samplePx)
        #print "- Patches ", imgPx, samplePx
        #print "-   Quilting Cost ", Cost
        # Getting boundary to stitch
        Boundary = FindMinCostPathVertical(Cost)
        #Quilting Patches
        QuiltVertical(Boundary, imgPx, samplePx)
    #check for leftmost layer
    elif imgPx[1] == 0:
        Cost = GetCostHorizntl(imgPx, samplePx)
        #Boundary to stitch
        Boundary = FindMinCostPathHorizntl(Cost)
        #Quilting Patches
        QuiltHorizntl(Boundary, imgPx, samplePx)
    #for pixel placed inside 
    else:
        CostVertical = GetCostVertical(imgPx, samplePx)
        CostHorizntl = GetCostHorizntl(imgPx, samplePx)
        BoundaryVertical = FindMinCostPathVertical(CostVertical)
        BoundaryHorizntl = FindMinCostPathHorizntl(CostHorizntl)
        QuiltVertical(BoundaryVertical, imgPx, samplePx)
        QuiltHorizntl(BoundaryHorizntl, imgPx, samplePx)

#--------------------------------------------------------------------------------------------------------#
#                                   Growing Image Patch-by-patch                                        |#
#--------------------------------------------------------------------------------------------------------#

def FillImage( imgPx, samplePx ):
    for i in range(PatchSize):
        for j in range(PatchSize):
            img[ imgPx[0] + i, imgPx[1] + j ] = img_sample[ samplePx[0] + i, samplePx[1] + j ]




def synthesisTexture(InputName, patchLen, overlapSize, InitialThresConstant, verbose):
    global img_sample
    global img
    global sample_width
    global sample_height
    global OverlapWidth
    global PatchSize
    
    OverlapWidth = overlapSize
    PatchSize = patchLen
    img_sample = cv2.imread(InputName)
    if verbose == True:
        print(  "- img_sample type: ",type(img_sample).__module__,type(img_sample).__name__)
    
    sample_width  = img_sample.shape[1]
    sample_height = img_sample.shape[0]
    img = np.zeros((img_height,img_width,3), np.uint8)
    if verbose == True:
        print(  "- Sample Image: %dX%d." % (sample_width, sample_height))
        print(  "- Overlap width=%d, InitialThreadConst=%d." % (OverlapWidth, InitialThresConstant))
        print(  "- Systhesised Image: %dX%d" % (img_width, img_height))

    #randomly pick a patch to begin
    randomPatchHeight = randint(0, sample_height - PatchSize)
    randomPatchWidth  = randint(0, sample_width  - PatchSize)
    for i in range(PatchSize):
        for j in range(PatchSize):
            img[i, j] = img_sample[randomPatchHeight + i, randomPatchWidth + j]

    #initializating next 
    GrowPatchLocation = (0,PatchSize)
    if verbose == True:
        print ("- Init Location: ", GrowPatchLocation)
         
    pixelsCompleted = 0
    TotalPatches = ( (img_height - 1 )/ PatchSize )*((img_width)/ PatchSize) - 1

    #Iterate 
    while GrowPatchLocation[0] + PatchSize < img_height:
        pixelsCompleted += 1
        ThresholdConstant = InitialThresConstant

        print ("- Current Location: ", GrowPatchLocation )
        cv2.imshow('Textured Image',img)
        cv2.waitKey(1)
        progress = 0 
        while progress == 0:
            ThresholdOverlapError = ThresholdConstant * PatchSize * OverlapWidth
            #Get Best matches for current pixel
            print( "- Iterate all possible Patches, ThresholdOverlapError = %f." % ThresholdOverlapError)
            List = GetBestPatches(GrowPatchLocation, ThresholdOverlapError)
            #Found matched candidates, do quilting
            if len(List) > 0:
                progress = 1
                #Randomly pick a patch from best fit patches
                sampleMatch = List[ randint(0, len(List) - 1) ]

                #deep copy
                tempImage = img_sample.copy();
                tempPt1   = sampleMatch
                tempPt2   = (sampleMatch[0] + PatchSize, sampleMatch[1] + PatchSize)
                print ("- Selected Patch Location: ", tempPt1, tempPt2)
                cv2.rectangle(tempImage, tempPt1, tempPt2, (0, 0, 255)) #draw selected patch
                cv2.imshow('Selected Patch',tempImage)
                cv2.waitKey(1)

                FillImage( GrowPatchLocation, sampleMatch )
               
                QuiltPatches( GrowPatchLocation, sampleMatch ) #Quilt Patches
                print( "- Quit patches ", GrowPatchLocation, sampleMatch )  

                #Upadate next patch location
                GrowPatchLocation = (GrowPatchLocation[0], GrowPatchLocation[1] + PatchSize)
                if GrowPatchLocation[1] + PatchSize > img_width:
                    GrowPatchLocation = (GrowPatchLocation[0] + PatchSize, 0)
            
            # no candidate patches found, adjust threshold, search again
            else: 
                ThresholdConstant *= 1.1
        # print pixelsCompleted, ThresholdConstant
    
    # Displaying Result
    sys.stdout.write('- Done.')
    cv2.imshow('Textured Image',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    print( "- Unit test.")
    InputName = '.\\dat\\pattern.jpg'
    PatchSize = 30
    OverlapWidth = 5
    InitialThresConstant = 75.0
    verbose = True
    synthesisTexture(InputName, PatchSize, OverlapWidth, InitialThresConstant, verbose)
