// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MyTextureFactory.generated.h"

/**
 * 
 */

struct Square
{	
	FVector2D TopLeft{};
	FVector2D BottomRight{};
	FColor Color;
};

struct Circle
{
	FVector2D Center{};
	float Radius;
	FColor Color;
};

UCLASS()
class LEZIONEGEOTECH_API UMyTextureFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UMyTextureFactory();

	UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename,
		const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
};
