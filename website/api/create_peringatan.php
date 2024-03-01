<?php
include 'database.php';
$connect         = mysqli_connect($host, $user, $pass, $data);

date_default_timezone_set("Asia/Jakarta");
$tanggal         = date("Y-m-d");
$waktu           = date("H:i:s");
$dempet_samping  = $_GET['dempet_samping'];
$dempet_depan    = $_GET['dempet_depan'];

if (isset($_GET['dempet_samping'])) {
  $dempet_samping = $_GET['dempet_samping'];
} else {
  echo "url tidak lengkap\nhttp://localhost:8080/website/api/create_peringatan.php?dempet_samping={dempet_samping}&dempet_depan={dempet_depan}";
  $dempet_samping = null;
}
if (isset($_GET['dempet_depan'])) {
  $dempet_depan = $_GET['dempet_depan'];
} else {
  echo "url tidak lengkap\nhttp://localhost:8080/website/api/create_peringatan.php?dempet_samping={dempet_samping}&dempet_depan={dempet_depan}";
  $dempet_depan = null;
}
// http://localhost:8080/website/api/create_peringatan.php?dempet_samping={dempet_samping}&dempet_depan={dempet_depan}
mysqli_query($connect, "ALTER TABLE tb_peringatan AUTO_INCREMENT=1");
$simpan = mysqli_query($connect, "INSERT INTO `tb_peringatan` (tanggal, waktu, dempet_samping, dempet_depan)VALUES('$tanggal', '$waktu', '$dempet_samping', '$dempet_depan')");
if ($simpan) {
  echo "Berhasil\n";
  echo $tanggal;
  echo " ";
  echo $waktu;
} else {
  echo "Gagal\n";
  echo $tanggal;
  echo " ";
  echo $waktu;
}
?>