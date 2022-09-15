// Fill out your copyright notice in the Description page of Project Settings.


#include "LongAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include "CatsKingdomCharacter.h"
#include "Engine.h"

// 키 값 지정
const FName ALongAIController::HomePosKey(TEXT("HomePos"));
const FName ALongAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ALongAIController::TargetKey(TEXT("Target"));

// 비헤이비어 트리 구동
ALongAIController::ALongAIController(FObjectInitializer const& object_intializer)
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_CKCharacter.BB_CKCharacter"));
    if (BBObject.Succeeded())
    {
        BBAsset = BBObject.Object;
    }

    static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("/Game/AI/BT_ThinLongCharacter.BT_ThinLongCharacter"));
    if (obj.Succeeded())
    {
        btree = obj.Object;
    }

    behavior_tree_Comp = object_intializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
    blackboard = object_intializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
    setup_perception_system();
}

void ALongAIController::BeginPlay()
{
    Super::BeginPlay();

    RunBehaviorTree(btree);
    behavior_tree_Comp->StartTree(*btree);
}

void ALongAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (UseBlackboard(BBAsset, Blackboard))
    {
        blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
    }
}

void ALongAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

/*FRotator ACKAIController::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return FRotator(0.f, 0.f, 0.f);
    }

    return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}*/

void ALongAIController::on_target_detected(AActor* actor, FAIStimulus const stimulus)
{
    if (auto const ch = Cast<ACatsKingdomCharacter>(actor))
    {
       // GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Find Player!!!!");
        get_blackboard()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
    }
}

UBlackboardComponent* ALongAIController::get_blackboard() const
{
    return blackboard;
}

void ALongAIController::setup_perception_system()
{
    sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

    sight_config->SightRadius = 2000.0f;
    sight_config->LoseSightRadius = sight_config->SightRadius + 50.0f;
    sight_config->PeripheralVisionAngleDegrees = 360.0f;
    sight_config->SetMaxAge(5.0f);
    sight_config->AutoSuccessRangeFromLastSeenLocation = 520.0f;
    sight_config->DetectionByAffiliation.bDetectEnemies = true;
    sight_config->DetectionByAffiliation.bDetectFriendlies = true;
    sight_config->DetectionByAffiliation.bDetectNeutrals = true;

    GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
    GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ALongAIController::on_target_detected);
    GetPerceptionComponent()->ConfigureSense(*sight_config);
}

