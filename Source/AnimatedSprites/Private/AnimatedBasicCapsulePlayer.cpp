// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimatedBasicCapsulePlayer.h"
#include "Classes/PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "Components/CapsuleComponent.h"
#include "PaperFlipbook.h"
#include "ConstructorHelpers.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"

AAnimatedBasicCapsulePlayer::AAnimatedBasicCapsulePlayer()
{
	//CALL SetState() passing in EAnimState::Idle
	SetState(EAnimState::Idle);
}

void AAnimatedBasicCapsulePlayer::MoveRight(float value)
{
	//CALL Super::MoveRight() passing in value
	Super::MoveRight(value);

	//COMMENT: This is the base class setup so we rotate the PlayerSprite using the Controller. Have a look at the setup in ABasicCapsulePlayer
	
	//IF GetSprite()
	if (GetSprite())
	{
		//IF TravelDirection LESS THAN 0.0f
		if (TravelDirection < 0.0f)
		{
			//CALL SetControlRotation() on Controller passing in FRotator(0.f, 0.f, 0.f)
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
			//ELSE IF TravelDirection GREATER THAN 0.0f
		}
		else if (TravelDirection > 0.0f)
		{
			//CALL SetControlRotation() on Controller passing in FRotator(0.f, 180.f, 0.f)
			Controller->SetControlRotation(FRotator(0.0f, 180.0f, 0.0f));
		}
		//ENDIF

		//IF AnimState IS EAnimState::Idle OR AnimState IS EAnimState::Walking
		if (AnimState == EAnimState::Idle || AnimState == EAnimState::Walking)
		{
			//IF TravelDirection IS 0.0
			if (TravelDirection == 0.0f)
			{
				//CALL SetState() passing in EAnimState::Idle
				SetState(EAnimState::Idle);
			}
			else
            {
            	//IF OnFloor AND AudioComponent IS NOT Playing 
				if (OnFloor == true && AudioComponent->IsPlaying() == false)
				{
               		//CALL SetSound() on AudioComponent passing in ScuffleSound
					AudioComponent->SetSound(ScuffleSound);
               		//CALL Play() on AudioComponent
					AudioComponent->Play();
                }
            	//ENDIF

            	//CALL SetState() passing in EAnimState::Walking
				SetState(EAnimState::Walking);
            }
			//ENDIF
		}
		//ENDIF
	}
	//ENDIF
}

void AAnimatedBasicCapsulePlayer::BeginPlay()
{
	//CALL Super::BeginPlay()
	Super::BeginPlay();

	//IF GetSprite() NOT EQUAL nullptr
	if (GetSprite() != nullptr)
	{
		//CALL GetSprite()->Stop()
		GetSprite()->Stop();
	}
	//ENDIF
}

void AAnimatedBasicCapsulePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//CALL UpdateAnimation()
	UpdateAnimation();
}

void AAnimatedBasicCapsulePlayer::UpdateAnimation()
{

	//DECLARE a variable called Velocity of type float and ASSIGN it to the return value of GetVelocity().Size()
	float Velocity = GetVelocity().Size();

	//IF Velocity GREATER THAN 0
	if (Velocity > 0.0f)
	{
		/** jumping  */
		//IF AnimState IS EAnimState::Jumping
		if (AnimState == EAnimState::Jumping)
		{
           //CALL GetSprite()->SetFlipbook() passing in JumpFlipbook
			GetSprite()->SetFlipbook(JumpFlipbook);
		   //CALL  GetSprite()->SetLooping() passing in false
			GetSprite()->SetLooping(false);
		   //CALL GetSprite()->Play()
			GetSprite()->Play();
		}
		//ELSE
		else
		{
			//CALL GetSprite()->SetFlipbook() passing in WalkFlipbook
			GetSprite()->SetFlipbook(WalkFlipbook);
			//CALL GetSprite()->SetLooping() passing in true
			GetSprite()->SetLooping(true);
			//CALL GetSprite()->Play()
			GetSprite()->Play();
		}
		//ENDIF
	}
	//ELSE /**  Velocity == 0 */
	else if (Velocity == 0.0f)
	{
		//CALL GetSprite()->SetFlipbook() passing in IdleFlipbook
		GetSprite()->SetFlipbook(IdleFlipbook);
		//CALL GetSprite()->SetLooping() passing in true
		GetSprite()->SetLooping(true);
		//CALL GetSprite()->Play()
		GetSprite()->Play();
	}
	//ENDIF
}

void AAnimatedBasicCapsulePlayer::Jump()
{
	//IF OnFloor IS true
	if (OnFloor == true)
	{
    	//CALL AudioComponent->SetSound() passing in JumpSound
		AudioComponent->SetSound(JumpSound);
    	//CALL Play() on AudioComponent
		AudioComponent->Play();

    	//CALL Super::Jump()
		Super::Jump();
    }
	//ENDIF

	//IF AnimState IS EAnimState::Walking OR AnimState IS EAnimState::Idle
	if (AnimState == EAnimState::Walking || AnimState == EAnimState::Idle)
	{
		//CALL SetState() passing in EAnimState::Jumping
		SetState(EAnimState::Jumping);
	}
	//ENDIF

	//SET OnFloor to false
	OnFloor = false;
   
}

void AAnimatedBasicCapsulePlayer::Land()
{
	//CALL Super::Land()
	Super::Land();

   	//CALL AudioComponent->SetSound() passing in ThudSound
	AudioComponent->SetSound(ThudSound);
   	//CALL  AudioComponent->Play()
	AudioComponent->Play();

	//IF AnimState IS EAnimState::Dead
	if (AnimState == EAnimState::Dead)
	{
		//CALL SetState() passing in EAnimState::Dead
		SetState(EAnimState::Dead);
	}

	else
	{
		//ELSE
			//CALL SetState() passing in EAnimState::Idle
		SetState(EAnimState::Idle);
	}
    
}

void AAnimatedBasicCapsulePlayer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	//CALL Super::OnHit() passing in HitComponent, OtherActor, OtherComp, NormalImpulse, Hit
	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
