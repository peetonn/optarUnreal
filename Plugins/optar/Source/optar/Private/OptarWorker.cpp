//
//  optar-worker.cpp
//  arcpp1
//
//  Created by Peter Gusev on 11/5/19.
//  Copyright © 2019 UCLA. All rights reserved.
//

#include "OptarWorker.h"
#include "ARPin.h"
#include "GoogleARCoreCameraImage.h"
#include "GoogleARCoreFunctionLibrary.h"
#include "GoogleARCoreCameraIntrinsics.h"
#include "GenericPlatform/GenericPlatformMisc.h"

#include <stdio.h>

using namespace std;
using namespace optar;

extern FString GFilePathBase;

void optarLibraryLog(const char* msg)
{
    GLog->Logf(TEXT("[optar-module-lib] %s"), ANSI_TO_TCHAR(msg));
}

#if PLATFORM_ANDROID
void UOptarWorker::optarOnNewTransform(const Transform& t, int64_t tsUsec, void *userData)
{
    UOptarWorker* optarWorker = (UOptarWorker*)userData;
    
    if (optarWorker)
    {
        optarWorker->onNewOptarTransform(t, tsUsec);
    }
}
#endif

UOptarWorker*
UOptarWorker::sharedInstance()
{
#if PLATFORM_ANDROID
    GLog->Logf(TEXT("[optar-module] get singleton"));
    
    static UOptarWorker* singleton = nullptr;
    if (!singleton)
    {
        GLog->Logf(TEXT("[optar-module] create singleton"));
        singleton = NewObject<UOptarWorker>(UOptarWorker::StaticClass());
    }
    return singleton;
#endif
    return nullptr;
}

UOptarWorker*
UOptarWorker::GetDefaultWorker()
{
    return (UOptarWorker*)UOptarWorker::StaticClass()->GetDefaultObject();
}

FString UOptarWorker::getOptarVersion() {
#if PLATFORM_ANDROID
    return optar::getLibraryVersion();
#else
    return "not supported";
#endif
    
}

UOptarWorker::UOptarWorker(){
#if PLATFORM_ANDROID
    GLog->Logf(TEXT("[optar-module] UOptarWorker ctor %x id %d name %s"), this,
               this->GetUniqueID(), (*(this->GetFName().ToString())));
    
    registerLogCallback(optarLibraryLog);
    initOptarClient();
#endif
}

UOptarWorker::~UOptarWorker()
{
#if PLATFORM_ANDROID
    GLog->Logf(TEXT("[optar-module] UOptarWorker dtor %x"), this);
#endif
}

void UOptarWorker::processArCoreImage(UGoogleARCoreCameraImage *arCoreImage, int &nKeypoints)
{
#if PLATFORM_ANDROID
    if (arCoreImage)
    {
        UGoogleARCoreCameraIntrinsics *cameraIntrinsics;
        EGoogleARCoreFunctionStatus status = UGoogleARCoreFrameFunctionLibrary::GetCameraImageIntrinsics(cameraIntrinsics);
        
        if (arCoreImage->GetPlaneCount() == 3  &&
            status == EGoogleARCoreFunctionStatus::Success)
        {
            CameraIntrinsics ci;
            
            cameraIntrinsics->GetFocalLength(ci.focalLengthX_, ci.focalLengthY_);
            cameraIntrinsics->GetPrincipalPoint(ci.principalPointX_, ci.principalPointY_);
            cameraIntrinsics->GetImageDimensions(ci.imageWidth_, ci.imageHeight_);
            
            int pixStride, rowStride, dataLen;
//            Point2D *keypoints;
            // here we get pointer to the first plane, hoping that other planes are adjacent to it
            // and form a continuous memory region
            uint8_t *yuvData = arCoreImage->GetPlaneData(0, pixStride, rowStride, dataLen);
            optarClient_->processTexture(arCoreImage->GetWidth(), arCoreImage->GetHeight(),
                                         rowStride, yuvData,
                                         nKeypoints,
                                         ci,
                                         true);
//            GLog->Logf(TEXT("[optar-module] found %d ORB keypoints"), nKeypoints);
        }

        arCoreImage->Release();
    }
    else
        GLog->Logf(TEXT("[optar-module] ARCore image is null"));

#endif
}

void UOptarWorker::publishArPose(FTransform pose)
{
#if PLATFORM_ANDROID
    FARSessionStatus sessionStatus = UARBlueprintLibrary::GetARSessionStatus();
    
    if (sessionStatus.Status == EARSessionStatus::Running)
    {
        EARWorldMappingState mappingState = UARBlueprintLibrary::GetWorldMappingStatus();
        EARTrackingQualityReason trackingQualityReason = UARBlueprintLibrary::GetTrackingQualityReason();
     
        if (!arWorldOriginAnchor_)
            GLog->Logf(TEXT("[optar-module] AR session status: %d (%s). mapping state %d. tracking quality %d"),
                       sessionStatus.Status,
                       *sessionStatus.AdditionalInfo,
                       mappingState,
                       trackingQualityReason);
        
        if (mappingState != EARWorldMappingState::NotAvailable && !arWorldOriginAnchor_)
        {
            GLog->Logf(TEXT("[optar-module] initializing world origin tracking"));

            arWorldOrigin_ = NewObject<USceneComponent>();
            arWorldOrigin_->AddToRoot();
            
            arWorldOriginAnchor_ = UARBlueprintLibrary::PinComponent(arWorldOrigin_, pose);
            
            if (arWorldOriginAnchor_)
            {
                onArTransformUpdateDelegate_.AddDynamic(this, &UOptarWorker::onTransformUpdated);
                arWorldOriginAnchor_->SetOnARTransformUpdated(onArTransformUpdateDelegate_);
                
                GLog->Logf(TEXT("[optar-module] anchored world origin succesfully"));
            }
            else
                GLog->Logf(TEXT("[optar-module] failed to anchor world origin"));
        }
        else if (arWorldOriginAnchor_ &&
                 arWorldOriginAnchor_->GetTrackingState() == EARTrackingState::Tracking)
        {

            FTransform localToTracking = arWorldOriginAnchor_->GetLocalToTrackingTransform();
            
            // transform pose from tracking space to local space of world origin
            pose = pose * localToTracking.Inverse();
//            pose = localToTracking.Inverse() * pose;
            // convert from cm (unreal) to meters (ROS)
            pose.ScaleTranslation(0.01);

            Pose optarPose;
            
            // we need to swap axes because Unreal has Z pointing up and Unity
            // (optar was built for Unity) has Y axis vertical
            // more info https://forums.unrealengine.com/development-discussion/c-gameplay-programming/103787-ue4-coordinate-system-not-right-handed
            
//            optarPose.position_.x_ = pose.GetLocation().X;
//            optarPose.position_.y_ = pose.GetLocation().Y;
//            optarPose.position_.z_ = pose.GetLocation().Z;
//            optarPose.rotation_.x_ = pose.GetRotation().X;
//            optarPose.rotation_.y_ = pose.GetRotation().Y;
//            optarPose.rotation_.z_ = pose.GetRotation().Z;
//            optarPose.rotation_.w_ = pose.GetRotation().W;
            optarPose.position_.x_ = pose.GetLocation().Y;
            optarPose.position_.y_ = pose.GetLocation().Z;
            optarPose.position_.z_ = pose.GetLocation().X;
            optarPose.rotation_.x_ = pose.GetRotation().Y;
            optarPose.rotation_.y_ = pose.GetRotation().Z;
            optarPose.rotation_.z_ = pose.GetRotation().X;
            optarPose.rotation_.w_ = pose.GetRotation().W;

            optarClient_->processArPose(optarPose);
        }
    }
#endif
}

#if PLATFORM_ANDROID
void UOptarWorker::debug_textureInfo(UTexture2D *tex)
{
    int width = tex->GetSizeX();
    int height = tex->GetSizeY();
    
    GLog->Logf(TEXT("[optar-module] texture size %d x %d"), width, height);
    
    if (tex->PlatformData)
    {
        GLog->Logf(TEXT("[optar-module] mips total %d pixel format %d"), tex->PlatformData->Mips.Num(), tex->PlatformData->PixelFormat);
        
        for (int mipIdx = 0; mipIdx < tex->PlatformData->Mips.Num(); ++mipIdx)
        {
            int mipWidth = tex->PlatformData->Mips[mipIdx].SizeX;
            int mipHeight = tex->PlatformData->Mips[mipIdx].SizeY;
            
            GLog->Logf(TEXT("[optar-module] mip %d -- %d x %d"), mipIdx, mipWidth, mipHeight);
        }
    }
}

void UOptarWorker::initOptarClient()
{
    Settings s;
    s.showDebugImage_ = true;
    s.rosMasterUri_ = "http://131.179.142.82:11311";
    
    char *devId = TCHAR_TO_ANSI(*FGenericPlatformMisc::GetDeviceId());
    s.deviceId_ = (char*)malloc(strlen(devId)+1);
    memset((void*)s.deviceId_, 0, strlen(devId)+1);
    strcpy((char*)s.deviceId_, devId);
    
    // NOTE: this will be called on ROS thread (not main thread)
    s.transformCallback_ = &UOptarWorker::optarOnNewTransform;
    s.userData_ = this;
    
    optarClient_ = make_shared<OptarClient>(s);
    
    GLog->Logf(TEXT("[optar-module] OptarClient initialized %x. Settings: %s"),
               optarClient_.get(),
               ANSI_TO_TCHAR(s.toString()));
}

void UOptarWorker::onNewOptarTransform(const Transform& t, int64_t tsUsec)
{
    GLog->Logf(TEXT("[optar-module] new OPTAR transform (world -> AR tracking space) received: %dusec [%.2f %.2f %.2f] [%.2f %.2f %.2f %.2f]"),
               tsUsec, t.position_.x_, t.position_.y_, t.position_.z_,
               t.rotation_.x_, t.rotation_.y_, t.rotation_.z_, t.rotation_.w_);
}

#endif

void UOptarWorker::onTransformUpdated(const FTransform&)
{
    GLog->Logf(TEXT("[optar-module] world origin transform updated"));
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green,
                                     TEXT("world origin transform changed"));
}
