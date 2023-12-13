// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OptionMenu.generated.h"

/**
 * 
 */
UCLASS()
class BIGJAM01_API UOptionMenu : public UUserWidget {
	GENERATED_BODY()

private:
	FString ResolutionPointToString(FIntPoint& Resolution);
	void InitializeGraphicSlider(class USlider* Slider);
	void InitializeGraphicSettings();

	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ApplyGraphicsButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* CancelGraphicsButton;

	/* Graphic Options*/

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UComboBoxString* GraphicResolution;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UComboBoxString* GraphicFullscreen;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UComboBoxString* GraphicAliasingQuality;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* GraphicShadowQuality;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* GraphicTextureQuality;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UCheckBox* GraphicVsync;

	virtual void NativeConstruct();

public:

	void ApplyGraphicSettings();
	void CancelGraphicSettings();
};