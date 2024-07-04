/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef TRINITY_DBCSTORES_H
#define TRINITY_DBCSTORES_H

#include "DBCStore.h"
#include "DBCStructure.h"
#include "DB2Structure.h"
#include "GameTables.h"
#include "SharedDefines.h"

// CharSections
TC_GAME_API CharSectionsEntry const* GetCharSectionEntry(uint8 race, CharSectionType genType, uint8 gender, uint8 type, uint8 color);

// ChrSpecialization
typedef ChrSpecializationEntry const* ChrSpecializationByIndexArray[MAX_CLASSES][MAX_SPECIALIZATIONS];

// EmotesText
TC_GAME_API EmotesTextSoundEntry const* FindTextSoundEmoteFor(uint32 emote, uint32 race, uint32 gender);

// Faction
TC_GAME_API std::vector<uint32> const* GetFactionTeamList(uint32 faction);

// ItemSetSpells
typedef std::vector<ItemSetSpellEntry const*> ItemSetSpells;
typedef std::unordered_map<uint32, ItemSetSpells> ItemSetSpellsStore;

// MapDifficulty
typedef std::unordered_map<uint32, std::unordered_map<uint32, MapDifficultyEntry const*>> MapDifficultyMap;
TC_GAME_API MapDifficultyEntry const* GetDefaultMapDifficulty(uint32 mapId, Difficulty* difficulty = nullptr);
TC_GAME_API MapDifficultyEntry const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty);
TC_GAME_API MapDifficultyEntry const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty);

// PvpDifficulty
TC_GAME_API PvPDifficultyEntry const* GetBattlegroundBracketByLevel(uint32 mapid, uint32 level);
TC_GAME_API PvPDifficultyEntry const* GetBattlegroundBracketById(uint32 mapid, BattlegroundBracketId id);

// SkillRaceClassInfo
typedef std::unordered_multimap<uint32, SkillRaceClassInfoEntry const*> SkillRaceClassInfoMap;
typedef std::pair<SkillRaceClassInfoMap::iterator, SkillRaceClassInfoMap::iterator> SkillRaceClassInfoBounds;
TC_GAME_API SkillRaceClassInfoEntry const* GetSkillRaceClassInfo(uint32 skill, uint8 race, uint8 class_);

// SpellEffectScaling
typedef std::unordered_map<uint32, uint32> SpellEffectScalingByEffectId;

// Talent
typedef std::vector<TalentEntry const*> TalentsByPosition[MAX_CLASSES][MAX_TALENT_TIERS][MAX_TALENT_COLUMNS];

// TotemCategory
TC_GAME_API bool IsTotemCategoryCompatibleWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId);

// WorldMapArea
TC_GAME_API uint32 GetVirtualMapForMapAndZone(uint32 mapid, uint32 zoneId);
TC_GAME_API void Map2ZoneCoordinates(float &x, float &y, uint32 worldMapAreaId);

// WorldMapTransforms
TC_GAME_API void DeterminaAlternateMapPosition(uint32 mapId, float x, float y, float z, uint32* newMapId = nullptr, DBCPosition2D* newPos = nullptr);

TC_GAME_API uint32 GetExpansionForLevel(uint32 level);

TC_GAME_API extern DBCStorage<Achievement_CategoryEntry>          sAchievementCategoryStore;
TC_GAME_API extern DBCStorage<AchievementEntry>                   sAchievementStore;
TC_GAME_API extern DBCStorage<AnimationDataEntry>                 sAnimationDataStore;
TC_GAME_API extern DBCStorage<AnimKitEntry>                       sAnimKitStore;
TC_GAME_API extern DBCStorage<AreaTableEntry>                     sAreaTableStore;
TC_GAME_API extern DBCStorage<AreaTriggerEntry>                   sAreaTriggerStore;
TC_GAME_API extern DBCStorage<AreaTriggerActionSetEntry>          sAreaTriggerActionSetStore;
TC_GAME_API extern DBCStorage<ArmorLocationEntry>                 sArmorLocationStore;
TC_GAME_API extern DBCStorage<AuctionHouseEntry>                  sAuctionHouseStore;
TC_GAME_API extern DBCStorage<BankBagSlotPricesEntry>             sBankBagSlotPricesStore;
TC_GAME_API extern DBCStorage<BannedAddOnsEntry>                  sBannedAddOnsStore;
TC_GAME_API extern DBCStorage<BarberShopStyleEntry>               sBarberShopStyleStore;
TC_GAME_API extern DBCStorage<BattlemasterListEntry>              sBattlemasterListStore;
TC_GAME_API extern DBCStorage<Cfg_CategoriesEntry>                sCfgCategoriesStore;
TC_GAME_API extern DBCStorage<Cfg_RegionsEntry>                   sCfgRegionsStore;
TC_GAME_API extern DBCStorage<CharacterLoadoutEntry>              sCharacterLoadoutStore;
TC_GAME_API extern DBCStorage<CharacterLoadoutItemEntry>          sCharacterLoadoutItemStore;
TC_GAME_API extern DBCStorage<CharBaseInfoEntry>                  sCharBaseInfoStore;
TC_GAME_API extern DBCStorage<CharSectionsEntry>                  sCharSectionsStore;
TC_GAME_API extern DBCStorage<CharStartOutfitEntry>               sCharStartOutfitStore;
TC_GAME_API extern DBCStorage<CharTitlesEntry>                    sCharTitlesStore;
TC_GAME_API extern DBCStorage<ChatChannelsEntry>                  sChatChannelsStore;
TC_GAME_API extern DBCStorage<ChrClassesEntry>                    sChrClassesStore;
TC_GAME_API extern DBCStorage<ChrClassesXPowerTypesEntry>         sChrClassesXPowerTypesStore;
TC_GAME_API extern DBCStorage<ChrRacesEntry>                      sChrRacesStore;
TC_GAME_API extern DBCStorage<ChrSpecializationEntry>             sChrSpecializationStore;
TC_GAME_API extern ChrSpecializationByIndexArray                  sChrSpecializationByIndexStore;
TC_GAME_API extern DBCStorage<CinematicCameraEntry>               sCinematicCameraStore;
TC_GAME_API extern DBCStorage<CinematicSequencesEntry>            sCinematicSequencesStore;
TC_GAME_API extern DBCStorage<CreatureDisplayInfoEntry>           sCreatureDisplayInfoStore;
TC_GAME_API extern DBCStorage<CreatureDisplayInfoExtraEntry>      sCreatureDisplayInfoExtraStore;
TC_GAME_API extern DBCStorage<CreatureFamilyEntry>                sCreatureFamilyStore;
TC_GAME_API extern DBCStorage<CreatureImmunitiesEntry>            sCreatureImmunitiesStore;
TC_GAME_API extern DBCStorage<CreatureModelDataEntry>             sCreatureModelDataStore;
TC_GAME_API extern DBCStorage<CreatureSpellDataEntry>             sCreatureSpellDataStore;
TC_GAME_API extern DBCStorage<CreatureTypeEntry>                  sCreatureTypeStore;
TC_GAME_API extern DBCStorage<CriteriaEntry>                      sCriteriaStore;
TC_GAME_API extern DBCStorage<CriteriaTreeEntry>                  sCriteriaTreeStore;
TC_GAME_API extern DBCStorage<CurrencyTypesEntry>                 sCurrencyTypesStore;
TC_GAME_API extern DBCStorage<DestructibleModelDataEntry>         sDestructibleModelDataStore;
TC_GAME_API extern DBCStorage<DifficultyEntry>                    sDifficultyStore;
TC_GAME_API extern DBCStorage<DungeonEncounterEntry>              sDungeonEncounterStore;
TC_GAME_API extern DBCStorage<DurabilityCostsEntry>               sDurabilityCostsStore;
TC_GAME_API extern DBCStorage<DurabilityQualityEntry>             sDurabilityQualityStore;
TC_GAME_API extern DBCStorage<EmotesEntry>                        sEmotesStore;
TC_GAME_API extern DBCStorage<EmotesTextEntry>                    sEmotesTextStore;
TC_GAME_API extern DBCStorage<EmotesTextSoundEntry>               sEmotesTextSoundStore;
TC_GAME_API extern DBCStorage<FactionEntry>                       sFactionStore;
TC_GAME_API extern DBCStorage<FactionTemplateEntry>               sFactionTemplateStore;
TC_GAME_API extern DBCStorage<FriendshipRepReactionEntry>         sFriendshipRepReactionStore;
TC_GAME_API extern DBCStorage<FriendshipReputationEntry>          sFriendshipReputationStore;
TC_GAME_API extern DBCStorage<GameObjectArtKitEntry>              sGameObjectArtKitStore;
TC_GAME_API extern DBCStorage<GameObjectDisplayInfoEntry>         sGameObjectDisplayInfoStore;
TC_GAME_API extern DBCStorage<GameTablesEntry>                    sGameTablesStore;
TC_GAME_API extern DBCStorage<GemPropertiesEntry>                 sGemPropertiesStore;
TC_GAME_API extern DBCStorage<GlyphPropertiesEntry>               sGlyphPropertiesStore;
TC_GAME_API extern DBCStorage<GuildColorBackgroundEntry>          sGuildColorBackgroundStore;
TC_GAME_API extern DBCStorage<GuildColorBorderEntry>              sGuildColorBorderStore;
TC_GAME_API extern DBCStorage<GuildColorEmblemEntry>              sGuildColorEmblemStore;
TC_GAME_API extern DBCStorage<GuildPerkSpellsEntry>               sGuildPerkSpellsStore;
TC_GAME_API extern DBCStorage<HolidaysEntry>                      sHolidaysStore;
TC_GAME_API extern DBCStorage<ImportPriceArmorEntry>              sImportPriceArmorStore;
TC_GAME_API extern DBCStorage<ImportPriceQualityEntry>            sImportPriceQualityStore;
TC_GAME_API extern DBCStorage<ImportPriceShieldEntry>             sImportPriceShieldStore;
TC_GAME_API extern DBCStorage<ImportPriceWeaponEntry>             sImportPriceWeaponStore;
TC_GAME_API extern DBCStorage<ItemArmorQualityEntry>              sItemArmorQualityStore;
TC_GAME_API extern DBCStorage<ItemArmorShieldEntry>               sItemArmorShieldStore;
TC_GAME_API extern DBCStorage<ItemArmorTotalEntry>                sItemArmorTotalStore;
TC_GAME_API extern DBCStorage<ItemBagFamilyEntry>                 sItemBagFamilyStore;
TC_GAME_API extern DBCStorage<ItemClassEntry>                     sItemClassStore;
TC_GAME_API extern DBCStorage<ItemDamageAmmoEntry>                sItemDamageAmmoStore;
TC_GAME_API extern DBCStorage<ItemDamageOneHandEntry>             sItemDamageOneHandStore;
TC_GAME_API extern DBCStorage<ItemDamageOneHandCasterEntry>       sItemDamageOneHandCasterStore;
// TODO: DATA ItemDamageRanged, ItemDamageThrown, ItemDamageWand
TC_GAME_API extern DBCStorage<ItemDamageTwoHandEntry>             sItemDamageTwoHandStore;
TC_GAME_API extern DBCStorage<ItemDamageTwoHandCasterEntry>       sItemDamageTwoHandCasterStore;
TC_GAME_API extern DBCStorage<ItemDisenchantLootEntry>            sItemDisenchantLootStore;
TC_GAME_API extern DBCStorage<ItemDisplayInfoEntry>               sItemDisplayInfoStore;
TC_GAME_API extern DBCStorage<ItemLimitCategoryEntry>             sItemLimitCategoryStore;
TC_GAME_API extern DBCStorage<ItemNameDescriptionEntry>           sItemNameDescriptionStore;
TC_GAME_API extern DBCStorage<ItemPriceBaseEntry>                 sItemPriceBaseStore;
TC_GAME_API extern DBCStorage<ItemRandomPropertiesEntry>          sItemRandomPropertiesStore;
TC_GAME_API extern DBCStorage<ItemRandomSuffixEntry>              sItemRandomSuffixStore;
TC_GAME_API extern DBCStorage<ItemSetEntry>                       sItemSetStore;
TC_GAME_API extern DBCStorage<ItemSpecEntry>                      sItemSpecStore;
TC_GAME_API extern DBCStorage<ItemSpecOverrideEntry>              sItemSpecOverrideStore;
// TODO: DATA ItemUpgrade, ItemUpgradePath
TC_GAME_API extern DBCStorage<JournalEncounterEntry>              sJournalEncounterStore;
TC_GAME_API extern DBCStorage<JournalEncounterSectionEntry>       sJournalEncounterSectionStore;
TC_GAME_API extern DBCStorage<JournalInstanceEntry>               sJournalInstanceStore;
TC_GAME_API extern DBCStorage<JournalTierEntry>                   sJournalTierStore;
TC_GAME_API extern DBCStorage<LanguagesEntry>                     sLanguagesStore;
TC_GAME_API extern DBCStorage<LanguageWordsEntry>                 sLanguageWordsStore;
TC_GAME_API extern DBCStorage<LFGDungeonsEntry>                   sLFGDungeonsStore;
TC_GAME_API extern DBCStorage<LightEntry>                         sLightStore;
TC_GAME_API extern DBCStorage<LiquidTypeEntry>                    sLiquidTypeStore;
TC_GAME_API extern DBCStorage<LockEntry>                          sLockStore;
TC_GAME_API extern DBCStorage<MailTemplateEntry>                  sMailTemplateStore;
TC_GAME_API extern DBCStorage<MapEntry>                           sMapStore;
TC_GAME_API extern DBCStorage<MapDifficultyEntry>                 sMapDifficultyStore;
TC_GAME_API extern MapDifficultyMap                               sMapDifficultyMap;
TC_GAME_API extern DBCStorage<ModifierTreeEntry>                  sModifierTreeStore;
TC_GAME_API extern DBCStorage<MountCapabilityEntry>               sMountCapabilityStore;
TC_GAME_API extern DBCStorage<MountTypeEntry>                     sMountTypeStore;
TC_GAME_API extern DBCStorage<MovieEntry>                         sMovieStore;
TC_GAME_API extern DBCStorage<NameGenEntry>                       sNameGenStore;
TC_GAME_API extern DBCStorage<NamesProfanityEntry>                sNamesProfanityStore;
TC_GAME_API extern DBCStorage<NamesReservedEntry>                 sNamesReservedStore;
TC_GAME_API extern DBCStorage<NamesReservedLocaleEntry>           sNamesReservedLocaleStore;
TC_GAME_API extern DBCStorage<OverrideSpellDataEntry>             sOverrideSpellDataStore;
TC_GAME_API extern DBCStorage<PhaseEntry>                         sPhaseStore;
TC_GAME_API extern DBCStorage<PhaseXPhaseGroupEntry>              sPhaseXPhaseGroupStore;
TC_GAME_API extern DBCStorage<PlayerConditionEntry>               sPlayerConditionStore;
TC_GAME_API extern DBCStorage<PowerDisplayEntry>                  sPowerDisplayStore;
TC_GAME_API extern DBCStorage<PvPDifficultyEntry>                 sPvpDifficultyStore;
TC_GAME_API extern DBCStorage<QuestFactionRewardEntry>            sQuestFactionRewardStore;
TC_GAME_API extern DBCStorage<QuestInfoEntry>                     sQuestInfoStore;
TC_GAME_API extern DBCStorage<QuestMoneyRewardEntry>              sQuestMoneyRewardStore;
// TODO: DATA QuestPOIBlob, QuestPOIPoint ??
TC_GAME_API extern DBCStorage<QuestSortEntry>                     sQuestSortStore;
TC_GAME_API extern DBCStorage<QuestV2Entry>                       sQuestV2Store;
TC_GAME_API extern DBCStorage<QuestXPEntry>                       sQuestXPStore;
TC_GAME_API extern DBCStorage<RandPropPointsEntry>                sRandPropPointsStore;
TC_GAME_API extern DBCStorage<ResearchBranchEntry>                sResearchBranchStore;
TC_GAME_API extern DBCStorage<ResearchProjectEntry>               sResearchProjectStore;
TC_GAME_API extern DBCStorage<ResearchSiteEntry>                  sResearchSiteStore;
// TODO: DATA ScalingStatDistribution, ScalingStatValues ??
TC_GAME_API extern DBCStorage<ScenarioEntry>                      sScenarioStore;
TC_GAME_API extern DBCStorage<ScenarioStepEntry>                  sScenarioStepStore;
TC_GAME_API extern DBCStorage<ServerMessagesEntry>                sServerMessagesStore;
TC_GAME_API extern DBCStorage<SkillLineEntry>                     sSkillLineStore;
TC_GAME_API extern DBCStorage<SkillLineAbilityEntry>              sSkillLineAbilityStore;
TC_GAME_API extern DBCStorage<SkillRaceClassInfoEntry>            sSkillRaceClassInfoStore;
// TODO: DATA FROM DB TC_GAME_API extern DBCStorage<SkillTiersEntry>                  sSkillTiersStore;
TC_GAME_API extern DBCStorage<SoundEntriesEntry>                  sSoundEntriesStore;
TC_GAME_API extern DBCStorage<SpecializationSpellsEntry>          sSpecializationSpellsStore;
TC_GAME_API extern DBCStorage<SpellEntry>                         sSpellStore;
TC_GAME_API extern DBCStorage<SpellAuraOptionsEntry>              sSpellAuraOptionsStore;
TC_GAME_API extern DBCStorage<SpellAuraRestrictionsEntry>         sSpellAuraRestrictionsStore;
TC_GAME_API extern DBCStorage<SpellCastingRequirementsEntry>      sSpellCastingRequirementsStore;
TC_GAME_API extern DBCStorage<SpellCastTimesEntry>                sSpellCastTimesStore;
TC_GAME_API extern DBCStorage<SpellCategoriesEntry>               sSpellCategoriesStore;
TC_GAME_API extern DBCStorage<SpellCategoryEntry>                 sSpellCategoryStore;
TC_GAME_API extern DBCStorage<SpellClassOptionsEntry>             sSpellClassOptionsStore;
TC_GAME_API extern DBCStorage<SpellCooldownsEntry>                sSpellCooldownsStore;
TC_GAME_API extern DBCStorage<SpellDurationEntry>                 sSpellDurationStore;
TC_GAME_API extern DBCStorage<SpellEffectEntry>                   sSpellEffectStore;
TC_GAME_API extern DBCStorage<SpellEffectScalingEntry>            sSpellEffectScalingStore;
TC_GAME_API extern DBCStorage<SpellEquippedItemsEntry>            sSpellEquippedItemsStore;
TC_GAME_API extern DBCStorage<SpellFocusObjectEntry>              sSpellFocusObjectStore;
TC_GAME_API extern DBCStorage<SpellInterruptsEntry>               sSpellInterruptsStore;
TC_GAME_API extern DBCStorage<SpellItemEnchantmentEntry>          sSpellItemEnchantmentStore;
TC_GAME_API extern DBCStorage<SpellItemEnchantmentConditionEntry> sSpellItemEnchantmentConditionStore;
TC_GAME_API extern DBCStorage<SpellKeyboundOverrideEntry>         sSpellKeyboundOverrideStore;
TC_GAME_API extern DBCStorage<SpellLearnSpellEntry>               sSpellLearnSpellStore;
TC_GAME_API extern DBCStorage<SpellLevelsEntry>                   sSpellLevelsStore;
TC_GAME_API extern DBCStorage<SpellMiscEntry>                     sSpellMiscStore;
TC_GAME_API extern DBCStorage<SpellPowerEntry>                    sSpellPowerStore;
TC_GAME_API extern DBCStorage<SpellProcsPerMinuteEntry>           sSpellProcsPerMinuteStore;
TC_GAME_API extern DBCStorage<SpellProcsPerMinuteModEntry>        sSpellProcsPerMinuteModStore;
TC_GAME_API extern DBCStorage<SpellRadiusEntry>                   sSpellRadiusStore;
TC_GAME_API extern DBCStorage<SpellRangeEntry>                    sSpellRangeStore;
// TODO: DATA ALSO DB2? TC_GAME_API extern DBCStorage<SpellReagentsEntry>                 sSpellReagentsStore;
TC_GAME_API extern DBCStorage<SpellRuneCostEntry>                 sSpellRuneCostStore;
TC_GAME_API extern DBCStorage<SpellScalingEntry>                  sSpellScalingStore;
TC_GAME_API extern DBCStorage<SpellShapeshiftEntry>               sSpellShapeshiftStore;
TC_GAME_API extern DBCStorage<SpellShapeshiftFormEntry>           sSpellShapeshiftFormStore;
TC_GAME_API extern DBCStorage<SpellTargetRestrictionsEntry>       sSpellTargetRestrictionsStore;
TC_GAME_API extern DBCStorage<SpellTotemsEntry>                   sSpellTotemsStore;
// TODO: DATA ALSO DB2? TC_GAME_API extern DBCStorage<SpellVisualEntry>                   sSpellVisualStore;
// TODO: DATA ALSO DB2? TC_GAME_API extern DBCStorage<SpellVisualEffectNameEntry>         sSpellVisualEffectNameStore;
// TODO: DATA ALSO DB2? TC_GAME_API extern DBCStorage<SpellVisualKitEntry>                sSpellVisualKitStore;
TC_GAME_API extern DBCStorage<SummonPropertiesEntry>              sSummonPropertiesStore;
TC_GAME_API extern DBCStorage<TalentEntry>                        sTalentStore;
TC_GAME_API extern DBCStorage<TaxiNodesEntry>                     sTaxiNodesStore;
TC_GAME_API extern DBCStorage<TaxiPathEntry>                      sTaxiPathStore;
TC_GAME_API extern DBCStorage<TaxiPathNodeEntry>                  sTaxiPathNodeStore;
TC_GAME_API extern DBCStorage<TotemCategoryEntry>                 sTotemCategoryStore;
TC_GAME_API extern DBCStorage<TransportAnimationEntry>            sTransportAnimationStore;
TC_GAME_API extern DBCStorage<TransportRotationEntry>             sTransportRotationStore;
TC_GAME_API extern DBCStorage<UnitConditionEntry>                 sUnitConditionStore;
TC_GAME_API extern DBCStorage<UnitPowerBarEntry>                  sUnitPowerBarStore;
TC_GAME_API extern DBCStorage<VehicleEntry>                       sVehicleStore;
TC_GAME_API extern DBCStorage<VehicleSeatEntry>                   sVehicleSeatStore;
TC_GAME_API extern DBCStorage<WMOAreaTableEntry>                  sWMOAreaTableStore;
TC_GAME_API extern DBCStorage<WorldEffectEntry>                   sWorldEffectStore;
TC_GAME_API extern DBCStorage<WorldMapAreaEntry>                  sWorldMapAreaStore;
TC_GAME_API extern DBCStorage<WorldMapOverlayEntry>               sWorldMapOverlayStore;
TC_GAME_API extern DBCStorage<WorldMapTransformsEntry>            sWorldMapTransformsStore;
TC_GAME_API extern DBCStorage<WorldSafeLocsEntry>                 sWorldSafeLocsStore;
TC_GAME_API extern DBCStorage<WorldStateExpressionEntry>          sWorldStateExpressionStore;
// TODO: DATA Meta entries ???


TC_GAME_API void LoadDBCStores(const std::string& dataPath, uint32 defaultLocale);

#endif
