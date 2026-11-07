# Catatan teknis karakter

Satu catatan per hero: bagaimana karakter itu bekerja di kode, setelah refactor v1.0.0-beta (#21). Ditulis untuk developer dan asisten AI yang akan mengubah atau menambah karakter.

Setiap PR kelas hero **menambah atau memperbarui catatan hero itu** di folder ini (lihat [`_TEMPLATE.md`](_TEMPLATE.md)). Hero yang belum dikerjakan belum punya catatan; perilakunya masih tersebar di `ActionManager.cpp`, `Element.cpp`, dan `UI/Game/HPBar.cpp`.

## Cara membaca data skill
Aksi di `Resources/Element/<Hero>/<Hero>.xml` dibaca berurutan oleh `HeroElement::setID`:

| Aksi XML | Di kode | Tombol / pemakaian |
|---|---|---|
| `nAttack` | `nattackArray`, `_nattack*` | serangan dasar |
| `skill01`–`skill03` | `skill1Array`–`skill3Array`, `_sattack*1..3` | skill 1–3 (`ACTION_STATE_SATTACK`) |
| `skill04` | `_skill4Action` | ougi 1 (`ACTION_STATE_OATTACK`) |
| `skill05` | `_skill5Action` | ougi 2 (`ACTION_STATE_O2ATTACK`) |
| `skill10`, `skill11`, `skill12` | `skillSPC1Array`, `SPC2`, `SPC3` | animasi pengganti selama buff/transformasi |
| `skill09`, `skill08` | `skillSPC4Array`, `SPC5` | animasi pengganti tambahan |

Tipe serangan (`attackType`) yang sering muncul: `nAttack`, `aAttack` (area), `cAttack`, `oAttack`, `Summon`, `hBuff` (heal), `cBuff` (buff skill: nilainya `setBuff`, mis. 17/18), `stBuff` (stealth).

## Status

Urutan pengerjaan = urutan layar select HardCore.

| Slot | Hero | Kelas | Catatan |
|---|---|---|---|
| 0 | Naruto (+ Sage, Rikudo) | `Naruto` | [Naruto.md](Naruto.md) |
| 1 | Sakura | `Sakura` | [Sakura.md](Sakura.md) |
| 2 | Sai | `Sai` : `RidingHero` | [Sai.md](Sai.md) |
| 3 | Kakashi | `Kakashi` | [Kakashi.md](Kakashi.md) |
| 7 | Shikamaru | — | berikutnya |
| 8 | Ino | — | |
| 9 | Choji | — | |
| 10 | Asuma | — | |
| 11 | Kiba | — | |
| 12 | Hinata | — | |
| 13 | Shino | — | |
| 14 | Neji | — | |
| 15 | Tenten | — | |
| 16 | Lee (+ RockLee) | — | |
| 17 | Kurenai | — | |
| 22 | Tobirama | — | |
| 23 | Hiruzen | — | |
| 24 | Minato | — | |
| 25 | Jiraiya (+ Sage) | — | |
| 26 | Tsunade | — | |
| 27 | Orochimaru | — | |
| 29 | Deidara | `Deidara` : `RidingHero` (sebagian) | [Deidara.md](Deidara.md) |
| 30 | Kakuzu | — | |
| 31 | Hidan | — | |
| 32 | Tobi | — | |
| 33 | Konan | — | |
| 34 | Pain | — | |
| 35 | Itachi | — | |
| 36 | Kisame | — | |
| 42 | Sasuke (+ Immortal) | — | |
| 43 | Karin | — | |
| 44 | Suigetsu | — | |
| 45 | Jugo | — | |
| 49 | Gaara | — | |
| 51 | Kankuro | — | |
| 52 | Chiyo | — | |

## Kelas bersama
| Kelas | Dipakai | Isi |
|---|---|---|
| `RidingHero` (`Heroes/Characters/RidingHero`) | Sai, Deidara | buff 17 (serangan dasar khusus) dan 18 (tunggangan yang menjatuhkan monster); lihat [Sai.md](Sai.md) |
| `RideMonster` (`Units/Monsters/RideMonster.h`) | Mouse (Sai), Spider (Deidara) | hanya mengincar hero |
