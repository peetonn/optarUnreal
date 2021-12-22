//
//  optar-worker.h
//  arcpp1
//
//  Created by Peter Gusev on 11/5/19.
//  Copyright © 2019 UCLA. All rights reserved.
//

#pragma once

#include "ARTextures.h"
#include "ARBlueprintLibrary.h"
#include <string>

#if PLATFORM_ANDROID || PLATFORM_IOS
#include "optar.hpp"
#endif

#include "OptarWorker.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewOptTransform,
                                            FTransform, transform);

UENUM(BlueprintType)
enum class OptarStatus : uint8 {
  Disconnected  UMETA(DisplayName = "Disconnected"),
  Processing     UMETA(DisplayName = "Processing"),
  Relocalized     UMETA(DisplayName = "Relocalized"),
};

UCLASS(config = Game, BlueprintType, ClassGroup = (Optar), meta=(BlueprintSpawnableComponent))
class UOptarWorker : public UObject {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (Keywords = "Optar", DisplayName = "Get Optar Version"), Category = "Optar")
    static FString getOptarVersion();

    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (DisplayName = "Get Singleton"), Category = "Optar")
    static UOptarWorker* sharedInstance();

    UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Optar")
    static UOptarWorker* GetDefaultWorker(); //TSubclassOf<UObject> ObjectClass);

    ~UOptarWorker();

    UPROPERTY(BlueprintCallable, BlueprintAssignable)
    FOnNewOptTransform OnNewOptTransformReceived;

    UFUNCTION(BlueprintCallable, Category="Optar")
    void init(FString deviceId, FString serverIpAddress);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Process Camera Image"), Category = "Optar")
    void processCameraImage(UARTexture *cameraImageTexture, int &nKeypoints);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Publish AR Pose"), Category = "Optar")
    void publishArPose(FTransform pose);

    UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Status"), Category = "Optar")
    OptarStatus getStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Optar")
    FString getServerUrl() const;

    UFUNCTION(BlueprintCallable, Category = "Optar")
    FString getDeviceId() const;

private:
    UOptarWorker();

    UPROPERTY()
    USceneComponent *arWorldOrigin_;

    FString serverUrl_, deviceId_;
    OptarStatus status_;
    bool initialized_;
#if PLATFORM_ANDROID || PLATFORM_IOS
    std::shared_ptr<optar::OptarClient> optarClient_;
    UARPin *arWorldOriginAnchor_;
    FOnARTransformUpdated onArTransformUpdateDelegate_;

    static void optarOnNewTransform(const optar::Transform& t, int64_t tsUsec, void *userData);

    void debug_textureInfo(UTexture2D *tex);
    void initOptarClient(std::string uuid, std::string serverIp);
    void onNewOptarTransform(const optar::Transform& t, int64_t tsUsec);
#endif

    UFUNCTION()
    void onTransformUpdated(const FTransform& transform);
};
// test
