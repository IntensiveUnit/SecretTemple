// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskSystem/CustomTask.h"

void ACustomTask::OnActivated_Implementation()
{
}

void ACustomTask::OnCompleted_Implementation()
{
	OnCompleted_Delegate.Broadcast(this);
}

void ACustomTask::OnAfterCompleted_Implementation()
{
}
