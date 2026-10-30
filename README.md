# Naruto Senki — Axmol Port

Port dari game Naruto Senki (game 2D *lane battle* Konoha vs Akatsuki dengan tower, creep, hero AI, gear, dan ougis) dari **cocos2d-x 2.2.2 / VS2010** ke **Axmol 2.11.5 / Visual Studio 2026 / Windows x64**.

> `ReadME.txt` adalah catatan asli dari pengembang lama dan disimpan sebagai arsip.
> Versi asli sebelum migrasi ada di tag git **`legacy-2.2.2`** (branch `main`).

| | |
|---|---|
| **Status** | **MVP selesai**: mode Training bisa dimainkan dari menu sampai GameOver di Windows x64 |
| Engine | [Axmol](https://github.com/axmolengine/axmol) **v2.11.5** (LTS) |
| Platform | Windows 10/11 **x64** (Android belum, lihat [Roadmap](#roadmap)) |
| Bahasa | C++20 (kode game legacy C++03 melalui lapisan kompatibilitas) |
| Branch kerja | `axmol-migration` |

---

## Daftar isi
1. [Requirement](#requirement)
2. [Setup lingkungan (sekali saja)](#setup-lingkungan-sekali-saja)
3. [Build](#build)
4. [Menjalankan game](#menjalankan-game)
5. [Cakupan MVP](#cakupan-mvp)
6. [Struktur proyek](#struktur-proyek)
7. [Catatan teknis migrasi](#catatan-teknis-migrasi)
8. [Debugging & crash](#debugging--crash)
9. [Known issues](#known-issues)
10. [Aturan kontribusi](#aturan-kontribusi)
11. [Roadmap](#roadmap)

---

## Requirement

### Perangkat lunak

| Tool | Versi minimum | Versi yang dipakai & teruji | Catatan |
|---|---|---|---|
| Windows | 10 / 11 x64 | Windows 11 (build 26300) | **Developer Mode wajib aktif**; dibutuhkan `setup.ps1` Axmol untuk symlink |
| Visual Studio | 2022 (17.x) | **2026 Community 18.10** | Workload **Desktop development with C++** |
| MSVC toolset | v143 | **14.51** | Ikut dengan workload di atas |
| Windows SDK | 10.0.19041 | **10.0.26100** | Ikut dengan workload di atas |
| CMake | **4.2** | 4.4.4 | Disyaratkan Axmol 2.10+ |
| PowerShell | **7.x** (`pwsh`) | 7.6.9 | Dibutuhkan `setup.ps1` dan CLI `axmol` |
| Git | 2.x | 2.37.3 | |
| Axmol | **2.11.5** | 2.11.5 | Di-clone ke `C:\dev\axmol` (lihat setup) |
| Python | 3.x (opsional) | 3.13 | Hanya untuk script bantu migrasi; tidak dibutuhkan untuk build |

### Hardware (perkiraan)
- Disk: ±6–10 GB untuk workload C++ VS, ±3 GB untuk Axmol + hasil build engine, ±50 MB untuk resource game.
- Build engine pertama kali: ±10–20 menit (tergantung CPU). Build berikutnya hanya kode game: ±1–2 menit.

### Dependensi yang ikut di repo
| Library | Lokasi | Alasan |
|---|---|---|
| SQLite (amalgamation) | `Classes/MyUtils/sqlite3.*` | Penyimpanan coin & record per hero (`sql.db`) |
| tinyxml2 **10.0.0** (lisensi zlib) | `Classes/MyUtils/tinyxml2/` | Pembaca data karakter (`Resources/Element/**.xml`); Axmol tidak lagi membundel tinyxml2 |

Library lain (rapidjson, curl, OpenAL, dll.) disediakan oleh Axmol.

---

## Setup lingkungan (sekali saja)

1. **Aktifkan Developer Mode**
   Settings → System → For developers → **Developer Mode: On**.

2. **Install tool** (PowerShell, akan muncul UAC):
   ```powershell
   winget install --id Kitware.CMake -e
   winget install --id Microsoft.PowerShell -e
   ```

3. **Tambahkan workload C++ ke Visual Studio 2026**
   Lewat Visual Studio Installer → *Modify* → centang **Desktop development with C++**. Atau via command line (UAC):
   ```powershell
   & "C:\Program Files (x86)\Microsoft Visual Studio\Installer\setup.exe" modify `
     --installPath "C:\Program Files\Microsoft Visual Studio\18\Community" `
     --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended --passive --norestart
   ```
   Jangan menutup jendela installer sampai selesai.

4. **Ambil Axmol 2.11.5 dan jalankan setup**
   ```powershell
   git clone --depth 1 --branch v2.11.5 https://github.com/axmolengine/axmol.git C:\dev\axmol
   pwsh C:\dev\axmol\setup.ps1
   ```
   `setup.ps1` mengatur environment variable user `AX_ROOT=C:\dev\axmol` dan menambahkan `C:\dev\axmol\tools\cmdline` ke `PATH`. **Restart terminal / VS Code** setelahnya.

5. **Verifikasi**
   ```powershell
   cmake --version     # >= 4.2
   pwsh --version      # 7.x
   echo $env:AX_ROOT   # C:\dev\axmol
   ```

---

## Build

Semua perintah dijalankan dari root repo (`C:\laragon\www\Naruto Senki`). Folder build: `build_x64/` (diabaikan git).

### 1. Configure (pertama kali / setelah menambah file `.cpp`)
Source di-*glob* oleh CMake, jadi **ulangi configure setiap menambah atau menghapus file source**.
```powershell
cmake -S . -B build_x64 -G "Visual Studio 18 2026" -A x64
```
Configure pertama ±5 menit (meng-generate seluruh engine).

### 2. Build
```powershell
# RelWithDebInfo: cepat + simbol debug (dipakai untuk uji main)
cmake --build build_x64 --config RelWithDebInfo --target NarutoSenki

# Debug: lebih lambat, assert engine aktif (paling bagus untuk menemukan bug)
cmake --build build_x64 --config Debug --target NarutoSenki
```
- Build pertama untuk tiap config ikut mem-build seluruh engine Axmol.
- Setelah engine ter-build, build cepat hanya kode game:
  ```powershell
  cmake --build build_x64 --config RelWithDebInfo --target NarutoSenki -- /p:BuildProjectReferences=false /m
  ```
- **Tutup game sebelum build ulang**, jika tidak, linker gagal karena `.exe` terkunci.

### Alternatif
- **Visual Studio IDE**: buka `build_x64\NarutoSenki.sln`, set `NarutoSenki` sebagai startup project, pilih config, F5.
- **CLI Axmol**: `build.bat` (menjalankan `axmol build -p win32`).

---

## Menjalankan game

```
build_x64\bin\NarutoSenki\<Config>\NarutoSenki.exe
```
`<Config>` = `RelWithDebInfo` atau `Debug`. Resource dari `Resources/` disalin otomatis ke folder `Content\` di samping exe saat build.

- Jendela 960×640 (resolusi desain 480×320, `FIXED_HEIGHT`). Jendela konsol ikut terbuka dan menampilkan log Axmol.
- **Kontrol**: mouse menggantikan sentuhan (joystick kiri bawah, tombol attack/skill/ougis kanan bawah, minimap kanan atas = **Pause**, ESC = tombol *back*).

### Lokasi data pemain
`%LOCALAPPDATA%\NarutoSenki\`

| File | Isi |
|---|---|
| `sql.db` | Coin & record per hero (SQLite, nilai di-obfuscate) |
| `CheckMD5_113.xml` | Hash `sql.db` (peninggalan anti-tamper, tidak lagi dicek) |
| `UserDefault.bin` | Pengaturan BGM/voice (format Axmol; save dari versi 2.x tidak ikut terbawa) |

Hapus folder ini untuk reset save.

---

## Cakupan MVP

**MVP = mode Training dapat dimainkan dari awal sampai akhir di Windows x64.**

| Fitur | Status |
|---|---|
| Intro → logo → *Push Start* → menu utama | ✅ |
| Training: pilih hero / lineup acak, loading, match 3v3 | ✅ |
| Joystick, attack, skill 1–3 + cooldown, item potion | ✅ |
| Ougis LV2 / LV4 (cut-in, buff) | ✅ |
| Creep, tower, level-up, respawn, kill feed, minimap | ✅ |
| Pause: resume, surrender, toggle BGM/voice | ✅ |
| GameOver (RESULT) → kembali ke menu → main lagi | ✅ |
| Coin tersimpan di `sql.db` | ✅ |
| Mode HardCore (pilih hero, peta 2–5) | 🟡 berjalan, belum diuji menyeluruh |
| Gear shop, guardian Roshi/Han | 🟡 belum diuji |
| Ranking, Skill/Record, Credits | 🟡 compile & tampil, belum diuji menyeluruh |
| Login / group / upload skor (online) | ⛔ dimatikan (server lama tidak ada; menampilkan "ServerMainten") |
| Android | ⛔ belum |

---

## Struktur proyek

```
Naruto Senki/
├─ CMakeLists.txt            # proyek Axmol: source dari Classes/, resource dari Resources/
├─ cmake/modules/            # modul CMake template Axmol (+ CrashHandler di AXGameSourceSetup)
├─ Classes/                  # kode game (cocos2d-x 2.x style, UTF-8)
│  ├─ Compat/                # ★ lapisan kompatibilitas 2.x → Axmol (Compat2x.h/.cpp)
│  ├─ ActionManager.*        # base class semua unit (state machine, damage, buff, AI helper)
│  ├─ Characters.*           # Hero + AI per karakter
│  ├─ Element.*              # HeroElement, Flog (creep), Tower, Monster, Bullet
│  ├─ GameLayer.*            # dunia game, spawn, kamera, kondisi menang
│  ├─ HudLayer.*, ActionButton.*, JoyStick.*   # HUD & input
│  ├─ StartMenu.*, SelectLayer.*, NetworkLayer.*, LoadLayer.*   # menu & alur
│  ├─ GameOver.*, PauseLayer.*, GearLayer.*, SkillLayer.*, RankingLayer.*
│  └─ MyUtils/               # KTools (SQLite/XML), MD5, sqlite3, tinyxml2, util UI
├─ Resources/                # aset (sprite plist/pvr, audio mp3/ogg, peta .tmx, data XML)
├─ proj.win32/               # entry point Windows + CrashHandler
├─ proj.android/             # template Axmol (belum dipakai)
├─ MIGRATION_ROADMAP.md      # urutan kerja migrasi, checklist, log keputusan
└─ MIGRATION_PLAN_AXMOL.md   # referensi teknis: pemetaan API, risiko
```

---

## Catatan teknis migrasi

Kode game sengaja **tidak ditulis ulang**. Kode tetap memakai API cocos2d-x 2.x, dan `Classes/Compat/Compat2x.h` menjembataninya ke Axmol. Tujuannya agar perilaku game sama dulu, baru di-refactor belakangan.

### Isi lapisan kompatibilitas (`Classes/Compat/`)
| 2.x | Di Axmol | Penanganan |
|---|---|---|
| `CCSprite`, `CCNode`, `CCPoint`, … | ada (tanpa prefix) | alias tipe |
| `CCDirector::sharedDirector()` dkk. | `getInstance()` | makro token |
| `CCString`, `CCArray`, `CCDictionary`, `CCARRAY_FOREACH` | **dihapus** | implementasi ulang dengan semantik 2.x |
| `CCNotificationCenter` | **dihapus** | implementasi ulang |
| `SimpleAudioEngine` | **dihapus** | `CocosDenshion::SimpleAudioEngine` di atas `ax::AudioEngine` |
| `CCTouchDelegate` + `addTargetedDelegate` | **dihapus** | `CCTouchDispatcher` di atas `EventListenerTouchOneByOne` |
| `CCLayer` (touch + keypad delegate) | `Layer` polos | subclass `CCLayer` |
| `CCCallFunc*`, `CCMenuItem*` dengan (target, selector) | hanya lambda | subclass yang membungkus selector |
| `CCSequence/CCSpawn::create(CCArray*)` | tidak ada | subclass |
| `CCLabelTTF`, `CCLabelBMFont` | `Label` | subclass |
| `CCHttpRequest` | `network::HttpRequest` | subclass |
| TMX object group → `CCArray` | `ValueVector` | `ccArrayFromValueVector()` |
| `getFileData()` | `getDataFromFile()` | `ccGetFileData()` |
| Snapshot `node->visit()` ke `RenderTexture` | butuh visiting camera | `ccVisitForSnapshot()` |

### Hal penting yang sering terlupa
- **`NULL` didefinisikan ulang menjadi `nullptr`** di `Compat2x.h`. Semua daftar variadic lama (`CCSequence::create(a, b, NULL)`) aman di x64. Konsekuensinya, `NULL` tidak boleh dipakai sebagai angka; gunakan `0`.
- Target game dikompilasi dengan `/Zc:strictStrings-` (string literal → `char*` legacy). `/permissive` **tidak** bisa dipakai karena merusak header engine.
- Prioritas touch 2.x dipetakan ke fixed priority Axmol (`p > -128 → p + 200`) agar tetap diproses setelah menu, seperti di 2.x.
- Source dikonversi dari **GBK → UTF-8** (18 file). Compile memakai `/utf-8` (otomatis dari Axmol).
- **Anti-tamper/sabotase asli sudah dihapus** (cek MD5 resource, `CCDirector::end()` diam-diam di GameOver, dll.). Rincian ada di commit `d7b51be`.

---

## Debugging & crash

- **`crash.log`**: saat crash (termasuk assert/abort), `proj.win32/CrashHandler.cpp` menulis **stack trace lengkap dengan nama fungsi + file + baris** ke `crash.log` dan minidump ke `crash.dmp` **di folder exe yang dijalankan** (`...\Debug\` atau `...\RelWithDebInfo\`). Lampirkan isinya saat melapor bug.
- **Debug vs RelWithDebInfo**: build Debug meng-assert hal-hal yang di Release lolos diam-diam (frame sprite hilang, index di luar batas). Uji di keduanya.
- **Crash yang tidak tertangkap handler** (mis. heap corruption) tetap tercatat di Windows Event Viewer → *Windows Logs → Application*, sumber *Application Error* (id 1000).
- Log engine tampil di jendela konsol yang terbuka bersama game. `CCLog`/`CCLOG` legacy diteruskan ke sana.

---

## Known issues

Perilaku berikut **bawaan kode asli**, bukan akibat migrasi. Sesuai aturan roadmap, perbaikannya dijadwalkan pasca-MVP.

| Masalah | Keterangan |
|---|---|
| Ougis LV2/LV4 bisa dipakai sejak level 1 | Cek chakra/lock di `ActionButton` dikomentari di kode asli |
| Ikon buff tidak tampil untuk sebagian hero | `UI.plist` hanya punya ikon untuk Sakura & Chiyo; buff tetap berjalan |
| HP bisa minus, stun 500 ms jadi 0 s, cek jarak tanpa `abs()` | Lihat daftar bug di memory/report proyek |
| Hero Orochimaru, Nagato, Pain diam jika dikendalikan AI | AI-nya kosong di kode asli |
| Fitur online mati | Server asli tidak ada; endpoint di-stub |
| Memory leak kecil di beberapa tempat (`retain` tanpa `release`) | Tidak memengaruhi MVP |
| Format save `UserDefault` berubah (`.bin`) | Pengaturan dari build 2.x tidak terbaca |

---

## Aturan kontribusi

- Kerjakan di branch `axmol-migration`. Satu commit untuk satu langkah kecil, dan tag tiap milestone (`m0`…`m4`, `mvp`).
- **Jangan mencampur** perubahan format/encoding, migrasi API, dan perbaikan bug dalam satu commit.
- Repo memakai `.gitattributes` `* -text`: **byte file disimpan apa adanya** (campuran CRLF/LF dari kode asli dipertahankan).
- ⚠️ **Jangan memakai `sed -i` dari Git Bash** untuk mengedit source, karena diam-diam mengubah CRLF → LF. Gunakan editor biasa atau script Python yang bekerja per byte.
- Jangan menyalin `.editorconfig` dari template Axmol (memaksa LF + trim whitespace pada kode lama).
- Binary engine cocos2d-x 2.2.2 lama (satu-satunya salinan) disimpan di `C:\dev\naruto-senki-legacy-bin\`, di luar repo.

---

## Roadmap

Detail lengkap dan checklist ada di [`MIGRATION_ROADMAP.md`](MIGRATION_ROADMAP.md). Ringkasnya:

| Milestone | Isi | Status |
|---|---|---|
| M0 | Tooling + Axmol HelloWorld | ✅ |
| M1 | Git, snapshot `legacy-2.2.2`, GBK → UTF-8 | ✅ |
| M2 | Kerangka proyek CMake Axmol | ✅ |
| M3 | Lapisan kompatibilitas | ✅ |
| M4 | Kompilasi semua modul + hapus anti-tamper | ✅ |
| M5 | MVP berjalan (Training end-to-end) | ✅ |
| M6 | Pasca-MVP: semua hero & HardCore, gear, ranking → perbaikan bug asli → hapus shim (API Axmol native) → Android | ⏳ |
