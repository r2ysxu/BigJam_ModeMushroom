// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionMenu.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/Slider.h"
#include "Components/CheckBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "GameFramework/GameUserSettings.h"


FString UOptionMenu::ResolutionPointToString(FIntPoint& Resolution) {
	return FString(UKismetStringLibrary::Conv_IntToString(Resolution.X) + 'x' + UKismetStringLibrary::Conv_IntToString(Resolution.Y));
}

void UOptionMenu::InitializeGraphicSlider(USlider* Slider) {
	Slider->SetMaxValue(4);
	Slider->SetMinValue(0);
	Slider->SetStepSize(1);
}

void UOptionMenu::InitializeGraphicSettings() {
	TArray<FIntPoint> resolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(resolutions);
	GraphicResolution->ClearOptions();
	for (FIntPoint resolution : resolutions) {
		GraphicResolution->AddOption(ResolutionPointToString(resolution));
	}
	UGameUserSettings* settings = UGameUserSettings::GetGameUserSettings();
	FIntPoint currentResolution = settings->GetScreenResolution();
	GraphicResolution->SetSelectedOption(ResolutionPointToString(currentResolution));

	GraphicFullscreen->ClearOptions();
	GraphicFullscreen->AddOption(FString(TEXT("Fullscreen")));
	GraphicFullscreen->AddOption(FString(TEXT("Borderless")));
	GraphicFullscreen->AddOption(FString(TEXT("Windowed")));
	GraphicFullscreen->SetSelectedIndex(UKismetMathLibrary::Conv_ByteToInt(settings->GetFullscreenMode()));

	GraphicAliasingQuality->ClearOptions();
	GraphicAliasingQuality->AddOption(FString(TEXT(" 1x")));
	GraphicAliasingQuality->AddOption(FString(TEXT(" 2x")));
	GraphicAliasingQuality->AddOption(FString(TEXT(" 4x")));
	GraphicAliasingQuality->AddOption(FString(TEXT(" 8x")));
	GraphicAliasingQuality->AddOption(FString(TEXT("16x")));
	GraphicAliasingQuality->SetSelectedIndex(settings->GetAntiAliasingQuality());

	InitializeGraphicSlider(GraphicTextureQuality);
	GraphicTextureQuality->SetValue(settings->GetTextureQuality());
	InitializeGraphicSlider(GraphicShadowQuality);
	GraphicShadowQuality->SetValue(settings->GetShadowQuality());

	GraphicVsync->SetIsChecked(settings->IsVSyncEnabled());
}

void UOptionMenu::ApplyGraphicSettings() {
	UGameUserSettings* settings = UGameUserSettings::GetGameUserSettings();
	settings->SetFullscreenMode(static_cast<EWindowMode::Type>(GraphicFullscreen->GetSelectedIndex()));
	settings->SetTextureQuality(GraphicTextureQuality->GetValue());
	settings->SetShadowQuality(GraphicShadowQuality->GetValue());
	settings->SetAntiAliasingQuality(GraphicAliasingQuality->GetSelectedIndex());
	settings->SetVSyncEnabled(GraphicVsync->IsChecked());

	FString selectedResolution = GraphicResolution->GetSelectedOption();
	TArray<FString> resolution = UKismetStringLibrary::ParseIntoArray(selectedResolution, FString("x"));
	int32 x = UKismetStringLibrary::Conv_StringToInt(resolution[0]);
	int32 y = UKismetStringLibrary::Conv_StringToInt(resolution[1]);
	settings->SetScreenResolution(FIntPoint(x, y));

	settings->ApplySettings(true);
}

void UOptionMenu::CancelGraphicSettings() {
	InitializeGraphicSettings();
}

void UOptionMenu::NativeConstruct() {
	InitializeGraphicSettings();
}
