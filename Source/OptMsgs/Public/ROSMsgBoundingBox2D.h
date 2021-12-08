#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "DataHelpers.h"
#include "ROSMsgBoundingBox2D.generated.h"

// wrapper for opt_msgs/BoundingBox2D
// https://github.com/OpenPTrack/open_ptrack_v2/blob/master/opt_msgs/msg/BoundingBox2D.msg
UCLASS(BlueprintType)
class OPTMSGS_API UROSMsgBoundingBox2D : public UROSMessageBase
{
    GENERATED_BODY()

public:
    /* Construction */
    UROSMsgBoundingBox2D(){};
    UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override { return "opt_msgs/BoundingBox2D"; }
    UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgBoundingBox2D* Create(const FBox2D& InBox);

    /* Data */
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Data;
    UFUNCTION(BlueprintCallable, BlueprintPure) FBox2D GetBoundingBox();
    UFUNCTION(BlueprintCallable) void SetBoundingBox(const FBox2D& InBox);

    int x_;
    int y_;
    int width_;
    int height_;

    /* Transformation Functions */
    void ToData(ROSData& OutMessage) const override;
    bool FromData(const ROSData& Message) override;
};
