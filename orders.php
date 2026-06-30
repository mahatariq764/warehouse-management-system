<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$conn = new mysqli("localhost", "root", "", "warehouse_management_system", 3306);
if ($conn->connect_error) { echo json_encode(["error" => "DB failed"]); exit; }

$action = $_POST['action'] ?? '';

// GET all orders
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
  $res    = $conn->query("SELECT * FROM orders ORDER BY created_at DESC");
  $orders = [];
  while ($row = $res->fetch_assoc()) $orders[] = $row;
  echo json_encode($orders);

// PLACE order
} elseif ($action === 'place') {
  $pid  = intval($_POST['product_id']);
  $qty  = intval($_POST['qty']);
  $type = $_POST['type'] === 'in' ? 'in' : 'out';

  if (!$pid || $qty <= 0) {
    echo json_encode(["success" => false, "message" => "Enter valid Product ID and Quantity."]);
    exit;
  }

  // Get product
  $s = $conn->prepare("SELECT name, quantity, price FROM products WHERE id = ?");
  $s->bind_param("i", $pid);
  $s->execute();
  $s->bind_result($pname, $pqty, $price);
  $s->fetch();
  $s->close();

  if (!$pname) {
    echo json_encode(["success" => false, "message" => "Product ID not found."]);
    exit;
  }

  // For dispatch, check stock
  if ($type === 'out' && $pqty < $qty) {
    echo json_encode(["success" => false, "message" => "Not enough stock. Available: $pqty"]);
    exit;
  }

  $cost    = $qty * $price;
  $new_qty = $type === 'in' ? $pqty + $qty : $pqty - $qty;

  // Insert order
  $s = $conn->prepare("INSERT INTO orders (product_id, product_name, qty, total_cost, type) VALUES (?,?,?,?,?)");
  $s->bind_param("issds", $pid, $pname, $qty, $cost, $type);
  $s->execute();
  $s->close();

  // Update product quantity
  $s = $conn->prepare("UPDATE products SET quantity = ? WHERE id = ?");
  $s->bind_param("ii", $new_qty, $pid);
  $s->execute();
  $s->close();

  echo json_encode(["success" => true, "message" => "Order placed! New stock: $new_qty"]);

} else {
  echo json_encode(["success" => false, "message" => "Unknown action."]);
}

$conn->close();
?>
