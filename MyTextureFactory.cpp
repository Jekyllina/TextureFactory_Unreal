// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTextureFactory.h"
#include "ImageUtils.h"
#include "Misc/FileHelper.h"

UMyTextureFactory::UMyTextureFactory()
{
	bEditorImport = true;

	SupportedClass = UTexture2D::StaticClass();

	Formats.Add(FString("txt;txt format"));
}

UObject* UMyTextureFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename,
	const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	TArray<FString> Lines;	
	FFileHelper::LoadFileToStringArray(Lines, *Filename);
	int32 Width = 0;
	int32 Height = 0;

	TArray<Square> Squares{};
	TArray<Circle> Circles{};
	
	for (const FString& Line : Lines)
	{		
		TArray<FString> TextureValues;
		Line.ParseIntoArray(TextureValues, TEXT(" "));
		
		if (!TextureValues[0].Compare(TEXT("width"), ESearchCase::IgnoreCase))		
		{		
			Width = FCString::Atoi(*TextureValues[1]);
		}
		
		if (!TextureValues[0].Compare(TEXT("height"), ESearchCase::IgnoreCase))		
		{			
			Height = FCString::Atoi(*TextureValues[1]);
		}
		
		if (!TextureValues[0].Compare(TEXT("square"), ESearchCase::IgnoreCase))
		{
			Square MySquare;

			MySquare.TopLeft.Add(FCString::Atoi(*TextureValues[1]));
			MySquare.TopLeft.Add(FCString::Atoi(*TextureValues[2]));
			MySquare.BottomRight.Add(FCString::Atoi(*TextureValues[3]));
			MySquare.BottomRight.Add(FCString::Atoi(*TextureValues[4]));

			MySquare.Color.R = FCString::Atoi(*TextureValues[5]);
			MySquare.Color.G = FCString::Atoi(*TextureValues[6]);
			MySquare.Color.B = FCString::Atoi(*TextureValues[7]);

			if (TextureValues.Num() == 9)
			{
				MySquare.Color.A = FCString::Atoi(*TextureValues[8]);
			}		
			else
			{
				MySquare.Color.A = 255;
			}

			Squares.Add(MySquare);
		}

		if (!TextureValues[0].Compare(TEXT("circle"), ESearchCase::IgnoreCase))
		{
			Circle MyCircle;

			MyCircle.Center.X = FCString::Atof(*TextureValues[1]);
			MyCircle.Center.Y = FCString::Atof(*TextureValues[2]);
			MyCircle.Radius = FCString::Atof(*TextureValues[3]);
			
			MyCircle.Color.R = FCString::Atoi(*TextureValues[4]);
			MyCircle.Color.G = FCString::Atoi(*TextureValues[5]);
			MyCircle.Color.B = FCString::Atoi(*TextureValues[6]);

			if (TextureValues.Num() == 8)
			{
				MyCircle.Color.A = FCString::Atoi(*TextureValues[7]);
			}
			else
			{
				MyCircle.Color.A = 255;
			}

			Circles.Add(MyCircle);
		}

		if (Line.IsEmpty())
		{
			continue;
		}
	}

	TArray<FColor> Pixels;
	Pixels.AddZeroed(Width * Height); 
	FCreateTexture2DParameters Params;	
	
	for (int32 Index = 0; Index < Squares.Num(); Index++)
	{
		for (int32 X = 0; X < Width; X++)
		{
			for (int32 Y = 0; Y < Height; Y++)
			{
				if (X > Squares[Index].TopLeft[0] && X < Squares[Index].BottomRight[0])
				{
					if (Y > Squares[Index].TopLeft[1] && Y < Squares[Index].BottomRight[1])
					{
						int32 Pixel = (Width * Y) + X;

						//alphablending
						float MyAlpha = Squares[Index].Color.A / 255.0;
						float AlphaBack = 1.0 - Squares[Index].Color.A;
						int32 RedBack = Pixels[Pixel].R;
						int32 GreenBack = Pixels[Pixel].G;
						int32 BlueBack = Pixels[Pixel].B;

						Pixels[Pixel].R = (Squares[Index].Color.R * MyAlpha) + (RedBack * AlphaBack);
						Pixels[Pixel].G = (Squares[Index].Color.G * MyAlpha) + (GreenBack * AlphaBack);
						Pixels[Pixel].B = (Squares[Index].Color.B * MyAlpha) + (BlueBack * AlphaBack);												
					}
				}				
			}
		}
	}	

	for (int32 Index = 0; Index < Circles.Num(); Index++)
	{
		for (int32 X = 0; X < Width; X++)
		{
			for (int32 Y = 0; Y < Height; Y++)
			{
				if (FVector2D::Distance(Circles[Index].Center, FVector2D(X, Y)) <= Circles[Index].Radius)
				{
					int32 Pixel = (Width * Y) + X;

					//alphablending
					float MyAlpha = Circles[Index].Color.A / 255.0;
					float AlphaBack = 1.0 - Circles[Index].Color.A;
					int32 RedBack = Pixels[Pixel].R;
					int32 GreenBack = Pixels[Pixel].G;
					int32 BlueBack = Pixels[Pixel].B;

					Pixels[Pixel].R = (Circles[Index].Color.R * MyAlpha) + (RedBack * AlphaBack);
					Pixels[Pixel].G = (Circles[Index].Color.G * MyAlpha) + (GreenBack * AlphaBack);
					Pixels[Pixel].B = (Circles[Index].Color.B * MyAlpha) + (BlueBack * AlphaBack);
				}
			}
		}
	}

	return FImageUtils::CreateTexture2D(Width, Height, Pixels, InParent, InName.ToString(), Flags, Params);	
}