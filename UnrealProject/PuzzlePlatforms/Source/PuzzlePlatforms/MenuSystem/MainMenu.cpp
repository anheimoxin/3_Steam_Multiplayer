// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "MenuInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "ServerRow.h"
#include "Components/TextBlock.h"
#include "LogMacros.h"
#include "PanelWidget.h"


UMainMenu::UMainMenu(const FObjectInitializer &ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames) {
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->HostUser->SetText(FText::FromString(ServerData.HostUsername));
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(FractionText));
		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}


}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelHostMenuButton != nullptr)) return false;
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConfirmHostMenuButton != nullptr)) return false;
	ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

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
		FString ServerName = ServerHostName->Text.ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Selected Index Not Set"));
	}

	UE_LOG(LogTemp, Warning, TEXT("I'm gonna join a server"));
}

void UMainMenu::OpenJoinMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("I'm gonna open JoinMenu"));
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (MenuInterface != nullptr)
	{
		MenuInterface->RefreshServerList();
	}
}

void UMainMenu::OpenHostMenu()
{
	MenuSwitcher->SetActiveWidget(HostMenu);
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
	if (!ensure(World != nullptr))
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

void UMainMenu::UpdateChildren()
{
	uint32 count = ServerList->GetChildrenCount();
	for (uint32 i = 0; i < count; ++i)
	{
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}
