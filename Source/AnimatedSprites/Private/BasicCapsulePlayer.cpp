// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicCapsulePlayer.h"

#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "Classes/PaperSpriteComponent.h"
#include "Classes/PaperSprite.h"
#include "ConstructorHelpers.h"
#include "Classes/GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"


#include "GameFramework/Character.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


#include "GameFramework/CharacterMovementComponent.h"

#include "PaperFlipbookComponent.h"

// Sets default values
ABasicCapsulePlayer::ABasicCapsulePlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#pragma region SpringArm
	//Do this after showing the FollowCamera attached to the RootComponent
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation( FRotator(0.f, -90.f, 0.f) );
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 300.0f);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;
	SpringArm->bInheritYaw = true;

	//Important call, Uncomment and run to see what happens
	SpringArm->SetUsingAbsoluteRotation(true);
#pragma endregion Setup the SpringArm Component which the Camera attaches to, for smooth camera Following of the player

#pragma region FollowCamera
	//Setup the Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Player Camera Component");
	FollowCamera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	FollowCamera->SetOrthoWidth(5000.0f);
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	FollowCamera->bUsePawnControlRotation = false;
	FollowCamera->bAutoActivate = true;
	
#pragma endregion Setup the FollowCamera which attaches to the SpringArm Component and is set to Orthographic Projection

	// Ask the Controller for the full rotation if desired (ie for aiming).
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

    // Create and set up sound stuff
    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEmitter"));
    AudioComponent->bAutoActivate = false;
    AudioComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	
#pragma region Setup CharacterMovement and Capsule
	//Setup CharacterMovement Component and Capsule Component
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, -1.f, 0.f));
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABasicCapsulePlayer::OnHit);
#pragma endregion Setup CharacterMovement and Capsule
}

// Called when the game starts or when spawned
void ABasicCapsulePlayer::BeginPlay()
{
	Super::BeginPlay();

	//SET OnFloor to false
	OnFloor = false;

	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (controller)
	{
		FString command = "show collision";
		controller->ConsoleCommand(command, true);
		controller->bShowMouseCursor = true;
		controller->bEnableClickEvents = true;
		controller->bEnableMouseOverEvents = true;
	}

}

// Called every frame
void ABasicCapsulePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasicCapsulePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ABasicCapsulePlayer::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasicCapsulePlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
}


void ABasicCapsulePlayer::MoveRight(float value)
{
	//IF value NOT EQUAL to 0.0f
	if (value != 0.0f)
    {
    	//DECLARE a variable called PlayerVelocity of type const FVector and ASSIGN it to the return value of  GetVelocity()
		const FVector PlayerVelocity = GetVelocity();
    	//SET TravelDirection to -PlayerVelocity.X
		TravelDirection = -PlayerVelocity.X;

    	//CALL AddMovementInput() passing in FVector(1.f, 0.f, 0.f), value * .4f
		AddMovementInput(FVector(1.0f, 0.0f, 0.0f), value * 0.4f);
    }
	//ELSE
    {
    	//SET TravelDirection to 0.0f Inorder to stop the walking animation
		TravelDirection = 0.0f;
    }
}

void ABasicCapsulePlayer::Land()
{
	//SET OnFloor to true
	OnFloor = true;
}

/// On Collision Hit callback function
void ABasicCapsulePlayer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	//IF OtherActor IS NOT nullptr
	if (OtherActor != nullptr)
	{
		//IF OtherActor Has Tag "Floor"
		if (OtherActor->ActorHasTag("Floor"))
		{
			//CALL Land()
			Land();
		}
		//ENDIF
	}
	//ENDIF
}

