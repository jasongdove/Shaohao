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

#ifndef TRINITY_DBCSFRM_H
#define TRINITY_DBCSFRM_H

// x - skip<uint32>, X - skip<uint8>, s - char*, f - float, i - uint32, b - uint8, d - index (not included)
// n - index (included), l - uint64, p - field present in sql dbc, a - field absent in sql dbc

char const AchievementCriteriafmt[] = "niiiixiiiisiiiiixxiiiii";
char const Achievementfmt[] = "niiissiiiiisiii";
char const AnimKitfmt[] = "nxxx";
char const AreaTablefmt[] = "niiiiixxxxxxisiiiiixxxixxxxiii";
char const AreaTriggerfmt[] = "nifffxxxfffffxxxx";
char const ArmorLocationfmt[] = "nfffff";
char const AuctionHousefmt[] = "niiis";
char const BankBagSlotPricesfmt[] = "ni";
char const BannedAddOnsfmt[] = "nxxxxxxxxxx";
char const BarberShopStylefmt[] = "nixxxiii";
char const BattlemasterListfmt[] = "niiiiiiiiiiiiiiiiixsiiiixxxxxxx";
char const CharacterLoadoutfmt[] = "niii";
char const CharacterLoadoutItemfmt[] = "niixx";
char const CharSectionsfmt[] = "diiixxxiii";
char const CharStartOutfitfmt[] = "dbbbXiiiiiiiiiiiiiiiiiiiiiiiixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxii";
char const CharTitlesfmt[] = "nxssix";
char const ChatChannelsfmt[] = "nixsx";
char const ChrClassesfmt[] = "nixsxxxiiiiiiixxxx";
char const ChrRacesfmt[] = "niixiixxiixxiisxxxxxxixxxxxxxxxxxxxx";
char const ChrSpecializationfmt[] = "nxiiiiiiiiisxi";
char const CinematicCamerafmt[] = "nsiffff";
char const CinematicSequencesfmt[] = "nxiiiiiiii";
char const CreatureDisplayInfoExtrafmt[] = "dixxxxxxxxxxxxxxxxxxxx";
char const CreatureDisplayInfofmt[] = "niiifxxxxxxxxxxxxiii";
char const CreatureFamilyfmt[] = "nfifiiiiixsx";
char const CreatureImmunitiesfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const CreatureModelDatafmt[] = "niixxxxxxxxxxxxffxxxxxxxxxxxxxxxxx";
char const CreatureSpellDatafmt[] = "niiiixxxx";
char const CreatureTypefmt[] = "nxx";
char const Criteriafmt[] = "niiiiiiiiiii";
char const CriteriaTreefmt[] = "niixiiis";
char const CurrencyTypesfmt[] = "nisxxiiiiixx";
char const DestructibleModelDatafmt[] = "nixxxixxxxixxxxixxxxixxx";
char const DifficultyFmt[] = "niiiiiiixxxs";
char const DungeonEncounterfmt[] = "niiixsxxx";
char const DurabilityCostsfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiii";
char const DurabilityQualityfmt[] = "nf";
char const Emotesfmt[] = "nxiiiixx";
char const EmotesTextfmt[] = "nxixxxxxxxxxxxxxxxx";
char const EmotesTextSoundfmt[] = "niiii";
char const Factionfmt[] = "niiiiiiiiiiiiiiiiiiffiisxixi";
char const FactionTemplatefmt[] = "niiiiiiiiiiiii";
char const GameObjectDisplayInfofmt[] = "nixxxxxxxxxxffffffxxx";
char const GameTablesfmt[] = "nsii";
char const GemPropertiesfmt[] = "nixxii";
char const GlyphPropertiesfmt[] = "niiix";
char const GtBarberShopCostBasefmt[] = "xf";
char const GtChanceToMeleeCritBasefmt[] = "xf";
char const GtChanceToMeleeCritfmt[] = "xf";
char const GtChanceToSpellCritBasefmt[] = "xf";
char const GtChanceToSpellCritfmt[] = "xf";
char const GtCombatRatingsfmt[] = "xf";
char const GtItemSocketCostPerLevelfmt[] = "xf";
char const GtNPCManaCostScalerfmt[] = "xf";
char const GtNpcTotalHpExp1fmt[] = "xf";
char const GtNpcTotalHpExp2fmt[] = "xf";
char const GtNpcTotalHpExp3fmt[] = "xf";
char const GtNpcTotalHpExp4fmt[] = "xf";
char const GtNpcTotalHpExp5fmt[] = "xf";
char const GtNpcTotalHpfmt[] = "xf";
char const GtOCTBaseHPByClassfmt[] = "df";
char const GtOCTBaseMPByClassfmt[] = "df";
char const GtOCTHpPerStaminafmt[] = "df";
char const GtOCTLevelExperiencefmt[] = "xf";
char const GtRegenMPPerSptfmt[] = "xf";
char const GtSpellScalingfmt[] = "df";
char const GuildColorBackgroundfmt[] = "nXXX";
char const GuildColorBorderfmt[] = "nXXX";
char const GuildColorEmblemfmt[] = "nXXX";
char const Holidaysfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiixxsiix";
char const ImportPriceArmorfmt[] = "nffff";
char const ImportPriceQualityfmt[] = "nf";
char const ImportPriceShieldfmt[] = "nf";
char const ImportPriceWeaponfmt[] = "nf";
char const ItemArmorQualityfmt[] = "nfffffffi";
char const ItemArmorShieldfmt[] = "nifffffff";
char const ItemArmorTotalfmt[] = "niffff";
char const ItemBagFamilyfmt[] = "nx";
char const ItemClassfmt[] = "difx";
char const ItemDamagefmt[] = "nfffffffi";
char const ItemDisenchantLootfmt[] = "niiiiii";
char const ItemDisplayInfofmt[] = "nxxxxsxxxxxxxxxxxxxxxxxxxx";
char const ItemLimitCategoryfmt[] = "nxii";
char const ItemNameDescriptionfmt[] = "nsi";
char const ItemRandomPropertiesfmt[] = "nxiiixxs";
char const ItemRandomSuffixfmt[] = "nsxiiiiiiiiii";
char const ItemSetfmt[] = "dsiiiiiiiiiixxxxxxxiiiiiiiiiiiiiiiiii";
char const ItemSpecfmt[] = "xiiiiii";
char const ItemSpecOverridefmt[] = "xii";
char const LFGDungeonfmt[] = "nsiiixxiiiixxixixxxxxxxxxxxxxx";
char const Lightfmt[] = "nifffxxxxxxxxxx";
char const LiquidTypefmt[] = "nxxixixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const Lockfmt[] = "niiiiiiiiiiiiiiiiiiiiiiiixxxxxxxx";
char const MailTemplatefmt[] = "nss";
char const MapDifficultyfmt[] = "diisiiii";
char const Mapfmt[] = "nxiixsixxixiffxiiii";
char const MinorTalentfmt[] = "niii";
char const ModifierTreefmt[] = "niiiixi";
char const MountCapabilityfmt[] = "niiiiiii";
char const MountTypefmt[] = "niiiiiiiiiiiiiiiiiiiiiiii";
char const Moviefmt[] = "nxxxx";
char const NameGenfmt[] = "dsii";
char const OverrideSpellDatafmt[] = "niiiiiiiiiixx";
char const Phasefmt[] = "nxi";
char const PlayerConditionfmt[] = "nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxiiiiiiiiiixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const PowerDisplayfmt[] = "nixXXX";
char const PvpDifficultyfmt[] = "diiii";
char const QuestFactionRewardfmt[] = "niiiiiiiiii";
char const QuestSortfmt[] = "nx";
char const QuestV2fmt[] = "ni";
char const QuestXPfmt[] = "niiiiiiiiii";
char const RandPropPointsfmt[] = "niiiiiiiiiiiiiii";
char const ResearchBranchfmt [] = "nxxixi";
char const ResearchProjectfmt [] = "nxxiiiixi";
char const ResearchSitefmt [] = "niisx";
char const Scenariofmt[] = "nxi";
char const ScenarioStepfmt[] = "niiixxi";
char const SkillLineAbilityfmt[] = "niiiiiiiiiiii";
char const SkillLinefmt[] = "nisxixiii";
char const SkillRaceClassInfofmt[] = "diiiiiii";
char const SoundEntriesfmt[] = "nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
char const SpecializationSpellsfmt[] = "niiix";
char const SpellAuraOptionsfmt[] = "niiiiiiii";
char const SpellAuraRestrictionsfmt[] = "dxxiiiiiiii";
char const SpellCastingRequirementsfmt[] = "dixxixi";
char const SpellCategoriesfmt[] = "diiiiiiiii";
char const SpellCategoryfmt[] = "nixxii";
char const SpellCooldownsfmt[] = "diiiii";
char const SpellEffectfmt[] = "niifiiiffiiiiiifiifiiiiifiiiii";
const std::string CustomSpellEffectfmt = "ppppppppppppppappppppppppp";
const std::string CustomSpellEffectIndex = "Id";
char const SpellEffectScalingfmt[] = "nfffi";
char const SpellEquippedItemsfmt[] = "diiiii";
char const Spellfmt[] = "nsxxxiiifiiiiiiiiiiiiiiii";
const std::string CustomSpellfmt = "ppppppppppppppapaaaaaaaaapaaaaaapapppaapppaaapa";
const std::string CustomSpellIndex = "Id";
char const SpellFocusObjectfmt[] = "nx";
char const SpellInterruptsfmt[] = "diiiiiii";
char const SpellItemEnchantmentfmt[] = "niiiiiiiiiixiiiiiiiiiiifff";
char const SpellLevelsfmt[] = "diiiii";
char const SpellMiscfmt[] = "nxxiiiiiiiiiiiiiiiiifiiiii";
const std::string CustomSpellMiscfmt = "paapppppppppppppppppaaaaapa";
const std::string CustomSpellMiscIndex = "Id";
char const SpellPowerfmt[] = "nixiiiixxffix";
char const SpellProcsPerMinutefmt[] = "nii";
char const SpellProcsPerMinuteModfmt[] = "niifi";
char const SpellRadiusfmt[] = "nffff";
char const SpellRangefmt[] = "nffffixx";
char const SpellRuneCostfmt[] = "niiiii";
char const SpellScalingfmt[] = "niiiifiii";
char const SpellShapeshiftfmt[] = "niiiix";
char const SpellShapeshiftFormfmt[] = "nxxiixiiiiiiiiiiiiixx";
char const SpellTargetRestrictionsfmt[] = "niiffiiii";
char const SpellTotemsfmt[] = "niiii";
char const SummonPropertiesfmt[] = "niiiii";
char const Talentfmt[] = "niiiiiiiiix";
char const TaxiNodesfmt[] = "nifffsiiiiff";
char const TaxiPathfmt[] = "niii";
char const TaxiPathNodefmt[] = "niiifffiiii";
char const TotemCategoryfmt[] = "nxii";
char const TransportAnimationfmt[] = "diifffx";
char const TransportRotationfmt[] = "diiffff";
char const UnitPowerBarfmt[] = "niixxxxxxxxxxxxxxxxxxxxxxxx";
char const Vehiclefmt[] = "niiffffiiiiiiiifffffffffffffffxxxxfifiiii";
char const VehicleSeatfmt[] = "niiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiiffffffffffffiiiiiiiii";
char const WMOAreaTablefmt[] = "niiixxxxxiixxxx";
char const WorldMapAreafmt[] = "xinxffffixxxxx";
char const WorldMapOverlayfmt[] = "nxiiiixxxxxxxxxx";
char const WorldMapTransformsfmt[] = "diffffffiffxxxf";
char const WorldSafeLocsfmt[] = "niffffx";

#endif
