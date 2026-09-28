#include "CDRMainMenuWidget.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Styling/CoreStyle.h"

TSharedRef<SWidget> UCDRMainMenuWidget::RebuildWidget()
{
	UCanvasPanel* RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(
		UCanvasPanel::StaticClass(), TEXT("RootCanvas"));

	WidgetTree->RootWidget = RootCanvas;

	UVerticalBox* MenuBox = WidgetTree->ConstructWidget<UVerticalBox>(
		UVerticalBox::StaticClass(), TEXT("MenuBox"));

	UCanvasPanelSlot* MenuSlot = RootCanvas->AddChildToCanvas(MenuBox);
	MenuSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	MenuSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	MenuSlot->SetAutoSize(true);

	UTextBlock* TitleText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(), TEXT("TitleText"));

	TitleText->SetText(FText::FromString(TEXT("CRYSTAL DELIVERY RUN")));
	TitleText->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 42));
	TitleText->SetJustification(ETextJustify::Center);

	UVerticalBoxSlot* TitleSlot = MenuBox->AddChildToVerticalBox(TitleText);
	TitleSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 40.0f));
	TitleSlot->SetHorizontalAlignment(HAlign_Center);

	HostButton = WidgetTree->ConstructWidget<UButton>(
		UButton::StaticClass(), TEXT("HostButton"));

	UTextBlock* HostText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(), TEXT("HostText"));

	HostText->SetText(FText::FromString(TEXT("HOST PARTIDA")));
	HostText->SetColorAndOpacity(FSlateColor(FLinearColor(0.1f, 1.0f, 0.2f, 1.0f)));
	HostText->SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 24));
	HostText->SetJustification(ETextJustify::Center);

	HostButton->AddChild(HostText);
	HostButton->OnClicked.AddDynamic(this, &UCDRMainMenuWidget::HandleHostClicked);

	USizeBox* HostSizeBox = WidgetTree->ConstructWidget<USizeBox>(
		USizeBox::StaticClass(), TEXT("HostSizeBox"));

	HostSizeBox->SetWidthOverride(360.0f);
	HostSizeBox->SetHeightOverride(60.0f);
	HostSizeBox->AddChild(HostButton);

	UVerticalBoxSlot* HostSlot = MenuBox->AddChildToVerticalBox(HostSizeBox);
	HostSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 20.0f));
	HostSlot->SetHorizontalAlignment(HAlign_Center);

	QuitButton = WidgetTree->ConstructWidget<UButton>(
		UButton::StaticClass(), TEXT("QuitButton"));

	UTextBlock* QuitText = WidgetTree->ConstructWidget<UTextBlock>(
		UTextBlock::StaticClass(), TEXT("QuitText"));

	QuitText->SetText(FText::FromString(TEXT("SALIR")));
	QuitText->SetColorAndOpacity(FSlateColor(FLinearColor(0.1f, 1.0f, 0.2f, 1.0f)));
	QuitText->SetFont(FCoreStyle::GetDefaultFontStyle("Regular", 24));
	QuitText->SetJustification(ETextJustify::Center);

	QuitButton->AddChild(QuitText);
	QuitButton->OnClicked.AddDynamic(this, &UCDRMainMenuWidget::HandleQuitClicked);

	USizeBox* QuitSizeBox = WidgetTree->ConstructWidget<USizeBox>(
		USizeBox::StaticClass(), TEXT("QuitSizeBox"));

	QuitSizeBox->SetWidthOverride(360.0f);
	QuitSizeBox->SetHeightOverride(60.0f);
	QuitSizeBox->AddChild(QuitButton);

	UVerticalBoxSlot* QuitSlot = MenuBox->AddChildToVerticalBox(QuitSizeBox);
	QuitSlot->SetHorizontalAlignment(HAlign_Center);

	return Super::RebuildWidget();
}

void UCDRMainMenuWidget::HandleHostClicked()
{
	APlayerController* PlayerController = GetOwningPlayer();

	if (IsValid(PlayerController))
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
	}

	UGameplayStatics::OpenLevel(this, FName("L_Game"), true, TEXT("listen"));
}


void UCDRMainMenuWidget::HandleQuitClicked()
{
	UKismetSystemLibrary::QuitGame(
		this,
		GetOwningPlayer(),
		EQuitPreference::Quit,
		false);
}
