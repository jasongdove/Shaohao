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

class TC_GAME_API DBCManager
{
public:
    DEFINE_DB2_SET_COMPARATOR(FriendshipRepReactionEntry)

    using FriendshipRepReactionSet = std::set<FriendshipRepReactionEntry const*, FriendshipRepReactionEntryComparator>;

    static DBCManager& Instance();

    uint32 LoadStores(const std::string &dataPath, uint32 defaultLocale);

    uint32 GetEmptyAnimStateID() const;
    MapDifficultyEntry const* GetDefaultMapDifficulty(uint32 mapId, Difficulty* difficulty = nullptr) const;
    MapDifficultyEntry const* GetMapDifficultyData(uint32 mapId, Difficulty difficulty) const;
    MapDifficultyEntry const* GetDownscaledMapDifficultyData(uint32 mapId, Difficulty &difficulty) const;
    static char const* GetChrRaceName(uint8 race, LocaleConstant locale = DEFAULT_LOCALE);
    static char const* GetClassName(uint8 class_, LocaleConstant locale = DEFAULT_LOCALE);
    static char const* GetCreatureFamilyPetName(uint32 petfamily, LocaleConstant locale);
    std::vector<uint32> const* GetFactionTeamList(uint32 faction) const;
    FriendshipRepReactionSet const* GetFriendshipRepReactions(uint32 friendshipRepID) const;
    ItemClassEntry const* GetItemClassByOldEnum(uint32 itemClass) const;
    std::vector<ItemSpecOverrideEntry const*> const* GetItemSpecOverrides(uint32 itemId) const;
    JournalTierEntry const* GetJournalTier(uint32 index) const;
    static LFGDungeonsEntry const* GetLfgDungeon(uint32 mapId, Difficulty difficulty);
    static uint32 GetLiquidFlags(uint32 liquidType);
    std::string GetNameGenEntry(uint8 race, uint8 gender) const;
    ResponseCodes ValidateName(std::wstring const& name, LocaleConstant locale) const;
    uint32 GetQuestUniqueBitFlag(uint32 questId);
    std::vector<uint32> const* GetPhasesForGroup(uint32 group) const;
    std::vector<SkillLineEntry const*> const* GetSkillLinesForParentSkill(uint32 parentSkillId) const;
    std::vector<SpecializationSpellsEntry const*> const* GetSpecializationSpells(uint32 specId) const;
    static bool IsValidSpellFamilyName(SpellFamilyNames family);
    std::vector<SpellProcsPerMinuteModEntry const*> GetSpellProcsPerMinuteMods(uint32 spellprocsPerMinuteId) const;
    std::vector<TalentEntry const*> const& GetTalentsByPosition(uint32 class_, uint32 tier, uint32 column) const;
    static bool IsTotemCategoryCompatibleWith(uint32 itemTotemCategoryId, uint32 requiredTotemCategoryId, bool requireAllTotems = true);
    WMOAreaTableEntry const* GetWMOAreaTable(int32 rootId, int32 adtId, int32 groupId) const;
    ChrSpecializationEntry const* GetChrSpecializationByIndex(uint32 class_, uint32 index) const;
    ChrSpecializationEntry const* GetDefaultChrSpecializationForClass(uint32 class_) const;
    SkillRaceClassInfoEntry const* GetSkillRaceClassInfo(uint32 skill, uint8 race, uint8 class_) const;
    std::vector<SkillRaceClassInfoEntry const*> GetSkillRaceClassInfo(uint32 skill) const;
    TaxiPathEntry const* GetTaxiPath(uint32 from, uint32 to) const;
    static bool IsInArea(uint32 objectAreaId, uint32 areaId);
    static bool GetUiMapPosition(float x, float y, float z, int32 mapId, int32 areaId, int32 wmoDoodadPlacementId, int32 wmoGroupId, UiMapSystem system, bool local,
                                 int32* uiMapId = nullptr, DBCPosition2D* newPos = nullptr);
    bool Zone2MapCoordinates(uint32 areaId, float& x, float& y) const;
    void Map2ZoneCoordinates(uint32 areaId, float& x, float& y) const;
};

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
//TC_GAME_API extern MapDifficultyMap                               sMapDifficultyMap;
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

#define sDBCManager DBCManager::Instance()

#endif
