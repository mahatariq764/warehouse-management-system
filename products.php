<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$conn = new mysqli("localhost", "root", "", "warehouse_management_system", 3306);
if ($conn->connect_error) { echo json_encode(["error" => "DB failed"]); exit; }

$action = $_POST['action'] ?? '';

// GET all products
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
  $res = $conn->query("SELECT * FROM products ORDER BY id");
  $products = [];
  while ($row = $res->fetch_assoc()) $products[] = $row;
  echo json_encode($products);

// ADD product
} elseif ($action === 'add') {
  $id       = intval($_POST['id']);
  $name     = trim($_POST['name']);
  $category = trim($_POST['category']);
  $quantity = intval($_POST['quantity']);
  $price    = floatval($_POST['price']);

  if (!$id || !$name || !$category || $quantity < 0 || $price < 0) {
    echo json_encode(["success" => false, "message" => "Fill all fields correctly."]);
    exit;
  }

  $s = $conn->prepare("INSERT INTO products (id, name, category, quantity, price) VALUES (?,?,?,?,?)");
  $s->bind_param("issid", $id, $name, $category, $quantity, $price);
  echo $s->execute()
    ? json_encode(["success" => true,  "message" => "Product added!"])
    : json_encode(["success" => false, "message" => "ID may already exist."]);

// DELETE product
} elseif ($action === 'delete') {
  $id = intval($_POST['id']);
  $s  = $conn->prepare("DELETE FROM products WHERE id = ?");
  $s->bind_param("i", $id);
  echo $s->execute()
    ? json_encode(["success" => true,  "message" => "Deleted."])
    : json_encode(["success" => false, "message" => "Delete failed."]);

} else {
  echo json_encode(["success" => false, "message" => "Unknown action."]);
}

$conn->close();
?>
