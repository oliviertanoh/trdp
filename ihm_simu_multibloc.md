# Simu Multibloc - Spécification de l'API REST

## Description

Ce document décrit les exigences de l'API REST que le simulateur Multibloc doit exposer pour l'IHM.

Le simulateur Multibloc gère des blocs simulés sur un bus CAN (250 kbps). Chaque bloc possède un identifiant (BlocId), un type, et un ensemble de paramètres qu'il échange sur le réseau.

### Structure d'un BlocId

Chaque bloc est identifié par :

| Élément | Type | Description |
|---|---|---|
| type | string | Type du bloc (BLOC_7, BLOC_9, BLOC_11, etc.) |
| coding | int | Adresse/codage CAN du bloc |
| subnet | int | Numéro de sous-réseau CAN |

### Structure de configuration d'un bloc

| Élément | Type | Description |
|---|---|---|
| type | string | Type du bloc |
| coding | int | Adresse/codage CAN |
| subnet | int | Sous-réseau CAN |
| version | string | Version firmware (format major.minor.patch) |
| lids | list | Descripteurs logiques optionnels |

### Types de blocs supportés

| Type | Description |
|---|---|
| BLOC_7 | 2x entrées digitales, 7x entrées analogiques, 3x sorties digitales, 1x IBS (optionnel), 3x compteurs |
| BLOC_9 | 1x entrée analogique, 6x sorties digitales, 1x IBS (optionnel) |
| BLOC_11 | 2x entrées analogiques, 12x entrées digitales, 7x sorties digitales |
| BLOC_AC1 | 2x entrées analogiques, 2x entrées digitales |
| SELECTEUR_AC1 | 6x entrées analogiques, 5x entrées digitales, 2x sorties digitales, 1x position sélecteur, 1x état groupe électrogène |
| SELECTEUR_MDP | 6x entrées analogiques, 5x entrées digitales, 2x sorties digitales, 1x position MDP |
| CHARGEUR_CRISTEC | Infos chargeur (vout, iout, vac, vacTriphase, voltageCaliber, currentCaliber, version) |
| CHARGEUR_MDP | Infos chargeur (vout, iout, vac, vacTriphase, voltageCaliber, currentCaliber, version) |
| NAVICOLOR_GT2 | 1x IBS v850 (optionnel, via lids) |
| INTERFACE_CAN_CAN_NMEA2000 | 18x paramètres CAN-CAN NMEA2000 |
| CLIMATISEUR | 1x commande climatisation (variantes : DOMETIC, WEBASTO, FRIGOMAR, TERMODINAMICA) |
| CHILLER | 1x commande chiller (variantes : TERMODINAMICA, WEBASTO, FRIGOMAR) |

---

## Gestion des blocs

#### REQ-MULTI-000
L'API REST doit avoir une route qui permet de créer un ensemble de blocs simulés en spécifiant une liste de configurations (type, coding, subnet, version, lids).

#### REQ-MULTI-001
L'API REST doit avoir une route qui permet de lister tous les blocs simulés avec leur BlocId (type, coding, subnet).

#### REQ-MULTI-002
L'API REST doit avoir une route qui permet de récupérer les informations d'un bloc par son BlocId.

#### REQ-MULTI-003
L'API REST doit avoir une route qui permet d'activer un bloc.

#### REQ-MULTI-004
L'API REST doit avoir une route qui permet de désactiver un bloc.

#### REQ-MULTI-005
L'API REST doit avoir une route qui permet d'activer tous les blocs simultanément.

#### REQ-MULTI-006
L'API REST doit avoir une route qui permet de désactiver tous les blocs simultanément.

#### REQ-MULTI-007
L'API REST doit avoir une route qui permet de réinitialiser tous les paramètres de tous les blocs à leurs valeurs par défaut.

#### REQ-MULTI-008
L'API REST doit avoir une route qui permet de réinitialiser tous les paramètres d'un bloc à ses valeurs par défaut.

---

## Sorties digitales

Blocs concernés : BLOC_7, BLOC_9, BLOC_11, SELECTEUR_AC1, SELECTEUR_MDP


#### REQ-MULTI-009
L'API REST doit avoir une route qui permet de récupérer l'état d'une sortie digitale d'un bloc.

#### REQ-MULTI-010
L'API REST doit avoir une route qui permet de récupérer l'état de toutes les sorties digitales d'un bloc.

#### REQ-MULTI-011
L'API REST doit avoir une route qui permet de définir l'état d'une sortie digitale d'un bloc.

#### REQ-MULTI-012
L'API REST doit avoir une route qui permet de définir l'état de toutes les sorties digitales d'un bloc en une seule requête.

#### REQ-MULTI-013
L'API REST doit avoir une route qui permet de récupérer la valeur PWM d'une sortie digitale d'un bloc.


---

## Entrées analogiques

Blocs concernés : BLOC_7, BLOC_9, BLOC_11, BLOC_AC1, SELECTEUR_AC1, SELECTEUR_MDP


#### REQ-MULTI-014
L'API REST doit avoir une route qui permet de récupérer la valeur d'une entrée analogique d'un bloc.

#### REQ-MULTI-015
L'API REST doit avoir une route qui permet de récupérer la valeur de toutes les entrées analogiques d'un bloc.

#### REQ-MULTI-016
L'API REST doit avoir une route qui permet de définir la valeur d'une entrée analogique d'un bloc.

#### REQ-MULTI-017
L'API REST doit avoir une route qui permet de définir la valeur de toutes les entrées analogiques d'un bloc en une seule requête.

---

## Entrées digitales

Blocs concernés : BLOC_7, BLOC_11, BLOC_AC1, SELECTEUR_AC1, SELECTEUR_MDP


#### REQ-MULTI-018
L'API REST doit avoir une route qui permet de récupérer la valeur d'une entrée digitale d'un bloc.

#### REQ-MULTI-019
L'API REST doit avoir une route qui permet de récupérer la valeur de toutes les entrées digitales d'un bloc.

#### REQ-MULTI-020
L'API REST doit avoir une route qui permet de définir la valeur d'une entrée digitale d'un bloc.

#### REQ-MULTI-021
L'API REST doit avoir une route qui permet de définir la valeur de toutes les entrées digitales d'un bloc en une seule requête.

---

## Compteurs

Blocs concernés : BLOC_7

Chaque compteur possède les paramètres : frequency, value.

#### REQ-MULTI-022
L'API REST doit avoir une route qui permet de récupérer la valeur d'un compteur d'un bloc.

#### REQ-MULTI-023
L'API REST doit avoir une route qui permet de récupérer la valeur de tous les compteurs d'un bloc.

#### REQ-MULTI-024
L'API REST doit avoir une route qui permet de définir la valeur d'un compteur d'un bloc (frequency et/ou value).

---

## Sélecteur AC1

Blocs concernés : SELECTEUR_AC1

#### REQ-MULTI-025
L'API REST doit avoir une route qui permet de récupérer la position du sélecteur (position, locked).

#### REQ-MULTI-026
L'API REST doit avoir une route qui permet de définir la position du sélecteur.

#### REQ-MULTI-027
L'API REST doit avoir une route qui permet de récupérer l'état du groupe électrogène (state).

#### REQ-MULTI-028
L'API REST doit avoir une route qui permet de définir l'état du groupe électrogène.

---

## Sélecteur MDP

Blocs concernés : SELECTEUR_MDP

#### REQ-MULTI-029
L'API REST doit avoir une route qui permet de récupérer la position MDP (position, locked).

#### REQ-MULTI-030
L'API REST doit avoir une route qui permet de définir la position MDP.

---

## Navicolor GT2

Blocs concernés : NAVICOLOR_GT2

Paramètres IBS v850 : voltage, current, temperature, centralTapeVoltage, stateOfChargeComputed, stateOfHealth, dischargeableAhComputed, availableCapacity, nominalCapacity, batteryType, ibsType, dischargeableAh, stateOfCharge, offsetSoc.

#### REQ-MULTI-031
L'API REST doit avoir une route qui permet de récupérer tous les paramètres IBS v850 du Navicolor GT2.

#### REQ-MULTI-032
L'API REST doit avoir une route qui permet de mettre à jour tous les paramètres IBS v850 du Navicolor GT2 en une seule requête.

#### REQ-MULTI-033
L'API REST doit avoir une route qui permet de récupérer un paramètre IBS v850 spécifique (ex: voltage, stateOfCharge, temperature).

#### REQ-MULTI-034
L'API REST doit avoir une route qui permet de mettre à jour un paramètre IBS v850 spécifique.

---

## Climatiseur

Blocs concernés : CLIMATISEUR_DOMETIC, CLIMATISEUR_WEBASTO, CLIMATISEUR_FRIGOMAR, CLIMATISEUR_TERMODINAMICA

Paramètres : fanspeed, mode, setpoint, auxHeaterStatus, temperature.

#### REQ-MULTI-035
L'API REST doit avoir une route qui permet de récupérer tous les paramètres du climatiseur d'un bloc.

#### REQ-MULTI-036
L'API REST doit avoir une route qui permet de mettre à jour tous les paramètres du climatiseur d'un bloc en une seule requête.

#### REQ-MULTI-037
L'API REST doit avoir une route qui permet de récupérer le mode du climatiseur.

#### REQ-MULTI-038
L'API REST doit avoir une route qui permet de mettre à jour le mode du climatiseur.

#### REQ-MULTI-039
L'API REST doit avoir une route qui permet de récupérer le fanspeed du climatiseur.

#### REQ-MULTI-040
L'API REST doit avoir une route qui permet de mettre à jour le fanspeed du climatiseur.

#### REQ-MULTI-041
L'API REST doit avoir une route qui permet de récupérer le setpoint du climatiseur.

#### REQ-MULTI-042
L'API REST doit avoir une route qui permet de mettre à jour le setpoint du climatiseur.

#### REQ-MULTI-043
L'API REST doit avoir une route qui permet de récupérer l'auxHeaterStatus du climatiseur.

#### REQ-MULTI-044
L'API REST doit avoir une route qui permet de mettre à jour l'auxHeaterStatus du climatiseur.

#### REQ-MULTI-045
L'API REST doit avoir une route qui permet de récupérer la temperature du climatiseur.

#### REQ-MULTI-046
L'API REST doit avoir une route qui permet de mettre à jour la temperature du climatiseur.

---

## Chiller

Blocs concernés : CHILLER_TERMODINAMICA, CHILLER_WEBASTO, CHILLER_FRIGOMAR

Paramètres : ecoMode, auxHeater, mode, globalOff.

#### REQ-MULTI-047
L'API REST doit avoir une route qui permet de récupérer tous les paramètres du chiller d'un bloc.

#### REQ-MULTI-048
L'API REST doit avoir une route qui permet de mettre à jour tous les paramètres du chiller d'un bloc en une seule requête.

#### REQ-MULTI-049
L'API REST doit avoir une route qui permet de récupérer le mode du chiller.

#### REQ-MULTI-050
L'API REST doit avoir une route qui permet de mettre à jour le mode du chiller.

#### REQ-MULTI-051
L'API REST doit avoir une route qui permet de récupérer l'ecoMode du chiller.

#### REQ-MULTI-052
L'API REST doit avoir une route qui permet de mettre à jour l'ecoMode du chiller.

#### REQ-MULTI-053
L'API REST doit avoir une route qui permet de récupérer l'auxHeater du chiller.

#### REQ-MULTI-054
L'API REST doit avoir une route qui permet de mettre à jour l'auxHeater du chiller.

#### REQ-MULTI-055
L'API REST doit avoir une route qui permet de récupérer le globalOff du chiller.

#### REQ-MULTI-056
L'API REST doit avoir une route qui permet de mettre à jour le globalOff du chiller.

---

## Chargeur

Blocs concernés : CHARGEUR_CRISTEC, CHARGEUR_MDP

Paramètres chargerInfo1 : vout, iout, vac, vacTriphase.
Paramètres chargerInfo2 : vout2, vout3, voltageCaliber, currentCaliber, version.

#### REQ-MULTI-057
L'API REST doit avoir une route qui permet de récupérer tous les paramètres du chargeur d'un bloc (chargerInfo1 et chargerInfo2).

#### REQ-MULTI-058
L'API REST doit avoir une route qui permet de mettre à jour tous les paramètres du chargeur d'un bloc en une seule requête.

#### REQ-MULTI-059
L'API REST doit avoir une route qui permet de récupérer le vout du chargeur.

#### REQ-MULTI-060
L'API REST doit avoir une route qui permet de mettre à jour le vout du chargeur.

#### REQ-MULTI-061
L'API REST doit avoir une route qui permet de récupérer le iout du chargeur.

#### REQ-MULTI-062
L'API REST doit avoir une route qui permet de mettre à jour le iout du chargeur.

#### REQ-MULTI-063
L'API REST doit avoir une route qui permet de récupérer le vac du chargeur.

#### REQ-MULTI-064
L'API REST doit avoir une route qui permet de mettre à jour le vac du chargeur.

#### REQ-MULTI-065
L'API REST doit avoir une route qui permet de récupérer le vacTriphase du chargeur.

#### REQ-MULTI-066
L'API REST doit avoir une route qui permet de mettre à jour le vacTriphase du chargeur.

#### REQ-MULTI-067
L'API REST doit avoir une route qui permet de récupérer le vout2 du chargeur.

#### REQ-MULTI-068
L'API REST doit avoir une route qui permet de mettre à jour le vout2 du chargeur.

#### REQ-MULTI-069
L'API REST doit avoir une route qui permet de récupérer le vout3 du chargeur.

#### REQ-MULTI-070
L'API REST doit avoir une route qui permet de mettre à jour le vout3 du chargeur.

#### REQ-MULTI-071
L'API REST doit avoir une route qui permet de récupérer le voltageCaliber du chargeur.

#### REQ-MULTI-072
L'API REST doit avoir une route qui permet de mettre à jour le voltageCaliber du chargeur.

#### REQ-MULTI-073
L'API REST doit avoir une route qui permet de récupérer le currentCaliber du chargeur.

#### REQ-MULTI-074
L'API REST doit avoir une route qui permet de mettre à jour le currentCaliber du chargeur.

#### REQ-MULTI-075
L'API REST doit avoir une route qui permet de récupérer la version du chargeur.

#### REQ-MULTI-076
L'API REST doit avoir une route qui permet de mettre à jour la version du chargeur.

---
