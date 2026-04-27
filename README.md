# ETRS606 — IA embarquée sur STM32N657

Dépôt du module **ETRS606** (Licence ESET, Université Savoie Mont-Blanc).  
Le module couvre l'ensemble de la chaîne IA embarquée : construction et entraînement de réseaux de neurones sous TensorFlow/Keras, conversion en TFLite, puis déploiement sur microcontrôleur STM32 via STM32Cube.AI.

---

## Structure du dépôt

```
ETRS_606/
├── TP1/   # Réseaux de neurones avec TensorFlow/Keras (Google Colab)
├── TP2/   # Prise en main du STM32N657 avec TrustZone
├── TP3/   # Modèle météo TFLite — entraînement et export
└── TP4/   # Communication TCP avec ThreadX/NetXDuo
```

---

## TP1 — Exploration des réseaux de neurones denses (Keras / MNIST)

**Objectif :** comprendre l'impact des hyperparamètres d'un réseau de neurones entièrement connecté sur les performances de classification.

Les travaux ont été réalisés sur **Google Colab** avec TensorFlow/Keras, sur le jeu de données **MNIST** (classification de chiffres manuscrits 0–9).

### Exercice 1 — Fonctions d'activation (`Exo_1.ipynb`)

Comparaison de différentes fonctions d'activation (ReLU, Sigmoid, Tanh, Softmax) sur une architecture Dense identique. Pour chaque configuration, les courbes d'accuracy et de loss (entraînement / validation) ont été tracées et exportées dans `Comparaison_fonction_activation.odg`.

### Exercice 2 — Algorithmes d'optimisation (`Exo_2.ipynb`)

Comparaison des optimiseurs (SGD, Adam, RMSprop, etc.) à architecture et fonction de coût fixées. L'effet sur la vitesse de convergence et la précision finale est analysé et visualisé dans `Comparaison_algo_optimisation.odg`.

### Exercice 3 — Fonctions de coût (`Exo_3.ipynb`)

Comparaison des fonctions de perte (binary cross-entropy, categorical cross-entropy, mean squared error) sur le problème de classification MNIST. Les résultats sont consolidés dans `Comparaison_fonction_cout.odg`.

### Fichiers

| Fichier | Description |
|---|---|
| `Exo_1.ipynb` | Notebook — comparaison des fonctions d'activation |
| `Exo_2.ipynb` | Notebook — comparaison des optimiseurs |
| `Exo_3.ipynb` | Notebook — comparaison des fonctions de coût |
| `test.ipynb` | Notebook de tests divers |
| `Comparaison_fonction_activation.odg` | Graphes comparatifs des fonctions d'activation |
| `Comparaison_algo_optimisation.odg` | Graphes comparatifs des algorithmes d'optimisation |
| `Comparaison_fonction_cout.odg` | Graphes comparatifs des fonctions de coût |
| `ETRS606_TP1.pdf` | Sujet du TP |

---

## TP2 — Prise en main du STM32N657 avec TrustZone

**Cible matérielle :** carte Nucleo-N657X0-Q (STM32N657, Cortex-M55 avec TrustZone ARMv8-M).  
**Environnement :** STM32CubeIDE, HAL, BSP STM32N6xx_Nucleo.

Chaque projet TP2 suit l'architecture à trois sous-projets imposée par TrustZone sur le STM32N6xx :

- **FSBL** (*First Stage Boot Loader*) — s'exécute en AXISRAM2, configure les horloges et transfère l'exécution à AppliSecure.
- **AppliSecure** — configure la partition de sécurité (SAU, RIFSC/RIF), initialise les périphériques sécurisés et démarre AppliNonSecure via `NonSecure_Init()`.
- **AppliNonSecure** — contient le code applicatif principal.

### Sous-projet `depression/` — Prise en main TrustZone

Premier projet de découverte de l'architecture multi-world du STM32N657.

- Configuration du FSBL : initialisation des horloges système, du BSP (bouton utilisateur, UART COM1 à 115 200 bps), affichage d'un message de démarrage via `printf`.
- AppliSecure : configuration minimale de la partition de sécurité avec `SystemIsolation_Config()`, démarrage de AppliNonSecure.
- AppliNonSecure : squelette vide (boucle principale sans périphérique).

L'objectif est de valider la chaîne de boot complète FSBL → Secure → NonSecure et de comprendre la configuration mémoire (`STM32N657X0HXQ_AXISRAM2_fsbl.ld`, `STM32N657X0HXQ_LRUN_s.ld`, `STM32N657X0HXQ_LRUN.ld`).

### Sous-projet `UART/` — Communication série dans AppliNonSecure

- FSBL et AppliSecure configurés de manière identique au projet `depression`.
- AppliNonSecure : initialisation de **USART2** (115 200 bps, 8N1) via HAL et MX_USART2_UART_Init(). Redirection de `printf` vers USART2 depuis le monde non sécurisé. Initialisation et allumage d'une LED via `NS_LED_InitAndOn()`.
- Objectif : valider l'accès aux périphériques UART depuis le monde non sécurisé et établir un canal de debug serie.

### Sous-projet `capteurs/` — Lecture de 6 capteurs I2C

Projet principal du TP2. Lecture en polling de l'ensemble des capteurs présents sur le shield Nucleo via le bus **I2C1**.

Les drivers ST utilisés (API `_reg.h`/`_reg.c`) sont les suivants :

| Capteur | Grandeur mesurée | Driver |
|---|---|---|
| HTS221 | Humidité relative + température | `hts221_reg` |
| LIS2DW12 | Accélération 3 axes | `lis2dw12_reg` |
| LIS2MDL | Champ magnétique 3 axes | `lis2mdl_reg` |
| LPS22HH | Pression atmosphérique | `lps22hh_reg` |
| LSM6DSO | Accéléromètre + gyroscope 6 axes (IMU) | `lsm6dso_reg` |
| STTS751 | Température | `stts751_reg` |

Dans AppliNonSecure, chaque capteur dispose de sa propre fonction de lecture polling (`hts221_read_data_polling()`, `lsm6dso_read_data_polling()`, etc.). La fonction `check_sensors_whoami()` vérifie le registre WHO_AM_I de chaque capteur au démarrage. Les données lues sont transmises sur USART2 via `printf`.

AppliSecure configure en plus la partition RIF pour autoriser l'accès au bus I2C1 depuis le monde non sécurisé.

### Fichiers

| Répertoire | Description |
|---|---|
| `TP2/depression/` | Projet de prise en main TrustZone |
| `TP2/UART/` | Projet UART dans AppliNonSecure |
| `TP2/capteurs/` | Projet lecture de 6 capteurs I2C |
| `TP2/Drivers/` | Drivers ST pour chaque capteur (lsm6dso, lis2mdl, lis2dw12, hts221, lps22hh, stts751) |
| `ETRS606_TP2.pdf` | Sujet du TP |

---

## TP3 — Modèle météo TFLite (entraînement et export)

**Objectif :** entraîner un réseau de neurones dense pour la classification météo à partir de données capteurs, puis l'exporter en TFLite en vue d'un déploiement sur STM32N657 (Neural-ART NPU via STM32Cube.AI).

Les notebooks ont été exécutés sur **Google Colab**.

### Données

Les données météo historiques (2021–2025) ont été récupérées via la bibliothèque **Meteostat** pour une station proche de Chambéry (coordonnées : 45.64° N, 5.87° E, alt. 113 m). Les données horaires interpolées fournissent les entrées du modèle : température, pression atmosphérique et humidité relative.

### Modèles entraînés

Deux versions du modèle ont été développées :

**Modèle binaire — prédiction de pluie (`rain_model.tflite`)**  
Classifieur binaire (pluie / pas de pluie) avec une architecture Dense simple. Entrées : température, pression, humidité (+ valeurs au pas de temps précédent t−1), soit un vecteur de dimension 10 (float32). Sortie : probabilité de pluie.

**Modèle multiclasse — reconnaissance météo (`meteo_multiclasse.tflite`)**  
Classification en 13 catégories météo (clair/ensoleillé, peu nuageux, partiellement nuageux, etc.). Même format d'entrée (10 features). Sortie : vecteur de 13 probabilités (softmax). Entraînement avec gestion du déséquilibre de classes (`compute_class_weight`), normalisation des entrées par `StandardScaler`.

### Export TFLite et validation

Après entraînement Keras, les modèles sont convertis en TFLite via `tf.lite.TFLiteConverter`. La validation des shapes d'entrée et de sortie est réalisée directement dans le notebook via l'interpréteur TFLite (`interpreter.get_input_details()`, `interpreter.get_output_details()`).

Les fichiers `.tflite` produits sont prêts à être importés dans **STM32Cube.AI** pour génération du code C ciblant le NPU Neural-ART du STM32N657.

### Fichiers

| Fichier | Description |
|---|---|
| `Model_test.ipynb` | Notebook d'exploration et de prototypage |
| `Model_final.ipynb` | Notebook final — entraînement multiclasse + export TFLite |
| `rain_model.tflite` | Modèle binaire (pluie / pas pluie) |
| `meteo_multiclasse.tflite` | Modèle multiclasse (13 catégories météo) |
| `ETRS606_TP3.pdf` | Sujet du TP |

---

# TP4 — Réseau, ThreadX / NetX Duo et envoi des données capteurs vers ThingSpeak sur STM32N657

## Objectif

L’objectif de ce TP est de mettre en œuvre une chaîne complète de communication IoT sur la carte **STM32N657**, en combinant :

- la lecture de données capteurs,
- la communication réseau Ethernet,
- l’utilisation du noyau temps réel **ThreadX**,
- la pile TCP/IP **NetX Duo**,
- et l’envoi des données vers la plateforme **ThingSpeak** pour stockage, visualisation et analyse.

Le système final permet de réaliser la chaîne suivante :

**Capteurs → STM32 → Ethernet → DNS/TCP/HTTP → ThingSpeak → Courbes et statistiques**

---

## Matériel et environnement

- **Carte cible** : Nucleo-N657X0-Q
- **Microcontrôleur** : STM32N657
- **Réseau** : Ethernet
- **Environnement logiciel** :
  - STM32CubeIDE
  - STM32CubeMX
  - Azure RTOS ThreadX
  - Azure RTOS NetX Duo
  - ThingSpeak / MATLAB

---

## Vue d’ensemble du projet

Le projet final s’appuie sur :

- **ThreadX** pour l’ordonnancement multi-thread,
- **NetX Duo** pour la pile réseau,
- **DHCP** pour obtenir automatiquement une adresse IP,
- **DNS** pour résoudre le nom du serveur ThingSpeak,
- **TCP/HTTP** pour envoyer les mesures,
- **ThingSpeak** pour stocker et afficher les données sous forme de graphes.

Les grandeurs utilisées dans ce TP sont :

- **température**
- **humidité**
- **pression**

Les capteurs retenus sont :

- **HTS221** pour la température et l’humidité
- **LPS22HH** pour la pression

---

## Étape 1 — Lecture des données capteurs

La première étape consiste à valider le bon fonctionnement des capteurs sur la carte STM32.

Les fichiers capteurs permettent de lire les mesures environnementales et de les afficher sur la console série.  
Les valeurs observées sont ensuite utilisées dans la partie réseau.

Les trois données principales retenues pour l’envoi vers le cloud sont :

- température (`temperature_c`)
- humidité (`humidity_pct`)
- pression (`pressure_hpa`)

---

## Étape 2 — Intégration I2C dans le projet réseau

Le projet réseau final doit être capable de dialoguer avec les capteurs.  
Pour cela, l’interface **I2C1** a été ajoutée au projet principal.

Les éléments nécessaires sont :

- activation du module HAL I2C,
- ajout des fichiers HAL I2C (`stm32n6xx_hal_i2c.*`, `stm32n6xx_hal_i2c_ex.*`),
- configuration de `I2C1` dans `main.c`,
- ajout des définitions de broches dans `main.h`,
- ajout du MSP I2C dans `stm32n6xx_hal_msp.c`.

Cela permet au projet réseau d’accéder directement aux capteurs.

---

## Étape 3 — Création d’une structure commune des données

Pour transmettre proprement les mesures au module réseau, une structure de données dédiée a été créée :

```c
typedef struct {
    float temperature_c;
    float humidity_pct;
    float pressure_hpa;
} sensor_packet_t;
## Structure des données

Cette structure permet de regrouper dans un même objet les données principales à transmettre vers le serveur distant.

Elle facilite l’organisation du code en séparant clairement :

- les données issues des capteurs,
- la construction des messages réseau,
- et la transmission vers ThingSpeak.

Les champs `sample_id` et `timestamp_s` permettent respectivement :

- d’identifier chaque échantillon envoyé,
- d’associer un temps à chaque mesure.

---

## Étape 4 — Initialisation du réseau avec ThreadX / NetX Duo

Le projet réseau repose sur **Azure RTOS ThreadX** et **NetX Duo**.

Au démarrage, l’application initialise les ressources nécessaires :

- création d’un pool de paquets (`NX_PACKET_POOL`) pour gérer les buffers réseau,
- création d’une instance IP (`NX_IP`),
- activation des protocoles :
  - ARP  
  - ICMP  
  - UDP  
  - TCP  
- création d’un client DHCP pour obtenir automatiquement une adresse IP,
- création d’un client DNS pour résoudre le nom du serveur ThingSpeak.

Cette étape prépare la carte à fonctionner comme un équipement réseau connecté.

---

## Étape 5 — Organisation en threads avec ThreadX

L’application est organisée autour de plusieurs threads.

### NxAppThread

Ce thread démarre automatiquement et permet de :

- démarrer le client DHCP,
- attendre l’attribution de l’adresse IP,
- initialiser le client DNS,
- effectuer les premiers tests réseau,
- relancer le thread d’envoi des données.

### AppTCPThread

Ce thread est initialement suspendu puis relancé une fois le réseau prêt.

Son rôle est de :

- préparer les données,
- construire la requête HTTP,
- résoudre l’adresse du serveur,
- ouvrir une connexion TCP,
- envoyer les mesures,
- recevoir la réponse HTTP,
- recommencer périodiquement.

Cette architecture sépare clairement :

- la mise en service du réseau,
- la logique applicative.

---

## Étape 6 — Obtention de l’adresse IP par DHCP

La carte obtient automatiquement une adresse IP grâce au protocole DHCP.

Le client DHCP fournit :

- l’adresse IP locale,
- le masque réseau,
- les paramètres de communication.

L’adresse IP est affichée sur la console série.

---

## Étape 7 — Vérification de la connectivité réseau

Un test de connectivité est réalisé via un **ping ICMP** afin de vérifier :

- le bon fonctionnement de la pile réseau,
- l’activité de la communication Ethernet,
- la capacité à joindre une autre machine.

---

## Étape 8 — Résolution DNS

Le nom de domaine utilisé est :
api.thingspeak.com


Le DNS permet de :

- traduire ce nom en adresse IP,
- permettre l’établissement d’une connexion TCP.

Le DNS joue le rôle d’un annuaire réseau.

---

## Étape 9 — Préparation des données

Les données sont stockées dans `sensor_packet_t` :

- température  
- humidité  
- pression  


Association avec ThingSpeak :

- `field1` → température  
- `field2` → humidité  
- `field3` → pression  


---

## Étape 10 — Construction de la requête HTTP

La requête HTTP contient :

- l’adresse du service ThingSpeak,
- la Write API Key,
- les champs `field1`, `field2`, etc.

Elle est construite dynamiquement avant chaque envoi.

---

## Étape 11 — Envoi des données

L’application :

1. crée un socket TCP  
2. se connecte au serveur  
3. envoie la requête HTTP  
4. lit la réponse  
5. ferme la connexion  
6. recommence périodiquement  

Cette étape valide toute la chaîne IoT.

---

## Étape 12 — Visualisation sur ThingSpeak

La plateforme permet :

- d’afficher les données sous forme de graphes,
- de suivre leur évolution,
- de vérifier la régularité,
- de relire les données via MATLAB.

Grandeurs visualisées :

- température  
- humidité  
- pression  

---

## Étape 13 — Analyse statistique avec MATLAB

Indicateurs calculés :

- moyenne  
- médiane  
- minimum  
- maximum  
- écart-type  

### Rôle :

- **Moyenne** : tendance générale  
- **Médiane** : valeur centrale robuste  
- **Minimum** : plus petite valeur  
- **Maximum** : plus grande valeur  
- **Écart-type** : dispersion des données  

---

## Étape 14 — Résultats attendus

- affichage de l’adresse IP  
- connectivité validée  
- résolution DNS correcte  
- envoi périodique des données  
- visualisation sur ThingSpeak  
- statistiques correctes  

Le système permet :

- la lecture des capteurs,  
- la transmission réseau,  
- l’exploitation cloud.  

---

## Étape 15 — Intérêt du TP

Ce TP couvre :

- lecture de capteurs  
- communication I2C  
- gestion temps réel (ThreadX)  
- pile réseau (NetX Duo)  
- DHCP  
- DNS  
- TCP / HTTP  
- cloud IoT  
- analyse des données  

---

## Conclusion

Ce TP met en œuvre une chaîne complète de communication embarquée sur **STM32N657**.

Les données capteurs sont :

1. acquises,  
2. structurées,  
3. transmises via Ethernet (DHCP, DNS, TCP, HTTP),  
4. stockées dans le cloud (ThingSpeak),  
5. visualisées et analysées (MATLAB).  

Ce travail illustre un système IoT complet, de la mesure physique jusqu’à l’exploitation des données.
### Fichiers

| Fichier | Description |
|---|---|
| `Nx_TCP_Echo_Client.ioc` | Configuration CubeMX (horloges, UART1, Ethernet, etc.) |
| `STM32CubeIDE/` | Projet STM32CubeIDE complet |
| `EWARM/` | Projet IAR Embedded Workbench |
| `MDK-ARM/` | Projet Keil MDK-ARM |
| `Middlewares/` | Stack ThreadX et NetXDuo (Azure RTOS) |
| `Drivers/` | Drivers HAL STM32N6xx |
| `FSBL/` | Premier étage bootloader |
| `readme.html` | Documentation HTML du projet |
| `README.md` | README du projet |
| `ETRS606_TP4.pdf` | Sujet du TP |

---

## Environnement de développement

| Outil | Usage |
|---|---|
| STM32CubeIDE | Développement firmware STM32 (TP2) |
| STM32CubeMX | Génération du code d'initialisation HAL (.ioc) |
| STM32Cube.AI | Conversion TFLite → code C pour NPU Neural-ART |
| Google Colab | Entraînement des modèles (TP1, TP3) |
| TensorFlow / Keras | Construction et entraînement des réseaux de neurones |
| Meteostat | Données météo historiques (TP3) |

## Auteurs

El-Hadji-Abdoul-Aziz Sarr - Ndeye-Rama Thiam - Benjamin Avocat-Maulaz
Licence ESET, Université Savoie Mont-Blanc (2025–2026)
