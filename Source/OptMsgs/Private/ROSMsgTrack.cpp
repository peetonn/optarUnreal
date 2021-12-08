#include "ROSMsgTrack.h"

UROSMsgTrack* UROSMsgTrack::CreateEmpty()
{
    UROSMsgTrack* Message = NewObject<UROSMsgTrack>();
    Message->BoundingBox = NewObject<UROSMsgBoundingBox2D>();
    return Message;
}

void UROSMsgTrack::ToData(ROSData& OutMessage) const
{
    DataHelpers::Append<int>(OutMessage, "id", Id);
    DataHelpers::Append<double>(OutMessage, "x", x_);
    DataHelpers::Append<double>(OutMessage, "y", y_);
    DataHelpers::Append<double>(OutMessage, "height", height_);
    DataHelpers::Append<double>(OutMessage, "distance", Distance);
    DataHelpers::Append<double>(OutMessage, "age", Age);
    DataHelpers::Append<double>(OutMessage, "confidence", Confidence);
    DataHelpers::Append<uint8_t>(OutMessage, "visibility", visibility_);
    DataHelpers::Append<int>(OutMessage, "stable_id", StableId);
    DataHelpers::Append<UROSMsgBoundingBox2D*>(OutMessage, "box_2D", BoundingBox);
    DataHelpers::Append<FString>(OutMessage, "object_name", ObjectName);
    DataHelpers::Append<FString>(OutMessage, "face_name", FaceName);
}

bool UROSMsgTrack::FromData(const ROSData& Message)
{
    return DataHelpers::Extract<int>(Message, "id", Id) &&
            DataHelpers::Extract<double>(Message, "x", x_) &&
            DataHelpers::Extract<double>(Message, "y", y_) &&
            DataHelpers::Extract<double>(Message, "height", height_) &&
            DataHelpers::Extract<double>(Message, "distance", Distance) &&
            DataHelpers::Extract<double>(Message, "age", Age) &&
            DataHelpers::Extract<double>(Message, "confidence", Confidence) &&
            DataHelpers::Extract<uint8_t>(Message, "visibility", visibility_) &&
            DataHelpers::Extract<int>(Message, "stable_id", StableId) &&
            DataHelpers::Extract<UROSMsgBoundingBox2D*>(Message, "box_2D", BoundingBox) &&
            DataHelpers::Extract<FString>(Message, "object_name", ObjectName) &&
            DataHelpers::Extract<FString>(Message, "face_name", FaceName);
}

FVector UROSMsgTrack::GetPositionAsVector()
{
    return FVector(x_, y_, height_);
}

ETrackVisibility UROSMsgTrack::GetVisibility()
{
    switch(visibility_) {
        case 0:
            return ETrackVisibility::Visible;
        case 1:
            return ETrackVisibility::Occluded;
        default:
            return ETrackVisibility::NotVisible;
    }
    return ETrackVisibility::NotVisible;
}
