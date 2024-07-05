/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "DB2Stores.h"
#include "DBCStores.h"
#include "Containers.h"
#include "DatabaseEnv.h"
#include "DB2LoadInfo.h"
#include "Hash.h"
#include "ItemTemplate.h"
#include "IteratorPair.h"
#include "Log.h"
#include "Random.h"
#include "Regex.h"
#include "Timer.h"
#include "Util.h"
#include "World.h"
#include <boost/filesystem/directory.hpp>
#include <boost/filesystem/operations.hpp>
#include <array>
#include <bitset>
#include <numeric>
#include <sstream>
#include <cctype>

// temporary hack until includes are sorted out (don't want to pull in Windows.h)
#ifdef GetClassName
#undef GetClassName
#endif

DB2Storage<AdventureJournalEntry>               sAdventureJournalStore("AdventureJournal.db2", &AdventureJournalLoadInfo::Instance);
DB2Storage<AdventureMapPOIEntry>                sAdventureMapPOIStore("AdventureMapPOI.db2", &AdventureMapPoiLoadInfo::Instance);
DB2Storage<ArtifactEntry>                       sArtifactStore("Artifact.db2", &ArtifactLoadInfo::Instance);
DB2Storage<ArtifactAppearanceEntry>             sArtifactAppearanceStore("ArtifactAppearance.db2", &ArtifactAppearanceLoadInfo::Instance);
DB2Storage<ArtifactAppearanceSetEntry>          sArtifactAppearanceSetStore("ArtifactAppearanceSet.db2", &ArtifactAppearanceSetLoadInfo::Instance);
DB2Storage<ArtifactCategoryEntry>               sArtifactCategoryStore("ArtifactCategory.db2", &ArtifactCategoryLoadInfo::Instance);
DB2Storage<ArtifactPowerEntry>                  sArtifactPowerStore("ArtifactPower.db2", &ArtifactPowerLoadInfo::Instance);
DB2Storage<ArtifactPowerLinkEntry>              sArtifactPowerLinkStore("ArtifactPowerLink.db2", &ArtifactPowerLinkLoadInfo::Instance);
DB2Storage<ArtifactPowerPickerEntry>            sArtifactPowerPickerStore("ArtifactPowerPicker.db2", &ArtifactPowerPickerLoadInfo::Instance);
DB2Storage<ArtifactPowerRankEntry>              sArtifactPowerRankStore("ArtifactPowerRank.db2", &ArtifactPowerRankLoadInfo::Instance);
DB2Storage<ArtifactQuestXPEntry>                sArtifactQuestXPStore("ArtifactQuestXP.db2", &ArtifactQuestXpLoadInfo::Instance);
DB2Storage<ArtifactTierEntry>                   sArtifactTierStore("ArtifactTier.db2", &ArtifactTierLoadInfo::Instance);
DB2Storage<ArtifactUnlockEntry>                 sArtifactUnlockStore("ArtifactUnlock.db2", &ArtifactUnlockLoadInfo::Instance);
DB2Storage<AzeriteEmpoweredItemEntry>           sAzeriteEmpoweredItemStore("AzeriteEmpoweredItem.db2", &AzeriteEmpoweredItemLoadInfo::Instance);
DB2Storage<AzeriteEssenceEntry>                 sAzeriteEssenceStore("AzeriteEssence.db2", &AzeriteEssenceLoadInfo::Instance);
DB2Storage<AzeriteEssencePowerEntry>            sAzeriteEssencePowerStore("AzeriteEssencePower.db2", &AzeriteEssencePowerLoadInfo::Instance);
DB2Storage<AzeriteItemEntry>                    sAzeriteItemStore("AzeriteItem.db2", &AzeriteItemLoadInfo::Instance);
DB2Storage<AzeriteItemMilestonePowerEntry>      sAzeriteItemMilestonePowerStore("AzeriteItemMilestonePower.db2", &AzeriteItemMilestonePowerLoadInfo::Instance);
DB2Storage<AzeriteKnowledgeMultiplierEntry>     sAzeriteKnowledgeMultiplierStore("AzeriteKnowledgeMultiplier.db2", &AzeriteKnowledgeMultiplierLoadInfo::Instance);
DB2Storage<AzeriteLevelInfoEntry>               sAzeriteLevelInfoStore("AzeriteLevelInfo.db2", &AzeriteLevelInfoLoadInfo::Instance);
DB2Storage<AzeritePowerEntry>                   sAzeritePowerStore("AzeritePower.db2", &AzeritePowerLoadInfo::Instance);
DB2Storage<AzeritePowerSetMemberEntry>          sAzeritePowerSetMemberStore("AzeritePowerSetMember.db2", &AzeritePowerSetMemberLoadInfo::Instance);
DB2Storage<AzeriteTierUnlockEntry>              sAzeriteTierUnlockStore("AzeriteTierUnlock.db2", &AzeriteTierUnlockLoadInfo::Instance);
DB2Storage<AzeriteTierUnlockSetEntry>           sAzeriteTierUnlockSetStore("AzeriteTierUnlockSet.db2", &AzeriteTierUnlockSetLoadInfo::Instance);
DB2Storage<AzeriteUnlockMappingEntry>           sAzeriteUnlockMappingStore("AzeriteUnlockMapping.db2", &AzeriteUnlockMappingLoadInfo::Instance);
DB2Storage<BattlePetAbilityEntry>               sBattlePetAbilityStore("BattlePetAbility.db2", &BattlePetAbilityLoadInfo::Instance);
DB2Storage<BattlePetBreedQualityEntry>          sBattlePetBreedQualityStore("BattlePetBreedQuality.db2", &BattlePetBreedQualityLoadInfo::Instance);
DB2Storage<BattlePetBreedStateEntry>            sBattlePetBreedStateStore("BattlePetBreedState.db2", &BattlePetBreedStateLoadInfo::Instance);
DB2Storage<BattlePetSpeciesEntry>               sBattlePetSpeciesStore("BattlePetSpecies.db2", &BattlePetSpeciesLoadInfo::Instance);
DB2Storage<BattlePetSpeciesStateEntry>          sBattlePetSpeciesStateStore("BattlePetSpeciesState.db2", &BattlePetSpeciesStateLoadInfo::Instance);
DB2Storage<BroadcastTextEntry>                  sBroadcastTextStore("BroadcastText.db2", &BroadcastTextLoadInfo::Instance);
DB2Storage<ChallengeModeItemBonusOverrideEntry> sChallengeModeItemBonusOverrideStore("ChallengeModeItemBonusOverride.db2", &ChallengeModeItemBonusOverrideLoadInfo::Instance);
DB2Storage<ChrClassUIDisplayEntry>              sChrClassUIDisplayStore("ChrClassUIDisplay.db2", &ChrClassUiDisplayLoadInfo::Instance);
DB2Storage<ChrCustomizationChoiceEntry>         sChrCustomizationChoiceStore("ChrCustomizationChoice.db2", &ChrCustomizationChoiceLoadInfo::Instance);
DB2Storage<ChrCustomizationDisplayInfoEntry>    sChrCustomizationDisplayInfoStore("ChrCustomizationDisplayInfo.db2", &ChrCustomizationDisplayInfoLoadInfo::Instance);
DB2Storage<ChrCustomizationElementEntry>        sChrCustomizationElementStore("ChrCustomizationElement.db2", &ChrCustomizationElementLoadInfo::Instance);
DB2Storage<ChrCustomizationOptionEntry>         sChrCustomizationOptionStore("ChrCustomizationOption.db2", &ChrCustomizationOptionLoadInfo::Instance);
DB2Storage<ChrCustomizationReqEntry>            sChrCustomizationReqStore("ChrCustomizationReq.db2", &ChrCustomizationReqLoadInfo::Instance);
DB2Storage<ChrCustomizationReqChoiceEntry>      sChrCustomizationReqChoiceStore("ChrCustomizationReqChoice.db2", &ChrCustomizationReqChoiceLoadInfo::Instance);
DB2Storage<ChrModelEntry>                       sChrModelStore("ChrModel.db2", &ChrModelLoadInfo::Instance);
DB2Storage<ChrRaceXChrModelEntry>               sChrRaceXChrModelStore("ChrRaceXChrModel.db2", &ChrRaceXChrModelLoadInfo::Instance);
DB2Storage<ConditionalChrModelEntry>            sConditionalChrModelStore("ConditionalChrModel.db2", &ConditionalChrModelLoadInfo::Instance);
DB2Storage<ConditionalContentTuningEntry>       sConditionalContentTuningStore("ConditionalContentTuning.db2", &ConditionalContentTuningLoadInfo::Instance);
DB2Storage<ContentTuningEntry>                  sContentTuningStore("ContentTuning.db2", &ContentTuningLoadInfo::Instance);
DB2Storage<ContentTuningXExpectedEntry>         sContentTuningXExpectedStore("ContentTuningXExpected.db2", &ContentTuningXExpectedLoadInfo::Instance);
DB2Storage<ContentTuningXLabelEntry>            sContentTuningXLabelStore("ContentTuningXLabel.db2", &ContentTuningXLabelLoadInfo::Instance);
DB2Storage<ConversationLineEntry>               sConversationLineStore("ConversationLine.db2", &ConversationLineLoadInfo::Instance);
DB2Storage<CorruptionEffectsEntry>              sCorruptionEffectsStore("CorruptionEffects.db2", &CorruptionEffectsLoadInfo::Instance);
DB2Storage<CurrencyContainerEntry>              sCurrencyContainerStore("CurrencyContainer.db2", &CurrencyContainerLoadInfo::Instance);
DB2Storage<CurveEntry>                          sCurveStore("Curve.db2", &CurveLoadInfo::Instance);
DB2Storage<CurvePointEntry>                     sCurvePointStore("CurvePoint.db2", &CurvePointLoadInfo::Instance);
DB2Storage<ExpectedStatEntry>                   sExpectedStatStore("ExpectedStat.db2", &ExpectedStatLoadInfo::Instance);
DB2Storage<ExpectedStatModEntry>                sExpectedStatModStore("ExpectedStatMod.db2", &ExpectedStatModLoadInfo::Instance);
DB2Storage<GameObjectsEntry>                    sGameObjectsStore("GameObjects.db2", &GameobjectsLoadInfo::Instance);
DB2Storage<GarrAbilityEntry>                    sGarrAbilityStore("GarrAbility.db2", &GarrAbilityLoadInfo::Instance);
DB2Storage<GarrBuildingEntry>                   sGarrBuildingStore("GarrBuilding.db2", &GarrBuildingLoadInfo::Instance);
DB2Storage<GarrBuildingPlotInstEntry>           sGarrBuildingPlotInstStore("GarrBuildingPlotInst.db2", &GarrBuildingPlotInstLoadInfo::Instance);
DB2Storage<GarrClassSpecEntry>                  sGarrClassSpecStore("GarrClassSpec.db2", &GarrClassSpecLoadInfo::Instance);
DB2Storage<GarrFollowerEntry>                   sGarrFollowerStore("GarrFollower.db2", &GarrFollowerLoadInfo::Instance);
DB2Storage<GarrFollowerXAbilityEntry>           sGarrFollowerXAbilityStore("GarrFollowerXAbility.db2", &GarrFollowerXAbilityLoadInfo::Instance);
DB2Storage<GarrMissionEntry>                    sGarrMissionStore("GarrMission.db2", &GarrMissionLoadInfo::Instance);
DB2Storage<GarrPlotEntry>                       sGarrPlotStore("GarrPlot.db2", &GarrPlotLoadInfo::Instance);
DB2Storage<GarrPlotBuildingEntry>               sGarrPlotBuildingStore("GarrPlotBuilding.db2", &GarrPlotBuildingLoadInfo::Instance);
DB2Storage<GarrPlotInstanceEntry>               sGarrPlotInstanceStore("GarrPlotInstance.db2", &GarrPlotInstanceLoadInfo::Instance);
DB2Storage<GarrSiteLevelEntry>                  sGarrSiteLevelStore("GarrSiteLevel.db2", &GarrSiteLevelLoadInfo::Instance);
DB2Storage<GarrSiteLevelPlotInstEntry>          sGarrSiteLevelPlotInstStore("GarrSiteLevelPlotInst.db2", &GarrSiteLevelPlotInstLoadInfo::Instance);
DB2Storage<GarrTalentTreeEntry>                 sGarrTalentTreeStore("GarrTalentTree.db2", &GarrTalentTreeLoadInfo::Instance);
DB2Storage<GlobalCurveEntry>                    sGlobalCurveStore("GlobalCurve.db2", &GlobalCurveLoadInfo::Instance);
DB2Storage<GlyphBindableSpellEntry>             sGlyphBindableSpellStore("GlyphBindableSpell.db2", &GlyphBindableSpellLoadInfo::Instance);
DB2Storage<GlyphRequiredSpecEntry>              sGlyphRequiredSpecStore("GlyphRequiredSpec.db2", &GlyphRequiredSpecLoadInfo::Instance);
DB2Storage<GossipNPCOptionEntry>                sGossipNPCOptionStore("GossipNPCOption.db2", &GossipNpcOptionLoadInfo::Instance);
DB2Storage<HeirloomEntry>                       sHeirloomStore("Heirloom.db2", &HeirloomLoadInfo::Instance);
DB2Storage<ItemAppearanceEntry>                 sItemAppearanceStore("ItemAppearance.db2", &ItemAppearanceLoadInfo::Instance);
DB2Storage<ItemBonusEntry>                      sItemBonusStore("ItemBonus.db2", &ItemBonusLoadInfo::Instance);
DB2Storage<ItemBonusListGroupEntryEntry>        sItemBonusListGroupEntryStore("ItemBonusListGroupEntry.db2", &ItemBonusListGroupEntryLoadInfo::Instance);
DB2Storage<ItemBonusListLevelDeltaEntry>        sItemBonusListLevelDeltaStore("ItemBonusListLevelDelta.db2", &ItemBonusListLevelDeltaLoadInfo::Instance);
DB2Storage<ItemBonusTreeEntry>                  sItemBonusTreeStore("ItemBonusTree.db2", &ItemBonusTreeLoadInfo::Instance);
DB2Storage<ItemBonusTreeNodeEntry>              sItemBonusTreeNodeStore("ItemBonusTreeNode.db2", &ItemBonusTreeNodeLoadInfo::Instance);
DB2Storage<ItemChildEquipmentEntry>             sItemChildEquipmentStore("ItemChildEquipment.db2", &ItemChildEquipmentLoadInfo::Instance);
DB2Storage<ItemContextPickerEntryEntry>         sItemContextPickerEntryStore("ItemContextPickerEntry.db2", &ItemContextPickerEntryLoadInfo::Instance);
DB2Storage<ItemCurrencyCostEntry>               sItemCurrencyCostStore("ItemCurrencyCost.db2", &ItemCurrencyCostLoadInfo::Instance);
DB2Storage<ItemEffectEntry>                     sItemEffectStore("ItemEffect.db2", &ItemEffectLoadInfo::Instance);
DB2Storage<ItemEntry>                           sItemStore("Item.db2", &ItemLoadInfo::Instance);
DB2Storage<ItemExtendedCostEntry>               sItemExtendedCostStore("ItemExtendedCost.db2", &ItemExtendedCostLoadInfo::Instance);
DB2Storage<ItemLevelSelectorEntry>              sItemLevelSelectorStore("ItemLevelSelector.db2", &ItemLevelSelectorLoadInfo::Instance);
DB2Storage<ItemLevelSelectorQualityEntry>       sItemLevelSelectorQualityStore("ItemLevelSelectorQuality.db2", &ItemLevelSelectorQualityLoadInfo::Instance);
DB2Storage<ItemLevelSelectorQualitySetEntry>    sItemLevelSelectorQualitySetStore("ItemLevelSelectorQualitySet.db2", &ItemLevelSelectorQualitySetLoadInfo::Instance);
DB2Storage<ItemLimitCategoryConditionEntry>     sItemLimitCategoryConditionStore("ItemLimitCategoryCondition.db2", &ItemLimitCategoryConditionLoadInfo::Instance);
DB2Storage<ItemModifiedAppearanceEntry>         sItemModifiedAppearanceStore("ItemModifiedAppearance.db2", &ItemModifiedAppearanceLoadInfo::Instance);
DB2Storage<ItemModifiedAppearanceExtraEntry>    sItemModifiedAppearanceExtraStore("ItemModifiedAppearanceExtra.db2", &ItemModifiedAppearanceExtraLoadInfo::Instance);
DB2Storage<ItemSearchNameEntry>                 sItemSearchNameStore("ItemSearchName.db2", &ItemSearchNameLoadInfo::Instance);
DB2Storage<ItemSetSpellEntry>                   sItemSetSpellStore("ItemSetSpell.db2", &ItemSetSpellLoadInfo::Instance);
DB2Storage<ItemSparseEntry>                     sItemSparseStore("ItemSparse.db2", &ItemSparseLoadInfo::Instance);
DB2Storage<ItemXBonusTreeEntry>                 sItemXBonusTreeStore("ItemXBonusTree.db2", &ItemXBonusTreeLoadInfo::Instance);
DB2Storage<ItemXItemEffectEntry>                sItemXItemEffectStore("ItemXItemEffect.db2", &ItemXItemEffectLoadInfo::Instance);
DB2Storage<KeychainEntry>                       sKeychainStore("Keychain.db2", &KeychainLoadInfo::Instance);
DB2Storage<KeystoneAffixEntry>                  sKeystoneAffixStore("KeystoneAffix.db2", &KeystoneAffixLoadInfo::Instance);
DB2Storage<LocationEntry>                       sLocationStore("Location.db2", &LocationLoadInfo::Instance);
DB2Storage<MapChallengeModeEntry>               sMapChallengeModeStore("MapChallengeMode.db2", &MapChallengeModeLoadInfo::Instance);
DB2Storage<MapDifficultyXConditionEntry>        sMapDifficultyXConditionStore("MapDifficultyXCondition.db2", &MapDifficultyXConditionLoadInfo::Instance);
DB2Storage<MawPowerEntry>                       sMawPowerStore("MawPower.db2", &MawPowerLoadInfo::Instance);
DB2Storage<MountEntry>                          sMountStore("Mount.db2", &MountLoadInfo::Instance);
DB2Storage<MountTypeXCapabilityEntry>           sMountTypeXCapabilityStore("MountTypeXCapability.db2", &MountTypeXCapabilityLoadInfo::Instance);
DB2Storage<MountXDisplayEntry>                  sMountXDisplayStore("MountXDisplay.db2", &MountXDisplayLoadInfo::Instance);
DB2Storage<MythicPlusSeasonEntry>               sMythicPlusSeasonStore("MythicPlusSeason.db2", &MythicPlusSeasonLoadInfo::Instance);
DB2Storage<NumTalentsAtLevelEntry>              sNumTalentsAtLevelStore("NumTalentsAtLevel.db2", &NumTalentsAtLevelLoadInfo::Instance);
DB2Storage<ParagonReputationEntry>              sParagonReputationStore("ParagonReputation.db2", &ParagonReputationLoadInfo::Instance);
DB2Storage<PathEntry>                           sPathStore("Path.db2", &PathLoadInfo::Instance);
DB2Storage<PathNodeEntry>                       sPathNodeStore("PathNode.db2", &PathNodeLoadInfo::Instance);
DB2Storage<PathPropertyEntry>                   sPathPropertyStore("PathProperty.db2", &PathPropertyLoadInfo::Instance);
DB2Storage<PowerTypeEntry>                      sPowerTypeStore("PowerType.db2", &PowerTypeLoadInfo::Instance);
DB2Storage<PrestigeLevelInfoEntry>              sPrestigeLevelInfoStore("PrestigeLevelInfo.db2", &PrestigeLevelInfoLoadInfo::Instance);
DB2Storage<PVPDifficultyEntry>                  sPVPDifficultyStore("PVPDifficulty.db2", &PvpDifficultyLoadInfo::Instance);
DB2Storage<PVPItemEntry>                        sPVPItemStore("PVPItem.db2", &PvpItemLoadInfo::Instance);
DB2Storage<PVPStatEntry>                        sPVPStatStore("PVPStat.db2", &PvpStatLoadInfo::Instance);
DB2Storage<PvpSeasonEntry>                      sPvpSeasonStore("PvpSeason.db2", &PvpSeasonLoadInfo::Instance);
DB2Storage<PvpTalentEntry>                      sPvpTalentStore("PvpTalent.db2", &PvpTalentLoadInfo::Instance);
DB2Storage<PvpTalentCategoryEntry>              sPvpTalentCategoryStore("PvpTalentCategory.db2", &PvpTalentCategoryLoadInfo::Instance);
DB2Storage<PvpTalentSlotUnlockEntry>            sPvpTalentSlotUnlockStore("PvpTalentSlotUnlock.db2", &PvpTalentSlotUnlockLoadInfo::Instance);
DB2Storage<PvpTierEntry>                        sPvpTierStore("PvpTier.db2", &PvpTierLoadInfo::Instance);
DB2Storage<QuestLineXQuestEntry>                sQuestLineXQuestStore("QuestLineXQuest.db2", &QuestLineXQuestLoadInfo::Instance);
DB2Storage<QuestPackageItemEntry>               sQuestPackageItemStore("QuestPackageItem.db2", &QuestPackageItemLoadInfo::Instance);
DB2Storage<RewardPackEntry>                     sRewardPackStore("RewardPack.db2", &RewardPackLoadInfo::Instance);
DB2Storage<RewardPackXCurrencyTypeEntry>        sRewardPackXCurrencyTypeStore("RewardPackXCurrencyType.db2", &RewardPackXCurrencyTypeLoadInfo::Instance);
DB2Storage<RewardPackXItemEntry>                sRewardPackXItemStore("RewardPackXItem.db2", &RewardPackXItemLoadInfo::Instance);
DB2Storage<SceneScriptEntry>                    sSceneScriptStore("SceneScript.db2", &SceneScriptLoadInfo::Instance);
DB2Storage<SceneScriptGlobalTextEntry>          sSceneScriptGlobalTextStore("SceneScriptGlobalText.db2", &SceneScriptGlobalTextLoadInfo::Instance);
DB2Storage<SceneScriptPackageEntry>             sSceneScriptPackageStore("SceneScriptPackage.db2", &SceneScriptPackageLoadInfo::Instance);
DB2Storage<SceneScriptTextEntry>                sSceneScriptTextStore("SceneScriptText.db2", &SceneScriptTextLoadInfo::Instance);
DB2Storage<SkillLineXTraitTreeEntry>            sSkillLineXTraitTreeStore("SkillLineXTraitTree.db2", &SkillLineXTraitTreeLoadInfo::Instance);
DB2Storage<SoulbindConduitRankEntry>            sSoulbindConduitRankStore("SoulbindConduitRank.db2", &SoulbindConduitRankLoadInfo::Instance);
DB2Storage<SoundKitEntry>                       sSoundKitStore("SoundKit.db2", &SoundKitLoadInfo::Instance);
DB2Storage<SpecSetMemberEntry>                  sSpecSetMemberStore("SpecSetMember.db2", &SpecSetMemberLoadInfo::Instance);
DB2Storage<SpellEmpowerEntry>                   sSpellEmpowerStore("SpellEmpower.db2", &SpellEmpowerLoadInfo::Instance);
DB2Storage<SpellEmpowerStageEntry>              sSpellEmpowerStageStore("SpellEmpowerStage.db2", &SpellEmpowerStageLoadInfo::Instance);
DB2Storage<SpellLabelEntry>                     sSpellLabelStore("SpellLabel.db2", &SpellLabelLoadInfo::Instance);
FakeDB2Storage<SpellNameEntry*>                 sSpellNameStore; //("SpellName.db2", &SpellNameLoadInfo::Instance);
DB2Storage<SpellPowerDifficultyEntry>           sSpellPowerDifficultyStore("SpellPowerDifficulty.db2", &SpellPowerDifficultyLoadInfo::Instance);
DB2Storage<SpellReagentsEntry>                  sSpellReagentsStore("SpellReagents.db2", &SpellReagentsLoadInfo::Instance);
DB2Storage<SpellReagentsCurrencyEntry>          sSpellReagentsCurrencyStore("SpellReagentsCurrency.db2", &SpellReagentsCurrencyLoadInfo::Instance);
DB2Storage<SpellVisualEntry>                    sSpellVisualStore("SpellVisual.db2", &SpellVisualLoadInfo::Instance);
DB2Storage<SpellVisualEffectNameEntry>          sSpellVisualEffectNameStore("SpellVisualEffectName.db2", &SpellVisualEffectNameLoadInfo::Instance);
DB2Storage<SpellVisualMissileEntry>             sSpellVisualMissileStore("SpellVisualMissile.db2", &SpellVisualMissileLoadInfo::Instance);
DB2Storage<SpellVisualKitEntry>                 sSpellVisualKitStore("SpellVisualKit.db2", &SpellVisualKitLoadInfo::Instance);
DB2Storage<SpellXSpellVisualEntry>              sSpellXSpellVisualStore("SpellXSpellVisual.db2", &SpellXSpellVisualLoadInfo::Instance);
DB2Storage<TactKeyEntry>                        sTactKeyStore("TactKey.db2", &TactKeyLoadInfo::Instance);
DB2Storage<ToyEntry>                            sToyStore("Toy.db2", &ToyLoadInfo::Instance);
DB2Storage<TraitCondEntry>                      sTraitCondStore("TraitCond.db2", &TraitCondLoadInfo::Instance);
DB2Storage<TraitCostEntry>                      sTraitCostStore("TraitCost.db2", &TraitCostLoadInfo::Instance);
DB2Storage<TraitCurrencyEntry>                  sTraitCurrencyStore("TraitCurrency.db2", &TraitCurrencyLoadInfo::Instance);
DB2Storage<TraitCurrencySourceEntry>            sTraitCurrencySourceStore("TraitCurrencySource.db2", &TraitCurrencySourceLoadInfo::Instance);
DB2Storage<TraitDefinitionEntry>                sTraitDefinitionStore("TraitDefinition.db2", &TraitDefinitionLoadInfo::Instance);
DB2Storage<TraitDefinitionEffectPointsEntry>    sTraitDefinitionEffectPointsStore("TraitDefinitionEffectPoints.db2", &TraitDefinitionEffectPointsLoadInfo::Instance);
DB2Storage<TraitEdgeEntry>                      sTraitEdgeStore("TraitEdge.db2", &TraitEdgeLoadInfo::Instance);
DB2Storage<TraitNodeEntry>                      sTraitNodeStore("TraitNode.db2", &TraitNodeLoadInfo::Instance);
DB2Storage<TraitNodeEntryEntry>                 sTraitNodeEntryStore("TraitNodeEntry.db2", &TraitNodeEntryLoadInfo::Instance);
DB2Storage<TraitNodeEntryXTraitCondEntry>       sTraitNodeEntryXTraitCondStore("TraitNodeEntryXTraitCond.db2", &TraitNodeEntryXTraitCondLoadInfo::Instance);
DB2Storage<TraitNodeEntryXTraitCostEntry>       sTraitNodeEntryXTraitCostStore("TraitNodeEntryXTraitCost.db2", &TraitNodeEntryXTraitCostLoadInfo::Instance);
DB2Storage<TraitNodeGroupEntry>                 sTraitNodeGroupStore("TraitNodeGroup.db2", &TraitNodeGroupLoadInfo::Instance);
DB2Storage<TraitNodeGroupXTraitCondEntry>       sTraitNodeGroupXTraitCondStore("TraitNodeGroupXTraitCond.db2", &TraitNodeGroupXTraitCondLoadInfo::Instance);
DB2Storage<TraitNodeGroupXTraitCostEntry>       sTraitNodeGroupXTraitCostStore("TraitNodeGroupXTraitCost.db2", &TraitNodeGroupXTraitCostLoadInfo::Instance);
DB2Storage<TraitNodeGroupXTraitNodeEntry>       sTraitNodeGroupXTraitNodeStore("TraitNodeGroupXTraitNode.db2", &TraitNodeGroupXTraitNodeLoadInfo::Instance);
DB2Storage<TraitNodeXTraitCondEntry>            sTraitNodeXTraitCondStore("TraitNodeXTraitCond.db2", &TraitNodeXTraitCondLoadInfo::Instance);
DB2Storage<TraitNodeXTraitCostEntry>            sTraitNodeXTraitCostStore("TraitNodeXTraitCost.db2", &TraitNodeXTraitCostLoadInfo::Instance);
DB2Storage<TraitNodeXTraitNodeEntryEntry>       sTraitNodeXTraitNodeEntryStore("TraitNodeXTraitNodeEntry.db2", &TraitNodeXTraitNodeEntryLoadInfo::Instance);
DB2Storage<TraitTreeEntry>                      sTraitTreeStore("TraitTree.db2", &TraitTreeLoadInfo::Instance);
DB2Storage<TraitTreeLoadoutEntry>               sTraitTreeLoadoutStore("TraitTreeLoadout.db2", &TraitTreeLoadoutLoadInfo::Instance);
DB2Storage<TraitTreeLoadoutEntryEntry>          sTraitTreeLoadoutEntryStore("TraitTreeLoadoutEntry.db2", &TraitTreeLoadoutEntryLoadInfo::Instance);
DB2Storage<TraitTreeXTraitCostEntry>            sTraitTreeXTraitCostStore("TraitTreeXTraitCost.db2", &TraitTreeXTraitCostLoadInfo::Instance);
DB2Storage<TraitTreeXTraitCurrencyEntry>        sTraitTreeXTraitCurrencyStore("TraitTreeXTraitCurrency.db2", &TraitTreeXTraitCurrencyLoadInfo::Instance);
DB2Storage<TransmogHolidayEntry>                sTransmogHolidayStore("TransmogHoliday.db2", &TransmogHolidayLoadInfo::Instance);
DB2Storage<TransmogIllusionEntry>               sTransmogIllusionStore("TransmogIllusion.db2", &TransmogIllusionLoadInfo::Instance);
DB2Storage<TransmogSetEntry>                    sTransmogSetStore("TransmogSet.db2", &TransmogSetLoadInfo::Instance);
DB2Storage<TransmogSetGroupEntry>               sTransmogSetGroupStore("TransmogSetGroup.db2", &TransmogSetGroupLoadInfo::Instance);
DB2Storage<TransmogSetItemEntry>                sTransmogSetItemStore("TransmogSetItem.db2", &TransmogSetItemLoadInfo::Instance);
DB2Storage<UiMapEntry>                          sUiMapStore("UiMap.db2", &UiMapLoadInfo::Instance);
DB2Storage<UiMapAssignmentEntry>                sUiMapAssignmentStore("UiMapAssignment.db2", &UiMapAssignmentLoadInfo::Instance);
DB2Storage<UiMapLinkEntry>                      sUiMapLinkStore("UiMapLink.db2", &UiMapLinkLoadInfo::Instance);
DB2Storage<UiMapXMapArtEntry>                   sUiMapXMapArtStore("UiMapXMapArt.db2", &UiMapXMapArtLoadInfo::Instance);
DB2Storage<UISplashScreenEntry>                 sUISplashScreenStore("UISplashScreen.db2", &UiSplashScreenLoadInfo::Instance);
DB2Storage<VignetteEntry>                       sVignetteStore("Vignette.db2", &VignetteLoadInfo::Instance);

TaxiMask                                        sTaxiNodesMask;
TaxiMask                                        sOldContinentsNodesMask;
TaxiMask                                        sHordeTaxiNodesMask;
TaxiMask                                        sAllianceTaxiNodesMask;
TaxiPathNodesByPath                             sTaxiPathNodesByPath;

DEFINE_DB2_SET_COMPARATOR(ChrClassesXPowerTypesEntry)

typedef std::map<uint32 /*hash*/, DB2StorageBase*> StorageMap;
typedef std::unordered_map<uint32 /*areaGroupId*/, std::vector<uint32/*areaId*/>> AreaGroupMemberContainer;
typedef std::unordered_map<uint32, std::vector<ArtifactPowerEntry const*>> ArtifactPowersContainer;
typedef std::unordered_map<uint32, std::vector<uint32>> ArtifactPowerLinksContainer;
typedef std::unordered_map<std::pair<uint32, uint8>, ArtifactPowerRankEntry const*> ArtifactPowerRanksContainer;
typedef std::unordered_map<uint32 /*curveID*/, std::vector<DBCPosition2D>> CurvePointsContainer;
typedef std::map<std::tuple<uint32, uint8, uint8, uint8>, EmotesTextSoundEntry const*> EmotesTextSoundContainer;
typedef std::unordered_map<uint32, HeirloomEntry const*> HeirloomItemsContainer;
typedef std::unordered_map<uint32 /*glyphPropertiesId*/, std::vector<uint32>> GlyphBindableSpellsContainer;
typedef std::unordered_map<uint32 /*glyphPropertiesId*/, std::vector<ChrSpecialization>> GlyphRequiredSpecsContainer;
typedef std::unordered_map<uint32 /*itemId*/, ItemChildEquipmentEntry const*> ItemChildEquipmentContainer;
typedef std::unordered_map<uint32, std::vector<ItemLimitCategoryConditionEntry const*>> ItemLimitCategoryConditionContainer;
typedef std::unordered_map<uint32 /*itemId | appearanceMod << 24*/, ItemModifiedAppearanceEntry const*> ItemModifiedAppearanceByItemContainer;
typedef std::unordered_map<uint32, std::vector<ItemSetSpellEntry const*>> ItemSetSpellContainer;
typedef std::unordered_map<uint32, std::unordered_map<uint32, MapDifficultyEntry const*>> MapDifficultyContainer;
typedef std::unordered_map<uint32, DB2Manager::MountTypeXCapabilitySet> MountCapabilitiesByTypeContainer;
typedef std::unordered_map<uint32, DB2Manager::MountXDisplayContainer> MountDisplaysCointainer;
typedef std::array<PowerTypeEntry const*, MAX_POWERS> PowerTypesContainer;
typedef std::unordered_map<uint32, std::pair<std::vector<QuestPackageItemEntry const*>, std::vector<QuestPackageItemEntry const*>>> QuestPackageItemContainer;
typedef std::unordered_map<uint32, std::vector<SpellPowerEntry const*>> SpellPowerContainer;
typedef std::unordered_map<uint32, std::unordered_map<uint32, std::vector<SpellPowerEntry const*>>> SpellPowerDifficultyContainer;
typedef std::unordered_set<uint32> ToyItemIdsContainer;
typedef std::pair<uint32 /*tableHash*/, int32 /*recordId*/> HotfixBlobKey;
typedef std::map<HotfixBlobKey, std::vector<uint8>> HotfixBlobMap;
using AllowedHotfixOptionalData = std::pair<uint32 /*optional data key*/, bool(*)(std::vector<uint8> const& data) /*validator*/>;

namespace
{
    StorageMap _stores;
    DB2Manager::HotfixContainer _hotfixData;
    std::array<HotfixBlobMap, TOTAL_LOCALES> _hotfixBlob;
    std::unordered_multimap<uint32 /*tableHash*/, AllowedHotfixOptionalData> _allowedHotfixOptionalData;
    std::array<std::map<HotfixBlobKey, std::vector<DB2Manager::HotfixOptionalData>>, TOTAL_LOCALES> _hotfixOptionalData;

    AreaGroupMemberContainer _areaGroupMembers;
    ArtifactPowersContainer _artifactPowers;
    ArtifactPowerLinksContainer _artifactPowerLinks;
    ArtifactPowerRanksContainer _artifactPowerRanks;
    std::unordered_map<uint32 /*itemId*/, AzeriteEmpoweredItemEntry const*> _azeriteEmpoweredItems;
    std::unordered_map<std::pair<uint32 /*azeriteEssenceId*/, uint32 /*rank*/>, AzeriteEssencePowerEntry const*> _azeriteEssencePowersByIdAndRank;
    std::vector<AzeriteItemMilestonePowerEntry const*> _azeriteItemMilestonePowers;
    std::array<AzeriteItemMilestonePowerEntry const*, MAX_AZERITE_ESSENCE_SLOT> _azeriteItemMilestonePowerByEssenceSlot;
    std::unordered_map<uint32 /*azeritePowerSetId*/, std::vector<AzeritePowerSetMemberEntry const*>> _azeritePowers;
    std::unordered_map<std::pair<uint32 /*azeriteUnlockSetId*/, ItemContext>, std::array<uint8, MAX_AZERITE_EMPOWERED_TIER>> _azeriteTierUnlockLevels;
    std::unordered_map<std::pair<int32 /*broadcastTextId*/, CascLocaleBit /*cascLocaleBit*/>, int32> _broadcastTextDurations;
    std::unordered_map<std::pair<uint8, uint8>, CharBaseInfoEntry const*> _charBaseInfoByRaceAndClass;
    std::array<ChrClassUIDisplayEntry const*, MAX_CLASSES> _uiDisplayByClass;
    std::array<std::array<uint32, MAX_POWERS>, MAX_CLASSES> _powersByClass;
    std::unordered_map<uint32 /*chrCustomizationOptionId*/, std::vector<ChrCustomizationChoiceEntry const*>> _chrCustomizationChoicesByOption;
    std::unordered_map<std::pair<uint8, uint8>, ChrModelEntry const*> _chrModelsByRaceAndGender;
    std::map<std::tuple<uint8 /*race*/, uint8/*gender*/, uint8/*shapeshift*/>, ShapeshiftFormModelData> _chrCustomizationChoicesForShapeshifts;
    std::unordered_map<std::pair<uint8 /*race*/, uint8/*gender*/>, std::vector<ChrCustomizationOptionEntry const*>> _chrCustomizationOptionsByRaceAndGender;
    std::unordered_map<uint32 /*chrCustomizationReqId*/, std::vector<std::pair<uint32 /*chrCustomizationOptionId*/, std::vector<uint32>>>> _chrCustomizationRequiredChoices;
    std::unordered_map<int32, ConditionalChrModelEntry const*> _conditionalChrModelsByChrModelId;
    std::unordered_map<uint32 /*contentTuningId*/, std::vector<ConditionalContentTuningEntry const*>> _conditionalContentTuning;
    std::unordered_set<std::pair<uint32, int32>> _contentTuningLabels;
    std::unordered_multimap<uint32, CurrencyContainerEntry const*> _currencyContainers;
    CurvePointsContainer _curvePoints;
    EmotesTextSoundContainer _emoteTextSounds;
    std::unordered_map<std::pair<uint32 /*level*/, int32 /*expansion*/>, ExpectedStatEntry const*> _expectedStatsByLevel;
    std::unordered_map<uint32 /*contentTuningId*/, std::vector<ContentTuningXExpectedEntry const*>> _expectedStatModsByContentTuning;
    HeirloomItemsContainer _heirlooms;
    GlyphBindableSpellsContainer _glyphBindableSpells;
    GlyphRequiredSpecsContainer _glyphRequiredSpecs;
    ItemChildEquipmentContainer _itemChildEquipment;
    std::unordered_set<uint32> _itemsWithCurrencyCost;
    ItemLimitCategoryConditionContainer _itemCategoryConditions;
    ItemModifiedAppearanceByItemContainer _itemModifiedAppearancesByItem;
    ItemSetSpellContainer _itemSetSpells;
    std::unordered_map<uint32, DB2Manager::MapDifficultyConditionsContainer> _mapDifficultyConditions;
    std::unordered_map<uint32, MountEntry const*> _mountsBySpellId;
    MountCapabilitiesByTypeContainer _mountCapabilitiesByType;
    MountDisplaysCointainer _mountDisplays;
    std::unordered_map<uint32, ParagonReputationEntry const*> _paragonReputations;
    std::unordered_map<uint32 /*pathID*/, PathDb2> _paths;
    PowerTypesContainer _powerTypes;
    std::unordered_map<uint32, uint8> _pvpItemBonus;
    PvpTalentSlotUnlockEntry const* _pvpTalentSlotUnlock[MAX_PVP_TALENT_SLOTS];
    std::unordered_map<uint32, std::vector<QuestLineXQuestEntry const*>> _questsByQuestLine;
    QuestPackageItemContainer _questPackages;
    std::unordered_map<uint32, std::vector<RewardPackXCurrencyTypeEntry const*>> _rewardPackCurrencyTypes;
    std::unordered_map<uint32, std::vector<RewardPackXItemEntry const*>> _rewardPackItems;
    std::unordered_map<uint32, std::vector<SkillLineAbilityEntry const*>> _skillLineAbilitiesBySkillupSkill;
    std::unordered_map<std::pair<int32, int32>, SoulbindConduitRankEntry const*> _soulbindConduitRanks;
    std::unordered_set<std::pair<int32, uint32>> _specsBySpecSet;
    std::unordered_map<int32, std::vector<SpellVisualMissileEntry const*>> _spellVisualMissilesBySet;
    ToyItemIdsContainer _toys;
    std::unordered_map<uint32, TransmogIllusionEntry const*> _transmogIllusionsByEnchantmentId;
    std::unordered_map<uint32, std::vector<TransmogSetEntry const*>> _transmogSetsByItemModifiedAppearance;
    std::unordered_map<uint32, std::vector<TransmogSetItemEntry const*>> _transmogSetItemsByTransmogSet;
    std::unordered_set<int32> _uiMapPhases;
    std::unordered_map<uint32, std::unordered_set<uint32>> _pvpStatIdsByMap;
}

void LoadDB2(std::bitset<TOTAL_LOCALES>& availableDb2Locales, std::vector<std::string>& errlist, StorageMap& stores, DB2StorageBase* storage, std::string const& db2Path,
    LocaleConstant defaultLocale, std::size_t cppRecordSize)
{
    // validate structure
    {
        DB2LoadInfo const* loadInfo = storage->GetLoadInfo();
        std::string clientMetaString, ourMetaString;
        for (std::size_t i = 0; i < loadInfo->Meta->FieldCount; ++i)
        {
            for (std::size_t j = 0; j < loadInfo->Meta->Fields[i].ArraySize; ++j)
            {
                if (i >= loadInfo->Meta->FileFieldCount && int32(i) == loadInfo->Meta->ParentIndexField)
                {
                    clientMetaString += char(FT_INT);
                    continue;
                }

                clientMetaString += char(loadInfo->Meta->Fields[i].Type);
            }
        }

        for (std::size_t i = loadInfo->Meta->HasIndexFieldInData() ? 0 : 1; i < loadInfo->FieldCount; ++i)
            ourMetaString += char(loadInfo->Fields[i].Type);

        ASSERT(clientMetaString == ourMetaString,
            "%s C++ structure fields %s do not match generated types from the client %s",
            storage->GetFileName().c_str(), ourMetaString.c_str(), clientMetaString.c_str());

        // compatibility format and C++ structure sizes
        ASSERT(loadInfo->Meta->GetRecordSize() == cppRecordSize,
            "Size of '%s' set by format string (%u) not equal size of C++ structure (" SZFMTD ").",
            storage->GetFileName().c_str(), loadInfo->Meta->GetRecordSize(), cppRecordSize);
    }

    try
    {
        storage->Load(db2Path + localeNames[defaultLocale] + '/', defaultLocale);
    }
    catch (std::system_error const& e)
    {
        if (e.code() == std::errc::no_such_file_or_directory)
        {
            errlist.push_back(Trinity::StringFormat("File {}{}/{} does not exist", db2Path, localeNames[defaultLocale], storage->GetFileName()));
        }
        else
            throw;
    }
    catch (std::exception const& e)
    {
        errlist.emplace_back(e.what());
        return;
    }

    // load additional data and enUS strings from db
    storage->LoadFromDB();

    for (LocaleConstant i = LOCALE_enUS; i < TOTAL_LOCALES; i = LocaleConstant(i + 1))
    {
        if (defaultLocale == i || !availableDb2Locales[i])
            continue;

        try
        {
            storage->LoadStringsFrom((db2Path + localeNames[i] + '/'), i);
        }
        catch (std::system_error const& e)
        {
            if (e.code() != std::errc::no_such_file_or_directory)
                throw;

            // locale db2 files are optional, do not error if nothing is found
        }
        catch (std::exception const& e)
        {
            errlist.emplace_back(e.what());
        }
    }

    for (LocaleConstant i = LOCALE_koKR; i < TOTAL_LOCALES; i = LocaleConstant(i + 1))
        if (availableDb2Locales[i])
            storage->LoadStringsFromDB(i);

    stores[storage->GetTableHash()] = storage;
}

DB2Manager& DB2Manager::Instance()
{
    static DB2Manager instance;
    return instance;
}

uint32 DB2Manager::LoadStores(std::string const& dataPath, LocaleConstant defaultLocale)
{
    uint32 oldMSTime = getMSTime();

    std::string db2Path = dataPath + "dbc/";

    std::vector<std::string> loadErrors;
    std::bitset<TOTAL_LOCALES> availableDb2Locales = [&]()
    {
        std::bitset<TOTAL_LOCALES> foundLocales;
        boost::filesystem::directory_iterator db2PathItr(db2Path), end;
        while (db2PathItr != end)
        {
            LocaleConstant locale = GetLocaleByName(db2PathItr->path().filename().string());
            if (IsValidLocale(locale) && (sWorld->getBoolConfig(CONFIG_LOAD_LOCALES) || locale == defaultLocale))
                foundLocales[locale] = true;

            ++db2PathItr;
        }
        return foundLocales;
    }();

    if (!availableDb2Locales[defaultLocale])
        return 0;

    auto LOAD_DB2 = [&]<typename T>(DB2Storage<T>& store)
    {
        LoadDB2(availableDb2Locales, loadErrors, _stores, &store, db2Path, defaultLocale, sizeof(T));
    };

//    LOAD_DB2(sAdventureJournalStore);
//    LOAD_DB2(sAdventureMapPOIStore);
//    LOAD_DB2(sArtifactStore);
//    LOAD_DB2(sArtifactAppearanceStore);
//    LOAD_DB2(sArtifactAppearanceSetStore);
//    LOAD_DB2(sArtifactCategoryStore);
//    LOAD_DB2(sArtifactPowerStore);
//    LOAD_DB2(sArtifactPowerLinkStore);
//    LOAD_DB2(sArtifactPowerPickerStore);
//    LOAD_DB2(sArtifactPowerRankStore);
//    LOAD_DB2(sArtifactTierStore);
//    LOAD_DB2(sArtifactUnlockStore);
//    LOAD_DB2(sAzeriteEmpoweredItemStore);
//    LOAD_DB2(sAzeriteEssenceStore);
//    LOAD_DB2(sAzeriteEssencePowerStore);
//    LOAD_DB2(sAzeriteItemStore);
//    LOAD_DB2(sAzeriteItemMilestonePowerStore);
//    LOAD_DB2(sAzeriteKnowledgeMultiplierStore);
//    LOAD_DB2(sAzeriteLevelInfoStore);
//    LOAD_DB2(sAzeritePowerStore);
//    LOAD_DB2(sAzeritePowerSetMemberStore);
//    LOAD_DB2(sAzeriteTierUnlockStore);
//    LOAD_DB2(sAzeriteTierUnlockSetStore);
//    LOAD_DB2(sAzeriteUnlockMappingStore);
//    LOAD_DB2(sBattlePetAbilityStore);
//    LOAD_DB2(sBattlePetBreedQualityStore);
//    LOAD_DB2(sBattlePetBreedStateStore);
//    LOAD_DB2(sBattlePetSpeciesStore);
//    LOAD_DB2(sBattlePetSpeciesStateStore);
//    LOAD_DB2(sBroadcastTextStore);
//    LOAD_DB2(sChallengeModeItemBonusOverrideStore);
//    LOAD_DB2(sChrClassUIDisplayStore);
//    LOAD_DB2(sChrCustomizationChoiceStore);
//    LOAD_DB2(sChrCustomizationDisplayInfoStore);
//    LOAD_DB2(sChrCustomizationElementStore);
//    LOAD_DB2(sChrCustomizationOptionStore);
//    LOAD_DB2(sChrCustomizationReqStore);
//    LOAD_DB2(sChrCustomizationReqChoiceStore);
//    LOAD_DB2(sChrModelStore);
//    LOAD_DB2(sChrRaceXChrModelStore);
//    LOAD_DB2(sConditionalChrModelStore);
//    LOAD_DB2(sConditionalContentTuningStore);
//    LOAD_DB2(sContentTuningStore);
//    LOAD_DB2(sContentTuningXExpectedStore);
//    LOAD_DB2(sContentTuningXLabelStore);
//    LOAD_DB2(sConversationLineStore);
//    LOAD_DB2(sCorruptionEffectsStore);
//    LOAD_DB2(sCurrencyContainerStore);
//    LOAD_DB2(sCurveStore);
//    LOAD_DB2(sCurvePointStore);
//    LOAD_DB2(sExpectedStatStore);
//    LOAD_DB2(sExpectedStatModStore);
//    LOAD_DB2(sGameObjectsStore);
//    LOAD_DB2(sGarrAbilityStore);
//    LOAD_DB2(sGarrBuildingStore);
//    LOAD_DB2(sGarrBuildingPlotInstStore);
//    LOAD_DB2(sGarrClassSpecStore);
//    LOAD_DB2(sGarrFollowerStore);
//    LOAD_DB2(sGarrFollowerXAbilityStore);
//    LOAD_DB2(sGarrMissionStore);
//    LOAD_DB2(sGarrPlotStore);
//    LOAD_DB2(sGarrPlotBuildingStore);
//    LOAD_DB2(sGarrPlotInstanceStore);
//    LOAD_DB2(sGarrSiteLevelStore);
//    LOAD_DB2(sGarrSiteLevelPlotInstStore);
//    LOAD_DB2(sGarrTalentTreeStore);
//    LOAD_DB2(sGlobalCurveStore);
//    LOAD_DB2(sGlyphBindableSpellStore);
//    LOAD_DB2(sGlyphRequiredSpecStore);
//    LOAD_DB2(sGossipNPCOptionStore);
//    LOAD_DB2(sHeirloomStore);
//    LOAD_DB2(sItemAppearanceStore);
//    LOAD_DB2(sItemBonusStore);
//    LOAD_DB2(sItemBonusListGroupEntryStore);
//    LOAD_DB2(sItemBonusListLevelDeltaStore);
//    LOAD_DB2(sItemBonusTreeStore);
//    LOAD_DB2(sItemBonusTreeNodeStore);
//    LOAD_DB2(sItemChildEquipmentStore);
//    LOAD_DB2(sItemContextPickerEntryStore);
//    LOAD_DB2(sItemCurrencyCostStore);
//    LOAD_DB2(sItemEffectStore);
//    LOAD_DB2(sItemStore);
//    LOAD_DB2(sItemExtendedCostStore);
//    LOAD_DB2(sItemLevelSelectorStore);
//    LOAD_DB2(sItemLevelSelectorQualityStore);
//    LOAD_DB2(sItemLevelSelectorQualitySetStore);
//    LOAD_DB2(sItemLimitCategoryConditionStore);
//    LOAD_DB2(sItemModifiedAppearanceStore);
//    LOAD_DB2(sItemModifiedAppearanceExtraStore);
//    LOAD_DB2(sItemSearchNameStore);
//    LOAD_DB2(sItemSetSpellStore);
//    LOAD_DB2(sItemSparseStore);
//    LOAD_DB2(sItemXBonusTreeStore);
//    LOAD_DB2(sItemXItemEffectStore);
//    LOAD_DB2(sKeychainStore);
//    LOAD_DB2(sKeystoneAffixStore);
//    LOAD_DB2(sLocationStore);
//    LOAD_DB2(sMapChallengeModeStore);
//    LOAD_DB2(sMapDifficultyXConditionStore);
//    LOAD_DB2(sMawPowerStore);
//    LOAD_DB2(sMountStore);
//    LOAD_DB2(sMountTypeXCapabilityStore);
//    LOAD_DB2(sMountXDisplayStore);
//    LOAD_DB2(sMythicPlusSeasonStore);
//    LOAD_DB2(sNumTalentsAtLevelStore);
//    LOAD_DB2(sParagonReputationStore);
//    LOAD_DB2(sPathStore);
//    LOAD_DB2(sPathNodeStore);
//    LOAD_DB2(sPathPropertyStore);
//    LOAD_DB2(sPowerTypeStore);
//    LOAD_DB2(sPrestigeLevelInfoStore);
//    LOAD_DB2(sPVPDifficultyStore);
//    LOAD_DB2(sPVPItemStore);
//    LOAD_DB2(sPVPStatStore);
//    LOAD_DB2(sPvpSeasonStore);
//    LOAD_DB2(sPvpTalentStore);
//    LOAD_DB2(sPvpTalentCategoryStore);
//    LOAD_DB2(sPvpTalentSlotUnlockStore);
//    LOAD_DB2(sPvpTierStore);
//    LOAD_DB2(sQuestLineXQuestStore);
//    LOAD_DB2(sQuestPackageItemStore);
//    LOAD_DB2(sRewardPackStore);
//    LOAD_DB2(sRewardPackXCurrencyTypeStore);
//    LOAD_DB2(sRewardPackXItemStore);
//    LOAD_DB2(sSceneScriptStore);
//    LOAD_DB2(sSceneScriptGlobalTextStore);
//    LOAD_DB2(sSceneScriptPackageStore);
//    LOAD_DB2(sSceneScriptTextStore);
//    LOAD_DB2(sSkillLineXTraitTreeStore);
//    LOAD_DB2(sSoulbindConduitRankStore);
//    LOAD_DB2(sSoundKitStore);
//    LOAD_DB2(sSpecSetMemberStore);
//    LOAD_DB2(sSpellEmpowerStore);
//    LOAD_DB2(sSpellEmpowerStageStore);
//    LOAD_DB2(sSpellLabelStore);
//    LOAD_DB2(sSpellNameStore);
//    LOAD_DB2(sSpellPowerDifficultyStore);
//    LOAD_DB2(sSpellReagentsStore);
//    LOAD_DB2(sSpellReagentsCurrencyStore);
//    LOAD_DB2(sSpellVisualStore);
//    LOAD_DB2(sSpellVisualEffectNameStore);
//    LOAD_DB2(sSpellVisualMissileStore);
//    LOAD_DB2(sSpellVisualKitStore);
//    LOAD_DB2(sSpellXSpellVisualStore);
//    LOAD_DB2(sTactKeyStore);
//    LOAD_DB2(sToyStore);
//    LOAD_DB2(sTraitCondStore);
//    LOAD_DB2(sTraitCostStore);
//    LOAD_DB2(sTraitCurrencyStore);
//    LOAD_DB2(sTraitCurrencySourceStore);
//    LOAD_DB2(sTraitDefinitionStore);
//    LOAD_DB2(sTraitDefinitionEffectPointsStore);
//    LOAD_DB2(sTraitEdgeStore);
//    LOAD_DB2(sTraitNodeStore);
//    LOAD_DB2(sTraitNodeEntryStore);
//    LOAD_DB2(sTraitNodeEntryXTraitCondStore);
//    LOAD_DB2(sTraitNodeEntryXTraitCostStore);
//    LOAD_DB2(sTraitNodeGroupStore);
//    LOAD_DB2(sTraitNodeGroupXTraitCondStore);
//    LOAD_DB2(sTraitNodeGroupXTraitCostStore);
//    LOAD_DB2(sTraitNodeGroupXTraitNodeStore);
//    LOAD_DB2(sTraitNodeXTraitCondStore);
//    LOAD_DB2(sTraitNodeXTraitCostStore);
//    LOAD_DB2(sTraitNodeXTraitNodeEntryStore);
//    LOAD_DB2(sTraitTreeStore);
//    LOAD_DB2(sTraitTreeLoadoutStore);
//    LOAD_DB2(sTraitTreeLoadoutEntryStore);
//    LOAD_DB2(sTraitTreeXTraitCostStore);
//    LOAD_DB2(sTraitTreeXTraitCurrencyStore);
//    LOAD_DB2(sTransmogHolidayStore);
//    LOAD_DB2(sTransmogIllusionStore);
//    LOAD_DB2(sTransmogSetStore);
//    LOAD_DB2(sTransmogSetGroupStore);
//    LOAD_DB2(sTransmogSetItemStore);
//    LOAD_DB2(sUiMapStore);
//    LOAD_DB2(sUiMapAssignmentStore);
//    LOAD_DB2(sUiMapLinkStore);
//    LOAD_DB2(sUiMapXMapArtStore);
//    LOAD_DB2(sUISplashScreenStore);
//    LOAD_DB2(sVignetteStore);

    // error checks
    if (!loadErrors.empty())
    {
        sLog->SetSynchronous(); // server will shut down after this, so set sync logging to prevent messages from getting lost

        for (std::string const& error : loadErrors)
            TC_LOG_FATAL("misc", "{}", error);

        return 0;
    }

    // TODO: DATA, move these checks to a better spot
//    // Check loaded DB2 files proper version
//    if (!sAreaTableStore.LookupEntry(15151) ||               // last area added in 10.2.5 (53007)
//        !sCharTitlesStore.LookupEntry(805) ||                // last char title added in 10.2.5 (53007)
//        !sGemPropertiesStore.LookupEntry(4081) ||            // last gem property added in 10.2.5 (53007)
//        !sItemStore.LookupEntry(215160) ||                   // last item added in 10.2.5 (53007)
//        !sItemExtendedCostStore.LookupEntry(8510) ||         // last item extended cost added in 10.2.5 (53007)
//        !sMapStore.LookupEntry(2708) ||                      // last map added in 10.2.5 (53007)
//        !sSpellNameStore.LookupEntry(438878))                // last spell added in 10.2.5 (53007)
//    {
//        TC_LOG_FATAL("misc", "You have _outdated_ DB2 files. Please extract correct versions from current using client.");
//        return 0;
//    }
//
//    for (AreaGroupMemberEntry const* areaGroupMember : sAreaGroupMemberStore)
//        _areaGroupMembers[areaGroupMember->AreaGroupID].push_back(areaGroupMember->AreaID);
//
//    for (AreaTableEntry const* areaTable : sAreaTableStore)
//    {
//        ASSERT(areaTable->AreaBit <= 0 || (size_t(areaTable->AreaBit / 64) < PLAYER_EXPLORED_ZONES_SIZE),
//            "PLAYER_EXPLORED_ZONES_SIZE must be at least %d", (areaTable->AreaBit + 63) / 64);
//    }

    for (ArtifactPowerEntry const* artifactPower : sArtifactPowerStore)
        _artifactPowers[artifactPower->ArtifactID].push_back(artifactPower);

    for (ArtifactPowerLinkEntry const* artifactPowerLink : sArtifactPowerLinkStore)
    {
        _artifactPowerLinks[artifactPowerLink->PowerA].push_back(artifactPowerLink->PowerB);
        _artifactPowerLinks[artifactPowerLink->PowerB].push_back(artifactPowerLink->PowerA);
    }

    for (ArtifactPowerRankEntry const* artifactPowerRank : sArtifactPowerRankStore)
        _artifactPowerRanks[std::pair<uint32, uint8>{ artifactPowerRank->ArtifactPowerID, artifactPowerRank->RankIndex }] = artifactPowerRank;

    for (AzeriteEmpoweredItemEntry const* azeriteEmpoweredItem : sAzeriteEmpoweredItemStore)
        _azeriteEmpoweredItems[azeriteEmpoweredItem->ItemID] = azeriteEmpoweredItem;

    for (AzeriteEssencePowerEntry const* azeriteEssencePower : sAzeriteEssencePowerStore)
        _azeriteEssencePowersByIdAndRank[std::pair<uint32, uint32>{ azeriteEssencePower->AzeriteEssenceID, azeriteEssencePower->Tier }] = azeriteEssencePower;

    for (AzeriteItemMilestonePowerEntry const* azeriteItemMilestonePower : sAzeriteItemMilestonePowerStore)
        _azeriteItemMilestonePowers.push_back(azeriteItemMilestonePower);

    std::sort(_azeriteItemMilestonePowers.begin(), _azeriteItemMilestonePowers.end(), [](AzeriteItemMilestonePowerEntry const* a1, AzeriteItemMilestonePowerEntry const* a2)
    {
        return a1->RequiredLevel < a2->RequiredLevel;
    });

    {
        uint32 azeriteEssenceSlot = 0;
        for (AzeriteItemMilestonePowerEntry const* azeriteItemMilestonePower : _azeriteItemMilestonePowers)
        {
            AzeriteItemMilestoneType type = AzeriteItemMilestoneType(azeriteItemMilestonePower->Type);
            if (type == AzeriteItemMilestoneType::MajorEssence || type == AzeriteItemMilestoneType::MinorEssence)
            {
                ASSERT(azeriteEssenceSlot < MAX_AZERITE_ESSENCE_SLOT);
                _azeriteItemMilestonePowerByEssenceSlot[azeriteEssenceSlot] = azeriteItemMilestonePower;
                ++azeriteEssenceSlot;
            }
        }
    }

    for (AzeritePowerSetMemberEntry const* azeritePowerSetMember : sAzeritePowerSetMemberStore)
        if (sAzeritePowerStore.LookupEntry(azeritePowerSetMember->AzeritePowerID))
            _azeritePowers[azeritePowerSetMember->AzeritePowerSetID].push_back(azeritePowerSetMember);

    for (AzeriteTierUnlockEntry const* azeriteTierUnlock : sAzeriteTierUnlockStore)
        _azeriteTierUnlockLevels[std::pair<uint32, ItemContext>{ azeriteTierUnlock->AzeriteTierUnlockSetID, ItemContext(azeriteTierUnlock->ItemCreationContext) }][azeriteTierUnlock->Tier] = azeriteTierUnlock->AzeriteLevel;

    // TODO: DATA rework these
//    _broadcastTextDurations.reserve(sBroadcastTextDurationStore.GetNumRows());
//    for (BroadcastTextDurationEntry const* broadcastTextDuration : sBroadcastTextDurationStore)
//        _broadcastTextDurations[{ broadcastTextDuration->BroadcastTextID, CascLocaleBit(broadcastTextDuration->Locale) }] = broadcastTextDuration->Duration;
//
//    for (CharBaseInfoEntry const* charBaseInfo : sCharBaseInfoStore)
//        _charBaseInfoByRaceAndClass[{ charBaseInfo->RaceID, charBaseInfo->ClassID }] = charBaseInfo;

    for (ChrClassUIDisplayEntry const* uiDisplay : sChrClassUIDisplayStore)
    {
        ASSERT(uiDisplay->ChrClassesID < MAX_CLASSES);
        _uiDisplayByClass[uiDisplay->ChrClassesID] = uiDisplay;
    }

    for (ChrCustomizationChoiceEntry const* customizationChoice : sChrCustomizationChoiceStore)
        _chrCustomizationChoicesByOption[customizationChoice->ChrCustomizationOptionID].push_back(customizationChoice);

    std::unordered_multimap<uint32, std::pair<uint32, uint8>> shapeshiftFormByModel;
    std::unordered_map<uint32, ChrCustomizationDisplayInfoEntry const*> displayInfoByCustomizationChoice;

    // build shapeshift form model lookup
    for (ChrCustomizationElementEntry const* customizationElement : sChrCustomizationElementStore)
    {
        if (ChrCustomizationDisplayInfoEntry const* customizationDisplayInfo = sChrCustomizationDisplayInfoStore.LookupEntry(customizationElement->ChrCustomizationDisplayInfoID))
        {
            if (ChrCustomizationChoiceEntry const* customizationChoice = sChrCustomizationChoiceStore.LookupEntry(customizationElement->ChrCustomizationChoiceID))
            {
                displayInfoByCustomizationChoice[customizationElement->ChrCustomizationChoiceID] = customizationDisplayInfo;
                if (ChrCustomizationOptionEntry const* customizationOption = sChrCustomizationOptionStore.LookupEntry(customizationChoice->ChrCustomizationOptionID))
                    shapeshiftFormByModel.emplace(customizationOption->ChrModelID, std::make_pair(customizationOption->ID, uint8(customizationDisplayInfo->ShapeshiftFormID)));
            }
        }
    }

    std::unordered_map<uint32, std::vector<ChrCustomizationOptionEntry const*>> customizationOptionsByModel;
    for (ChrCustomizationOptionEntry const* customizationOption : sChrCustomizationOptionStore)
        customizationOptionsByModel[customizationOption->ChrModelID].push_back(customizationOption);

    for (ChrCustomizationReqChoiceEntry const* reqChoice : sChrCustomizationReqChoiceStore)
    {
        if (ChrCustomizationChoiceEntry const* customizationChoice = sChrCustomizationChoiceStore.LookupEntry(reqChoice->ChrCustomizationChoiceID))
        {
            std::vector<std::pair<uint32, std::vector<uint32>>>& requiredChoicesForReq = _chrCustomizationRequiredChoices[reqChoice->ChrCustomizationReqID];
            std::vector<uint32>* choices = nullptr;
            for (std::pair<uint32, std::vector<uint32>>& choicesForOption : requiredChoicesForReq)
            {
                if (int32(choicesForOption.first) == customizationChoice->ChrCustomizationOptionID)
                {
                    choices = &choicesForOption.second;
                    break;
                }
            }
            if (!choices)
            {
                std::pair<uint32, std::vector<uint32>>& choicesForReq = requiredChoicesForReq.emplace_back();
                choicesForReq.first = customizationChoice->ChrCustomizationOptionID;
                choices = &choicesForReq.second;
            }
            choices->push_back(reqChoice->ChrCustomizationChoiceID);
        }
    }

    for (ConditionalChrModelEntry const* conditionalChrModel : sConditionalChrModelStore)
        _conditionalChrModelsByChrModelId[conditionalChrModel->ChrModelID] = conditionalChrModel;

    {
        for (ConditionalContentTuningEntry const* conditionalContentTuning : sConditionalContentTuningStore)
            _conditionalContentTuning[conditionalContentTuning->ParentContentTuningID].push_back(conditionalContentTuning);

        for (auto& [parentContentTuningId, conditionalContentTunings] : _conditionalContentTuning)
            std::ranges::sort(conditionalContentTunings, std::greater(), &ConditionalContentTuningEntry::OrderIndex);
    }

    for (ContentTuningXExpectedEntry const* contentTuningXExpectedStat : sContentTuningXExpectedStore)
        if (sExpectedStatModStore.LookupEntry(contentTuningXExpectedStat->ExpectedStatModID))
            _expectedStatModsByContentTuning[contentTuningXExpectedStat->ContentTuningID].push_back(contentTuningXExpectedStat);

    for (ContentTuningXLabelEntry const* contentTuningXLabel : sContentTuningXLabelStore)
        _contentTuningLabels.emplace(contentTuningXLabel->ContentTuningID, contentTuningXLabel->LabelID);

    for (CurrencyContainerEntry const* currencyContainer : sCurrencyContainerStore)
        _currencyContainers.emplace(currencyContainer->CurrencyTypesID, currencyContainer);

    {
        std::unordered_map<uint32 /*curveID*/, std::vector<CurvePointEntry const*>> unsortedPoints;
        for (CurvePointEntry const* curvePoint : sCurvePointStore)
            if (sCurveStore.LookupEntry(curvePoint->CurveID))
                unsortedPoints[curvePoint->CurveID].push_back(curvePoint);

        for (auto& [curveId, curvePoints] : unsortedPoints)
        {
            std::sort(curvePoints.begin(), curvePoints.end(), [](CurvePointEntry const* point1, CurvePointEntry const* point2) { return point1->OrderIndex < point2->OrderIndex; });
            std::vector<DBCPosition2D>& points = _curvePoints[curveId];
            points.resize(curvePoints.size());
            std::transform(curvePoints.begin(), curvePoints.end(), points.begin(), [](CurvePointEntry const* point) { return point->Pos; });
        }
    }

    // TODO: DATA
//    for (EmotesTextSoundEntry const* emoteTextSound : sEmotesTextSoundStore)
//        _emoteTextSounds[EmotesTextSoundContainer::key_type(emoteTextSound->EmotesTextID, emoteTextSound->RaceID, emoteTextSound->SexID, emoteTextSound->ClassID)] = emoteTextSound;

    for (ExpectedStatEntry const* expectedStat : sExpectedStatStore)
        _expectedStatsByLevel[std::make_pair(expectedStat->Lvl, expectedStat->ExpansionID)] = expectedStat;

    for (HeirloomEntry const* heirloom : sHeirloomStore)
        _heirlooms[heirloom->ItemID] = heirloom;

    for (GlyphBindableSpellEntry const* glyphBindableSpell : sGlyphBindableSpellStore)
        _glyphBindableSpells[glyphBindableSpell->GlyphPropertiesID].push_back(glyphBindableSpell->SpellID);

    for (GlyphRequiredSpecEntry const* glyphRequiredSpec : sGlyphRequiredSpecStore)
        _glyphRequiredSpecs[glyphRequiredSpec->GlyphPropertiesID].push_back(ChrSpecialization(glyphRequiredSpec->ChrSpecializationID));

    for (ItemChildEquipmentEntry const* itemChildEquipment : sItemChildEquipmentStore)
    {
        ASSERT(_itemChildEquipment.find(itemChildEquipment->ParentItemID) == _itemChildEquipment.end(), "Item must have max 1 child item.");
        _itemChildEquipment[itemChildEquipment->ParentItemID] = itemChildEquipment;
    }

    for (ItemCurrencyCostEntry const* itemCurrencyCost : sItemCurrencyCostStore)
        _itemsWithCurrencyCost.insert(itemCurrencyCost->ItemID);

    for (ItemLimitCategoryConditionEntry const* condition : sItemLimitCategoryConditionStore)
        _itemCategoryConditions[condition->ParentItemLimitCategoryID].push_back(condition);

    for (ItemModifiedAppearanceEntry const* appearanceMod : sItemModifiedAppearanceStore)
    {
        ASSERT(appearanceMod->ItemID <= 0xFFFFFF);
        _itemModifiedAppearancesByItem[appearanceMod->ItemID | (appearanceMod->ItemAppearanceModifierID << 24)] = appearanceMod;
    }

    for (ItemSetSpellEntry const* itemSetSpell : sItemSetSpellStore)
        _itemSetSpells[itemSetSpell->ItemSetID].push_back(itemSetSpell);

    std::vector<MapDifficultyXConditionEntry const*> mapDifficultyConditions;
    mapDifficultyConditions.reserve(sMapDifficultyXConditionStore.GetNumRows());
    for (MapDifficultyXConditionEntry const* mapDifficultyCondition : sMapDifficultyXConditionStore)
        mapDifficultyConditions.push_back(mapDifficultyCondition);

    std::sort(mapDifficultyConditions.begin(), mapDifficultyConditions.end(), [](MapDifficultyXConditionEntry const* left, MapDifficultyXConditionEntry const* right)
    {
        return left->OrderIndex < right->OrderIndex;
    });

    // Shaohao: no MapDifficultyConditions.db2 (or dbc)
//    for (MapDifficultyXConditionEntry const* mapDifficultyCondition : mapDifficultyConditions)
//        if (PlayerConditionEntry const* playerCondition = sPlayerConditionStore.LookupEntry(mapDifficultyCondition->PlayerConditionID))
//            _mapDifficultyConditions[mapDifficultyCondition->MapDifficultyID].emplace_back(mapDifficultyCondition->ID, playerCondition);

    for (MountEntry const* mount : sMountStore)
        _mountsBySpellId[mount->SourceSpellID] = mount;

    for (MountTypeXCapabilityEntry const* mountTypeCapability : sMountTypeXCapabilityStore)
        _mountCapabilitiesByType[mountTypeCapability->MountTypeID].insert(mountTypeCapability);

    for (MountXDisplayEntry const* mountDisplay : sMountXDisplayStore)
        _mountDisplays[mountDisplay->MountID].push_back(mountDisplay);

    // Shaohao: MOP doesn't have paragon reputation
//    for (ParagonReputationEntry const* paragonReputation : sParagonReputationStore)
//        if (sFactionStore.HasRecord(paragonReputation->FactionID))
//            _paragonReputations[paragonReputation->FactionID] = paragonReputation;

    {
        std::unordered_map<uint32 /*pathID*/, std::vector<PathNodeEntry const*>> unsortedNodes;
        for (PathNodeEntry const* pathNode : sPathNodeStore)
            if (sPathStore.HasRecord(pathNode->PathID) && sLocationStore.HasRecord(pathNode->LocationID))
                unsortedNodes[pathNode->PathID].push_back(pathNode);

        for (auto&& [pathId, pathNodes] : unsortedNodes)
        {
            PathDb2& path = _paths[pathId];

            path.Locations.resize(pathNodes.size());
            std::ranges::sort(pathNodes, std::ranges::less(), &PathNodeEntry::Sequence);
            std::ranges::transform(pathNodes, path.Locations.begin(), [](PathNodeEntry const* node)
            {
                return sLocationStore.AssertEntry(node->LocationID)->Pos;
            });
        }

        for (PathPropertyEntry const* pathProperty : sPathPropertyStore)
            if (sPathStore.HasRecord(pathProperty->PathID))
                _paths[pathProperty->PathID].Properties.push_back(pathProperty);
    }

    for (PowerTypeEntry const* powerType : sPowerTypeStore)
    {
        ASSERT(powerType->PowerTypeEnum < MAX_POWERS);
        ASSERT(!_powerTypes[powerType->PowerTypeEnum]);

        _powerTypes[powerType->PowerTypeEnum] = powerType;
    }

    for (PVPDifficultyEntry const* entry : sPVPDifficultyStore)
    {
        ASSERT(entry->RangeIndex < MAX_BATTLEGROUND_BRACKETS, "PvpDifficulty bracket (%d) exceeded max allowed value (%d)", entry->RangeIndex, MAX_BATTLEGROUND_BRACKETS);
    }

    for (PVPItemEntry const* pvpItem : sPVPItemStore)
        _pvpItemBonus[pvpItem->ItemID] = pvpItem->ItemLevelDelta;

    for (PvpTalentSlotUnlockEntry const* talentUnlock : sPvpTalentSlotUnlockStore)
    {
        ASSERT(talentUnlock->Slot < (1 << MAX_PVP_TALENT_SLOTS));
        for (int8 i = 0; i < MAX_PVP_TALENT_SLOTS; ++i)
        {
            if (talentUnlock->Slot & (1 << i))
            {
                ASSERT(!_pvpTalentSlotUnlock[i]);
                _pvpTalentSlotUnlock[i] = talentUnlock;
            }
        }
    }

    for (QuestLineXQuestEntry const* questLineQuest : sQuestLineXQuestStore)
        _questsByQuestLine[questLineQuest->QuestLineID].push_back(questLineQuest);

    for (QuestPackageItemEntry const* questPackageItem : sQuestPackageItemStore)
    {
        if (questPackageItem->DisplayType != QUEST_PACKAGE_FILTER_UNMATCHED)
            _questPackages[questPackageItem->PackageID].first.push_back(questPackageItem);
        else
            _questPackages[questPackageItem->PackageID].second.push_back(questPackageItem);
    }

    for (RewardPackXCurrencyTypeEntry const* rewardPackXCurrencyType : sRewardPackXCurrencyTypeStore)
        _rewardPackCurrencyTypes[rewardPackXCurrencyType->RewardPackID].push_back(rewardPackXCurrencyType);

    for (RewardPackXItemEntry const* rewardPackXItem : sRewardPackXItemStore)
        _rewardPackItems[rewardPackXItem->RewardPackID].push_back(rewardPackXItem);

    // TODO: DATA MOP doesn't have SkillupSkillLineID
//    for (SkillLineAbilityEntry const* skillLineAbility : sSkillLineAbilityStore)
//        _skillLineAbilitiesBySkillupSkill[skillLineAbility->SkillupSkillLineID ? skillLineAbility->SkillupSkillLineID : skillLineAbility->SkillLine].push_back(skillLineAbility);

    for (SoulbindConduitRankEntry const* soulbindConduitRank : sSoulbindConduitRankStore)
        _soulbindConduitRanks[{ soulbindConduitRank->SoulbindConduitID, soulbindConduitRank->RankIndex }] = soulbindConduitRank;

    for (SpecSetMemberEntry const* specSetMember : sSpecSetMemberStore)
        _specsBySpecSet.insert(std::make_pair(specSetMember->SpecSetID, uint32(specSetMember->ChrSpecializationID)));

    // Shaohao: SpellNames.db2 doesn't exist, so fake something for compatibility
    for (auto spell : sSpellStore)
    {
        SpellNameEntry spellName = SpellNameEntry();
        spellName.ID = spell->ID;
        spellName.Name = LocalizedString();
        spellName.Name.Str[defaultLocale] = spell->Name;
        sSpellNameStore.emplace(spellName.ID, &spellName);
    }

    for (SpellVisualMissileEntry const* spellVisualMissile : sSpellVisualMissileStore)
        _spellVisualMissilesBySet[spellVisualMissile->SpellVisualMissileSetID].push_back(spellVisualMissile);

    for (ToyEntry const* toy : sToyStore)
        _toys.insert(toy->ItemID);

    for (TransmogIllusionEntry const* transmogIllusion : sTransmogIllusionStore)
        _transmogIllusionsByEnchantmentId[transmogIllusion->SpellItemEnchantmentID] = transmogIllusion;

    for (TransmogSetItemEntry const* transmogSetItem : sTransmogSetItemStore)
    {
        TransmogSetEntry const* set = sTransmogSetStore.LookupEntry(transmogSetItem->TransmogSetID);
        if (!set)
            continue;

        _transmogSetsByItemModifiedAppearance[transmogSetItem->ItemModifiedAppearanceID].push_back(set);
        _transmogSetItemsByTransmogSet[transmogSetItem->TransmogSetID].push_back(transmogSetItem);
    }

    for (UiMapXMapArtEntry const* uiMapArt : sUiMapXMapArtStore)
        if (uiMapArt->PhaseID)
            _uiMapPhases.insert(uiMapArt->PhaseID);

    for (PVPStatEntry const* pvpStat : sPVPStatStore)
        _pvpStatIdsByMap[pvpStat->MapID].insert(pvpStat->ID);

    TC_LOG_INFO("server.loading", ">> Initialized {} DB2 data stores in {} ms", _stores.size(), GetMSTimeDiffToNow(oldMSTime));

    return availableDb2Locales.to_ulong();
}

DB2StorageBase const* DB2Manager::GetStorage(uint32 type) const
{
    auto itr = _stores.find(type);
    if (itr != _stores.end())
        return itr->second;

    return nullptr;
}

void DB2Manager::LoadHotfixData(uint32 localeMask)
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = HotfixDatabase.Query("SELECT Id, UniqueId, TableHash, RecordId, Status FROM hotfix_data ORDER BY Id");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 hotfix info entries.");
        return;
    }

    uint32 count = 0;

    std::map<std::pair<uint32, int32>, bool> deletedRecords;

    do
    {
        Field* fields = result->Fetch();

        int32 id = fields[0].GetInt32();
        uint32 uniqueId = fields[1].GetUInt32();
        uint32 tableHash = fields[2].GetUInt32();
        int32 recordId = fields[3].GetInt32();
        HotfixRecord::Status status = static_cast<HotfixRecord::Status>(fields[4].GetUInt8());
        std::bitset<TOTAL_LOCALES> availableDb2Locales = localeMask;
        if (status == HotfixRecord::Status::Valid && !_stores.contains(tableHash))
        {
            HotfixBlobKey key = std::make_pair(tableHash, recordId);
            for (std::size_t locale = 0; locale < TOTAL_LOCALES; ++locale)
            {
                if (!availableDb2Locales[locale])
                    continue;

                if (!_hotfixBlob[locale].contains(key))
                    availableDb2Locales[locale] = false;
            }

            if (availableDb2Locales.none())
            {
                TC_LOG_ERROR("sql.sql", "Table `hotfix_data` references unknown DB2 store by hash 0x{:X} and has no reference to `hotfix_blob` in hotfix id {} with RecordID: {}", tableHash, id, recordId);
                continue;
            }
        }

        HotfixRecord hotfixRecord;
        hotfixRecord.TableHash = tableHash;
        hotfixRecord.RecordID = recordId;
        hotfixRecord.ID.PushID = id;
        hotfixRecord.ID.UniqueID = uniqueId;
        hotfixRecord.HotfixStatus = status;
        hotfixRecord.AvailableLocalesMask = availableDb2Locales.to_ulong();

        HotfixPush& push = _hotfixData[id];
        push.Records.push_back(hotfixRecord);
        push.AvailableLocalesMask |= hotfixRecord.AvailableLocalesMask;

        _maxHotfixId = std::max(_maxHotfixId, id);
        deletedRecords[std::make_pair(tableHash, recordId)] = status == HotfixRecord::Status::RecordRemoved;
        ++count;
    } while (result->NextRow());

    for (auto itr = deletedRecords.begin(); itr != deletedRecords.end(); ++itr)
        if (itr->second)
            if (DB2StorageBase* store = Trinity::Containers::MapGetValuePtr(_stores, itr->first.first))
                store->EraseRecord(itr->first.second);

    TC_LOG_INFO("server.loading", ">> Loaded {} hotfix records in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
}

void DB2Manager::LoadHotfixBlob(uint32 localeMask)
{
    uint32 oldMSTime = getMSTime();

    QueryResult result = HotfixDatabase.Query("SELECT TableHash, RecordId, locale, `Blob` FROM hotfix_blob ORDER BY TableHash");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 hotfix blob entries.");
        return;
    }

    std::bitset<TOTAL_LOCALES> availableDb2Locales = localeMask;
    uint32 hotfixBlobCount = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 tableHash = fields[0].GetUInt32();
        auto storeItr = _stores.find(tableHash);
        if (storeItr != _stores.end())
        {
            TC_LOG_ERROR("sql.sql", "Table hash 0x{:X} points to a loaded DB2 store {}, fill related table instead of hotfix_blob",
                tableHash, storeItr->second->GetFileName());
            continue;
        }

        int32 recordId = fields[1].GetInt32();
        std::string localeName = fields[2].GetString();
        LocaleConstant locale = GetLocaleByName(localeName);

        if (!IsValidLocale(locale))
        {
            TC_LOG_ERROR("sql.sql", "`hotfix_blob` contains invalid locale: {} at TableHash: 0x{:X} and RecordID: {}", localeName, tableHash, recordId);
            continue;
        }

        if (!availableDb2Locales[locale])
            continue;

        _hotfixBlob[locale][std::make_pair(tableHash, recordId)] = fields[3].GetBinary();
        hotfixBlobCount++;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded {} hotfix blob records in {} ms", hotfixBlobCount, GetMSTimeDiffToNow(oldMSTime));
}

bool ValidateBroadcastTextTactKeyOptionalData(std::vector<uint8> const& data)
{
    return data.size() == 8 + 16;
}

void DB2Manager::LoadHotfixOptionalData(uint32 localeMask)
{
    // Register allowed optional data keys
    _allowedHotfixOptionalData.emplace(sBroadcastTextStore.GetTableHash(), std::make_pair(sTactKeyStore.GetTableHash(), &ValidateBroadcastTextTactKeyOptionalData));

    uint32 oldMSTime = getMSTime();

    QueryResult result = HotfixDatabase.Query("SELECT TableHash, RecordId, locale, `Key`, `Data` FROM hotfix_optional_data ORDER BY TableHash");

    if (!result)
    {
        TC_LOG_INFO("server.loading", ">> Loaded 0 hotfix optional data records.");
        return;
    }

    std::bitset<TOTAL_LOCALES> availableDb2Locales = localeMask;
    uint32 hotfixOptionalDataCount = 0;
    do
    {
        Field* fields = result->Fetch();

        uint32 tableHash = fields[0].GetUInt32();
        auto allowedHotfixes = Trinity::Containers::MapEqualRange(_allowedHotfixOptionalData, tableHash);
        if (allowedHotfixes.begin() == allowedHotfixes.end())
        {
            TC_LOG_ERROR("sql.sql", "Table `hotfix_optional_data` references DB2 store by hash 0x{:X} that is not allowed to have optional data", tableHash);
            continue;
        }

        uint32 recordId = fields[1].GetInt32();
        auto storeItr = _stores.find(tableHash);
        if (storeItr == _stores.end())
        {
            TC_LOG_ERROR("sql.sql", "Table `hotfix_optional_data` references unknown DB2 store by hash 0x{:X} with RecordID: {}", tableHash, recordId);
            continue;
        }

        std::string localeName = fields[2].GetString();
        LocaleConstant locale = GetLocaleByName(localeName);

        if (!IsValidLocale(locale))
        {
            TC_LOG_ERROR("sql.sql", "`hotfix_optional_data` contains invalid locale: {} at TableHash: 0x{:X} and RecordID: {}", localeName, tableHash, recordId);
            continue;
        }

        if (!availableDb2Locales[locale])
            continue;

        DB2Manager::HotfixOptionalData optionalData;
        optionalData.Key = fields[3].GetUInt32();
        auto allowedHotfixItr = std::find_if(allowedHotfixes.begin(), allowedHotfixes.end(), [&](std::pair<uint32 const, AllowedHotfixOptionalData> const& v)
        {
            return v.second.first == optionalData.Key;
        });
        if (allowedHotfixItr == allowedHotfixes.end())
        {
            TC_LOG_ERROR("sql.sql", "Table `hotfix_optional_data` references non-allowed optional data key 0x{:X} for DB2 store by hash 0x{:X} and RecordID: {}",
                optionalData.Key, tableHash, recordId);
            continue;
        }

        optionalData.Data = fields[4].GetBinary();
        if (!allowedHotfixItr->second.second(optionalData.Data))
        {
            TC_LOG_ERROR("sql.sql", "Table `hotfix_optional_data` contains invalid data for DB2 store 0x{:X}, RecordID: {} and Key: 0x{:X}",
                tableHash, recordId, optionalData.Key);
            continue;
        }

        _hotfixOptionalData[locale][std::make_pair(tableHash, recordId)].push_back(std::move(optionalData));
        hotfixOptionalDataCount++;
    } while (result->NextRow());

    TC_LOG_INFO("server.loading", ">> Loaded {} hotfix optional data records in {} ms", hotfixOptionalDataCount, GetMSTimeDiffToNow(oldMSTime));
}

uint32 DB2Manager::GetHotfixCount() const
{
    return _hotfixData.size();
}

DB2Manager::HotfixContainer const& DB2Manager::GetHotfixData() const
{
    return _hotfixData;
}

std::vector<uint8> const* DB2Manager::GetHotfixBlobData(uint32 tableHash, int32 recordId, LocaleConstant locale) const
{
    ASSERT(IsValidLocale(locale), "Locale %u is invalid locale", uint32(locale));

    return Trinity::Containers::MapGetValuePtr(_hotfixBlob[locale], std::make_pair(tableHash, recordId));
}

std::vector<DB2Manager::HotfixOptionalData> const* DB2Manager::GetHotfixOptionalData(uint32 tableHash, int32 recordId, LocaleConstant locale) const
{
    ASSERT(IsValidLocale(locale), "Locale %u is invalid locale", uint32(locale));

    return Trinity::Containers::MapGetValuePtr(_hotfixOptionalData[locale], std::make_pair(tableHash, recordId));
}

void DB2Manager::InsertNewHotfix(uint32 tableHash, uint32 recordId)
{
    HotfixRecord hotfixRecord;
    hotfixRecord.TableHash = tableHash;
    hotfixRecord.RecordID = recordId;
    hotfixRecord.ID.PushID = ++_maxHotfixId;
    hotfixRecord.ID.UniqueID = rand32();
    hotfixRecord.AvailableLocalesMask = 0xDFF;

    HotfixPush& push = _hotfixData[hotfixRecord.ID.PushID];
    push.Records.push_back(hotfixRecord);
    push.AvailableLocalesMask |= hotfixRecord.AvailableLocalesMask;
}

std::vector<uint32> DB2Manager::GetAreasForGroup(uint32 areaGroupId) const
{
    auto itr = _areaGroupMembers.find(areaGroupId);
    if (itr != _areaGroupMembers.end())
        return itr->second;

    return std::vector<uint32>();
}

ContentTuningEntry const* DB2Manager::GetContentTuningForArea(AreaTableEntry const* /*areaEntry*/)
{
    // Shaohao: MOP doesn't have content tuning
//    if (!areaEntry)
//        return nullptr;
//
//    // Get ContentTuning for the area
//    if (ContentTuningEntry const* contentTuning = sContentTuningStore.LookupEntry(areaEntry->ContentTuningID))
//        return contentTuning;
//
//    // If there is no data for the current area and it has a parent area, get data from the last (recursive)
//    if (AreaTableEntry const* parentAreaEntry = sAreaTableStore.LookupEntry(areaEntry->ParentAreaID))
//        return GetContentTuningForArea(parentAreaEntry);

    return nullptr;
}

std::vector<ArtifactPowerEntry const*> DB2Manager::GetArtifactPowers(uint8 artifactId) const
{
    auto itr = _artifactPowers.find(artifactId);
    if (itr != _artifactPowers.end())
        return itr->second;

    return std::vector<ArtifactPowerEntry const*>{};
}

std::vector<uint32> const* DB2Manager::GetArtifactPowerLinks(uint32 artifactPowerId) const
{
    return Trinity::Containers::MapGetValuePtr(_artifactPowerLinks, artifactPowerId);
}

ArtifactPowerRankEntry const* DB2Manager::GetArtifactPowerRank(uint32 artifactPowerId, uint8 rank) const
{
    return Trinity::Containers::MapGetValuePtr(_artifactPowerRanks, { artifactPowerId, rank });
}

AzeriteEmpoweredItemEntry const* DB2Manager::GetAzeriteEmpoweredItem(uint32 itemId) const
{
    return Trinity::Containers::MapGetValuePtr(_azeriteEmpoweredItems, itemId);
}

bool DB2Manager::IsAzeriteItem(uint32 itemId) const
{
    return std::find_if(sAzeriteItemStore.begin(), sAzeriteItemStore.end(),
        [&](AzeriteItemEntry const* azeriteItem) { return azeriteItem->ItemID == int32(itemId); }) != sAzeriteItemStore.end();
}

AzeriteEssencePowerEntry const* DB2Manager::GetAzeriteEssencePower(uint32 azeriteEssenceId, uint32 rank) const
{
    return Trinity::Containers::MapGetValuePtr(_azeriteEssencePowersByIdAndRank, std::make_pair(azeriteEssenceId, rank));
}

std::vector<AzeriteItemMilestonePowerEntry const*> const& DB2Manager::GetAzeriteItemMilestonePowers() const
{
    return _azeriteItemMilestonePowers;
}

AzeriteItemMilestonePowerEntry const* DB2Manager::GetAzeriteItemMilestonePower(uint8 slot) const
{
    ASSERT(slot < MAX_AZERITE_ESSENCE_SLOT, "Slot %u must be lower than MAX_AZERITE_ESSENCE_SLOT (%u)", uint32(slot), MAX_AZERITE_ESSENCE_SLOT);
    return _azeriteItemMilestonePowerByEssenceSlot[slot];
}

std::vector<AzeritePowerSetMemberEntry const*> const* DB2Manager::GetAzeritePowers(uint32 itemId) const
{
    if (AzeriteEmpoweredItemEntry const* azeriteEmpoweredItem = GetAzeriteEmpoweredItem(itemId))
        return Trinity::Containers::MapGetValuePtr(_azeritePowers, azeriteEmpoweredItem->AzeritePowerSetID);

    return nullptr;
}

uint32 DB2Manager::GetRequiredAzeriteLevelForAzeritePowerTier(uint32 azeriteUnlockSetId, ItemContext context, uint32 tier) const
{
    ASSERT(tier < MAX_AZERITE_EMPOWERED_TIER);
    if (std::array<uint8, MAX_AZERITE_EMPOWERED_TIER> const* levels = Trinity::Containers::MapGetValuePtr(_azeriteTierUnlockLevels, std::make_pair(azeriteUnlockSetId, context)))
        return (*levels)[tier];

    AzeriteTierUnlockSetEntry const* azeriteTierUnlockSet = sAzeriteTierUnlockSetStore.LookupEntry(azeriteUnlockSetId);
    if (azeriteTierUnlockSet && azeriteTierUnlockSet->Flags & AZERITE_TIER_UNLOCK_SET_FLAG_DEFAULT)
        if (std::array<uint8, MAX_AZERITE_EMPOWERED_TIER> const* levels = Trinity::Containers::MapGetValuePtr(_azeriteTierUnlockLevels, std::make_pair(azeriteUnlockSetId, ItemContext::NONE)))
            return (*levels)[tier];

    return sAzeriteLevelInfoStore.GetNumRows();
}

char const* DB2Manager::GetBroadcastTextValue(BroadcastTextEntry const* broadcastText, LocaleConstant locale /*= DEFAULT_LOCALE*/, uint8 gender /*= GENDER_MALE*/, bool forceGender /*= false*/)
{
    if ((gender == GENDER_FEMALE || gender == GENDER_NONE) && (forceGender || broadcastText->Text1[DEFAULT_LOCALE][0] != '\0'))
    {
        if (broadcastText->Text1[locale][0] != '\0')
            return broadcastText->Text1[locale];

        return broadcastText->Text1[DEFAULT_LOCALE];
    }

    if (broadcastText->Text[locale][0] != '\0')
        return broadcastText->Text[locale];

    return broadcastText->Text[DEFAULT_LOCALE];
}

int32 const* DB2Manager::GetBroadcastTextDuration(int32 broadcastTextId, LocaleConstant locale /*= DEFAULT_LOCALE*/) const
{
    return Trinity::Containers::MapGetValuePtr(_broadcastTextDurations, { broadcastTextId, WowLocaleToCascLocaleBit[locale] });
}

CharBaseInfoEntry const* DB2Manager::GetCharBaseInfo(Races race, Classes class_)
{
    return Trinity::Containers::MapGetValuePtr(_charBaseInfoByRaceAndClass, { race, class_ });
}

ChrClassUIDisplayEntry const* DB2Manager::GetUiDisplayForClass(Classes unitClass) const
{
    ASSERT(unitClass < MAX_CLASSES);
    return _uiDisplayByClass[unitClass];
}

uint32 DB2Manager::GetPowerIndexByClass(Powers power, uint32 classId) const
{
    return _powersByClass[classId][power];
}

std::vector<ChrCustomizationChoiceEntry const*> const* DB2Manager::GetCustomiztionChoices(uint32 chrCustomizationOptionId) const
{
    return Trinity::Containers::MapGetValuePtr(_chrCustomizationChoicesByOption, chrCustomizationOptionId);
}

std::vector<ChrCustomizationOptionEntry const*> const* DB2Manager::GetCustomiztionOptions(uint8 race, uint8 gender) const
{
    return Trinity::Containers::MapGetValuePtr(_chrCustomizationOptionsByRaceAndGender, { race,gender });
}

std::vector<std::pair<uint32, std::vector<uint32>>> const* DB2Manager::GetRequiredCustomizationChoices(uint32 chrCustomizationReqId) const
{
    return Trinity::Containers::MapGetValuePtr(_chrCustomizationRequiredChoices, chrCustomizationReqId);
}

ChrModelEntry const* DB2Manager::GetChrModel(uint8 race, uint8 gender) const
{
    return Trinity::Containers::MapGetValuePtr(_chrModelsByRaceAndGender, { race, gender });
}

ConditionalChrModelEntry const* DB2Manager::GetConditionalChrModel(int32 chrModelId)
{
    return Trinity::Containers::MapGetValuePtr(_conditionalChrModelsByChrModelId, chrModelId);
}

uint32 DB2Manager::GetRedirectedContentTuningId(uint32 contentTuningId, uint32 redirectFlag) const
{
    if (std::vector<ConditionalContentTuningEntry const*> const* conditionalContentTunings = Trinity::Containers::MapGetValuePtr(_conditionalContentTuning, contentTuningId))
        for (ConditionalContentTuningEntry const* conditionalContentTuning : *conditionalContentTunings)
            if (conditionalContentTuning->RedirectFlag & redirectFlag)
                return conditionalContentTuning->RedirectContentTuningID;

    return contentTuningId;
}

Optional<ContentTuningLevels> DB2Manager::GetContentTuningData(uint32 contentTuningId, uint32 redirectFlag, bool forItem /*= false*/) const
{
    ContentTuningEntry const* contentTuning = sContentTuningStore.LookupEntry(GetRedirectedContentTuningId(contentTuningId, redirectFlag));
    if (!contentTuning)
        return {};

    if (forItem && contentTuning->GetFlags().HasFlag(ContentTuningFlag::DisabledForItem))
        return {};

    auto getLevelAdjustment = [](ContentTuningCalcType type) -> int32
    {
        switch (type)
        {
            case ContentTuningCalcType::MinLevel:
                return 1;
            case ContentTuningCalcType::MaxLevel:
                return GetMaxLevelForExpansion(sWorld->getIntConfig(CONFIG_EXPANSION));
            case ContentTuningCalcType::PrevExpansionMaxLevel:
                return GetMaxLevelForExpansion(std::max<int32>(sWorld->getIntConfig(CONFIG_EXPANSION) - 1, 0));
            default:
                break;
        }

        return 0;
    };

    ContentTuningLevels levels;
    levels.MinLevel = contentTuning->MinLevel + getLevelAdjustment(static_cast<ContentTuningCalcType>(contentTuning->MinLevelType));
    levels.MaxLevel = contentTuning->MaxLevel + getLevelAdjustment(static_cast<ContentTuningCalcType>(contentTuning->MaxLevelType));
    levels.MinLevelWithDelta = std::clamp<int32>(levels.MinLevel + contentTuning->TargetLevelDelta, 1, MAX_LEVEL);
    levels.MaxLevelWithDelta = std::clamp<int32>(levels.MaxLevel + contentTuning->TargetLevelMaxDelta, 1, MAX_LEVEL);

    // clamp after calculating levels with delta (delta can bring "overflown" level back into correct range)
    levels.MinLevel = std::clamp<int32>(levels.MinLevel, 1, MAX_LEVEL);
    levels.MaxLevel = std::clamp<int32>(levels.MaxLevel, 1, MAX_LEVEL);

    if (contentTuning->TargetLevelMin)
        levels.TargetLevelMin = contentTuning->TargetLevelMin;
    else
        levels.TargetLevelMin = levels.MinLevelWithDelta;

    if (contentTuning->TargetLevelMax)
        levels.TargetLevelMax = contentTuning->TargetLevelMax;
    else
        levels.TargetLevelMax = levels.MaxLevelWithDelta;

    return levels;
}

bool DB2Manager::HasContentTuningLabel(uint32 contentTuningId, int32 label) const
{
    return _contentTuningLabels.contains({ contentTuningId, label });
}

CurrencyContainerEntry const* DB2Manager::GetCurrencyContainerForCurrencyQuantity(uint32 currencyId, int32 quantity) const
{
    for (std::pair<uint32 const, CurrencyContainerEntry const*> const& p : Trinity::Containers::MapEqualRange(_currencyContainers, currencyId))
        if (quantity >= p.second->MinAmount && (!p.second->MaxAmount || quantity <= p.second->MaxAmount))
            return p.second;

    return nullptr;
}

std::pair<float, float> DB2Manager::GetCurveXAxisRange(uint32 curveId) const
{
    if (std::vector<DBCPosition2D> const* points = Trinity::Containers::MapGetValuePtr(_curvePoints, curveId))
        return { points->front().X, points->back().X };

    return { 0.0f, 0.0f };
}

static CurveInterpolationMode DetermineCurveType(CurveEntry const* curve, std::vector<DBCPosition2D> const& points)
{
    switch (curve->Type)
    {
        case 1:
            return points.size() < 4 ? CurveInterpolationMode::Cosine : CurveInterpolationMode::CatmullRom;
        case 2:
        {
            switch (points.size())
            {
                case 1:
                    return CurveInterpolationMode::Constant;
                case 2:
                    return CurveInterpolationMode::Linear;
                case 3:
                    return CurveInterpolationMode::Bezier3;
                case 4:
                    return CurveInterpolationMode::Bezier4;
                default:
                    break;
            }
            return CurveInterpolationMode::Bezier;
        }
        case 3:
            return CurveInterpolationMode::Cosine;
        default:
            break;
    }

    return points.size() != 1 ? CurveInterpolationMode::Linear : CurveInterpolationMode::Constant;
}

float DB2Manager::GetCurveValueAt(uint32 curveId, float x) const
{
    auto itr = _curvePoints.find(curveId);
    if (itr == _curvePoints.end())
        return 0.0f;

    CurveEntry const* curve = sCurveStore.AssertEntry(curveId);
    std::vector<DBCPosition2D> const& points = itr->second;
    if (points.empty())
        return 0.0f;

    return GetCurveValueAt(DetermineCurveType(curve, points), points, x);
}

float DB2Manager::GetCurveValueAt(CurveInterpolationMode mode, std::span<DBCPosition2D const> points, float x) const
{
    switch (mode)
    {
        case CurveInterpolationMode::Linear:
        {
            std::size_t pointIndex = 0;
            while (pointIndex < points.size() && points[pointIndex].X <= x)
                ++pointIndex;
            if (!pointIndex)
                return points[0].Y;
            if (pointIndex >= points.size())
                return points.back().Y;
            float xDiff = points[pointIndex].X - points[pointIndex - 1].X;
            if (xDiff == 0.0)
                return points[pointIndex].Y;
            return (((x - points[pointIndex - 1].X) / xDiff) * (points[pointIndex].Y - points[pointIndex - 1].Y)) + points[pointIndex - 1].Y;
        }
        case CurveInterpolationMode::Cosine:
        {
            std::size_t pointIndex = 0;
            while (pointIndex < points.size() && points[pointIndex].X <= x)
                ++pointIndex;
            if (!pointIndex)
                return points[0].Y;
            if (pointIndex >= points.size())
                return points.back().Y;
            float xDiff = points[pointIndex].X - points[pointIndex - 1].X;
            if (xDiff == 0.0)
                return points[pointIndex].Y;
            return ((points[pointIndex].Y - points[pointIndex - 1].Y) * (1.0f - std::cos((x - points[pointIndex - 1].X) / xDiff * float(M_PI))) * 0.5f) + points[pointIndex - 1].Y;
        }
        case CurveInterpolationMode::CatmullRom:
        {
            std::size_t pointIndex = 1;
            while (pointIndex < points.size() && points[pointIndex].X <= x)
                ++pointIndex;
            if (pointIndex == 1)
                return points[1].Y;
            if (pointIndex >= points.size() - 1)
                return points[points.size() - 2].Y;
            float xDiff = points[pointIndex].X - points[pointIndex - 1].X;
            if (xDiff == 0.0)
                return points[pointIndex].Y;

            float mu = (x - points[pointIndex - 1].X) / xDiff;
            float a0 = -0.5f * points[pointIndex - 2].Y + 1.5f * points[pointIndex - 1].Y - 1.5f * points[pointIndex].Y + 0.5f * points[pointIndex + 1].Y;
            float a1 = points[pointIndex - 2].Y - 2.5f * points[pointIndex - 1].Y + 2.0f * points[pointIndex].Y - 0.5f * points[pointIndex + 1].Y;
            float a2 = -0.5f * points[pointIndex - 2].Y + 0.5f * points[pointIndex].Y;
            float a3 = points[pointIndex - 1].Y;

            return a0 * mu * mu * mu + a1 * mu * mu + a2 * mu + a3;
        }
        case CurveInterpolationMode::Bezier3:
        {
            float xDiff = points[2].X - points[0].X;
            if (xDiff == 0.0)
                return points[1].Y;
            float mu = (x - points[0].X) / xDiff;
            return ((1.0f - mu) * (1.0f - mu) * points[0].Y) + (1.0f - mu) * 2.0f * mu * points[1].Y + mu * mu * points[2].Y;
        }
        case CurveInterpolationMode::Bezier4:
        {
            float xDiff = points[3].X - points[0].X;
            if (xDiff == 0.0)
                return points[1].Y;
            float mu = (x - points[0].X) / xDiff;
            return (1.0f - mu) * (1.0f - mu) * (1.0f - mu) * points[0].Y
                + 3.0f * mu * (1.0f - mu) * (1.0f - mu) * points[1].Y
                + 3.0f * mu * mu * (1.0f - mu) * points[2].Y
                + mu * mu * mu * points[3].Y;
        }
        case CurveInterpolationMode::Bezier:
        {
            float xDiff = points.back().X - points[0].X;
            if (xDiff == 0.0f)
                return points.back().Y;

            std::vector<float> tmp(points.size());
            for (std::size_t i = 0; i < points.size(); ++i)
                tmp[i] = points[i].Y;

            float mu = (x - points[0].X) / xDiff;
            int32 i = int32(points.size()) - 1;
            while (i > 0)
            {
                for (int32 k = 0; k < i; ++k)
                {
                    float val = tmp[k] + mu * (tmp[k + 1] - tmp[k]);
                    tmp[k] = val;
                }
                --i;
            }
            return tmp[0];
        }
        case CurveInterpolationMode::Constant:
            return points[0].Y;
        default:
            break;
    }

    return 0.0f;
}

EmotesTextSoundEntry const* DB2Manager::GetTextSoundEmoteFor(uint32 emote, uint8 race, uint8 gender, uint8 class_) const
{
    if (EmotesTextSoundEntry const* emotesTextSound = Trinity::Containers::MapGetValuePtr(_emoteTextSounds, { emote, race, gender, class_ }))
        return emotesTextSound;

    if (EmotesTextSoundEntry const* emotesTextSound = Trinity::Containers::MapGetValuePtr(_emoteTextSounds, { emote, race, gender, uint8(0) }))
        return emotesTextSound;

    return nullptr;
}

template<float(ExpectedStatModEntry::*field)>
struct ExpectedStatModReducer
{
    explicit ExpectedStatModReducer(int32 mythicPlusMilestoneSeason) : ActiveMilestoneSeason(mythicPlusMilestoneSeason) { }

    float operator()(float mod, ContentTuningXExpectedEntry const* contentTuningXExpected) const
    {
        if (!contentTuningXExpected)
            return mod;

        if (contentTuningXExpected->MinMythicPlusSeasonID)
            if (MythicPlusSeasonEntry const* mythicPlusSeason = sMythicPlusSeasonStore.LookupEntry(contentTuningXExpected->MinMythicPlusSeasonID))
                if (ActiveMilestoneSeason < mythicPlusSeason->MilestoneSeason)
                    return mod;

        if (contentTuningXExpected->MaxMythicPlusSeasonID)
            if (MythicPlusSeasonEntry const* mythicPlusSeason = sMythicPlusSeasonStore.LookupEntry(contentTuningXExpected->MaxMythicPlusSeasonID))
                if (ActiveMilestoneSeason >= mythicPlusSeason->MilestoneSeason)
                    return mod;

        return mod * sExpectedStatModStore.AssertEntry(contentTuningXExpected->ExpectedStatModID)->*field;
    }

    int32 ActiveMilestoneSeason = 0;
};

float DB2Manager::EvaluateExpectedStat(ExpectedStatType stat, uint32 level, int32 expansion, uint32 contentTuningId, Classes unitClass, int32 mythicPlusMilestoneSeason) const
{
    auto expectedStatItr = _expectedStatsByLevel.find(std::make_pair(level, expansion));
    if (expectedStatItr == _expectedStatsByLevel.end())
        expectedStatItr = _expectedStatsByLevel.find(std::make_pair(level, -2));

    if (expectedStatItr == _expectedStatsByLevel.end())
        return 1.0f;

    ExpectedStatModEntry const* classMod = nullptr;
    switch (unitClass)
    {
        case CLASS_WARRIOR:
            classMod = sExpectedStatModStore.LookupEntry(4);
            break;
        case CLASS_PALADIN:
            classMod = sExpectedStatModStore.LookupEntry(2);
            break;
        case CLASS_ROGUE:
            classMod = sExpectedStatModStore.LookupEntry(3);
            break;
        case CLASS_MAGE:
            classMod = sExpectedStatModStore.LookupEntry(1);
            break;
        default:
            break;
    }

    std::vector<ContentTuningXExpectedEntry const*> const* contentTuningMods = Trinity::Containers::MapGetValuePtr(_expectedStatModsByContentTuning, contentTuningId);
    float value = 0.0f;
    switch (stat)
    {
        case ExpectedStatType::CreatureHealth:
            value = expectedStatItr->second->CreatureHealth;
            if (contentTuningMods)
                value *= std::accumulate(contentTuningMods->begin(), contentTuningMods->end(), 1.0f,
                    ExpectedStatModReducer<&ExpectedStatModEntry::CreatureHealthMod>(mythicPlusMilestoneSeason));
            if (classMod)
                value *= classMod->CreatureHealthMod;
            break;
        case ExpectedStatType::PlayerHealth:
            value = expectedStatItr->second->PlayerHealth;
            if (contentTuningMods)
                value *= std::accumulate(contentTuningMods->begin(), contentTuningMods->end(), 1.0f,
                    ExpectedStatModReducer<&ExpectedStatModEntry::PlayerHealthMod>(mythicPlusMilestoneSeason));
            if (classMod)
                value *= classMod->PlayerHealthMod;
            break;
        case ExpectedStatType::CreatureAutoAttackDps:
            value = expectedStatItr->second->CreatureAutoAttackDps;
            if (contentTuningMods)
                value *= std::accumulate(contentTuningMods->begin(), contentTuningMods->end(), 1.0f,
                    ExpectedStatModReducer<&ExpectedStatModEntry::CreatureAutoAttackDPSMod>(mythicPlusMilestoneSeason));
            if (classMod)
                value *= classMod->CreatureAutoAttackDPSMod;
            break;
        case ExpectedStatType::CreatureArmor:
            value = expectedStatItr->second->CreatureArmor;
            if (contentTuningMods)
                value *= std::accumulate(contentTuningMods->begin(), contentTuningMods->end(), 1.0f,
                    ExpectedStatModReducer<&ExpectedStatModEntry::CreatureArmorMod>(mythicPlusMilestoneSeason));
            if (classMod)
                value *= classMod->CreatureArmorMod;
            break;
        case ExpectedStatType::PlayerMana:
            value = expectedStatItr->second->PlayerMana;
            if (contentTuningMods)
                value *= std::accumulate(contentTuningMods->begin(), contentTuningMods->end(), 1.0f,
                    ExpectedStatModReducer<&ExpectedStatModEntry::PlayerManaMod>(mythicPlusMilestoneSeason));
            if (classMod)
                value *= classMod->PlayerManaMod;
            break;
        case ExpectedStatType::PlayerPrimaryStat:
            value = expectedStatItr->second->PlayerPrimaryStat;
            if (contentTuningMods)
                value *= std::accumulate(contentTuningMods->begin(), contentTuningMods->end(), 1.0f,
                    ExpectedStatModReducer<&ExpectedStatModEntry::PlayerPrimaryStatMod>(mythicPlusMilestoneSeason));
            if (classMod)
                value *= classMod->PlayerPrimaryStatMod;
            break;
        case ExpectedStatType::PlayerSecondaryStat:
            value = expectedStatItr->second->PlayerSecondaryStat;
            if (contentTuningMods)
                value *= std::accumulate(contentTuningMods->begin(), contentTuningMods->end(), 1.0f,
                    ExpectedStatModReducer<&ExpectedStatModEntry::PlayerSecondaryStatMod>(mythicPlusMilestoneSeason));
            if (classMod)
                value *= classMod->PlayerSecondaryStatMod;
            break;
        case ExpectedStatType::ArmorConstant:
            value = expectedStatItr->second->ArmorConstant;
            if (contentTuningMods)
                value *= std::accumulate(contentTuningMods->begin(), contentTuningMods->end(), 1.0f,
                    ExpectedStatModReducer<&ExpectedStatModEntry::ArmorConstantMod>(mythicPlusMilestoneSeason));
            if (classMod)
                value *= classMod->ArmorConstantMod;
            break;
        case ExpectedStatType::None:
            break;
        case ExpectedStatType::CreatureSpellDamage:
            value = expectedStatItr->second->CreatureSpellDamage;
            if (contentTuningMods)
                value *= std::accumulate(contentTuningMods->begin(), contentTuningMods->end(), 1.0f,
                    ExpectedStatModReducer<&ExpectedStatModEntry::CreatureSpellDamageMod>(mythicPlusMilestoneSeason));
            if (classMod)
                value *= classMod->CreatureSpellDamageMod;
            break;
        default:
            break;
    }

    return value;
}

uint32 DB2Manager::GetGlobalCurveId(GlobalCurve globalCurveType) const
{
    for (GlobalCurveEntry const* globalCurveEntry : sGlobalCurveStore)
        if (GlobalCurve(globalCurveEntry->Type) == globalCurveType)
            return globalCurveEntry->CurveID;

    return 0;
}

std::vector<uint32> const* DB2Manager::GetGlyphBindableSpells(uint32 glyphPropertiesId) const
{
    return Trinity::Containers::MapGetValuePtr(_glyphBindableSpells, glyphPropertiesId);
}

std::vector<ChrSpecialization> const* DB2Manager::GetGlyphRequiredSpecs(uint32 glyphPropertiesId) const
{
    return Trinity::Containers::MapGetValuePtr(_glyphRequiredSpecs, glyphPropertiesId);
}

HeirloomEntry const* DB2Manager::GetHeirloomByItemId(uint32 itemId) const
{
    return Trinity::Containers::MapGetValuePtr(_heirlooms, itemId);
}

ItemChildEquipmentEntry const* DB2Manager::GetItemChildEquipment(uint32 itemId) const
{
    return Trinity::Containers::MapGetValuePtr(_itemChildEquipment, itemId);
}

bool DB2Manager::HasItemCurrencyCost(uint32 itemId) const
{
    return _itemsWithCurrencyCost.count(itemId) > 0;
}

std::vector<ItemLimitCategoryConditionEntry const*> const* DB2Manager::GetItemLimitCategoryConditions(uint32 categoryId) const
{
    return Trinity::Containers::MapGetValuePtr(_itemCategoryConditions, categoryId);
}

uint32 DB2Manager::GetItemDisplayId(uint32 itemId, uint32 appearanceModId) const
{
    if (ItemModifiedAppearanceEntry const* modifiedAppearance = GetItemModifiedAppearance(itemId, appearanceModId))
        if (ItemAppearanceEntry const* itemAppearance = sItemAppearanceStore.LookupEntry(modifiedAppearance->ItemAppearanceID))
            return itemAppearance->ItemDisplayInfoID;

    return 0;
}

ItemModifiedAppearanceEntry const* DB2Manager::GetItemModifiedAppearance(uint32 itemId, uint32 appearanceModId) const
{
    auto itr = _itemModifiedAppearancesByItem.find(itemId | (appearanceModId << 24));
    if (itr != _itemModifiedAppearancesByItem.end())
        return itr->second;

    // Fall back to unmodified appearance
    if (appearanceModId)
    {
        itr = _itemModifiedAppearancesByItem.find(itemId);
        if (itr != _itemModifiedAppearancesByItem.end())
            return itr->second;
    }

    return nullptr;
}

ItemModifiedAppearanceEntry const* DB2Manager::GetDefaultItemModifiedAppearance(uint32 itemId) const
{
    return Trinity::Containers::MapGetValuePtr(_itemModifiedAppearancesByItem, itemId);
}

std::vector<ItemSetSpellEntry const*> const* DB2Manager::GetItemSetSpells(uint32 itemSetId) const
{
    return Trinity::Containers::MapGetValuePtr(_itemSetSpells, itemSetId);
}

DB2Manager::MapDifficultyConditionsContainer const* DB2Manager::GetMapDifficultyConditions(uint32 mapDifficultyId) const
{
    return Trinity::Containers::MapGetValuePtr(_mapDifficultyConditions, mapDifficultyId);
}

MountEntry const* DB2Manager::GetMount(uint32 spellId) const
{
    return Trinity::Containers::MapGetValuePtr(_mountsBySpellId, spellId);
}

MountEntry const* DB2Manager::GetMountById(uint32 id) const
{
    return sMountStore.LookupEntry(id);
}

DB2Manager::MountTypeXCapabilitySet const* DB2Manager::GetMountCapabilities(uint32 mountType) const
{
    return Trinity::Containers::MapGetValuePtr(_mountCapabilitiesByType, mountType);
}

DB2Manager::MountXDisplayContainer const* DB2Manager::GetMountDisplays(uint32 mountId) const
{
    return Trinity::Containers::MapGetValuePtr(_mountDisplays, mountId);
}

int32 DB2Manager::GetNumTalentsAtLevel(uint32 level, Classes playerClass)
{
    NumTalentsAtLevelEntry const* numTalentsAtLevel = sNumTalentsAtLevelStore.LookupEntry(level);
    if (!numTalentsAtLevel)
        numTalentsAtLevel = sNumTalentsAtLevelStore.LookupEntry(sNumTalentsAtLevelStore.GetNumRows() - 1);

    if (numTalentsAtLevel)
    {
        switch (playerClass)
        {
            case CLASS_DEATH_KNIGHT:
                return numTalentsAtLevel->NumTalentsDeathKnight;
            case CLASS_DEMON_HUNTER:
                return numTalentsAtLevel->NumTalentsDemonHunter;
            default:
                return numTalentsAtLevel->NumTalents;
        }
    }

    return 0;
}

ParagonReputationEntry const* DB2Manager::GetParagonReputation(uint32 factionId) const
{
    return Trinity::Containers::MapGetValuePtr(_paragonReputations, factionId);
}

PathDb2 const* DB2Manager::GetPath(uint32 pathId) const
{
    return Trinity::Containers::MapGetValuePtr(_paths, pathId);
}

PVPDifficultyEntry const* DB2Manager::GetBattlegroundBracketByLevel(uint32 mapid, uint32 level)
{
    PVPDifficultyEntry const* maxEntry = nullptr;           // used for level > max listed level case
    for (PVPDifficultyEntry const* entry : sPVPDifficultyStore)
    {
        // skip unrelated and too-high brackets
        if (entry->MapID != mapid || entry->MinLevel > level)
            continue;

        // exactly fit
        if (entry->MaxLevel >= level)
            return entry;

        // remember for possible out-of-range case (search higher from existed)
        if (!maxEntry || maxEntry->MaxLevel < entry->MaxLevel)
            maxEntry = entry;
    }

    return maxEntry;
}

PVPDifficultyEntry const* DB2Manager::GetBattlegroundBracketById(uint32 mapid, BattlegroundBracketId id)
{
    for (PVPDifficultyEntry const* entry : sPVPDifficultyStore)
        if (entry->MapID == mapid && entry->GetBracketId() == id)
            return entry;

    return nullptr;
}

uint32 DB2Manager::GetRequiredLevelForPvpTalentSlot(uint8 slot, Classes class_) const
{
    ASSERT(slot < MAX_PVP_TALENT_SLOTS);
    if (_pvpTalentSlotUnlock[slot])
    {
        switch (class_)
        {
            case CLASS_DEATH_KNIGHT:
                return _pvpTalentSlotUnlock[slot]->DeathKnightLevelRequired;
            case CLASS_DEMON_HUNTER:
                return _pvpTalentSlotUnlock[slot]->DemonHunterLevelRequired;
            default:
                break;
        }
        return _pvpTalentSlotUnlock[slot]->LevelRequired;
    }

    return 0;
}

int32 DB2Manager::GetPvpTalentNumSlotsAtLevel(uint32 level, Classes class_) const
{
    int32 slots = 0;
    for (uint8 slot = 0; slot < MAX_PVP_TALENT_SLOTS; ++slot)
        if (level >= GetRequiredLevelForPvpTalentSlot(slot, class_))
            ++slots;

    return slots;
}

std::vector<QuestLineXQuestEntry const*> const* DB2Manager::GetQuestsForQuestLine(uint32 questLineId) const
{
    return Trinity::Containers::MapGetValuePtr(_questsByQuestLine, questLineId);
}

std::vector<QuestPackageItemEntry const*> const* DB2Manager::GetQuestPackageItems(uint32 questPackageID) const
{
    auto itr = _questPackages.find(questPackageID);
    if (itr != _questPackages.end())
        return &itr->second.first;

    return nullptr;
}

std::vector<QuestPackageItemEntry const*> const* DB2Manager::GetQuestPackageItemsFallback(uint32 questPackageID) const
{
    auto itr = _questPackages.find(questPackageID);
    if (itr != _questPackages.end())
        return &itr->second.second;

    return nullptr;
}

PowerTypeEntry const* DB2Manager::GetPowerTypeEntry(Powers power) const
{
    ASSERT(power < MAX_POWERS);
    return _powerTypes[power];
}

PowerTypeEntry const* DB2Manager::GetPowerTypeByName(std::string const& name) const
{
    for (PowerTypeEntry const* powerType : sPowerTypeStore)
    {
        std::string powerName = powerType->NameGlobalStringTag;
        strToLower(powerName);
        if (powerName == name)
            return powerType;

        powerName.erase(std::remove(powerName.begin(), powerName.end(), '_'), powerName.end());
        if (powerName == name)
            return powerType;
    }

    return nullptr;
}

uint8 DB2Manager::GetPvpItemLevelBonus(uint32 itemId) const
{
    auto itr = _pvpItemBonus.find(itemId);
    if (itr != _pvpItemBonus.end())
        return itr->second;

    return 0;
}

std::vector<RewardPackXCurrencyTypeEntry const*> const* DB2Manager::GetRewardPackCurrencyTypesByRewardID(uint32 rewardPackID) const
{
    return Trinity::Containers::MapGetValuePtr(_rewardPackCurrencyTypes, rewardPackID);
}

std::vector<RewardPackXItemEntry const*> const* DB2Manager::GetRewardPackItemsByRewardID(uint32 rewardPackID) const
{
    return Trinity::Containers::MapGetValuePtr(_rewardPackItems, rewardPackID);
}

ShapeshiftFormModelData const* DB2Manager::GetShapeshiftFormModelData(uint8 race, uint8 gender, uint8 form) const
{
    return Trinity::Containers::MapGetValuePtr(_chrCustomizationChoicesForShapeshifts, { race, gender, form });
}

std::vector<SkillLineAbilityEntry const*> const* DB2Manager::GetSkillLineAbilitiesBySkill(uint32 skillId) const
{
    return Trinity::Containers::MapGetValuePtr(_skillLineAbilitiesBySkillupSkill, skillId);
}

SoulbindConduitRankEntry const* DB2Manager::GetSoulbindConduitRank(int32 soulbindConduitId, int32 rank) const
{
    return Trinity::Containers::MapGetValuePtr(_soulbindConduitRanks, { soulbindConduitId, rank });
}

bool DB2Manager::IsSpecSetMember(int32 specSetId, uint32 specId) const
{
    return _specsBySpecSet.count(std::make_pair(specSetId, specId)) > 0;
}

std::vector<SpellVisualMissileEntry const*> const* DB2Manager::GetSpellVisualMissiles(int32 spellVisualMissileSetId) const
{
    return Trinity::Containers::MapGetValuePtr(_spellVisualMissilesBySet, spellVisualMissileSetId);
}

bool DB2Manager::IsToyItem(uint32 toy) const
{
    return _toys.count(toy) > 0;
}

TransmogIllusionEntry const* DB2Manager::GetTransmogIllusionForEnchantment(uint32 spellItemEnchantmentId) const
{
    return Trinity::Containers::MapGetValuePtr(_transmogIllusionsByEnchantmentId, spellItemEnchantmentId);
}

std::vector<TransmogSetEntry const*> const* DB2Manager::GetTransmogSetsForItemModifiedAppearance(uint32 itemModifiedAppearanceId) const
{
    return Trinity::Containers::MapGetValuePtr(_transmogSetsByItemModifiedAppearance, itemModifiedAppearanceId);
}

std::vector<TransmogSetItemEntry const*> const* DB2Manager::GetTransmogSetItems(uint32 transmogSetId) const
{
    return Trinity::Containers::MapGetValuePtr(_transmogSetItemsByTransmogSet, transmogSetId);
}

bool DB2Manager::IsUiMapPhase(uint32 phaseId) const
{
    return _uiMapPhases.find(phaseId) != _uiMapPhases.end();
}

std::unordered_set<uint32> const* DB2Manager::GetPVPStatIDsForMap(uint32 mapId) const
{
    return Trinity::Containers::MapGetValuePtr(_pvpStatIdsByMap, mapId);
}

bool ChrClassesXPowerTypesEntryComparator::Compare(ChrClassesXPowerTypesEntry const* left, ChrClassesXPowerTypesEntry const* right)
{
    if (left->ClassID != right->ClassID)
        return left->ClassID < right->ClassID;
    return left->PowerType < right->PowerType;
}

bool DB2Manager::MountTypeXCapabilityEntryComparator::Compare(MountTypeXCapabilityEntry const* left, MountTypeXCapabilityEntry const* right)
{
    if (left->MountTypeID == right->MountTypeID)
        return left->OrderIndex < right->OrderIndex;
    return left->MountTypeID < right->MountTypeID;
}
