//
//  optar-worker.cpp
//  arcpp1
//
//  Created by Peter Gusev on 11/5/19.
//  Copyright © 2019 UCLA. All rights reserved.
//

#include "OptarWorker.h"
#include "ARPin.h"
#include "ARTypes.h"
#include "GenericPlatform/GenericPlatformMisc.h"

#if PLATFORM_ANDROID
#include "GoogleARCoreCameraImage.h"
#include "GoogleARCoreFunctionLibrary.h"
#include "GoogleARCoreCameraIntrinsics.h"
#endif

#if PLATFORM_IOS
#import <CoreVideo/CoreVideo.h>
#import <CoreImage/CoreImage.h>
#include "AppleARKitTextures.h"
#include "RHICommandList.h"
#endif

#include <stdio.h>
#include <cstdio>
#include <cstring>

using namespace std;
#if PLATFORM_ANDROID || PLATFORM_IOS
using namespace optar;
#endif

extern FString GFilePathBase;

void optarLibraryLog(const char* msg)
{
    GLog->Logf(TEXT("[optar-module-lib] %s"), ANSI_TO_TCHAR(msg));
}

#if PLATFORM_ANDROID || PLATFORM_IOS
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
#if PLATFORM_ANDROID || PLATFORM_IOS
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
#if PLATFORM_ANDROID || PLATFORM_IOS
    return optar::getLibraryVersion();
#else
    return "not supported";
#endif

}

UOptarWorker::UOptarWorker() : initialized_(false){
#if PLATFORM_ANDROID || PLATFORM_IOS
    GLog->Logf(TEXT("[optar-module] UOptarWorker ctor %x id %d name %s"), this,
               this->GetUniqueID(), (*(this->GetFName().ToString())));

    registerLogCallback(optarLibraryLog);
#endif
}

UOptarWorker::~UOptarWorker()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    GLog->Logf(TEXT("[optar-module] UOptarWorker dtor %x"), this);
#endif
}

void UOptarWorker::init(FString deviceId, FString serverIpAddress)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    initOptarClient(TCHAR_TO_ANSI(*deviceId), TCHAR_TO_ANSI(*serverIpAddress));
#endif
}

void UOptarWorker::processCameraImage(UARTexture *cameraImageTexture, int &nKeypoints)
{
    if (!cameraImageTexture)
    {
#if PLATFORM_ANDROID || PLATFORM_IOS
        GLog->Logf(TEXT("[optar-module] camera image texture is invalid"));
#endif
        return ;
    }

#if PLATFORM_IOS
    if (initialized_)
    {
        if (cameraImageTexture->GetResource())
        {
            FRHITexture2D *tex2D = cameraImageTexture->GetResource()->GetTexture2DRHI();

            GLog->Logf(TEXT("processing camera image. size %dx%d pixel format %d"),
                        tex2D->GetSizeX(), tex2D->GetSizeY(), tex2D->GetFormat());

            TArray<FColor> outPixels;
            outPixels.Reset();

            ENQUEUE_RENDER_COMMAND()(
                [tex2D, &outPixels](FRHICommandListImmediate& RHICmdList){
                    RHICmdList.ReadSurfaceData(
                        tex2D,
                        FIntRect(0, 0, tex2D->GetSizeXY().X, tex2D->GetSizeXY().Y),
                        outPixels,
                        FReadSurfaceDataFlags(RCM_UNorm, CubeFace_MAX)
                    );
            });
            FlushRenderingCommands();

            // process image
            FARCameraIntrinsics cameraIntrinsics;
            // TODO: how often should camera intrinsics be queried? (once at startup?)
            if (!UARBlueprintLibrary::GetCameraIntrinsics(cameraIntrinsics))
                GLog->Logf(TEXT("[optar-module] couldn't retrieve camera intrinsics"));
            else
            {
                CameraIntrinsics ci;
                ci.focalLengthX_ = cameraIntrinsics.FocalLength.X;
                ci.focalLengthY_ = cameraIntrinsics.FocalLength.Y;
                ci.principalPointX_ = cameraIntrinsics.PrincipalPoint.X;
                ci.principalPointY_ = cameraIntrinsics.PrincipalPoint.Y;
                ci.imageWidth_ = cameraIntrinsics.ImageResolution.X;
                ci.imageHeight_ = cameraIntrinsics.ImageResolution.Y;

                optarClient_->processTexture(tex2D->GetSizeX(), tex2D->GetSizeY(),
                                             outPixels.GetData(),
                                             nKeypoints,
                                             ci,
                                             true);
                GLog->Logf(TEXT("[optar-module] image processed. found %d ORB keypoints"), nKeypoints);
            }
        }
        else
            GLog->Logf(TEXT("texture resource is NULL"));
    }
#endif
}

void UOptarWorker::publishArPose(FTransform pose)
{
#if PLATFORM_ANDROID || PLATFORM_IOS
    FARSessionStatus sessionStatus = UARBlueprintLibrary::GetARSessionStatus();

    if (initialized_ && sessionStatus.Status == EARSessionStatus::Running)
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

#if PLATFORM_ANDROID || PLATFORM_IOS
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

void UOptarWorker::initOptarClient(string uuid, string serverIp)
{
    Settings s;
    s.rosIp_ = nullptr;
    s.rosHostname_ = nullptr;
    s.showDebugImage_ = true;

    static char serverAddress[256];
    memset(serverAddress, 0, 256);

    sprintf(serverAddress, "http://%s:11311", serverIp.c_str());
    s.rosUri_ = serverAddress;

    static char deviceId[512];
    memset(deviceId, 0, 512);
    sprintf(deviceId, "%s", uuid.c_str());
    // memcpy(deviceId, uuid.c_str(), uuid.size());

    s.deviceId_ = deviceId;

    // NOTE: this will be called on ROS thread (not main thread)
    s.transformCallback_ = &UOptarWorker::optarOnNewTransform;
    s.userData_ = this;
    s.rawImageScaleDownF_ = 4;
    s.targetFps_ = 2;

    optarClient_ = make_shared<OptarClient>(s);
    initialized_ = true;

    GLog->Logf(TEXT("[optar-module] OptarClient initialized %x. Settings: %s"),
               optarClient_.get(),
               ANSI_TO_TCHAR(s.toString()));
}

void UOptarWorker::onNewOptarTransform(const Transform& t, int64_t tsUsec)
{
    GLog->Logf(TEXT("[optar-module] new OPTAR transform (world -> AR tracking space) received"));
    // GLog->Logf(TEXT("[optar-module] new OPTAR transform (world -> AR tracking space) received: %dusec [%.2f %.2f %.2f] [%.2f %.2f %.2f %.2f]"),
               // tsUsec, t.position_.x_, t.position_.y_, t.position_.z_,
               // t.rotation_.x_, t.rotation_.y_, t.rotation_.z_, t.rotation_.w_);
}

#endif

// TODO: supposedly this is only needed for ARCore (Android)
void UOptarWorker::onTransformUpdated(const FTransform&)
{
    GLog->Logf(TEXT("[optar-module] world origin transform updated"));
    GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green,
                                     TEXT("world origin transform changed"));
}
