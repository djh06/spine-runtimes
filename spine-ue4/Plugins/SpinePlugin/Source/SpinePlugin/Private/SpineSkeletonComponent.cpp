#include "SpinePluginPrivatePCH.h"

#define LOCTEXT_NAMESPACE "Spine"

USpineSkeletonComponent::USpineSkeletonComponent () {
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;
}

bool USpineSkeletonComponent::SetSkin(const FString& skinName) {
	CheckState();
	if (skeleton) return spSkeleton_setSkinByName(skeleton, TCHAR_TO_UTF8(*skinName)) != 0;
	else return false;
}

bool USpineSkeletonComponent::setAttachment (const FString& slotName, const FString& attachmentName) {
	CheckState();
	if (skeleton) return spSkeleton_setAttachment(skeleton, TCHAR_TO_UTF8(*slotName), TCHAR_TO_UTF8(*attachmentName));
	return false;
}

FTransform USpineSkeletonComponent::GetBoneWorldTransform (const FString& BoneName) {
	return FTransform();
}

FTransform USpineSkeletonComponent::GetBoneLocalTransform (const FString& BoneName) {
	return FTransform();
}

void USpineSkeletonComponent::SetToSetupPose () {
	CheckState();
	if (skeleton) spSkeleton_setToSetupPose(skeleton);
}

void USpineSkeletonComponent::SetBonesToSetupPose () {
	CheckState();
	if (skeleton) spSkeleton_setBonesToSetupPose(skeleton);
}

void USpineSkeletonComponent::SetSlotsToSetupPose () {
	CheckState();
	if (skeleton) spSkeleton_setSlotsToSetupPose(skeleton);
}

void USpineSkeletonComponent::BeginPlay() {
	Super::BeginPlay();
}

void USpineSkeletonComponent::TickComponent (float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	CheckState();

	if (skeleton) {
		BeforeUpdateWorldTransform.Broadcast(this);
		spSkeleton_updateWorldTransform(skeleton);
		AfterUpdateWorldTransform.Broadcast(this);
	}
}

void USpineSkeletonComponent::CheckState () {
	if (lastAtlas != Atlas || lastData != SkeletonData) {
		DisposeState();
		
		if (Atlas && SkeletonData) {
			spSkeletonData* data = SkeletonData->GetSkeletonData(Atlas->GetAtlas(false), false);
			skeleton = spSkeleton_create(data);
		}
		
		lastAtlas = Atlas;
		lastData = SkeletonData;
	}
}

void USpineSkeletonComponent::DisposeState () {
	if (skeleton) {
		spSkeleton_dispose(skeleton);
		skeleton = nullptr;
	}
}

void USpineSkeletonComponent::FinishDestroy () {
	DisposeState();
	Super::FinishDestroy();
}

#undef LOCTEXT_NAMESPACE
