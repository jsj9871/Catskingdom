// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FatLongChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class CATSKINGDOM_API UBTService_FatLongChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_FatLongChangeSpeed();
	void OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

protected:
	FString GetStaticServiceDescription() const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float Speed = 100.f;

	//UPROPERTY(VisibleAnywhere)
		//class USkelatalMeshComponent* TVMesh;

	//UPROPERTY(EditAnywhere)
		//class UMaterial* OnMaterial;

	//UPROPERTY(EditAnywhere)
		//class UMaterial* OffMaterial;

public:
	UFUNCTION()
		void OnOverlapBegin();
};
