// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/AVGameInstance.h"

UAVGameInstance* UAVGameInstance::s_Instance = nullptr;

UAVGameInstance* UAVGameInstance::Get()
{
	return s_Instance;
}

void UAVGameInstance::Init()
{
	s_Instance = this;

	Super::Init();
}

void UAVGameInstance::BeginDestroy()
{
	s_Instance = nullptr;

	Super::BeginDestroy();
}

