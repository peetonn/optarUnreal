#include "CoreMinimal.h"
#include "Messages/ROSMessageBase.h"
#include "DataHelpers.h"
#include "ROSMsgTrack.h"
#include "ROSMsgHeader.h"
#include "ROSMsgTrackArray.generated.h"


// wrapper for opt_msgs/TrackArray
// https://github.com/OpenPTrack/open_ptrack_v2/blob/master/opt_msgs/msg/TrackArray.msg
UCLASS(BlueprintType)
class OPTMSGS_API UROSMsgTrackArray : public UROSMessageBase
{
    GENERATED_BODY()

public:
    /* Construction */
    UROSMsgTrackArray(){};
    UFUNCTION(BlueprintCallable, BlueprintPure) FString GetMessageType() override { return "opt_msgs/TrackArray"; }
    UFUNCTION(BlueprintCallable, BlueprintPure) static UROSMsgTrackArray* CreateEmpty();

    UPROPERTY(EditAnywhere, BlueprintReadWrite) UROSMsgHeader* Header;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UROSMsgTrack*> Tracks;

    /* Transformation Functions */
    void ToData(ROSData& OutMessage) const override;
    bool FromData(const ROSData& Message) override;
};
