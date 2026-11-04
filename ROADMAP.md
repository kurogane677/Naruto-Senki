# Naruto Senki — Progres, Rencana, dan Update Mendatang

Status per **4 November 2026**. Cakupan dokumen ini: **game** (server online dikerjakan sebagai proyek terpisah). Rilis terakhir: [`v0.0.4-beta`](https://github.com/kurogane677/Naruto-Senki/releases/tag/v0.0.4-beta) (pre-release, Android).

---

## 1. Sudah selesai

### Rilis
| Versi | Tanggal | Isi utama |
|---|---|---|
| `mvp` | 31 Okt 2026 | Migrasi engine cocos2d-x 2.2 → Axmol 2.11.5 selesai, MVP disetujui |
| `v0.0.1-beta` | 31 Okt 2026 | Rilis Android pertama |
| `v0.0.2-beta` | 2 Nov 2026 | Perapian AI, APK kedua `-android-sdk29` untuk modding |
| `v0.0.3-beta` | 2 Nov 2026 | Pain dan Orochimaru, AI summon baru, fix crash naik level, kemenangan tercatat lagi |
| `v0.0.4-beta` | 4 Nov 2026 | Sistem gear baru, Kurenai, tombol back Android, pilih map HardCore, teks skill dan laporan kill dari XML, branding baru |

### Isi `v0.0.4-beta`
- **Sistem gear baru** (PR #18).
  - Gear bisa dibeli, di-upgrade sampai Lv3, dan dijual.
    - Biaya upgrade: Lv2 = 1× harga beli, Lv3 = 2× harga beli.
    - Jual: kembali 50% dari total coin yang sudah dibayar.
  - 2 gear baru: Lifesteal dan Gulungan Kuchiyose.
  - Toko gear memakai teks dari `Element/gears.xml` (font Pixelify Sans, bahasa Indonesia + Inggris).
  - Bot HardCore meng-upgrade gear setelah 3 gear terbeli.
- **Kurenai bisa dimainkan** (PR #17).
  - Skill 1 untuk menghilang, skill 2 untuk teleport ke target, ougi 2 berupa serangan area dengan kebal.
  - Bot Kurenai bermain sebagai fighter: pola Naruto tanpa clone, ditambah sergapan dari kondisi tak terlihat ala Konan.
  - Terdaftar di semua daftar hero (Network/HardCore, pool bot Training, ranking, autoplay, record).
- **Teks laporan kill memakai font** (PR #16, #15).
  - Nama hero dan frasa "has slain"/"has destroyed" ditulis dengan Arimo, teksnya ada di `Element/report.xml`.
  - Wajah kecil hero dan efek multi-kill tetap gambar.
- **Penjelasan skill berbahasa Inggris** dari `Element/skills.xml`, ditampilkan dengan `Fonts/1.fnt` (PR #14, #13).
- **Kursor "COM"** saat memilih teman di HardCore (PR #12, #11).
- **Pilih map HardCore + map berbasis file** `Tiles/<n>.tmx`; APK kembali otomatis terlampir di Release (PR #10).
- **Tombol back Android** (targetSdk 36) lewat `OnBackInvokedCallback` (PR #9, #2).
- **Kemenangan hero yang belum punya baris record** (Pain, Orochimaru, Kurenai) sekarang tersimpan.
- **Branding baru** dan dialog keluar Android dengan package `com.yusrilcode.game` (PR #19).

### Ringkasan PR yang sudah di-merge
| PR | Isi |
|---|---|
| #3 | Perapian AI, CI manual |
| #4 | APK targetSdk 29 untuk modding |
| #5 | Fix pointer NULL di tombol ougis (force close saat naik level) |
| #6 | Clean code C1: buang kode mati, fix kemenangan tidak tercatat, konstanta |
| #7 | AI summon mengikuti role pemanggil; bot HardCore menabung untuk gear; fix Hidan; Orochimaru |
| #8 | Pain (Ougis 1 Jibaku, Ougis 2 tanpa berubah jadi Nagato, efek teks ougis); CI hanya Android |
| #9 | Tombol back Android |
| #10 | Pilih map HardCore, map berbasis file, fix lampiran APK di rilis |
| #12 | Kursor "COM" saat memilih teman HardCore |
| #14 | Penjelasan skill berbahasa Inggris dari `skills.xml` |
| #16 | Teks laporan kill memakai font (`report.xml`, Arimo) |
| #17 | Karakter baru Kurenai |
| #18 | Gear: upgrade, jual, 2 gear baru, toko berbasis teks |
| #19 | Branding intro/menu, dialog keluar Android |

---

## 2. Tertinggal (belum tuntas)

### Catatan lokal
- `plan.md` (rincian teknis) sengaja **tidak** di-commit ke repo publik, karena memuat detail proteksi versi Privat.
- Tidak akan di-commit: bypass ougis `// TESTING` di `ActionButton.cpp` dan `Naruto.xml` versi tes (attack 65000).

### Kurenai
- [ ] Suara `Kurenai_hurt`, `Kurenai_dead`, `Kurenai_skill02_2` (path-nya sudah ada di `Kurenai.xml`, file suaranya belum).
- [ ] `Element/Skills/Kurenai_Skill.plist/.pvr.ccz` masih salinan milik Konan. Ganti dengan sheet efek Kurenai sendiri, atau sheet kosong.
- [ ] Deskripsi skill di `skills.xml` masih "Lorem ipsum".

### Gear
- [ ] Efek Lifesteal dan Gulungan Kuchiyose belum terbukti dalam permainan nyata, karena bot belum pernah membelinya.
- [ ] Tambahkan Kuchiyose ke build AI hero pemanggil (Kiba, Kankuro, Chiyo, dan lainnya) di `profiles.json`.
- [ ] Ikon asli Lifesteal dan Gulungan Kuchiyose (sekarang placeholder).
- [ ] Gambar tombol Upgrade dan Jual (sekarang kotak warna dengan teks).

### Keamanan mod
- [ ] Pindahkan profil AI/bot dari `Resources/AI/profiles.json` ke kode, agar perilaku bot tidak mudah diubah modder ([#61](https://github.com/kurogane677/Naruto-Senki/issues/61)).

### Temuan kecil
- [ ] Frame `menu02_1.png` tidak ada di plist mana pun (`StartMenu.cpp:452`, tombol HardCore).
- [ ] Judul "TRAINING" tampil di mode Network; perlu dicek.

### Dari roadmap migrasi (M6), belum dipastikan
- [ ] Mode **Carnival 4v4**.
- [ ] Bug dari laporan audit awal: HP underflow, stun hilang karena pembagian integer, `abs()` hilang di cek jarak, ougis tanpa cek chakra. Perlu dicek mana yang sudah dan belum diperbaiki.
- [ ] Build **Windows** kembali di CI (dicopot sejak PR #8).
- [ ] Checklist M5 di `MIGRATION_ROADMAP.md` belum dicentang, padahal MVP sudah disetujui. Dokumennya saja yang belum diperbarui.

---

## 3. To-do list (urutan prioritas)

1. **Lengkapi Kurenai:** suara, sheet skill sendiri, deskripsi skill.
2. **Gear:** buktikan Lifesteal/Kuchiyose dalam permainan, masukkan Kuchiyose ke build AI pemanggil, buat ikon asli.
3. **Bug kecil:** `menu02_1.png`, judul TRAINING di Network.
4. **Layar penuh di HP layar lebar**, tanpa garis hitam ([#20](https://github.com/kurogane677/Naruto-Senki/issues/20)).
5. **Profil AI pindah ke kode** ([#61](https://github.com/kurogane677/Naruto-Senki/issues/61)).
6. **Audit bug lama** dari laporan awal (HP underflow, stun, `abs()`, ougis tanpa chakra).
7. **Clean code: karakter OOP + design pattern** ([#21](https://github.com/kurogane677/Naruto-Senki/issues/21), sub-issue #22–#60). Mulai dari fondasi #22 (unit test, hook `Hero`, `HeroFactory`).
8. **Distribusi Privat (4.1):** colokan `NS_PRIVATE_DIR` di repo publik, lalu repo privat + `Protect/`.
9. **Modul `Online/` di game (4.2)**, setelah API server (proyek terpisah) siap.

---

## 4. Rencana berikutnya

### 4.1 Distribusi: versi publik dan versi Privat
**Keputusan:**
- Semua versi **gratis**: tanpa IAP, tanpa battle pass atau skin berbayar. IP Naruto bukan milik kita, dan developer asli mensyaratkan non-komersial.
- **Repo publik `Naruto-Senki`:** kedua APK (`-android` sdk36 dan `-android-sdk29`) tetap **terbuka untuk mod**.
- **Repo privat `Naruto-Senki-Private`:**
  - proteksi: enkripsi aset dan cek tanda tangan APK di kode native;
  - fitur online, event, dan battle pass/skin gratis;
  - package sendiri, jadi bisa terpasang berdampingan dengan versi publik.

**Langkah (sisi game):**
1. Repo publik: tambah "colokan" kosong (`onAppStart`, `onStartMenu`, `onMatchEnd`, `addAssetPaths`) lewat opsi CMake `NS_PRIVATE_DIR`.
2. Repo privat: modul `Protect/` dengan kunci di GitHub Secret.
3. Modul `Online/`, setelah API server siap.
4. Event dan skin.

### 4.2 Akun online (versi Privat) — sisi game
- **Server dikerjakan sebagai proyek terpisah** (Go + PostgreSQL, repo `naruto-senki-server`, VPS). Dokumen ini hanya mencakup sisi game.
- **Login opsional**, dengan daftar akun di dalam game. Progres offline dan online terpisah.
- **Coin dan hasil match dihitung di server**, bukan dipercaya dari klien.
- Di game: ApiClient + Session, UI login/daftar di menu utama, upload hasil match (dengan antrean saat offline), dan cache progres per akun.
- **Prasyarat:** API server siap, dengan endpoint auth (register/login/refresh/logout), progres, dan upload match.

### 4.3 Clean code (tanpa mengubah perilaku)
Dilacak di [#21](https://github.com/kurogane677/Naruto-Senki/issues/21), dengan satu sub-issue per hero (#22–#60):
- unit test sebagai jaring pengaman;
- satu kelas per hero (`Classes/Heroes/<Hero>.cpp`) menggantikan 199 cabang `if/else isCharacter`;
- design pattern: Template Method, Factory + Registry, Strategy, Command, Observer;
- pecah fungsi dan file raksasa, rapikan memori, buang shim compat cocos2d-x 2.x.

---

## 5. Update mendatang (ide, belum dijadwalkan)

- **Gear tambahan** dari daftar ide di [`GEAR_PLAN.md`](GEAR_PLAN.md) (30 ide: offensive, defensive, mobility, utility, special).
  - Dipilih dan disesuaikan satu per satu.
  - Efeknya tetap ditulis di kode, karena gear sengaja **tidak** bisa ditambah modder.
- **News** di menu utama (setelah login online jadi; datanya dari server).
- **Event, battle pass, dan skin gratis** (versi Privat).
- **Link donasi sukarela** (Saweria/Trakteer/Ko-fi), tanpa membuka fitur apa pun. *Ditunda.*
- **Orochimaru:** suara karakter, dan Ougis 2 dengan ular (menunggu aset monster ular).
- **Terjemahan menu** yang masih berbahasa Cina.

---

## 6. Known issues
- Kurenai belum punya suara saat terkena serangan dan saat mati, dan deskripsi skill-nya masih teks sementara.
- Orochimaru belum punya suara karakter.
- Di layar yang lebih lebar dari 16:9 ada strip hitam tipis di kiri-kanan. Ini disengaja agar kecepatan dan jangkauan AI sama di semua perangkat.
- Beberapa teks menu masih berbahasa Cina.

---

## 7. Catatan kerja
- **Repack atlas** (UI, Record, Report, Select, dan lainnya) dari folder `Resources/Element/Original` bisa menghilangkan info trim dan membuang frame. Setelah setiap repack, bandingkan frame dan offset tiap plist dengan versi git sebelum commit.
- **Hero baru** perlu didaftarkan di: `NetworkLayer.cpp` (dua daftar), `SelectLayer.cpp` (pool bot Training), `RankingLayer.cpp`, `KTools.cpp` (baris record awal), `AutoPlay.cpp`, dan profil AI di `Resources/AI/profiles.json`.

---

## 8. Keputusan desain yang dipegang
| Topik | Keputusan |
|---|---|
| Komersial | Tidak ada. Semua gratis, dana hanya dari donasi sukarela |
| Mod | Map, aset karakter, dan teks (`skills.xml`, `gears.xml`, `report.xml`) boleh diubah modder. **Efek/daftar gear dan perilaku AI/bot tidak** (AI pindah ke kode: #61) |
| APK publik | Keduanya tetap terbuka; hanya versi Privat yang diproteksi |
| Terjemahan | Teks hasil terjemahan diterima apa adanya; typo diperbaiki saat dilaporkan |
| Branch | Selalu berprefix `feat/`, `fix/`, `chore/`, `docs/`; PR ke `main` setelah dites user |
| CI | Hanya jalan saat tag `v*` atau dijalankan manual; versi action tidak diubah |
