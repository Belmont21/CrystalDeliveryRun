#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDRGameHUDWidget.generated.h"

class UTextBlock;

UCLASS()
class CRYSTALDELIVERYRUN_API UCDRGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetScore(int32 NewScore);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> ScoreText;
};
