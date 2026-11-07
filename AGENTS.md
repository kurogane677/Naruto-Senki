# Panduan untuk asisten AI

Dokumen ini untuk asisten AI (Claude Code dan lainnya) yang ikut mengerjakan repo ini. Isinya hal-hal yang tidak terlihat dari kode saja: cara build dan test, arsitektur yang sedang dibangun, aturan yang disepakati dengan pemilik proyek, dan jebakan yang sudah pernah terjadi. Untuk setup lingkungan dan detail engine, lihat [`README.md`](README.md).

## 1. Konteks

- **Proyek**: port Naruto Senki (game 2D lane battle) dari cocos2d-x 2.2.2 ke **Axmol 2.11.5**, Windows x64 dan Android arm64. Kode lama tetap memakai API cocos2d-x 2.x lewat `Classes/Compat/Compat2x.h`.
- **Fase sekarang**: **v1.0.0-beta = refactor saja** (issue #21). Tidak ada fitur baru, perbaikan bug gameplay, perubahan balance, atau aset baru, kecuali pemilik proyek memintanya secara eksplisit. Fitur gameplay (#72, #75, #77, #78, #79, dst.) menunggu setelah v1.0.0-beta.
- **Bahasa**: pemilik proyek berkomunikasi dalam **bahasa Indonesia**. Jawaban, deskripsi PR, dan issue ditulis dalam bahasa Indonesia; **komentar kode dan pesan commit dalam bahasa Inggris**.
- **Prinsip kode**: DRY, SOLID, KISS, YAGNI, dengan nama yang jelas. Hindari nama satu huruf atau awalan `temp`, dan ganti angka ajaib dengan konstanta bernama saat kodenya dipindah.

## 2. Build dan test

Semua perintah dijalankan dari root repo di PowerShell.

```powershell
$env:AX_ROOT = "C:\dev\axmol"
cmake -S . -B build_x64 -D_AX_TESTS=ON          # ulangi setiap kali menambah/menghapus file
cmake --build build_x64 --config RelWithDebInfo --target NarutoSenki_test --parallel 2
.\build_x64\bin\NarutoSenki_test\RelWithDebInfo\NarutoSenki_test.exe
cmake --build build_x64 --config RelWithDebInfo --target NarutoSenki --parallel 2
```

- `cmake.exe` ada di dalam instalasi Visual Studio, tidak selalu di `PATH`.
- **Batasi paralelisme (`--parallel 2`)**. RAM mesin terbatas, dan build penuh pernah dihentikan karena memori habis. Proses `cl.exe` sisa build yang terhenti bisa mengunci file sumber; minta pemilik proyek menutupnya bila `git` gagal dengan `unable to unlink`.
- Mengubah `ActionManager.h` atau `Heroes/CharacterId.h` memicu kompilasi ulang hampir semua file.
- **Batas unit test**: target test mengompilasi semua `Classes/`, tetapi tidak punya renderer. Membuat sprite, `Hero`, atau `Monster` di test akan crash. Test hanya untuk logika murni dan data (tabel factory, registry, file di `Resources/` lewat `NS_RESOURCES_DIR`).
- **Soak autoplay**: `tools\soak.ps1 -MatchCount N [-Hero X] [-HardCore] -Out x.json [-Baseline y.json]`. Bandingkan rata-rata, bukan match per match (urutan timer engine bergantung alamat memori). Soak berat untuk RAM: **jangan menjalankan atau mengulang soak tanpa diminta**.

## 3. Arsitektur yang sedang dibangun

Ringkasan lengkap ada di README, bagian *Arsitektur kode (v1.0.0-beta)*. Yang perlu diingat saat menulis kode:

- **Identitas unit = `CharacterId`** (`Heroes/CharacterId.h`). Bandingkan dengan `isCharacter(CharacterId::X)` atau `characterId()`, jangan dengan string. `CHARACTER_IDS` adalah daftar tertutup: nama baru di data harus ditambahkan di sana, dan `Tests/CharacterIdTest.cpp` memeriksa semua XML dan peta.
- **Tidak ada karakter mod.** Hanya tim yang menambah karakter; jangan menambah fallback "unknown/modded".
- **Daftar hero** (slot select, record, COM, stealth, bentuk) ada di `Heroes/HeroRegistry.cpp`. **Aset per hero** di `Heroes/HeroAssets.cpp`.
- **Kelas per karakter** dibuat lewat tabel: `HeroFactory` (hero dan summon berbentuk `Hero`), `MonsterFactory` (monster). Keduanya memakai `UnitClasses<Base>` di `Units/UnitFactory.h`.
- **Hook virtual** di `ActionManager.h` (dan `Monster` di `Element.h`) adalah titik ekstensi. Default hook = perilaku unit biasa. **Tambahkan hook hanya ketika sebuah kelas membutuhkannya** (YAGNI), letakkan di titik yang sama persis dengan cabang lama, dan beri komentar satu baris.
- **Event `setClone`**: jenis klon dibaca dari skill yang sedang dipakai (`_actionState`), bukan dari angka di XML; angka itu hanya durasi (`cloneLifetime`). Jangan kembali memakai angka sebagai penanda.

### Menambah kelas hero (pola dari Naruto #88 dan Sakura #93)
1. Kumpulkan semua cabang `isCharacter(CharacterId::<Hero>)` dan `strcmp` nama hero itu. Pisahkan cabang **khusus hero itu** dari cabang **bersama** (misalnya `Sakura || Tsunade`). Cabang bersama tetap di tempatnya.
2. Buat `Classes/Heroes/Characters/<Hero>.h/.cpp` (`class <Hero> : public Hero`, `CREATE_FUNC`), lalu pindahkan cabang khusus ke override hook. Kalau hook yang dibutuhkan belum ada, tambahkan di `ActionManager.h` dengan default "tidak ditangani".
3. Daftarkan di tabel `HeroFactory::heroClasses()`; semua bentuk hero memakai kelas yang sama.
4. Summon dan monster milik hero itu ikut dipindah di PR yang sama bila punya perilaku khusus (#54, #98): summon lewat `HeroFactory` + `cloneCharacter()` (kelasnya di `Classes/Heroes/Summons/`, AI-nya lewat `ownAI()`), monster lewat `MonsterFactory` + `Monster::onSpawn()`. Mekanik yang identik di beberapa hero dipindah ke base bersama (contoh: `RidingHero` untuk Sai dan Deidara).
5. Pastikan perilaku identik. Baca kode sebelum dan sesudah baris yang dipindah: urutan pemanggilan, nilai yang ditimpa kemudian, dan `return` lebih awal.

### Akses member protected
Kelas hero tidak bisa mengakses member `protected` lewat pointer `Hero*` milik objek lain (error C2248). Gunakan `dynamic_cast<KelasSendiri*>` bila objeknya memang kelas yang sama (contoh: klon bayangan Naruto), atau batasi pada member public.

## 4. Mengedit file

- **Line ending campuran.** Repo memakai `.gitattributes` `* -text`; banyak file mencampur CRLF dan LF per baris. Pertahankan byte apa adanya.
- **Jangan memakai `sed -i` dari Git Bash** untuk source (mengubah CRLF diam-diam). `sed -n`/`cat` di Git Bash juga **menyembunyikan `\r`**; untuk memeriksa byte, pakai Python (`repr`).
- Untuk penggantian massal, tulis skrip Python yang bekerja per byte: pola dengan `\r?\n`, `assert` jumlah kecocokan, dan tulis file **hanya setelah semua penggantian berhasil** (supaya tidak ada perubahan setengah jalan). Simpan skrip di direktori scratchpad, bukan di repo.
- Python di Windows: `write_text` mengubah `\n` menjadi CRLF; pakai `write_bytes`. Skrip berisi backslash lebih aman ditulis ke file daripada lewat heredoc.
- Kode lama memakai gaya tab cocos2d-x; modul baru (`Heroes/`, `Units/`, `AI/`, `Tests/`) memakai 4 spasi. Ikuti gaya file yang disentuh.
- Jebakan kompatibilitas: `NULL` didefinisikan ulang sebagai `nullptr` (jangan dipakai sebagai angka); target game dikompilasi dengan `/Zc:strictStrings-`.

## 5. Alur kerja dengan pemilik proyek

- **Commit, push, PR, dan merge hanya atas permintaan eksplisit.** Kalimat yang dipakai pemilik proyek: "commit dan buat PR", "commit dan push", "merge PR #N ke main". Pemilik proyek menguji manual dulu sebelum meminta commit.
- **Satu cakupan per commit.** Perbaikan lain yang ditemukan di tengah jalan menjadi commit terpisah, atau ditawarkan sebagai PR/issue sendiri.
- **Jalankan build dan unit test tepat sebelum commit**, pada isi yang akan di-commit. Pemilik proyek kadang mengubah file sendiri di tengah pekerjaan; periksa `git status`/`git diff` dan jangan ikut meng-commit perubahan yang tidak dibahas.
- Branch dari `main` dengan prefix `chore/`, `feat/`, `fix/`, atau `docs/`. Setelah merge: hapus branch lokal dan remote, lalu sinkronkan `main` (`git pull --ff-only`).
- **PR bertumpuk**: bila branch B dibuat di atas branch A yang belum di-merge, base PR B = branch A. Setelah A di-merge, pindahkan base PR B ke `main` **sebelum** menghapus branch A (kalau tidak, GitHub menutup PR B).
- **Jangan pernah meng-commit**:
  - hack uji lokal pemilik proyek: coin 5000 (`Classes/Element.cpp`, baris `TESTING`), bypass ougi (`Classes/ActionButton.cpp`), `attackValue` uji di `Resources/Element/Naruto/Naruto.xml`;
  - `plan.md`, `ROADMAP.md`, `PLAN_*.md`, `promo/*`;
  - hasil repack atlas yang tidak dibahas (`Resources/Ougis2.plist`, `Resources/Result.plist/png`) dan gambar yang diubah pemilik proyek (`logo*.png`, `push_start*.png`) kecuali diminta;
  - keystore, password, APK.

  Untuk file yang berisi hack sekaligus perubahan sungguhan (misalnya `Element.cpp`), matikan hack-nya dulu, stage file, lalu nyalakan lagi.
- Pesan commit dalam bahasa Inggris, diakhiri baris atribusi yang diminta lingkungan. Deskripsi PR dalam bahasa Indonesia: bagian *Isi*, *Sengaja belum*, dan *Tes* (checklist build, test, tes manual, soak).
- **Tag rilis**: selalu minta konfirmasi sebelum push tag (memicu CI). Mulai v1.0.0-beta tag ditandatangani (`git tag -s`); jangan membuat kunci tanda tangan tanpa izin.
- **Jangan menjalankan GitHub Actions secara manual, dan jangan mengubah versi tool atau action** di workflow.
- Profil AI bot, daftar gear, dan daftar karakter **tidak bisa di-mod**: semuanya di kode.
- Tanyakan dulu sebelum menjalankan autoplay untuk pekerjaan AI.

## 6. Jejak keputusan

| Issue/PR | Isi |
|---|---|
| #21 | Rencana besar refactor v1.0.0-beta (target nol `strcmp`, kosakata strict) |
| #22–#23, #88 | Kelas hero pertama (Naruto) dan pola hook |
| #89 | Rantai bentuk hero sebagai data (`HeroRegistry`) |
| #90 | `CharacterId`; Nagato/DevaPath (kode mati) dihapus |
| #91, #92 | `HeroAssets` simetris; opsi Preload suara dihapus |
| #93 | Kelas Sakura; tabel `HeroFactory` |
| #95, #96 | Fondasi `UnitClasses`/`MonsterFactory`; monster Naruto |
| #97, #98 | `setClone`: identitas klon dari skill; durasi Kurama strict |
| #100 | Kelas Sai; base `RidingHero` bersama Deidara; `RideMonster`, `InkDragon` |
| #54 | Sisa summon dan monster, dipindah bersama hero pemiliknya |
| #85 | Perubahan roster COM (sudah di-merge, soak belum) |

Urutan pengerjaan kelas hero mengikuti layar select HardCore: Naruto, Sakura, Sai, Kakashi, **Shikamaru** (berikutnya), Ino, Choji, Asuma, dan seterusnya. Daftar lengkapnya: `HeroRegistry::heroes()` (diurutkan menurut slot HardCore).
