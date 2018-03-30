// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "MenuInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer &ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(HostButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConfirmJoinMenuButton != nullptr)) return false;
	ConfirmJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

	return true;
}


void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm gonna host a server"));
	if (MenuInterface != nullptr) {
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm gonna join a server"));
	if (MenuInterface != nullptr) {
		/*if (!ensure(IPAddressField != nullptr))
		{
			UE_LOG(LogTemp, Error, TEXT("IPAddressField is nullptr"));
			return;
		}
		const FString& Address = IPAddressField->GetText().ToString();
		MenuInterface->Join(Address);*/
		UWorld* World = this->GetWorld();
		if (!ensure(World != nullptr)) return;

		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::OpenJoinMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm gonna open JoinMenu"));
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm gonna open MainMenu"));
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitPressed()
{
	UWorld* World = GetWorld();
	if (!ensure(World!=nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("World in nullptr"));
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController in nullptr"));
		return;
	}
	PlayerController->ConsoleCommand("quit");
}
