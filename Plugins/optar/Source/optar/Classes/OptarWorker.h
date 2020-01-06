//
//  optar-worker.h
//  arcpp1
//
//  Created by Peter Gusev on 11/5/19.
//  Copyright © 2019 UCLA. All rights reserved.
//

#pragma once

#include "optar.hpp"
#include "ARTextures.h"
#include "ARBlueprintLibrary.h"
#include "OptarWorker.generated.h"

UCLASS(config = Game, BlueprintType, ClassGroup = (Optar), meta=(BlueprintSpawnableComponent))
class UOptarWorker : public UObject {
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable, meta = (Keywords = "Optar", DisplayName = "Get Optar Version"), Category = "Optar")
    static FString getOptarVersion();
    
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Singleton"), Category = "Optar")
    static UOptarWorker* sharedInstance();
    
    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Optar")
    static UOptarWorker* GetDefaultWorker(); //TSubclassOf<UObject> ObjectClass);
    
    ~UOptarWorker();
    
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Process ARCore Camera Image"), Category = "Optar")
    void processArCoreImage(UGoogleARCoreCameraImage *arCoreImage, int &nKeypoints);
    
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Publish AR Pose"), Category = "Optar")
    void publishArPose(FTransform pose);
    
    
private:
    UOptarWorker();
    
    UPROPERTY()
    USceneComponent *arWorldOrigin_;
    
#if PLATFORM_ANDROID
    std::shared_ptr<optar::OptarClient> optarClient_;
    UARPin *arWorldOriginAnchor_;
    FOnARTransformUpdated onArTransformUpdateDelegate_;
    
    static void optarOnNewTransform(const optar::Transform& t, int64_t tsUsec, void *userData);
    
    void debug_textureInfo(UTexture2D *tex);
    void initOptarClient();
    void onNewOptarTransform(const optar::Transform& t, int64_t tsUsec);
#endif
    
    UFUNCTION()
    void onTransformUpdated(const FTransform& transform);
};
