// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// 当等级信息数组中只设置了 2 个等级时，直接返回对应的等级
		if (LevelUpInformation.Num() - 1 <= Level)
			return Level;

		// 循环判断 XP 是否大于升级需要的经验值，大于就表示可以升级
		if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			Level++;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
