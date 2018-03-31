// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"
/**
 *
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
public:
	UMainMenu(const FObjectInitializer &ObjectInitializer);

	void SetServerList(TArray<FString> ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize();

private:

	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(Meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(Meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(Meta = (BindWidget))
		class UButton* CancelJoinMenuButton;

	UPROPERTY(Meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(Meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(Meta = (BindWidget))
		class UPanelWidget* ServerList;

	UPROPERTY(Meta = (BindWidget))
		class UButton* ConfirmJoinMenuButton;

	UPROPERTY(Meta = (BindWidget))
		class UButton* QuitButton;

	TOptional<uint32> SelectedIndex;

	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void JoinServer();

	UFUNCTION()
		void OpenJoinMenu();
	
	UFUNCTION()
		void OpenMainMenu();

	UFUNCTION()
		void QuitPressed();
};
