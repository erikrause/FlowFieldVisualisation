// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorMaterial.h"

USensorMaterial::USensorMaterial()
{
	//EmissiveColor = FColorMaterialInput()
	//EmissiveColor = FVector(1, 0, 0);
	EmissiveColor.Constant = FColor(1,0,0,0);
	//EmissiveColor.
	//FAssetRegistryModule::AssetCreated(UnrealMaterial);
}