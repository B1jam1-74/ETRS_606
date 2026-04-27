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

## TP4 — Réseau et RTOS ThreadX/NetXDuo sur STM32N657

**Objectif :** développer une application de communication réseau utilisant le stack **Azure RTOS NetX/NetXDuo** sur le STM32N657, en exploitant la gestion multi-threads du noyau temps réel **ThreadX**.

**Cible matérielle :** carte Nucleo-N657X0-Q (STM32N657) avec interface Ethernet.  
**Environnement :** STM32CubeIDE, ThreadX, NetXDuo, middleware Azure RTOS.

### Projet `Nx_TCP_Echo_Client/` — Client TCP avec ThreadX/NetXDuo

Client TCP communicant avec un serveur distant via le protocole TCP/IP, intégrant une architecture multi-thread ThreadX et la gestion de pile réseau NetXDuo.

#### Architecture applicative

L'initialisation des ressources NetX/NetXDuo s'effectue dans `tx_application_define()` appelée au démarrage du kernel ThreadX :

- **Allocation d'un pool de paquets** (`NX_PACKET_POOL`) pour gérer les buffers réseau.
- **Initialisation d'une instance IP** (`NX_IP`) utilisant ce pool.
- **Activation des protocoles** ARP, ICMP, UDP et TCP sur l'instance IP.
- **Création d'un client DHCP** pour l'acquisition automatique d'adresse IP.

Deux threads applicatifs sont créés avec priorité identique (10) :

| Thread | Flags de création | Rôle |
|---|---|---|
| **NxAppThread** | TX_AUTO_START | Démarre automatiquement ; lance le client DHCP et attend la résolution IP avant de relancer AppTCPThread |
| **AppTCPThread** | TX_DONT_START | Créée mais attendant le signal de NxAppThread ; effectue la connexion TCP et l'échange de messages |

#### Fonctionnement de NxAppThread

1. Lance le client DHCP.
2. Attend la résolution de l'adresse IP du board.
3. Reprend l'exécution de `AppTCPThread`.

#### Fonctionnement de AppTCPThread

1. Crée un socket TCP.
2. Se connecte au serveur TCP distant sur le port prédéfini.
3. À succès de connexion, envoie jusqu'à `MAX_PACKET_COUNT` messages au serveur.
4. Pour chaque message envoyé :
   - Lit la réponse du serveur (echo).
   - Affiche la réponse sur la HyperTerminal (USART1 à 115 200 bps).
   - Bascule la LED verte pour indication visuelle.

#### Résultats attendus

- Affichage de l'adresse IP du board sur la HyperTerminal.
- Affichage de chaque réponse du serveur.
- Récapitulatif de succès : `SUCCESS : 100 / 100 packets sent` (selon `MAX_PACKET_COUNT`).
- Basculement continu de la LED verte pendant le transfert.

#### Comportements en cas d'erreur

- **LED rouge qui bascule** : indication d'une erreur au cours de l'exécution.
- **Message d'échec affiché** : affichage des messages attendus vs. reçus en cas d'incomplétude de l'échange.

#### Configuration et déploiement

**Configuration TCP (fichier `app_netxduo.h`)**  
Éditer les valeurs suivantes :

```c
#define TCP_SERVER_ADDRESS  "192.168.1.100"   // Adresse IP du serveur
#define TCP_SERVER_PORT     6000              // Port TCP d'écoute
```

**Test avec echotool (serveur d'écho)**  
Sur PC Windows, lancer un serveur d'écho :

```cmd
c:\> .\echotool.exe /p tcp /s 6000
```

Le board client se connecte et envoie les messages, qui sont renvoyés par le serveur.

**Prérequis réseau**

- Un serveur DHCP accessible sur le LAN (pour l'acquisition IP).
- L'adresse MAC du board doit être unique sur le LAN. Adresse MAC statique définie dans `main.c` — `MX_ETH_Init()` :
  ```c
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE0;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x10;
  MACAddr[5] = 0x00;
  ```

**Exécution en développement (debug)**

1. Placer les switchs en mode développement (BOOT1 = 2-3).
2. Ouvrir le projet dans STM32CubeIDE.
3. Éditer `app_netxduo.h` (adresse et port du serveur).
4. Compiler et charger en RAM du board.
5. Lancer le débogage (debug mode).

**Exécution depuis la flash externe**

1. Générer le binaire `Nx_TCP_Echo_Client.bin`.
2. Signer avec CubeProgrammer et STM32_SigningTool_CLI :
   ```cmd
   STM32_SigningTool_CLI.exe -bin Nx_TCP_Echo_Client.bin -nk -of 0x80000000 -t fsbl -o Nx_TCP_Echo_Client-trusted.bin -hv 2.3 -dump Nx_TCP_Echo_Client-trusted.bin
   ```
3. Charger `Nx_TCP_Echo_Client-trusted.bin` en flash externe à l'adresse `0x70000000` via CubeProgrammer.
4. Configurer les switchs : BOOT0 = 1-2, BOOT1 = 1-2.
5. Appuyer sur reset.

> **Note :** Pour CubeProgrammer v2.21+, ajouter l'option `-align` à la commande de signature.

#### Conseils d'utilisation ThreadX

- ThreadX utilise **Systick** comme base de temps ; la HAL doit utiliser un timer IP distinct (TIMx).
- Fréquence des ticks configurée par défaut à **100 ticks/sec** (`TX_TIMER_TICKS_PER_SECOND` dans `tx_user.h`). Modifier aussi `tx_initialize_low_level.S` en cas de changement.
- ThreadX désactive tous les interrupts au démarrage du kernel ; tout appel système (HAL, BSP) doit être effectué en début d'application ou dans les fonctions d'entrée des threads.
- Pour allocation dynamique, éditer le fichier linker (`.ld`) afin d'exposer la première adresse libre en RAM vers `tx_application_define()` (voir détails dans projet ou documentation STM32CubeIDE).

#### Conseils d'utilisation NetX Duo

- Le pool de paquets `NX_PACKET_POOL` doit être alloué dans une section mémoire dédiée (`.NetXPoolSection`).
- En cas de congestion réseau, augmenter les paramètres `ETH_TX_DESC_CNT` et `ETH_RX_DESC_CNT` dans `stm32n6xx_hal_conf.h` (coûteux en mémoire).

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

## Auteur

Benjamin Avocat-Maulaz — Licence ESET, Université Savoie Mont-Blanc (2025–2026)
