<?php
header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

$conn = new mysqli("localhost", "root", "", "warehouse_management_system", 3306);
if ($conn->connect_error) { echo json_encode(["error" => "DB failed"]); exit; }

$r1 = $conn->query("SELECT COUNT(*) FROM products")->fetch_row()[0];
$r2 = $conn->query("SELECT COUNT(*) FROM products WHERE quantity <= 10")->fetch_row()[0];
$r3 = $conn->query("SELECT COUNT(*) FROM orders")->fetch_row()[0];

$orders = [];
$res = $conn->query("SELECT order_id, product_name, qty, total_cost, type, created_at FROM orders ORDER BY created_at DESC LIMIT 5");
while ($row = $res->fetch_assoc()) $orders[] = $row;

echo json_encode([
  "total_products" => $r1,
  "low_stock"      => $r2,
  "total_orders"   => $r3,
  "recent_orders"  => $orders
]);

$conn->close();
?>
