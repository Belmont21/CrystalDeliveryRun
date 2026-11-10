#include "CDRGameHUDWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "CrystalDeliveryRunPlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
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
   ScoreText->SetColorAndOpacity(
      FSlateColor(FLinearColor(0.1f, 1.0f, 0.2f, 1.0f)));

   UCanvasPanelSlot* ScoreSlot = RootCanvas->AddChildToCanvas(ScoreText);
   ScoreSlot->SetPosition(FVector2D(40.0f, 40.0f));
   ScoreSlot->SetAutoSize(true);

   TimerText = WidgetTree->ConstructWidget<UTextBlock>(
      UTextBlock::StaticClass(),
      TEXT("TimerText"));

   TimerText->SetText(FText::FromString(TEXT("Tiempo: 05:00")));
   TimerText->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 24));

   UCanvasPanelSlot* TimerSlot = RootCanvas->AddChildToCanvas(TimerText);
   TimerSlot->SetPosition(FVector2D(40.0f, 85.0f));
   TimerSlot->SetAutoSize(true);

   HealthBar = WidgetTree->ConstructWidget<UProgressBar>(
      UProgressBar::StaticClass(),
      TEXT("HealthBar"));

   HealthBar->SetPercent(1.0f);
   HealthBar->SetFillColorAndOpacity(
      FLinearColor(0.1f, 1.0f, 0.2f, 1.0f));

   UCanvasPanelSlot* HealthSlot = RootCanvas->AddChildToCanvas(HealthBar);
   HealthSlot->SetPosition(FVector2D(40.0f, 125.0f));
   HealthSlot->SetSize(FVector2D(220.0f, 22.0f));

   ResultText = WidgetTree->ConstructWidget<UTextBlock>(
      UTextBlock::StaticClass(),
      TEXT("ResultText"));

   ResultText->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 30));
   ResultText->SetColorAndOpacity(
      FSlateColor(FLinearColor(0.1f, 1.0f, 0.2f, 1.0f)));
   ResultText->SetShadowColorAndOpacity(FLinearColor::Black);
   ResultText->SetShadowOffset(FVector2D(2.0f, 2.0f));
   ResultText->SetJustification(ETextJustify::Center);
   ResultText->SetVisibility(ESlateVisibility::Collapsed);

   UCanvasPanelSlot* ResultSlot = RootCanvas->AddChildToCanvas(ResultText);
   ResultSlot->SetAnchors(FAnchors(0.5f, 0.5f));
   ResultSlot->SetAlignment(FVector2D(0.5f, 0.5f));
   ResultSlot->SetAutoSize(true);

   OutcomeText = WidgetTree->ConstructWidget<UTextBlock>(
      UTextBlock::StaticClass(),
      TEXT("OutcomeText"));

   OutcomeText->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 36));
   OutcomeText->SetJustification(ETextJustify::Center);
   OutcomeText->SetShadowColorAndOpacity(FLinearColor::Black);
   OutcomeText->SetShadowOffset(FVector2D(2.0f, 2.0f));
   OutcomeText->SetVisibility(ESlateVisibility::Collapsed);

   UCanvasPanelSlot* OutcomeSlot = RootCanvas->AddChildToCanvas(OutcomeText);
   OutcomeSlot->SetAnchors(FAnchors(0.5f, 0.5f));
   OutcomeSlot->SetAlignment(FVector2D(0.5f, 0.5f));
   OutcomeSlot->SetPosition(FVector2D(0.0f, -230.0f));
   OutcomeSlot->SetAutoSize(true);

   RestartButton = WidgetTree->ConstructWidget<UButton>(
      UButton::StaticClass(),
      TEXT("RestartButton"));

   UTextBlock* RestartText = WidgetTree->ConstructWidget<UTextBlock>(
      UTextBlock::StaticClass(),
      TEXT("RestartText"));

   RestartText->SetText(FText::FromString(TEXT("REINICIAR PARTIDA")));
   RestartText->SetJustification(ETextJustify::Center);
   RestartButton->AddChild(RestartText);
   RestartButton->OnClicked.AddDynamic(
      this,
      &UCDRGameHUDWidget::HandleRestartClicked);
   RestartButton->SetVisibility(ESlateVisibility::Collapsed);

   UCanvasPanelSlot* RestartSlot = RootCanvas->AddChildToCanvas(RestartButton);
   RestartSlot->SetAnchors(FAnchors(0.5f, 0.5f));
   RestartSlot->SetAlignment(FVector2D(0.5f, 0.5f));
   RestartSlot->SetPosition(FVector2D(0.0f, 200.0f));
   RestartSlot->SetSize(FVector2D(250.0f, 45.0f));

   ExitButton = WidgetTree->ConstructWidget<UButton>(
      UButton::StaticClass(),
      TEXT("ExitButton"));

   UTextBlock* ExitText = WidgetTree->ConstructWidget<UTextBlock>(
      UTextBlock::StaticClass(),
      TEXT("ExitText"));

   ExitText->SetText(FText::FromString(TEXT("SALIR")));
   ExitText->SetJustification(ETextJustify::Center);
   ExitButton->AddChild(ExitText);
   ExitButton->OnClicked.AddDynamic(
      this,
      &UCDRGameHUDWidget::HandleExitClicked);
   ExitButton->SetVisibility(ESlateVisibility::Collapsed);

   UCanvasPanelSlot* ExitSlot = RootCanvas->AddChildToCanvas(ExitButton);
   ExitSlot->SetAnchors(FAnchors(0.5f, 0.5f));
   ExitSlot->SetAlignment(FVector2D(0.5f, 0.5f));
   ExitSlot->SetPosition(FVector2D(0.0f, 260.0f));
   ExitSlot->SetSize(FVector2D(250.0f, 45.0f));

   CrystalLostText = WidgetTree->ConstructWidget<UTextBlock>(
      UTextBlock::StaticClass(),
      TEXT("CrystalLostText"));

   CrystalLostText->SetText(FText::FromString(TEXT("Perdiste el cristal")));
   CrystalLostText->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 22));
   CrystalLostText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
   CrystalLostText->SetShadowColorAndOpacity(FLinearColor::Black);
   CrystalLostText->SetShadowOffset(FVector2D(2.0f, 2.0f));
   CrystalLostText->SetVisibility(ESlateVisibility::Collapsed);

   UCanvasPanelSlot* LostSlot = RootCanvas->AddChildToCanvas(CrystalLostText);
   LostSlot->SetPosition(FVector2D(40.0f, 195.0f));
   LostSlot->SetAutoSize(true);
   
   TotalScoreText = WidgetTree->ConstructWidget<UTextBlock>(
   UTextBlock::StaticClass(),
   TEXT("TotalScoreText"));

   TotalScoreText->SetText(FText::FromString(TEXT("Total de la partida: 0")));
   TotalScoreText->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 20));

   UCanvasPanelSlot* TotalScoreSlot = RootCanvas->AddChildToCanvas(TotalScoreText);
   TotalScoreSlot->SetPosition(FVector2D(40.0f, 160.0f));
   TotalScoreSlot->SetAutoSize(true);
   
   LobbyText = WidgetTree->ConstructWidget<UTextBlock>(
   UTextBlock::StaticClass(),
   TEXT("LobbyText"));

   LobbyText->SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 26));
   LobbyText->SetJustification(ETextJustify::Center);
   LobbyText->SetShadowColorAndOpacity(FLinearColor::Black);
   LobbyText->SetShadowOffset(FVector2D(2.0f, 2.0f));
   LobbyText->SetVisibility(ESlateVisibility::Collapsed);

   UCanvasPanelSlot* LobbySlot = RootCanvas->AddChildToCanvas(LobbyText);
   LobbySlot->SetAnchors(FAnchors(0.5f, 0.5f));
   LobbySlot->SetAlignment(FVector2D(0.5f, 0.5f));
   LobbySlot->SetPosition(FVector2D(0.0f, -60.0f));
   LobbySlot->SetAutoSize(true);

   StartButton = WidgetTree->ConstructWidget<UButton>(
      UButton::StaticClass(),
      TEXT("StartButton"));

   UTextBlock* StartText = WidgetTree->ConstructWidget<UTextBlock>(
      UTextBlock::StaticClass(),
      TEXT("StartText"));

   StartText->SetText(FText::FromString(TEXT("INICIAR PARTIDA")));
   StartText->SetJustification(ETextJustify::Center);
   StartButton->AddChild(StartText);
   StartButton->OnClicked.AddDynamic(
      this,
      &UCDRGameHUDWidget::HandleStartClicked);
   StartButton->SetVisibility(ESlateVisibility::Collapsed);

   UCanvasPanelSlot* StartSlot = RootCanvas->AddChildToCanvas(StartButton);
   StartSlot->SetAnchors(FAnchors(0.5f, 0.5f));
   StartSlot->SetAlignment(FVector2D(0.5f, 0.5f));
   StartSlot->SetPosition(FVector2D(0.0f, 20.0f));
   StartSlot->SetSize(FVector2D(250.0f, 50.0f));

   
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

void UCDRGameHUDWidget::SetRemainingTime(int32 Seconds)
{
   if (!IsValid(TimerText))
   {
      return;
   }

   const int32 Minutes = Seconds / 60;
   const int32 RemainingSeconds = Seconds % 60;

   TimerText->SetText(FText::FromString(
      FString::Printf(TEXT("Tiempo: %02d:%02d"), Minutes, RemainingSeconds)));
}

void UCDRGameHUDWidget::SetHealth(float NewHealth)
{
   if (IsValid(HealthBar))
   {
      const float HealthPercentage = FMath::Clamp(NewHealth / 100.0f, 0.0f, 1.0f);
      HealthBar->SetPercent(HealthPercentage);
   }
}

void UCDRGameHUDWidget::ShowMatchResult(const FText& Result)
{
   if (!IsValid(ResultText))
   {
      return;
   }

   FString Outcome;
   FString Details;

   if (Result.ToString().Split(TEXT("\n\n"), &Outcome, &Details))
   {
      if (Outcome == TEXT("EMPATE") && Details.StartsWith(TEXT("EMPATE\n")))
      {
         Details.RemoveFromStart(TEXT("EMPATE\n"));
      }

      ResultText->SetText(FText::FromString(Details));

      if (IsValid(OutcomeText))
      {
         OutcomeText->SetText(FText::FromString(Outcome));

         FLinearColor OutcomeColor(0.1f, 1.0f, 0.2f);

         if (Outcome == TEXT("DERROTA"))
         {
            OutcomeColor = FLinearColor::Red;
         }
         else if (Outcome == TEXT("EMPATE"))
         {
            OutcomeColor = FLinearColor(1.0f, 0.35f, 0.0f);
         }

         OutcomeText->SetColorAndOpacity(FSlateColor(OutcomeColor));
         OutcomeText->SetVisibility(ESlateVisibility::Visible);
      }
   }
   else
   {
      ResultText->SetText(Result);
   }

   ResultText->SetVisibility(ESlateVisibility::Visible);

   if (IsValid(RestartButton))
   {
      RestartButton->SetVisibility(ESlateVisibility::Visible);
   }

   if (IsValid(ExitButton))
   {
      ExitButton->SetVisibility(ESlateVisibility::Visible);
   }
}

void UCDRGameHUDWidget::HandleExitClicked()
{
   UGameplayStatics::OpenLevel(this, FName("L_MainMenu"));
}

void UCDRGameHUDWidget::HandleRestartClicked()
{
   ACrystalDeliveryRunPlayerController* PlayerController =
      Cast<ACrystalDeliveryRunPlayerController>(GetOwningPlayer());

   if (IsValid(PlayerController))
   {
      PlayerController->ServerRequestRestart();
   }
}

void UCDRGameHUDWidget::ShowCrystalLost()
{
   if (!IsValid(CrystalLostText) || !IsValid(GetWorld()))
   {
      return;
   }

   CrystalLostText->SetText(FText::FromString(TEXT("Perdiste el cristal")));
   CrystalLostText->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
   CrystalLostText->SetVisibility(ESlateVisibility::Visible);

   GetWorld()->GetTimerManager().SetTimer(
      CrystalLostTimerHandle,
      this,
      &UCDRGameHUDWidget::HideCrystalLost,
      3.0f,
      false);
}

void UCDRGameHUDWidget::HideCrystalLost()
{
   if (IsValid(CrystalLostText))
   {
      CrystalLostText->SetVisibility(ESlateVisibility::Collapsed);
   }
}

void UCDRGameHUDWidget::ShowCrystalDelivered(int32 Points)
{
   if (!IsValid(CrystalLostText) || !IsValid(GetWorld()))
   {
      return;
   }

   CrystalLostText->SetText(FText::FromString(
      FString::Printf(TEXT("Entregado: +%d puntos"), Points)));

   CrystalLostText->SetColorAndOpacity(
      FSlateColor(FLinearColor(0.1f, 1.0f, 0.2f, 1.0f)));

   CrystalLostText->SetVisibility(ESlateVisibility::Visible);

   GetWorld()->GetTimerManager().SetTimer(
      CrystalLostTimerHandle,
      this,
      &UCDRGameHUDWidget::HideCrystalLost,
      3.0f,
      false);
}
void UCDRGameHUDWidget::SetTotalScore(int32 NewTotalScore)
{
   if (IsValid(TotalScoreText))
   {
      TotalScoreText->SetText(FText::FromString(
         FString::Printf(TEXT("Total de la partida: %d"), NewTotalScore)));
   }
}
void UCDRGameHUDWidget::SetLobbyState(bool bIsLobby, bool bIsHost)
{
   if (IsValid(LobbyText))
   {
      if (bIsHost)
      {
         LobbyText->SetText(FText::FromString(
            TEXT("Esperando jugadores. Iniciá cuando quieras.")));
      }
      else
      {
         LobbyText->SetText(FText::FromString(
            TEXT("Esperando que el host inicie la partida.")));
      }

      LobbyText->SetVisibility(
         bIsLobby ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
   }

   if (IsValid(StartButton))
   {
      StartButton->SetVisibility(
         bIsLobby && bIsHost
            ? ESlateVisibility::Visible
            : ESlateVisibility::Collapsed);
   }
}

void UCDRGameHUDWidget::HandleStartClicked()
{
   ACrystalDeliveryRunPlayerController* PlayerController =
      Cast<ACrystalDeliveryRunPlayerController>(GetOwningPlayer());

   if (IsValid(PlayerController))
   {
      PlayerController->ServerRequestStartRound();
   }
}

