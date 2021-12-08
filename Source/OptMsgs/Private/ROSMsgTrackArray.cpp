#include "ROSMsgTrackArray.h"

UROSMsgTrackArray* UROSMsgTrackArray::CreateEmpty()
{
    UROSMsgTrackArray* Message = NewObject<UROSMsgTrackArray>();
    Message->Header = NewObject<UROSMsgHeader>(Message);
    Message->Tracks = TArray<UROSMsgTrack*>();
    return Message;
}

void UROSMsgTrackArray::ToData(ROSData& OutMessage) const
{
    DataHelpers::Append<UROSMsgHeader*>(OutMessage, "header", Header);
    DataHelpers::Append<TArray<UROSMsgTrack*>>(OutMessage, "tracks", Tracks);
}

bool UROSMsgTrackArray::FromData(const ROSData& Message)
{
    return DataHelpers::Extract<UROSMsgHeader*>(Message, "header", Header) &&
            DataHelpers::Extract<TArray<UROSMsgTrack*>>(Message, "tracks", Tracks);
}
