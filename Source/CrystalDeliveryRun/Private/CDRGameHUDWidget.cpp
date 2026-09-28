#include "CDRGameHUDWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Styling/CoreStyle.h"

TSharedRef<SWidget> UCDRGameHUDWidget::RebuildWidget()
{
	UCanvasPanel* RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(
		UCanvasPanel::StaticClass(),
		TEXT("RootCanvas"));

	WidgetTree->RootWidget = RootCanvas;

	ScoreText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(),
		TEXT("ScoreText"));

	ScoreText->SetText(FText::FromString(TEXT("Puntaje: 0")));
	ScoreText->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 30));
	ScoreText->SetColorAndOpacity(FSlateColor(FLinearColor(0.1f, 1.0f, 0.2f, 1.0f)));

	UCanvasPanelSlot* ScoreSlot = RootCanvas->AddChildToCanvas(ScoreText);
	ScoreSlot->SetPosition(FVector2D(40.0f, 40.0f));
	ScoreSlot->SetAutoSize(true);

	return Super::RebuildWidget();
}

void UCDRGameHUDWidget::SetScore(int32 NewScore)
{
	if (IsValid(ScoreText))
	{
		ScoreText->SetText(
			FText::FromString(
				FString::Printf(TEXT("Puntaje: %d"), NewScore)));
	}
}
