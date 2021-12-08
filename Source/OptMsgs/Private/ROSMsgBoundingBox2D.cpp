#include "ROSMsgBoundingBox2D.h"

UROSMsgBoundingBox2D* UROSMsgBoundingBox2D::Create(const FBox2D& InBox)
{
    UROSMsgBoundingBox2D* Message = NewObject<UROSMsgBoundingBox2D>();
    Message->SetBoundingBox(InBox);
    return Message;
}

void UROSMsgBoundingBox2D::ToData(ROSData& OutMessage) const
{
    DataHelpers::Append<int>(OutMessage, "x", x_);
    DataHelpers::Append<int>(OutMessage, "y", y_);
    DataHelpers::Append<int>(OutMessage, "width", width_);
    DataHelpers::Append<int>(OutMessage, "height", height_);
}

bool UROSMsgBoundingBox2D::FromData(const ROSData& Message)
{
    return DataHelpers::Extract<int>(Message, "x", x_) &&
            DataHelpers::Extract<int>(Message, "y", y_) &&
            DataHelpers::Extract<int>(Message, "width", width_) &&
            DataHelpers::Extract<int>(Message, "height", height_);
}

FBox2D UROSMsgBoundingBox2D::GetBoundingBox()
{
    return FBox2D(FVector2D(x_-width_/2, y_-height_/2),
                  FVector2D(x_+width_/2, y_+height_/2));
}

void UROSMsgBoundingBox2D::SetBoundingBox(const FBox2D& InBox)
{
    x_ = (InBox.Min.X + InBox.Max.X)/2;
    y_ = (InBox.Min.Y + InBox.Max.Y)/2;
    width_ = (InBox.Max.X - InBox.Min.X);
    height_ = (InBox.Max.Y - InBox.Min.Y);
}
