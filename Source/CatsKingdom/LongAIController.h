// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CatsKingdom.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "LongAIController.generated.h"

/**
 *
 */

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class CATSKINGDOM_API ALongAIController : public AAIController
{
    GENERATED_BODY()

public:
    virtual void OnPossess(APawn* InPawn) override;
    ALongAIController(FObjectInitializer const& object_intializer = FObjectInitializer::Get());
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds)override;

    UFUNCTION(BlueprintCallable, Category = Behavior)
        void on_target_detected(AActor* actor, FAIStimulus const stimulus);

    class UBlackboardComponent* get_blackboard() const;

    static const FName HomePosKey;
    static const FName PatrolPosKey;
    static const FName TargetKey;

private:
    UPROPERTY()
        class UBehaviorTree* BTAsset;

    UPROPERTY()
        class UBlackboardData* BBAsset;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
        class UBehaviorTreeComponent* behavior_tree_Comp;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
        class UBehaviorTree* btree;

    class UBlackboardComponent* blackboard;
    class UAISenseConfig_Sight* sight_config;

    void setup_perception_system();
};
