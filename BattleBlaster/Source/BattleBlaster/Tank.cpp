// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"



ATank::ATank() 
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

}


// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
	
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer()) {
		
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer)) {
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) {
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.0f, 12, FColor::Blue,false, 0.0f);
	
		RotateTurret(HitResult.ImpactPoint);
	}
		
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
	
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered,this, &ATank::MoveInput);
		EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);
		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
	}


}	

void ATank::MoveInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	FVector DeltaLocation = FVector::ZeroVector;
	DeltaLocation.X = Speed * InputValue * UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
	AddActorLocalOffset(DeltaLocation, true);

	//UE_LOG(LogTemp, Display, TEXT("InputValue: %f") , InputValue);

}

void ATank::TurnInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	FRotator DeltaLocation = FRotator::ZeroRotator;
	DeltaLocation.Yaw = TurnRate * InputValue * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(DeltaLocation, true);
	UE_LOG(LogTemp, Display,TEXT("TurnInputValues: %f"), InputValue);
}



