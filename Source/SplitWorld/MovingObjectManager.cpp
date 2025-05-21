#include "MovingObjectManager.h"
#include "TimerManager.h"

AMovingObjectManager::AMovingObjectManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMovingObjectManager::BeginPlay()
{
	Super::BeginPlay();


	InitializePool(InitialPoolSize);
	ScheduleNextSpawn();
}

void AMovingObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
} 

void AMovingObjectManager::InitializePool(int32 InitialSize)
{
	for (int32 i = 0; i < InitialSize; ++i)
	{
		ATrap* NewObj = GetWorld()->SpawnActor<ATrap>(ObjectClass);
		if (AMovingObject* MovingObj = Cast<AMovingObject>(NewObj))
		{
			MovingObj->Deactivate();
		}
		ObjectPool.Add(NewObj);
	}
}

ATrap* AMovingObjectManager::GetPooledObject()
{
	for (ATrap* Obj : ObjectPool)
	{
		if (AMovingObject* MovingObj = Cast<AMovingObject>(Obj))
		{
			if (!MovingObj->IsActorTickEnabled()) // 비활성 상태
			{
				return MovingObj;
			}
		}
	}

	// 풀 부족 시 새로 생성
	ATrap* NewObj = GetWorld()->SpawnActor<ATrap>(ObjectClass);
	ObjectPool.Add(NewObj);
	return NewObj;
}


void AMovingObjectManager::SpawnObject()
{
	FVector BaseLocation = GetActorLocation();
	FVector FantasyLoc = BaseLocation;  // 왼쪽
	FVector SFLoc = BaseLocation + FVector(MapOffset, 0.f, 0.f); // 오른쪽
	
	ATrap* Trap1 = GetPooledObject();
	ATrap* Trap2 = GetPooledObject();

	// 중복 방지: 같은 객체 반환됐을 경우 강제로 새로 생성
	if (Trap1 == Trap2)
	{
		Trap2 = GetWorld()->SpawnActor<ATrap>(ObjectClass);
		ObjectPool.Add(Trap2);
	}
	

	if (AMovingObject* Obj1 = Cast<AMovingObject>(Trap1))
	{
		Obj1->SetActorLocation(FantasyLoc);
		Obj1->SetLifeSpanTime(ObjectLifeSpan);
		Obj1->Activate();

		// 디버그 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("Spawned Fantasy MovingObject at: %s"), *FantasyLoc.ToString());
		DrawDebugSphere(GetWorld(), FantasyLoc, 100.f, 12, FColor::Blue, false, 5.0f);

		UE_LOG(LogTemp, Warning, TEXT("Obj1 IsValid: %s | IsHidden: %d"), 
	*Obj1->GetName(),
	Obj1->IsHidden()
);

		// 판타지쪽이라고 알려주기

	}

	if (AMovingObject* Obj2 = Cast<AMovingObject>(Trap2))
	{
		Obj2->SetActorLocation(SFLoc);
		Obj2->SetLifeSpanTime(ObjectLifeSpan);
		Obj2->Activate();

		// 디버그 로그 출력
		UE_LOG(LogTemp, Warning, TEXT("Spawned SF MovingObject at: %s"), *SFLoc.ToString());
		DrawDebugSphere(GetWorld(), SFLoc, 100.f, 12, FColor::Red, false, 5.0f);

		UE_LOG(LogTemp, Warning, TEXT("Obj1 IsValid: %s | IsHidden: %d"), 
*Obj2->GetName(),
Obj2->IsHidden()
);
	}

	// SF 쪽이라고 알려주기

	
	ScheduleNextSpawn();
}

void AMovingObjectManager::ScheduleNextSpawn()
{
	float Delay = FMath::FRandRange(MinSpawnInterval, MaxSpawnInterval);
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AMovingObjectManager::SpawnObject, Delay, false);
}