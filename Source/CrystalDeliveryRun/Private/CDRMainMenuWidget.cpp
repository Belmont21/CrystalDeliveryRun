#include "CDRMainMenuWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Multiplayer/CDRGameInstance.h"
#include "Sound/SoundBase.h"
#include "Styling/CoreStyle.h"
#include "UObject/ConstructorHelpers.h"
#include "Brushes/SlateColorBrush.h"

UCDRMainMenuWidget::UCDRMainMenuWidget(
   const FObjectInitializer& ObjectInitializer)
   : Super(ObjectInitializer)
{
   static ConstructorHelpers::FObjectFinder<USoundBase> ClickSoundAsset(
      TEXT("/Game/Audio/sonido_de_clic.sonido_de_clic"));

   if (ClickSoundAsset.Succeeded())
   {
      ClickSound = ClickSoundAsset.Object;
   }

   static ConstructorHelpers::FObjectFinder<UTexture2D> BackgroundAsset(
      TEXT("/Game/UI/T_MenuPrincipal.T_MenuPrincipal"));

   if (BackgroundAsset.Succeeded())
   {
      MenuBackground = BackgroundAsset.Object;
   }
}

TSharedRef<SWidget> UCDRMainMenuWidget::RebuildWidget()
{
   

   UScaleBox* ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(
      UScaleBox::StaticClass(),
      TEXT("MenuScaleBox"));

   ScaleBox->SetStretch(EStretch::ScaleToFit);
   WidgetTree->RootWidget = ScaleBox;

   USizeBox* DesignSize = WidgetTree->ConstructWidget<USizeBox>(
      USizeBox::StaticClass(),
      TEXT("MenuDesignSize"));

   DesignSize->SetWidthOverride(1672.0f);
   DesignSize->SetHeightOverride(941.0f);
   ScaleBox->AddChild(DesignSize);

   UCanvasPanel* RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(
      UCanvasPanel::StaticClass(),
      TEXT("RootCanvas"));

   DesignSize->AddChild(RootCanvas);

   // Fondo del menu - imagen temporal (hacer la definitivadespues)

   UImage* BackgroundImage = WidgetTree->ConstructWidget<UImage>(
      UImage::StaticClass(),
      TEXT("BackgroundImage"));

   if (IsValid(MenuBackground))
   {
      BackgroundImage->SetBrushFromTexture(MenuBackground, false);
   }

   BackgroundImage->SetVisibility(ESlateVisibility::HitTestInvisible);

   UCanvasPanelSlot* BackgroundSlot =
      RootCanvas->AddChildToCanvas(BackgroundImage);

   BackgroundSlot->SetPosition(FVector2D(0.0f, 0.0f));
   BackgroundSlot->SetSize(FVector2D(1672.0f, 941.0f));
   BackgroundSlot->SetZOrder(0);

   // Boton transparente sobre HOST PARTIDA.

   HostButton = WidgetTree->ConstructWidget<UButton>(
      UButton::StaticClass(),
      TEXT("HostButton"));

   HostButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
   HostButton->OnClicked.AddDynamic(
      this,
      &UCDRMainMenuWidget::HandleHostClicked);

   UCanvasPanelSlot* HostSlot = RootCanvas->AddChildToCanvas(HostButton);
   HostSlot->SetPosition(FVector2D(580.0f, 468.0f));
   HostSlot->SetSize(FVector2D(525.0f, 82.0f));
   HostSlot->SetZOrder(1);

   // Campo editable para INSERTAR IP.
   
   UImage* AddressBackdrop = WidgetTree->ConstructWidget<UImage>(
   UImage::StaticClass(),
   TEXT("AddressBackdrop"));

   AddressBackdrop->SetBrush(FSlateColorBrush(
      FLinearColor(0.85f, 0.85f, 0.85f, 1.0f)));

   AddressBackdrop->SetVisibility(ESlateVisibility::HitTestInvisible);

   UCanvasPanelSlot* BackdropSlot =
      RootCanvas->AddChildToCanvas(AddressBackdrop);

   BackdropSlot->SetPosition(FVector2D(748.0f, 574.0f));
   BackdropSlot->SetSize(FVector2D(300.0f, 42.0f));
   BackdropSlot->SetZOrder(1);


   AddressInput = WidgetTree->ConstructWidget<UEditableTextBox>(
      UEditableTextBox::StaticClass(),
      TEXT("AddressInput"));

   AddressInput->SetForegroundColor(FLinearColor::Black);
   AddressInput->SetHintText(FText::FromString(TEXT("Insertar IP")));

   FEditableTextBoxStyle AddressStyle = AddressInput->GetWidgetStyle();
   AddressStyle.SetForegroundColor(FSlateColor(FLinearColor::Black));
   AddressStyle.SetFocusedForegroundColor(FSlateColor(FLinearColor::Black));
   AddressInput->SetWidgetStyle(AddressStyle);

   UCanvasPanelSlot* AddressSlot = RootCanvas->AddChildToCanvas(AddressInput);
   AddressSlot->SetPosition(FVector2D(748.0f, 574.0f));
   AddressSlot->SetSize(FVector2D(300.0f, 42.0f));
   AddressSlot->SetZOrder(2);

   

   JoinButton = WidgetTree->ConstructWidget<UButton>(
      UButton::StaticClass(),
      TEXT("JoinButton"));

   JoinButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
   JoinButton->OnClicked.AddDynamic(
      this,
      &UCDRMainMenuWidget::HandleJoinClicked);

   UCanvasPanelSlot* JoinSlot = RootCanvas->AddChildToCanvas(JoinButton);
   JoinSlot->SetPosition(FVector2D(585.0f, 638.0f));
   JoinSlot->SetSize(FVector2D(505.0f, 70.0f));
   JoinSlot->SetZOrder(1);

   

   QuitButton = WidgetTree->ConstructWidget<UButton>(
      UButton::StaticClass(),
      TEXT("QuitButton"));

   QuitButton->SetBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 0.0f));
   QuitButton->OnClicked.AddDynamic(
      this,
      &UCDRMainMenuWidget::HandleQuitClicked);

   UCanvasPanelSlot* QuitSlot = RootCanvas->AddChildToCanvas(QuitButton);
   QuitSlot->SetPosition(FVector2D(585.0f, 720.0f));
   QuitSlot->SetSize(FVector2D(495.0f, 70.0f));
   QuitSlot->SetZOrder(1);

   // Avisos del menu desde esta linea

   MenuMessage = WidgetTree->ConstructWidget<UTextBlock>(
      UTextBlock::StaticClass(),
      TEXT("MenuMessage"));

   MenuMessage->SetText(FText::GetEmpty());
   MenuMessage->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 22));
   MenuMessage->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
   MenuMessage->SetShadowColorAndOpacity(FLinearColor::Black);
   MenuMessage->SetShadowOffset(FVector2D(2.0f, 2.0f));
   MenuMessage->SetJustification(ETextJustify::Center);
   MenuMessage->SetVisibility(ESlateVisibility::HitTestInvisible);

   UCanvasPanelSlot* MessageSlot = RootCanvas->AddChildToCanvas(MenuMessage);
   MessageSlot->SetPosition(FVector2D(380.0f, 792.0f));
   MessageSlot->SetSize(FVector2D(912.0f, 32.0f));
   MessageSlot->SetZOrder(2);

   if (UWorld* World = GetWorld())
   {
      if (UCDRGameInstance* CDRGameInstance =
         World->GetGameInstance<UCDRGameInstance>())
      {
         const FText NetworkError = CDRGameInstance->ConsumeNetworkError();

         if (!NetworkError.IsEmpty() && IsValid(MenuMessage))
         {
            MenuMessage->SetText(NetworkError);
         }
      }
   }

   return Super::RebuildWidget();
}

void UCDRMainMenuWidget::HandleHostClicked()
{
   PlayMenuClick();

   APlayerController* PlayerController = GetOwningPlayer();

   if (IsValid(PlayerController))
   {
      FInputModeGameOnly InputMode;
      PlayerController->SetInputMode(InputMode);
      PlayerController->bShowMouseCursor = false;
   }

   UGameplayStatics::OpenLevel(
      this,
      FName("L_CrystalArena"),
      true,
      TEXT("listen"));
}

void UCDRMainMenuWidget::HandleJoinClicked()
{
   PlayMenuClick();

   if (!IsValid(AddressInput))
   {
      return;
   }

   const FString Address = AddressInput->GetText().ToString().TrimStartAndEnd();

   if (Address.IsEmpty())
   {
      if (IsValid(MenuMessage))
      {
         MenuMessage->SetText(
            FText::FromString(TEXT("Escribí la IP del host.")));

         if (UWorld* World = GetWorld())
         {
            World->GetTimerManager().SetTimer(
               MenuMessageTimerHandle,
               this,
               &UCDRMainMenuWidget::ClearMenuMessage,
               3.0f,
               false);
         }
      }


      return;
   }

   APlayerController* PlayerController = GetOwningPlayer();

   if (IsValid(PlayerController))
   {
      if (IsValid(MenuMessage))
      {
         if (UWorld* World = GetWorld())
         {
            World->GetTimerManager().ClearTimer(MenuMessageTimerHandle);
         }

         MenuMessage->SetText(
            FText::FromString(TEXT("Conectando...")));
      }

      PlayerController->ClientTravel(Address, TRAVEL_Absolute);
   }
}

void UCDRMainMenuWidget::HandleQuitClicked()
{
   PlayMenuClick();

   if (UWorld* World = GetWorld())
   {
      World->GetTimerManager().SetTimer(
         QuitTimerHandle,
         this,
         &UCDRMainMenuWidget::QuitGameAfterClick,
         0.3f,
         false);
   }
}

void UCDRMainMenuWidget::PlayMenuClick()
{
   if (IsValid(ClickSound))
   {
      UGameplayStatics::SpawnSound2D(
         this,
         ClickSound,
         1.0f,
         1.0f,
         0.0f,
         nullptr,
         true,
         true);
   }
}

void UCDRMainMenuWidget::QuitGameAfterClick()
{
   UKismetSystemLibrary::QuitGame(
      this,
      GetOwningPlayer(),
      EQuitPreference::Quit,
      false);
}
void UCDRMainMenuWidget::ClearMenuMessage()
{
   if (IsValid(MenuMessage))
   {
      MenuMessage->SetText(FText::GetEmpty());
   }
}
