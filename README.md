# Naruto Senki — Axmol Port

Port dari game Naruto Senki (game 2D *lane battle* Konoha vs Akatsuki dengan tower, creep, hero AI, gear, dan ougis) dari **cocos2d-x 2.2.2 / VS2010** ke **Axmol 2.11.5 / Visual Studio 2026 / Windows x64**.

> Game asli dibuat oleh **BlackIsle** ([zakume.net](http://www.zakume.net/)). Lihat [Credits](#credits).
> `ReadME.txt` adalah catatan asli dari pengembang lama dan disimpan sebagai arsip.
> Versi asli sebelum migrasi ada di tag git **`legacy-2.2.2`** (branch `main`).

| | |
|---|---|
| **Status** | **MVP selesai**. M6 berjalan: Training dengan semua hero dan mode HardCore bisa dimainkan di Windows x64 |
| Engine | [Axmol](https://github.com/axmolengine/axmol) **v2.11.5** (LTS) |
| Platform | Windows 10/11 **x64**. Android: panduan build tersedia, tetapi kode belum di-port (lihat [Build APK Android](#build-apk-android)) |
| Bahasa | C++20 (kode game legacy C++03 melalui lapisan kompatibilitas) |
| Branch kerja | `axmol-migration` |

---

## Daftar isi
1. [Requirement](#requirement)
2. [Setup lingkungan (sekali saja)](#setup-lingkungan-sekali-saja)
3. [Build](#build)
4. [Menjalankan game](#menjalankan-game)
5. [Build APK Android](#build-apk-android)
6. [Cakupan MVP](#cakupan-mvp)
7. [Struktur proyek](#struktur-proyek)
8. [Catatan teknis migrasi](#catatan-teknis-migrasi)
9. [Debugging & crash](#debugging--crash)
10. [Known issues](#known-issues)
11. [Aturan kontribusi](#aturan-kontribusi)
12. [Roadmap](#roadmap)
13. [Credits](#credits)

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

Tambahan khusus build Android (versi diambil dari `C:\dev\axmol\1k\build.profiles`). Semuanya **dipasang otomatis** oleh CLI `axmol` saat build Android pertama, kecuali sudah ada:

| Tool | Versi yang diminta Axmol 2.11.5 | Catatan |
|---|---|---|
| JDK | **17** (17.0.10–17.0.17) | JDK lain (mis. 25) tidak dipakai; Axmol memasang JDK 17 sendiri |
| Android SDK cmdline-tools | 19.0 | |
| Android NDK | **r23d** | Minimum untuk Axmol v2 |
| Build-tools / target SDK / min SDK | 35.0.0 / 36 / 17 | |
| Gradle / Android Gradle Plugin | 9.2.1 / 8.11.1 | Lewat `proj.android/gradlew` |
| Android Studio (opsional) | 2025.1.1+ | Hanya jika ingin debug lewat IDE |
| HP Android | arm64, Android 4.2+ | Untuk uji; aktifkan *USB debugging* |

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

### Mode permainan

| Menu | Alur | Isi pertandingan |
|---|---|---|
| **TRAINING** | Pilih hero (`SelectLayer`) → Start | Peta 1, tanpa gear dan guardian |
| **NETWORK** | Dialog konfirmasi HardCore (hanya sekali) → pilih tim (`NetworkLayer`) → Start | **HardCore**: peta acak 2–5, guardian Roshi/Han, gear shop, musik battle berbeda |

Cara memilih di layar HardCore: **double-click** = kunci pilihan. Urutannya **COM1 → COM2 → hero Anda**; label yang berkedip menunjukkan slot yang sedang dipilih. Jika Start ditekan sebelum hero Anda dikunci, hero dipilih acak (bonus skor *random char*). Pain dan Orochimaru terkunci ("LimitedChar"), sama seperti game asli.

### Lokasi data pemain
`%LOCALAPPDATA%\NarutoSenki\`

| File | Isi |
|---|---|
| `sql.db` | Coin & record per hero (SQLite, nilai di-obfuscate) |
| `CheckMD5_113.xml` | Hash `sql.db` (peninggalan anti-tamper, tidak lagi dicek) |
| `UserDefault.bin` | Pengaturan BGM/voice (format Axmol; save dari versi 2.x tidak ikut terbawa) |

Hapus folder ini untuk reset save.

---

## Build APK Android

> ⚠️ **Status: belum bisa di-build.** Langkah di bawah adalah alur resmi Axmol yang sudah disiapkan untuk proyek ini (`proj.android/` + `.axproj`), tetapi **belum pernah dijalankan sampai APK jadi**. Sebelum APK bisa dibuat, kode game harus di-port dulu (milestone M6 Android):
> - 5 file masih meng-include header JNI cocos2d-x 2.x yang tidak ada di Axmol: `GameLayer.h`, `GameOver.h`, `LoadLayer.h`, `NetworkLayer.cpp`, `StartMenu.cpp` (`../cocos2dx/platform/android/jni/JniHelper.h`, `<jni.h>`). Pada build Android, header ini harus diganti `platform/android/jni/JniHelper.h` milik Axmol atau dihapus.
> - Kode lama memanggil class Java `net/zakume/game/NarutoSenki` (iklan, webview berita, MAC/IMEI). Class itu tidak ada di template Axmol (`dev.axmol.app.AppActivity`), jadi pemanggilannya harus di-stub atau di-port.
> - Anti-tamper khusus Android (cek MD5 dan nama paket) sudah dihapus di M4.10, jadi tidak lagi menghalangi.
>
> Begitu poin di atas selesai, bagian ini akan diperbarui menjadi "teruji".

### 1. Persiapan (sekali saja)
1. Selesaikan [Setup lingkungan](#setup-lingkungan-sekali-saja) untuk Windows terlebih dahulu (CMake, PowerShell 7, Axmol di `C:\dev\axmol`, `AX_ROOT`).
2. **Pasang toolchain Android.** Pilih salah satu:
   - **Otomatis (disarankan)**: tidak perlu melakukan apa pun. Build Android pertama memasang JDK 17, Android SDK cmdline-tools, build-tools, platform SDK, dan NDK r23d ke `%USERPROFILE%\.1kiss\` (SDK di `%USERPROFILE%\.1kiss\adt\sdk`). Download-nya beberapa GB; pastikan koneksi stabil.
     Bisa juga dipicu lebih awal dengan:
     ```powershell
     pwsh C:\dev\axmol\setup.ps1 -p android
     ```
   - **Pakai Android Studio / SDK yang sudah ada**: set environment variable user `ANDROID_HOME` ke folder SDK (mis. `%LOCALAPPDATA%\Android\Sdk`), lalu pasang **NDK r23d** lewat *SDK Manager → SDK Tools → NDK (Side by side)*. Axmol akan memakai SDK itu dan hanya mengunduh yang kurang.
3. **Restart terminal** agar environment variable baru terbaca.

### 2. Identitas aplikasi
| Item | Nilai | Lokasi |
|---|---|---|
| Package / applicationId | `net.zakume.narutosenki` | `.axproj` (`package_name`) |
| Nama aplikasi | `NarutoSenki` | `proj.android/app/res/values/strings.xml` |
| ABI | `arm64-v8a` | `proj.android/gradle.properties` (`__1K_ARCHS`), atau opsi `-a` |
| Icon | `proj.android/app/res/mipmap-*/ic_launcher.png` | ganti dengan icon game |
| Orientasi | `sensorLandscape` | `proj.android/app/AndroidManifest.xml` |

Resource diambil dari `Resources/` dan kode dari `Classes/` lewat `CMakeLists.txt` yang sama dengan build Windows.

### 3. Build APK
Jalankan dari root repo:
```powershell
# APK debug: ditandatangani otomatis dengan debug key, bisa langsung di-install ke HP
axmol build -p android -a arm64 -O0

# APK release (RelWithDebInfo, default): TANPA keystore hasilnya unsigned dan tidak bisa di-install
axmol build -p android -a arm64

# Beberapa ABI sekaligus (HP arm64 + emulator x86_64)
axmol build -p android -a 'arm64;x64' -O0
```
Build pertama memakan waktu lama (mengunduh toolchain + mengompilasi seluruh engine untuk tiap ABI).

Hasil APK (lokasi standar Gradle):
```
proj.android\app\build\outputs\apk\debug\*.apk     # -O0
proj.android\app\build\outputs\apk\release\*.apk   # default / release
```
Jika lokasinya berbeda, cari dengan `Get-ChildItem proj.android -Recurse -Filter *.apk`.

### 4. APK release yang ditandatangani
1. Buat keystore (sekali saja; `keytool` ada di JDK 17 yang dipasang Axmol, `%USERPROFILE%\.1kiss\jdk\bin`):
   ```powershell
   keytool -genkeypair -v -keystore C:\keys\narutosenki.jks -alias narutosenki -keyalg RSA -keysize 2048 -validity 10000
   ```
2. Build dengan keystore tersebut (**jangan commit keystore maupun password ke repo**):
   ```powershell
   axmol build -p android -a arm64 -xc "-PKEY_STORE_FILE=C:/keys/narutosenki.jks,-PKEY_STORE_PASSWORD=<password>,-PKEY_ALIAS=narutosenki,-PKEY_PASSWORD=<password>"
   ```
3. Tambahkan `-aab` untuk menghasilkan `.aab` (format Google Play) alih-alih `.apk`.

### 5. Install & uji di HP
```powershell
$adb = "$env:USERPROFILE\.1kiss\adt\sdk\platform-tools\adb.exe"   # atau adb dari ANDROID_HOME
& $adb devices                                    # HP harus muncul (USB debugging aktif)
& $adb install -r proj.android\app\build\outputs\apk\debug\<nama>.apk
& $adb logcat -s axmol debug AndroidRuntime       # lihat log / crash
```

### Alternatif: Android Studio
```powershell
axmol build -p android -c     # hanya generate/sync proyek, tanpa build
```
Lalu buka folder `proj.android` di Android Studio, pilih varian *debug*, dan tekan Run.

---

## Cakupan MVP

**MVP = mode Training dapat dimainkan dari awal sampai akhir di Windows x64.**

| Fitur | Status |
|---|---|
| Intro → logo → *Push Start* → menu utama | ✅ |
| Training: pilih hero, loading, match 3v3 | ✅ |
| Semua hero yang bisa dipilih + transformasi (SageNaruto, dll.) sampai GameOver | ✅ (M6) |
| Joystick, attack, skill 1–3 + cooldown, item potion | ✅ |
| Ougis LV2 / LV4 (cut-in, buff) | ✅ |
| Creep, tower, level-up, respawn, kill feed, minimap | ✅ |
| Pause: resume, surrender, toggle BGM/voice | ✅ |
| GameOver (RESULT) → kembali ke menu → main lagi | ✅ |
| Coin tersimpan di `sql.db` | ✅ |
| Mode HardCore: dialog konfirmasi, pilih COM1/COM2/hero, peta 2–5 | ✅ (M6), belum diuji menyeluruh di semua peta |
| Gear shop, guardian Roshi/Han | 🟡 belum diuji |
| Ranking, Skill/Record, Credits | 🟡 compile & tampil, belum diuji menyeluruh |
| Login / group / upload skor (online) | ⛔ dimatikan (server lama tidak ada; tombol upload disembunyikan) |
| Android | ⛔ kode belum di-port (lihat [Build APK Android](#build-apk-android)) |

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
├─ proj.android/             # proyek Gradle Android dari template Axmol (lihat Build APK Android)
├─ .axproj                   # metadata proyek Axmol: versi engine, package name Android
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
| `RenderTexture::end()` menggambar langsung | command baru dieksekusi di akhir frame | `ccEndSnapshot()` me-render saat itu juga (mencegah crash GameOver/Pause/Gear bila ada node yang terhapus di frame yang sama) |

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
| Banyak frame sprite yang dirujuk kode tidak ada di resource (`select_title.png`, `hardcore_title.png`, UI grup/profil online, dll.) | Di build Debug langsung assert. Yang ada di jalur Training/HardCore sudah diganti; sisanya ada di fitur online yang mati |
| Rekor menang hero transformasi tercatat dengan nama transformasinya (mis. `SageNaruto`, bukan `Naruto`) | Akibat perubahan urutan di `GameOver::listResult` (commit `107e8b1`); mapping-nya tertimpa oleh `resultChar=...getCharacter()` |

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
| M6.1 | Semua hero + transformasi, mode HardCore | ✅ (perlu uji menyeluruh peta 2–5 & gear) |
| M6.2 | Gear, guardian, ranking/skill/credits | ⏳ |
| M6.3 | Perbaikan bug asli (lihat [Known issues](#known-issues)) | ⏳ |
| M6.4 | Hapus shim → API Axmol native | ⏳ |
| M6.5 | Android: port JNI → APK teruji | ⏳ |

---

## Credits

**Naruto Senki** (火影战记) dibuat oleh **BlackIsle** — [http://www.zakume.net/](http://www.zakume.net/), dengan cocos2d-x 2.2.2. Proyeknya dihentikan pada 3 Desember 2016 (pesan perpisahan di `ReadME.txt`: *"Farewell, Good Luck."*).

Teks dari layar Credits asli (`credits01.png` / `credits02.png` di `Resources/Menu.plist`):

> NarutoSenki was made with the resources for non-commercial use. If you are having any problems or suggestions, please contact us.
> Special thanks to **Neimad**, **Zoso**, **benhazard**, a contributor credited under a Chinese name, and the others.
> ALL RIGHTS RESERVED NAMCO BANDAI
> Dev by **BlackIsle**

Teks berbahasa Cina di layar yang sama menyatakan bahwa game ini dibuat untuk tujuan **non-komersial**, dan hak cipta gambar, audio, serta materi lain milik Bandai Namco dan pembuat aslinya di internet.

- *Naruto* © Masashi Kishimoto / Shueisha; karakter dan aset terkait milik pemegang hak masing-masing (Bandai Namco).
- Port ke Axmol: branch `axmol-migration` (repo ini), di atas engine [Axmol](https://github.com/axmolengine/axmol) (lisensi MIT).
- Library pihak ketiga di repo: SQLite (public domain), tinyxml2 (zlib).

Proyek ini **non-komersial** dan mengikuti syarat dari developer asli. Jangan menjual atau mendistribusikan build untuk tujuan komersial.
