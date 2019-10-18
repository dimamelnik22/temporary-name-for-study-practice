// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "TestGenerator.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*void AMyActor::Test() {
	UE_LOG(LogTemp, Warning, TEXT("Global Actor Func"));
}*/

void AMyActor::GenerateLevel() {
	//TestGenerator Generator;
	TestGenerator Generator(16, 32);
	Width = Generator.GetWidth();
	Height = Generator.GetHeight();
	Matrix = Generator.GetBlocks();
	NumberOfEnemies = Generator.GetNumberOfEnemies();
	NumberOfBlocks = Generator.GetNumberOfBlocks();

	TestGenerator::FVector* points = Generator.GetStartAndFinish();
	Start[0] = points[0].X; Start[1] = points[0].Y; Start[2] = points[0].Z;
	Finish[0] = points[1].X; Finish[1] = points[1].Y; Finish[2] = points[1].Z;
	
	/*points = Generator.GetArrayOfEnemies();
	for (int j = 0; j < NumberOfEnemies; ++j) {
		ArrayOfEnemies[j][0] = points[j].X;
		ArrayOfEnemies[j][1] = points[j].Y;
		ArrayOfEnemies[j][2] = points[j].Z;
	}*/

	/*points = Generator.GetArrayOfBlocks();
	for (int j = 0; j < NumberOfEnemies; ++j) {
		ArrayOfBlocks[j][0] = points[j].X;
		ArrayOfBlocks[j][1] = points[j].Y;
		ArrayOfBlocks[j][2] = points[j].Z;
	}*/
}

bool AMyActor::GetCell(int i, int j) {
	return Matrix[i][j];
}

int AMyActor::GetWidth() {
	return Width;
}

int AMyActor::GetHeight() {
	return Height;
}

int AMyActor::GetNumberOfEnemies() {
	return NumberOfEnemies;
}

FVector AMyActor::GetEnemyPosition(int i) {
	return ArrayOfEnemies[i];
}

int AMyActor::GetNumberOfBlocks() {
	return NumberOfBlocks;
}

FVector AMyActor::GetBlockPosition(int i) {
	return ArrayOfBlocks[i];
}

FVector AMyActor::GetStartPosition() {
	return Start;
}

FVector AMyActor::GetFinishPosition() {
	return Finish;
}
