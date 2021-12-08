#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "DataHelpers.h"
#include "ROSMsgBoundingBox2D.h"
#include "ROSMsgTrack.generated.h"

UENUM(BlueprintType)
enum class ETrackVisibility : uint8 {
    Visible     UMETA(DisplayName="Visible"),
    Occluded    UMETA(DisplayName="Occluded"),
    NotVisible  UMETA(DisplayName="Not Visible"),
};

// wrapper for opt_msgs/Track
// https://github.com/OpenPTrack/open_ptrack_v2/blob/master/opt_msgs/msg/Track.msg
UCLASS(BlueprintType)
class OPTMSGS_API UROSMsgTrack : public UROSMessageBase
{
    GENERATED_BODY()

public:
    /* Construction */
    UROSMsgTrack(){};
    UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override { return "opt_msgs/Track"; }
    UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTrack* CreateEmpty();

    UFUNCTION(BlueprintCallable, BlueprintPure) FVector GetPositionAsVector();
    UFUNCTION(BlueprintCallable, BlueprintPure) ETrackVisibility GetVisibility();

    UPROPERTY(EditAnywhere, BlueprintReadWrite) int Id;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int StableId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) double Distance;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) double Age;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) double Confidence;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString ObjectName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FString FaceName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgBoundingBox2D* BoundingBox;

    double x_;
    double y_;
    double height_;

    uint8_t visibility_;

    /* Transformation Functions */
    void ToData(ROSData& OutMessage) const override;
    bool FromData(const ROSData& Message) override;
};
