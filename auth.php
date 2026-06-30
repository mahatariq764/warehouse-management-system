<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');
$conn = new mysqli("localhost","root","","warehouse_management_system",3306);
if($conn->connect_error){echo json_encode(["success"=>false,"message"=>"DB failed"]);exit;}
$action=$_POST['action']??'';
if($action==='register'){
  $name=trim($_POST['name']??'');
  $email=trim($_POST['email']??'');
  $password=$_POST['password']??'';
  if(!$name||!$email||!$password){echo json_encode(["success"=>false,"message"=>"All fields required."]);exit;}
  $s=$conn->prepare("SELECT id FROM users WHERE email=?");
  $s->bind_param("s",$email);$s->execute();$s->store_result();
  if($s->num_rows>0){echo json_encode(["success"=>false,"message"=>"Email already registered."]);exit;}
  $s->close();
  $h=password_hash($password,PASSWORD_BCRYPT);
  $s=$conn->prepare("INSERT INTO users(name,email,password) VALUES(?,?,?)");
  $s->bind_param("sss",$name,$email,$h);
  echo $s->execute()?json_encode(["success"=>true,"message"=>"Account created!"]):json_encode(["success"=>false,"message"=>"Registration failed."]);
}else if($action==='login'){
  $email=trim($_POST['email']??'');
  $password=$_POST['password']??'';
  $s=$conn->prepare("SELECT id,name,password FROM users WHERE email=?");
  $s->bind_param("s",$email);$s->execute();$s->bind_result($id,$name,$h);$s->fetch();$s->close();
  if($id&&password_verify($password,$h)){echo json_encode(["success"=>true,"message"=>"Welcome back, $name!"]);}
  else{echo json_encode(["success"=>false,"message"=>"Invalid email or password."]);}
}else{echo json_encode(["success"=>false,"message"=>"Unknown action."]);}
$conn->close();?>