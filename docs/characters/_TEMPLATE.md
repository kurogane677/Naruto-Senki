# <Hero>

| | |
|---|---|
| Kelas | `<Hero>` (`Classes/Heroes/Characters/<Hero>.h/.cpp`) |
| Bentuk | — / rantai bentuk di `HeroRegistry` |
| Slot select | Training: … · HardCore: … |
| Record / COM Training / stealth | … / … / … |
| AI | role …, build `default` {…} (`AI/ProfilesData.cpp`) |
| Aset tambahan | `HeroAssets`: … |

## Skill
| Aksi | Tombol | Tipe | Nilai | CD | Event penting | Ditangani di |
|---|---|---|---|---|---|---|
| skill01 | skill 1 | … | … | … | … | … |

## Perilaku di kelas
Hook yang di-override, apa yang dilakukannya, dan dari mana dipanggil.

## Summon dan monster
| Unit | Dari | Kelas | Perilaku |
|---|---|---|---|

## Konstanta
Angka yang dikunci di kode (bukan dari XML) dan artinya.

## Masih di kode bersama
Cabang `isCharacter(CharacterId::<Hero>)` yang tersisa, beserta alasannya (dipakai bersama hero lain, daftar frame, dst.).

## Riwayat
PR yang mengubah karakter ini.
