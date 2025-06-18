// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitWorldGameInstance.h" 
#include "OnlineSessionSettings.h" 
#include "Online/OnlineSessionNames.h" 

void USplitWorldGameInstance::Init()
{
	Super::Init();

	if (auto SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USplitWorldGameInstance::OnCreateSessionComplete); 
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USplitWorldGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USplitWorldGameInstance::OnJoinSessionComplete); 
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &USplitWorldGameInstance::OnExitRoomComplete);
	}

	SessionIndex = -1; 
}

void USplitWorldGameInstance::CreateSession()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsDedicated = false;
	FName SubSystem = IOnlineSubsystem::Get()->GetSubsystemName();
	SessionSettings.bIsLANMatch = SubSystem == "NULL";
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true; 
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.NumPublicConnections = 2; 

	SessionSettings.Set(FName("ROOM_NAME"), SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("Host_NAME"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	FUniqueNetIdPtr NetID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	SessionInterface->CreateSession(*NetID, FName(SessionName), SessionSettings);
	UE_LOG(LogTemp, Warning,  TEXT("Create Session")); 
}

void USplitWorldGameInstance::FindSession()
{ 
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals); 
	SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");
	SessionSearch->MaxSearchResults = 10; 
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef()); 
}

void USplitWorldGameInstance::JoinSession()
{
	auto sr = SessionSearch->SearchResults[SessionIndex];
	sr.Session.SessionSettings.bUseLobbiesIfAvailable = true;
	SessionInterface->JoinSession(0, FName(SessionName), sr); 
}

void USplitWorldGameInstance::GameStart()
{ 
	GetWorld()->ServerTravel(TEXT("/Game/KS/Levels/KS_TestMap?listen?port=7777")); 
}

void USplitWorldGameInstance::ExitRoom()
{
	ServerRPC_ExitRoom(); 
}

void USplitWorldGameInstance::ServerRPC_ExitRoom_Implementation()
{
	MultiRPC_ExitRoom(); 
}

void USplitWorldGameInstance::MultiRPC_ExitRoom_Implementation()
{
	SessionInterface->DestroySession(FName(*SessionName)); 
}

void USplitWorldGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("SessionName = %s, bWasSuccessful = %d"), *sessionName.ToString(), bWasSuccessful) 
	if (bWasSuccessful) 
	{ 
		GetWorld()->ServerTravel(TEXT("/Game/WaitingMap?listen?port=7777")); 
	}
}

void USplitWorldGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session search failed!!!"));
		return; 
	}

	auto results = SessionSearch->SearchResults;
	UE_LOG(LogTemp, Warning, TEXT("Search Result Count : %d"), results.Num());

	for (int i = 0; i < results.Num(); i++)
	{
		auto sr = results[i];
		
		if (!sr.IsValid())
		{ 
			continue; 
		} 

		int PlayerCount = sr.Session.NumOpenPublicConnections;

		if (!PlayerCount)
		{
			continue; 
		} 
		
		sr.Session.SessionSettings.Get(FName("ROOM_NAME"), FindSessionName);

		if (FindSessionName.Equals(SessionName) == false)
		{
			continue;;
		}
		SessionIndex = i; 
		break; 
	}

	if (SessionIndex == -1)
	{
		CreateSession(); 
	}
	else
	{
		JoinSession(); 
	}
} 

void USplitWorldGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{ 
		auto pc = GetWorld()->GetFirstPlayerController(); 
		FString url;
		SessionInterface->GetResolvedConnectString(sessionName, url); 
		UE_LOG(LogTemp, Warning, TEXT("Join session URL : %s"), *url); 
		if (!url.IsEmpty())
		{
			pc->ClientTravel(url, TRAVEL_Absolute); 
		}
	} 
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("JoinSessionComplete Failed : %d"), result); 
	}
}

void USplitWorldGameInstance::OnExitRoomComplete(FName sessionName, bool bWasSuccessful)
{
	auto pc = GetWorld()->GetFirstPlayerController();
	FString url = TEXT("/Game/LobbyMap");
	pc->ClientTravel(url, TRAVEL_Absolute); 
}

void USplitWorldGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
	const FString& ErrorString)
{
	if (FailureType == ENetworkFailure::Type::ConnectionLost)
	{
		MultiRPC_ExitRoom_Implementation(); 
	}
}
