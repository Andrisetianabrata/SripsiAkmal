<?php
include 'database.php';
$connect         = mysqli_connect($host, $user, $pass, $data);

date_default_timezone_set("Asia/Jakarta");
$tanggal         = date("Y-m-d");
$waktu           = date("H:i:s");
$kondisi         = $_GET['kondisi'];
$jumlah          = $_GET['jumlah'];

if (isset($_GET['jumlah'])) {
  $jumlah          = $_GET['jumlah'];
} else {
  echo "url tidak lengkap\nhttp://localhost:8080/website/api/create.php?kondisi={kondisi}&jumlah={jumlah}";
  $jumlah = null;
}
// http://localhost:8080/website/api/create.php?kondisi={kondisi}&jumlah={jumlah}
mysqli_query($connect, "ALTER TABLE tb_data AUTO_INCREMENT=1");
$simpan = mysqli_query($connect, "INSERT INTO tb_data(tanggal, waktu, kondisi, jumlah)VALUES('$tanggal', '$waktu', '$kondisi', '$jumlah')");
if ($simpan) {
  echo "Berhasil Kirim Counter\n";
  echo $tanggal;
  echo " ";
  echo $waktu;
} else {
  echo "Gagal Kirim Counter\n";
  echo $tanggal;
  echo " ";
  echo $waktu;
}
?>