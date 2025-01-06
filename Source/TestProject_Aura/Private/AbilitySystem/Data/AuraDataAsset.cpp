// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AuraDataAsset.h"

#include "TestProject_Aura/AuraLogChannels.h"

FAuraAttributeInfo UAuraDataAsset::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	// 遍历数组，用 MatchesTagExact 匹配 Tag，匹配成功则返回对应的信息
	for (const FAuraAttributeInfo& Info: AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("AttributeTag [%s] not found in [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	
	return FAuraAttributeInfo();
}
