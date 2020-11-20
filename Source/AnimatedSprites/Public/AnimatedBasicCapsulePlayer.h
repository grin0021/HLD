// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AnimatedSprites.h"
#include "BasicCapsulePlayer.h"
#include "AnimatedBasicCapsulePlayer.generated.h"


/** describes all available character's states  */

/**
 * 
 */
UCLASS()
class ANIMATEDSPRITES_API AAnimatedBasicCapsulePlayer : public ABasicCapsulePlayer
{
	GENERATED_BODY()
		
public :
	AAnimatedBasicCapsulePlayer();

	//~ ------------- F L I P B O O K S -----------------------------------------------------------------------------
	//SET In Editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbook* JumpFlipbook;

	//SET In Editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbook* WalkFlipbook;

	//SET In Editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbook* IdleFlipbook;

	/** current animation state  */	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", meta = (AllowPrivateAccess = "true"))
		EAnimState AnimState = EAnimState::Idle;

	//~ ------------- F L I P B O O K S ----------------------------------------------------------------------------

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void MoveRight(float value) override;

	virtual void BeginPlay() override;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UPaperFlipbookComponent* AnimatedSpriteComponent;

	virtual void UpdateAnimation();

	/** sets character state to NewState  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
		void SetState(EAnimState NewState) { AnimState = NewState; }

	virtual void Jump() override;
	virtual void Land() override;

	virtual	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult &Hit) override;

};
